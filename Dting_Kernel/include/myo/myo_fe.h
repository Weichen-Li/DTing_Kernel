/* @file				myo_fe.h
 * @auther				Weichen Li
 * @date
 * @brief				Definitions and prototypes for myo_fe.c.
 */
#ifndef _MYO_FE_H_
#define _MYO_FE_H_
/* includes */
#include <stdio.h>
#include <math.h>
#include <arm_math.h>
#include <MYO_def.h>

/******************* Local definition *******************************/
#define FE_FEATURE_NUMBER			MYO_FEATURE_NUMBER
#define FE_SAMPLE_LENGTH			MYO_SEGMENT_LENGTH
#define FE_TRAINING_LENGTH			MYO_TRAINING_LENGTH
#define FE_THRESHOLD				0.0

/******************** Functions prototype ***************************/
float32_t FE_MeanAbsoluteValue(float32_t * vptrS);
float32_t FE_SlopSignChange(float32_t * vptrS);
float32_t FE_WaveformLength(float32_t * vptrS);
float32_t FE_ZeroCrossing(float32_t * vptrS);
void FE_FeatureExtract(float32_t * vptrS, float32_t * vptrFo);
void FE_Training_FeatureIntegrate(float32_t * vptrTS, arm_matrix_instance_f32 * mptrTFo);
void FE_Recognizing_FeatureIntegrate(float32_t * vptrRS, arm_matrix_instance_f32 * mptrRFo);
int FE_Compare_Float(float32_t F1, float32_t F2);

#endif /* MYO_FE_H_ */
