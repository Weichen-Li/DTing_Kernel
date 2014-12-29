/* 	@file		myo_autoadj.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 * 	@brief		Digital Potentiometer auto-adjustment.
 */
#include <myo_autoadj.h>

/****************** Global variable prototype ********************/
AD_AdjustTypeDef SSAdjust;

/*********************** Private functions **********************/

/* @brief			Shifting resistor adjustment iteration.
 * 					Need to be invoked with Scaling resistor adjustment in a loop.
 * @param *CS		Reference signal sample.
 * @param *AD		The auto-adjustment parameter struct.
 * 					@ref AD_AdjustTypeDef
 * @retval			Adjusting state.
 */
AD_AdjustStateTypeDef MYO_AD_ShiftAdjust(arm_matrix_instance_f32 * CS, AD_AdjustTypeDef * AD)
{
	float Vo, Delta, Shift;
	uint16_t Count = 10;
	MYO_ADC_RecogConvert(CS);
	arm_mean_f32(CS->pData, MYO_TUNNEL_NUMBER * MYO_SEGMENT_LENGTH, &Vo);
	Vo = Vo + 3.3/2.0;
	if(Vo > (DP_V_MID - MYO_AD_SHIFT_MARGIN) && Vo < (DP_V_MID + MYO_AD_SHIFT_MARGIN))
		return AD_SUCCESS;
	else
	{
		while(Count--)
		{
		Delta = DP_V_MID - Vo;
		Shift = AD->R_Shift.Resistance - (Delta * AD->R_Scale.Resistance * (AD->R_Shift.Resistance + MYO_R3_VALUE) * (AD->R_Shift.Resistance + MYO_R3_VALUE)) / (DP_V_AUX * MYO_R3_VALUE * (AD->R_Scale.Resistance + MYO_R4_VALUE));
		if(Shift > DP_R_AB)
		{
			AD->R_Shift.Resistance = DP_R_AB;
			return AD_OUTRANGE;
		}
		else if(Shift < DP_R_W)
		{
			AD->R_Shift.Resistance = DP_R_W;
			return AD_OUTRANGE;
		}
		else
			AD->R_Shift.Resistance = Shift;
		MYO_DP_Command(DP_COMMAND(DP_WRITE_RADC, MYO_R_SHIFT, DP_RADC_DATA(AD->R_Shift.Resistance)));
		timer_sleep(DP_SETTLE_TIME_US);
		MYO_ADC_RecogConvert(CS);
		arm_mean_f32(CS->pData, MYO_TUNNEL_NUMBER * MYO_SEGMENT_LENGTH, &Vo);
		Vo = Vo + 3.3/2.0;
		if(Vo < (DP_V_MID + MYO_AD_SHIFT_MARGIN) && Vo > (DP_V_MID - MYO_AD_SHIFT_MARGIN))
			return AD_ONGOING;
		}
		return AD_ONGOING;
	}
}

/* @brief			Scaling resistor adjustment iteration.
 * 					Need to be invoked with Shifting resistor adjustment in a loop.
 * @param *CS		Reference signal sample.
 * @param *AD		The auto-adjustment parameter struct.
 * 					@ref AD_AdjustTypeDef
 * @retval			Adjusting state.
 */
