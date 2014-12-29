/* 	@file		main.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 * 	@brief		The main file of DTing_Kernel project.
 */

#include <stdio.h>
#include "diag/Trace.h"
#include "Timer.h"
#include <MYO_def.h>
#include <arm_math.h>
#include <MYO_head.h>

/****************** variable prefix and suffix definition************************/
/* The prefix and suffix of variables are define in the myo algorithm
 * for the benefit of understanding and reusability.
 *
 * Define:
 * 			m-			Matrix
 * 			ptr-		Pointer
 *  		d-			Data
 *  		v-			Vector
 *			-o			Output
 *			-[none]		Input
 * Example:
 *  		arm_matrix_instance_f32 *		mptrScatterMatrix;
 *  		float32_t 						mdScatterMatrix;
 *
 * Notice:
 * 			There are non-prefix defined variables in the algorithm as well.
 * 			They could be any datatype define in standard C99 definition.
 * 			Please see the details in the specific function description.
 *
 * 	See the same description in MYO_def.h
 */
/*********************************************************************/

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#ifdef MYO_DATASIM
	extern float32_t	Datasim_freq[];
	extern float32_t	Datasim_mag[];
#endif

/* Application access point.
 * Notice the function "__initialize_hardware()" is called directly after "_start()".
 * All the hardware configuration process should be done in "__initialize_hardware()
 * before application access point.
 */
