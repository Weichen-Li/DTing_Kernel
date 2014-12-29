/* 	@file		myo_datasim.h
 * 	@author		Weichen Li
 * 	@date		20141229
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 * 	@brief		Sinwave generation benchmark header file.
 */
#ifndef MYO_DATASIM_H_
#define MYO_DATASIM_H_

#include <MYO_def.h>
#include <arm_math.h>

/******************* Local definition *******************************/
#define DATASIM_CLASS_NUMBER		MYO_GESTURE_NUMBER
#define DATASIM_SIM_LENGTH			MYO_SEGMENT_LENGTH * MYO_TRAINING_LENGTH * MYO_TUNNEL_NUMBER

/******************** Functions prototype ***************************/
void MYO_SinWaveSample(int Freq, float32_t PP, float32_t * So);
float MYO_Function_Test(void);

#endif /* MYO_DATASIM_H_ */
