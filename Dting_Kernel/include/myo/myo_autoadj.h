/*
 * myo_autoadj.h
 *
 *  Created on: Jul 23, 2014
 *      Author: li_we
 */

#ifndef MYO_AUTOADJ_H_
#define MYO_AUTOADJ_H_

#include <myo_dp.h>
#include <myo_adc.h>

/******************** Local type define ***************************/
typedef enum
{
  AD_SUCCESS		= 0x00,
  AD_ONGOING		= 0x01,
  AD_OUTRANGE		= 0x02,
}AD_AdjustStateTypeDef;

typedef struct
{
	uint32_t Resistance;
	AD_AdjustStateTypeDef State;
}AD_ElementTypeDef;

typedef struct
{
	AD_ElementTypeDef R_Shift;
	AD_ElementTypeDef R_Scale;
	AD_AdjustStateTypeDef State;
}AD_AdjustTypeDef;

/******************* Local definition *******************************/
#define MYO_R_SHIFT								DP_ADDR_RDAC2
#define MYO_R_SCALE								DP_ADDR_RDAC1
#define MYO_R3_VALUE							1000
#define MYO_R4_VALUE							1000

#define MYO_AD_SHIFT_MARGIN						0.05
#define MYO_AD_SCALE_MARGIN						0.3
#define MYO_AD_JUMP								100

#define MYO_AD_V_MAX							3.3
#define MYO_AD_V_MIN							0

/******************** Functions prototype ***************************/
AD_AdjustStateTypeDef MYO_AD_ShiftAdjust(arm_matrix_instance_f32 * CS, AD_AdjustTypeDef * AD);
AD_AdjustStateTypeDef MYO_AD_ScaleAdjust(arm_matrix_instance_f32 * CS, AD_AdjustTypeDef * AD);
AD_AdjustStateTypeDef MYO_AD_AutoAdjust(void);

#endif /* MYO_AUTOADJ_H_ */