AD_AdjustStateTypeDef MYO_AD_ScaleAdjust(arm_matrix_instance_f32 * CS, AD_AdjustTypeDef * AD)
{
	float Vmax, Vmin;
	uint32_t Index;
	uint16_t Count = 10;
	uint16_t Jump = MYO_AD_JUMP;
	MYO_ADC_RecogConvert(CS);
	arm_max_f32(CS->pData, MYO_TUNNEL_NUMBER * MYO_SEGMENT_LENGTH, &Vmax, &Index);
	arm_min_f32(CS->pData, MYO_TUNNEL_NUMBER * MYO_SEGMENT_LENGTH, &Vmin, &Index);
	Vmax = Vmax + 3.3/2.0;
	Vmin = Vmin + 3.3/2.0;
	if(Vmax < (MYO_AD_V_MAX - MYO_AD_SCALE_MARGIN) && Vmin > (MYO_AD_V_MIN + MYO_AD_SCALE_MARGIN))
		if(Vmax > (MYO_AD_V_MAX - MYO_AD_SCALE_MARGIN*2) && Vmin < (MYO_AD_V_MIN + MYO_AD_SCALE_MARGIN*2))
			return AD_SUCCESS;
	{
		while(Count--)
		{
			if(!(Vmax < (MYO_AD_V_MAX - MYO_AD_SCALE_MARGIN) && Vmin > (MYO_AD_V_MIN + MYO_AD_SCALE_MARGIN)))
			{
				AD->R_Scale.Resistance += Jump;
				Jump -= Jump/2;
			}
			else if(!(Vmax > (MYO_AD_V_MAX - MYO_AD_SCALE_MARGIN*2) && Vmin < (MYO_AD_V_MIN + MYO_AD_SCALE_MARGIN*2)))
			{
				AD->R_Scale.Resistance -= Jump;
				Jump -= Jump/2;
			}
			else
				return AD_ONGOING;
			if(AD->R_Scale.Resistance > DP_R_AB)
			{
				AD->R_Scale.Resistance = DP_R_AB;
				return AD_OUTRANGE;
			}
			else if(AD->R_Scale.Resistance < DP_R_W)
			{
				AD->R_Scale.Resistance = DP_R_W;
				return AD_OUTRANGE;
			}
			MYO_DP_Command(DP_COMMAND(DP_WRITE_RADC, MYO_R_SCALE, DP_RADC_DATA(AD->R_Scale.Resistance)));
			timer_sleep(DP_SETTLE_TIME_US);
			if(Jump <= 5)
				return AD_ONGOING;
			else
			{
				MYO_ADC_RecogConvert(CS);
				arm_max_f32(CS->pData, MYO_TUNNEL_NUMBER * MYO_SEGMENT_LENGTH, &Vmax, &Index);
				arm_min_f32(CS->pData, MYO_TUNNEL_NUMBER * MYO_SEGMENT_LENGTH, &Vmin, &Index);
				Vmax = Vmax + 3.3/2.0;
				Vmin = Vmin + 3.3/2.0;
			}
		}
		return AD_ONGOING;
	}
}

/* @brief			Auto-adjustment implementation.
 * 					This function adjust the sample to all positive
 * 					value but in the range between 0 and 3.3V.
 * @retval			Adjusting state.
 */

AD_AdjustStateTypeDef MYO_AD_AutoAdjust(void)
{
	uint16_t OR_Scale, OR_Shift;
	uint16_t Counter = 20;
	float32_t mdAdjustSample[MYO_SEGMENT_LENGTH * MYO_TUNNEL_NUMBER];
	arm_matrix_instance_f32 mAdjustSample;

	SSAdjust.R_Scale.Resistance = 500;
	SSAdjust.R_Shift.Resistance = 5000;
	SSAdjust.State = AD_ONGOING;
	arm_mat_init_f32(&mAdjustSample, MYO_TUNNEL_NUMBER, MYO_SEGMENT_LENGTH, mdAdjustSample);

	MYO_DP_Command(DP_COMMAND(DP_WRITE_RADC, MYO_R_SCALE, DP_RADC_DATA(SSAdjust.R_Scale.Resistance)));
	MYO_DP_Command(DP_COMMAND(DP_WRITE_RADC, MYO_R_SHIFT, DP_RADC_DATA(SSAdjust.R_Shift.Resistance)));
	trace_printf("DP auto adjustment initial value set.\n");

	while(SSAdjust.State)
	{
		OR_Shift = SSAdjust.R_Shift.Resistance;
		OR_Scale = SSAdjust.R_Scale.Resistance;
		SSAdjust.R_Shift.State = MYO_AD_ShiftAdjust(&mAdjustSample, &SSAdjust);
		SSAdjust.R_Scale.State = MYO_AD_ScaleAdjust(&mAdjustSample, &SSAdjust);
		if(SSAdjust.R_Shift.State > AD_ONGOING || SSAdjust.R_Scale.State > AD_ONGOING)
		{
			if(SSAdjust.R_Shift.State > AD_ONGOING)
				SSAdjust.R_Scale.Resistance = OR_Scale;
			else
				SSAdjust.R_Shift.Resistance = OR_Shift;
			MYO_DP_Command(DP_COMMAND(DP_WRITE_RADC, MYO_R_SCALE, DP_RADC_DATA(SSAdjust.R_Scale.Resistance)));
			MYO_DP_Command(DP_COMMAND(DP_WRITE_RADC, MYO_R_SHIFT, DP_RADC_DATA(SSAdjust.R_Shift.Resistance)));
			SSAdjust.State = AD_OUTRANGE;
			break;
		}
		SSAdjust.State = SSAdjust.R_Shift.State | SSAdjust.R_Scale.State;
		if(!Counter--)
			break;
		trace_printf("DP auto adjustment iteration %u.\n", Counter);
	}
	return SSAdjust.State;
}
/******************************** End of file *********************************/