int
main(int argc, char* argv[])
{
	/***** variable and instance definition ****/
	int i,j;
	int TestOut = 0;
	uint8_t TxBuf[Buffer_Size] = {0};
	//uint8_t RxBuf[Buffer_Size];
	//AD_AdjustStateTypeDef DP_State = AD_ONGOING;
#ifdef MYO_DATASIM
	int sim_counter = 0;
#endif
	float32_t mdTrainingSample[MYO_SEGMENT_LENGTH * MYO_TRAINING_LENGTH * MYO_TUNNEL_NUMBER * MYO_GESTURE_NUMBER];
	float32_t mdTrainingFeature[MYO_TRAINING_LENGTH * MYO_FEATURE_NUMBER * MYO_TUNNEL_NUMBER * MYO_GESTURE_NUMBER];
	float32_t mdRecognizingSample[MYO_SEGMENT_LENGTH * MYO_TUNNEL_NUMBER];
	float32_t mdRecognizingFeature[MYO_FEATURE_NUMBER * MYO_TUNNEL_NUMBER];
	float32_t vBoundary[LDA_COMBO_NUMBER];
	float32_t vProjectionVector[MYO_FEATURE_NUMBER * LDA_COMBO_NUMBER * MYO_TUNNEL_NUMBER];

	arm_matrix_instance_f32 mTrainingSampleCTView[MYO_GESTURE_NUMBER][MYO_TUNNEL_NUMBER];
	arm_matrix_instance_f32 mTrainingFeatureCTView[MYO_GESTURE_NUMBER][MYO_TUNNEL_NUMBER];
	arm_matrix_instance_f32 mTrainingFeatureCompleteView;
	arm_matrix_instance_f32 mRecongnizingSampleTView[MYO_TUNNEL_NUMBER];
	arm_matrix_instance_f32 mRecongnizingFeatureTView[MYO_TUNNEL_NUMBER];
	arm_matrix_instance_f32 mRecongnizingFeatureCompleteView;
	arm_matrix_instance_f32 mRecongnizingSampleCompleteView;

	/**** variable and instance initialization ****/
	for(i=0;i<MYO_GESTURE_NUMBER;i++)
		for(j=0;j<MYO_TUNNEL_NUMBER;j++)
		{
			arm_mat_init_f32(&mTrainingSampleCTView[i][j], 1, MYO_SEGMENT_LENGTH * MYO_TRAINING_LENGTH, &mdTrainingSample[i*MYO_SEGMENT_LENGTH * MYO_TRAINING_LENGTH * MYO_TUNNEL_NUMBER + j*MYO_SEGMENT_LENGTH * MYO_TRAINING_LENGTH]);
			arm_mat_init_f32(&mTrainingFeatureCTView[i][j], MYO_FEATURE_NUMBER, MYO_TRAINING_LENGTH, &mdTrainingFeature[i * MYO_TRAINING_LENGTH *MYO_FEATURE_NUMBER * MYO_TUNNEL_NUMBER + j*MYO_TRAINING_LENGTH *MYO_FEATURE_NUMBER]);
		}
	for(i=0;i<MYO_TUNNEL_NUMBER;i++)
	{
		arm_mat_init_f32(&mRecongnizingSampleTView[i], 1, MYO_SEGMENT_LENGTH, &mdRecognizingSample[i*MYO_SEGMENT_LENGTH]);
		arm_mat_init_f32(&mRecongnizingFeatureTView[i], MYO_FEATURE_NUMBER, 1, &mdRecognizingFeature[i*MYO_FEATURE_NUMBER]);
	}
	arm_mat_init_f32(&mTrainingFeatureCompleteView, MYO_FEATURE_NUMBER * MYO_TUNNEL_NUMBER * MYO_GESTURE_NUMBER, MYO_TRAINING_LENGTH, mdTrainingFeature);
	arm_mat_init_f32(&mRecongnizingSampleCompleteView, MYO_TUNNEL_NUMBER, MYO_SEGMENT_LENGTH, mdRecognizingSample);
	arm_mat_init_f32(&mRecongnizingFeatureCompleteView, MYO_FEATURE_NUMBER * MYO_TUNNEL_NUMBER, 1, mdRecognizingFeature);

	/**** related hardware initialization ****/
	timer_start();
	trace_printf("System clock: %uHz\n", SystemCoreClock);
	MYO_MUX_Init();
	trace_printf("Multiplexer initialization success.\n");
	MYO_ADC_DMA_Init();
	MYO_ADC_Init();
	trace_printf("Analog-digital Converter initialization success.\n");
	MYO_DP_Init();
	trace_printf("Digital Potentiometer initialization success.\n");
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	trace_printf("Board configuration initialization success.\n");
	//nRF24L01_HW_Init();
	//TX_Mode();

	/* sEMG voltage range auto adjustment */
	i=600;	//ref
	j=27000;//ref
	//i= 10000;
	//j= 3000;
	MYO_DP_Command(DP_COMMAND(DP_WRITE_RADC, MYO_R_SCALE, DP_RADC_DATA(i)));
	MYO_DP_Command(DP_COMMAND(DP_WRITE_RADC, MYO_R_SHIFT, DP_RADC_DATA(j)));

	/**** model training procedure ****/
#ifdef MYO_DATASIM
	for(sim_counter=0; sim_counter < MYO_GESTURE_NUMBER; sim_counter++)
	{
		MYO_SinWaveSample(Datasim_freq[sim_counter], Datasim_mag[sim_counter], mTrainingSampleCTView[sim_counter][0].pData);
		FE_Training_FeatureIntegrate(mTrainingSampleCTView[sim_counter][0].pData, &mTrainingFeatureCTView[sim_counter][0]);
	}
#else
	/* class 1 training data acquisition */
	while(BSP_PB_GetState(BUTTON_KEY) != RESET)
	{
	}
	trace_printf("Key pressed for C1.\n");
	MYO_ADC_TrainingConvert(&mTrainingSampleCTView[0][0]);
	trace_printf("Sampling finished for C1.\n");

	/* class 2 training data acquisition */
	while(BSP_PB_GetState(BUTTON_KEY) != RESET)
	{
	}
	trace_printf("Key pressed for C2.\n");
	MYO_ADC_TrainingConvert(&mTrainingSampleCTView[1][0]);
	trace_printf("Sampling finished for C2.\n");

	while(BSP_PB_GetState(BUTTON_KEY) != RESET)
	{
	}
	trace_printf("Key pressed for C3.\n");
	MYO_ADC_TrainingConvert(&mTrainingSampleCTView[2][0]);
	trace_printf("Sampling finished for C3.\n");
#endif
	for(i=0;i<MYO_GESTURE_NUMBER;i++)
		for(j=0;j<MYO_TUNNEL_NUMBER;j++)
			FE_Training_FeatureIntegrate(mTrainingSampleCTView[i][j].pData, &mTrainingFeatureCTView[i][j]);
	LDA_MC_Train(&mTrainingFeatureCTView[0][0], vProjectionVector, vBoundary);

	/**** gesture recognizing procedure ****/
	while(BSP_PB_GetState(BUTTON_KEY) != RESET)
	{
	}
	trace_printf("Key pressed for recognizing .\n");
	while(1)
	{
#ifdef MYO_DATASIM
		for(sim_counter=0;sim_counter<MYO_GESTURE_NUMBER;sim_counter++)
		{
			MYO_SinWaveSample(Datasim_freq[sim_counter], Datasim_mag[sim_counter], mRecongnizingSampleCompleteView.pData);
#else
		MYO_ADC_RecogConvert(&mRecongnizingSampleCompleteView);
#endif
		for(i=0;i<MYO_TUNNEL_NUMBER;i++)
			FE_Recognizing_FeatureIntegrate(mRecongnizingSampleTView[i].pData, &mRecongnizingFeatureTView[i]);
		j++;
		TestOut = LDA_MC_Classify(&mRecongnizingFeatureCompleteView, vProjectionVector, vBoundary);
		if(j==100)
			j=0;
		TxBuf[0] = TestOut;
		nRF24L01_TxPacket(TxBuf);
		trace_printf("Class %d .\n",TestOut);
#ifdef MYO_DATASIM
		}
#endif
	}
	timer_sleep(10000);

	/* The end of main program */
	while (1)
    {
		timer_sleep(10000);
    }

  // Infinite loop, never return.
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
