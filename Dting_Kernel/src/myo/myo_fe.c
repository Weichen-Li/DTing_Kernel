/* 	@file		myo_fe.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 * 	@brief		Feature extraction functions.
 */
#include <myo_fe.h>

/*********************** Private functions **********************/
float32_t FE_MeanAbsoluteValue(float32_t * vptrS)
{
	float32_t MAV = 0;
	int i;
	for(i=0; i<FE_SAMPLE_LENGTH; i++)
		MAV += fabs(*(vptrS + i));
	MAV = MAV / FE_SAMPLE_LENGTH;
	return MAV;
}

/* This function is not perfect.
 * There are some occasions that cause the wrong result.
 * fmaxf and fminf is dump and slow as well.
 * Think about it.
 */
float32_t FE_SlopSignChange(float32_t * vptrS)
{
	int i;
	float32_t Counter = 0.0;
	for(i=1; i<FE_SAMPLE_LENGTH; i++)
		if(FE_Compare_Float(fmaxf(*(vptrS + i),*(vptrS + i + 1)),fmaxf(*(vptrS + i),*(vptrS + i - 1))) || \
			FE_Compare_Float(fminf(*(vptrS + i),*(vptrS + i + 1)),fminf(*(vptrS + i),*(vptrS + i - 1))))
			if((fabs(*(vptrS + i + 1) - *(vptrS + i)) >= FE_THRESHOLD) || (fabs(*(vptrS + i - 1) - *(vptrS + i)) >= FE_THRESHOLD))
				Counter++;
	return Counter;
	//return 0;
}

float32_t FE_WaveformLength(float32_t * vptrS)
{
	float32_t Delta;
	float32_t Sum = 0;
	int i;
	for(i=1; i<FE_SAMPLE_LENGTH; i++)
	{
		Delta = fabs(*(vptrS + i) - *(vptrS + i - 1));
		Sum += Delta;
	}
	return Sum;
}

float32_t FE_ZeroCrossing(float32_t * vptrS)
{
	int i;
	float32_t counter = 0;
	for(i=0; i<FE_SAMPLE_LENGTH-1; i++)
		if(*(vptrS + i) == 0)
			if(fabs(*(vptrS + i + 1) - *(vptrS + i)) >= FE_THRESHOLD)
				counter++;
	for(i=0; i<FE_SAMPLE_LENGTH-1; i++)
		if((*(vptrS + i + 1) * (*(vptrS + i))) < 0)
			if(fabs(*(vptrS + i + 1) - *(vptrS + i)) >= FE_THRESHOLD)
				counter++;
	return counter;
}

void FE_FeatureExtract(float32_t * vptrS, float32_t * vptrFo)
{
	*vptrFo = FE_MeanAbsoluteValue(vptrS);
	*(vptrFo + 1) = FE_SlopSignChange(vptrS);
	*(vptrFo + 2) = FE_WaveformLength(vptrS);
	*(vptrFo + 3) = FE_ZeroCrossing(vptrS);
}

int FE_Compare_Float(float f1, float f2)
{
 float precision = 0.00001;
 if (((f1 - precision) < f2) &&
     ((f1 + precision) > f2))
  {
   return 1;
  }
 else
  {
   return 0;
  }
}

/* @brief			Extract and integrate features in a matrix.
 * 					Selected features are(in order):
 * 					1. Mean Absolute Value
 * 					2. Number of Slop Sign Change
 * 					3. Waveform Length
 * 					4. Number of Zero Crossing
 * @param *vptrTS	Training samples.
 * 					Dimension:	[FE_SAMPLE_LENGTH * FE_TRAINING_LENGTH, 1]
 * @param *mptrTFo	Extracted features.
 * 					Dimension:	[FE_FEATURE_NUMBER, FE_TRAINING_LENGTH]
 */
void FE_Training_FeatureIntegrate(float32_t * vptrTS, arm_matrix_instance_f32 * mptrTFo)
{
	int i,j;
	float32_t vTrainingFeature[FE_FEATURE_NUMBER];
	for(i=0; i<FE_TRAINING_LENGTH; i++)
	{
		FE_FeatureExtract(vptrTS + i*FE_SAMPLE_LENGTH, vTrainingFeature);
		for(j=0; j<FE_FEATURE_NUMBER; j++)
			*(mptrTFo->pData + j + i*FE_FEATURE_NUMBER) = vTrainingFeature[j];
	}
}

/* @brief			Extract and integrate features in a matrix.
 * @param *vptrTS	Recognizing samples.
 * 					Dimension:	[FE_SAMPLE_LENGTH, 1]
 * @param *mptrTFo	Extracted features.
 * 					Dimension:	[FE_FEATURE_NUMBER, 1]
 */
void FE_Recognizing_FeatureIntegrate(float32_t * vptrRS, arm_matrix_instance_f32 * mptrRFo)
{
	int j;
	float32_t vRecognizingFeature[FE_FEATURE_NUMBER];
	FE_FeatureExtract(vptrRS, vRecognizingFeature);
	for(j=0; j<FE_FEATURE_NUMBER; j++)
		*(mptrRFo->pData + j) = vRecognizingFeature[j];
}

// ----------------------------------------------------------------------------
