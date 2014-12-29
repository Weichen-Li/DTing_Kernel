/* @file				myo_lda.h
 * @auther				Weichen Li
 * @date
 * @brief				Definitions and prototypes for myo_lda.c.
 */
#ifndef _MYO_LDA_H
#define _MYO_LDA_H
#include <math.h>
#include <arm_math.h>
#include <MYO_def.h>

/******************* Local definition *******************************/
#ifdef MYO_UNDEFINED_CLASS
#define LDA_UNDEFINED_CLASS
#endif

#define LDA_SAMPLE_DIMENSION	(MYO_FEATURE_NUMBER * MYO_TUNNEL_NUMBER)
#define LDA_SAMPLE_NUMBER		MYO_TRAINING_LENGTH
#define LDA_2_CLASS				2
#define LDA_CLASS_NUMBER		MYO_GESTURE_NUMBER
#define LDA_COMBO_NUMBER		(LDA_CLASS_NUMBER * (LDA_CLASS_NUMBER - 1) / 2)

/******************** Functions prototype ***************************/
void LDA_MeanVector(arm_matrix_instance_f32 * mptrTF, float32_t * vptrMVo);
void LDA_ScatterMatrix(arm_matrix_instance_f32 * mptrTF, float32_t * vptrMV, arm_matrix_instance_f32 * mptrSMo);
void LDA_WithinScatterMatrix(arm_matrix_instance_f32 * mptrTF, float32_t * vptrMV, arm_matrix_instance_f32 * mptrWSMo);
void LDA_ProjectionVector(arm_matrix_instance_f32 * mptrWSM, float32_t * vptrMV, float32_t * vptrPVo);
void LDA_MC_Train(arm_matrix_instance_f32 * mptrTF, float32_t * vptrPVo, float32_t * vptrBo);
int LDA_MC_Classify(arm_matrix_instance_f32 * mptrRF, float32_t * vptrPV, float32_t * vptrB);
int LDA_MC_UD_Classify(arm_matrix_instance_f32 * mptrRF, float32_t * vptrPV, float32_t * vptrB);
void LDA_2C_MeanVector(arm_matrix_instance_f32 * mptrTF1, arm_matrix_instance_f32 * mptrTF2, float32_t * vptrMVo);
void LDA_2C_WithinScatterMatrix(arm_matrix_instance_f32 * mptrTF1, arm_matrix_instance_f32 * mptrTF2, float32_t * vptrMV, arm_matrix_instance_f32 * mptrWSMo);
void LDA_2C_ProjectionVector(arm_matrix_instance_f32 * mptrWSM, float32_t * vptrMV, float32_t * vptrPVo);
void LDA_2C_Train(arm_matrix_instance_f32 * mptrTF1, arm_matrix_instance_f32 * mptrTF2, float32_t * vptrPVo, float32_t * ptrBo);
int LDA_2C_Classify(float32_t * vptrSF, float32_t * vptrPV, float32_t * ptrB);
int LDA_Compare_Float(float f1, float f2);

#endif /* MYO_LDA_H_ */
