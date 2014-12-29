/* 	@file		myo_lda.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 * 	@brief		LDA implementation functions.
 */
#include <myo_lda.h>

/**************************************** LDA multiclass functions ****************************************/
void LDA_MeanVector(arm_matrix_instance_f32 * mptrTF, float32_t * vptrMVo)
{
	int i,j;

	/* variable and data set definition */
	float32_t * vptrTemp;
	float32_t vTemp[LDA_SAMPLE_DIMENSION];
	float32_t vMTemp[LDA_SAMPLE_DIMENSION] = {0};

	/* mean vector 1 calculation */
	vptrTemp = mptrTF->pData;
	for(j=0;j<MYO_TUNNEL_NUMBER;j++)
	{
		for(i=0;i<LDA_SAMPLE_NUMBER;i++)
		{
		arm_add_f32 (&vMTemp[j*MYO_FEATURE_NUMBER], vptrTemp, &vMTemp[j*MYO_FEATURE_NUMBER], MYO_FEATURE_NUMBER);
		//arm_add_f32 (vptrMVo, vptrTemp, vptrMVo, LDA_SAMPLE_DIMENSION);
		vptrTemp = vptrTemp + MYO_FEATURE_NUMBER;
		}
	}
	arm_copy_f32(vMTemp, vptrMVo,LDA_SAMPLE_DIMENSION);
	arm_fill_f32(1.0/LDA_SAMPLE_NUMBER, vTemp, LDA_SAMPLE_DIMENSION);
	arm_mult_f32(vptrMVo, vTemp, vptrMVo,  LDA_SAMPLE_DIMENSION);
}

void LDA_ScatterMatrix(arm_matrix_instance_f32 * mptrTF, float32_t * vptrMV, arm_matrix_instance_f32 * mptrSMo)
{
	int i,j;

	/* variable and data set definition */
	float32_t * vptrTemp;
	float32_t mdDelta[LDA_SAMPLE_DIMENSION];
	float32_t mdDeltaTranspose[LDA_SAMPLE_DIMENSION];
	float32_t mdSum[LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION];
	float32_t mdScatterMatrix[LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION];

	/* matrix definition */
	arm_matrix_instance_f32 mDelta;
	arm_matrix_instance_f32 mDeltaTranspose;
	arm_matrix_instance_f32 mSum;
	arm_matrix_instance_f32 mScatterMatrix;

	/* matrix initialization */
	arm_mat_init_f32(&mDelta, LDA_SAMPLE_DIMENSION, 1, mdDelta);
	arm_mat_init_f32(&mDeltaTranspose, 1, LDA_SAMPLE_DIMENSION, mdDeltaTranspose);
	arm_mat_init_f32(&mSum, LDA_SAMPLE_DIMENSION, LDA_SAMPLE_DIMENSION, mdSum);
	arm_mat_init_f32(&mScatterMatrix, LDA_SAMPLE_DIMENSION, LDA_SAMPLE_DIMENSION, mdScatterMatrix);
	arm_fill_f32(0, mdScatterMatrix, LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION);
	arm_fill_f32(0, mdSum, LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION);

	vptrTemp = mptrTF->pData;
	/* scatter matrix 1 calculation */
	for(i=0;i<LDA_SAMPLE_NUMBER;i++)
	{
		vptrTemp = mptrTF->pData;
		vptrTemp += MYO_FEATURE_NUMBER*i;

		for(j=0;j<MYO_TUNNEL_NUMBER;j++)
		{
			arm_sub_f32(vptrTemp, vptrMV, &mdDelta[j*MYO_FEATURE_NUMBER], MYO_FEATURE_NUMBER);
			vptrTemp = vptrTemp + LDA_SAMPLE_NUMBER * MYO_FEATURE_NUMBER;
			vptrMV = vptrMV + MYO_FEATURE_NUMBER;
		}
		//arm_sub_f32(vptrTemp, vptrMV, mdDelta, LDA_SAMPLE_DIMENSION);
		arm_mat_trans_f32(&mDelta, &mDeltaTranspose);
		arm_mat_mult_f32(&mDelta, &mDeltaTranspose, &mSum);
		arm_mat_add_f32(&mScatterMatrix, &mSum, &mScatterMatrix);
		vptrMV = vptrMV - MYO_FEATURE_NUMBER*MYO_TUNNEL_NUMBER;
	}
	arm_copy_f32(mdScatterMatrix, mptrSMo->pData, LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION);
}

void LDA_WithinScatterMatrix(arm_matrix_instance_f32 * mptrTF, float32_t * vptrMV, arm_matrix_instance_f32 * mptrWSMo)
{
	int i,j;
	float32_t mdScatterMatrix[LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION * LDA_CLASS_NUMBER];
	arm_matrix_instance_f32 mScatterMatrix[LDA_CLASS_NUMBER];

	for(i=0;i<LDA_CLASS_NUMBER;i++)
	{
		arm_mat_init_f32 (&mScatterMatrix[i], LDA_SAMPLE_DIMENSION, LDA_SAMPLE_DIMENSION, &mdScatterMatrix[i* LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION]);
		LDA_ScatterMatrix(mptrTF, vptrMV, &mScatterMatrix[i]);
		vptrMV = vptrMV + LDA_SAMPLE_DIMENSION;
		mptrTF = mptrTF + MYO_TUNNEL_NUMBER;
	}
	for(i=0;i<LDA_CLASS_NUMBER-1;i++)
	{
		for(j=i+1;j<LDA_CLASS_NUMBER;j++)
		{
			arm_mat_add_f32(&mScatterMatrix[i], &mScatterMatrix[j], mptrWSMo);
			mptrWSMo++;
		}
	}
}

void LDA_ProjectionVector(arm_matrix_instance_f32 * mptrWSM, float32_t * vptrMV, float32_t * vptrPVo)
{
	int i,j,num;
	/* variable and data set definition */
	float32_t mdProjectionVector[LDA_SAMPLE_DIMENSION];
	float32_t mdDelta[LDA_SAMPLE_DIMENSION * LDA_COMBO_NUMBER];
	float32_t mdWSMinverse[LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION];
	float32_t mdWSMCopy[LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION];

	/* matrix definition */
	arm_matrix_instance_f32 mProjectionVector;
	arm_matrix_instance_f32 mDelta[LDA_COMBO_NUMBER];
	arm_matrix_instance_f32 mWSMinverse;
	arm_matrix_instance_f32 mWSMCopy;

	num = 0;
	/* matrix initialization */
	arm_mat_init_f32(&mProjectionVector, LDA_SAMPLE_DIMENSION, 1, mdProjectionVector);
	for(i=0;i<LDA_COMBO_NUMBER;i++)
		arm_mat_init_f32(&mDelta[i], LDA_SAMPLE_DIMENSION, 1, &mdDelta[i*LDA_SAMPLE_DIMENSION]);
	arm_mat_init_f32(&mWSMinverse, LDA_SAMPLE_DIMENSION, LDA_SAMPLE_DIMENSION, mdWSMinverse);
	arm_mat_init_f32(&mWSMCopy, LDA_SAMPLE_DIMENSION, LDA_SAMPLE_DIMENSION, mdWSMCopy);

	for(i=0;i<LDA_CLASS_NUMBER-1;i++)
	{
		for(j=i+1;j<LDA_CLASS_NUMBER;j++)
		{
			arm_sub_f32(&vptrMV[i*LDA_SAMPLE_DIMENSION], &vptrMV[j*LDA_SAMPLE_DIMENSION], &mdDelta[num * LDA_SAMPLE_DIMENSION], LDA_SAMPLE_DIMENSION);
			num++;
		}
	}
	for(i=0;i<LDA_COMBO_NUMBER;i++)
	{
		arm_copy_f32 (mptrWSM->pData, mdWSMCopy, LDA_SAMPLE_DIMENSION*LDA_SAMPLE_DIMENSION);
		/* projection vector calculation */
		arm_mat_inverse_f32(&mWSMCopy, &mWSMinverse);
		arm_mat_mult_f32(&mWSMinverse, &mDelta[i], &mProjectionVector);
		arm_copy_f32(mdProjectionVector, &vptrPVo[i*LDA_SAMPLE_DIMENSION], LDA_SAMPLE_DIMENSION);
		mptrWSM++;
	}
}

/* @brief				Train the LDA model by amount of training feature.
 * 						Implement Fisher's Linear Discriminant formula.
 * @param *mptrTF		Training feature for classes.
 * 						Dimension:	[LDA_SAMPLE_DIMENSION, LDA_SAMPLE_NUMBER*LDA_CLASS_NUMBER]
 * @param *vptrPVo		Optimized projection vector.
 * 						Dimension:	[1, LDA_SAMPLE_DIMENSION]]
 * @param *vptrBo		Decision boundary.
 */
void LDA_MC_Train(arm_matrix_instance_f32 * mptrTF, float32_t * vptrPVo, float32_t * vptrBo)
{

	float32_t vMeanVector[LDA_CLASS_NUMBER * LDA_SAMPLE_DIMENSION] = {0};
	int i,j,num = 0;
	float32_t mdWithinScatterMatrix[LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION * LDA_COMBO_NUMBER];
	float32_t Projection[2];
/*#ifdef LDA_UNDEFINED_CLASS
	float32_t Boundary;
	//float32_t SampleProj[LDA_SAMPLE_NUMBER];
#endif
*/
	arm_matrix_instance_f32 mWithinScatterMatrix[LDA_COMBO_NUMBER];

	for(i=0;i<LDA_COMBO_NUMBER;i++)
	{
		arm_mat_init_f32 (&mWithinScatterMatrix[i], LDA_SAMPLE_DIMENSION, LDA_SAMPLE_DIMENSION, &mdWithinScatterMatrix[i* LDA_SAMPLE_DIMENSION * LDA_SAMPLE_DIMENSION]);
	}

	for(i=0;i<LDA_CLASS_NUMBER;i++)
	{
		LDA_MeanVector(mptrTF, &vMeanVector[i*LDA_SAMPLE_DIMENSION]);
		//mptrTF++;
		mptrTF = mptrTF + MYO_TUNNEL_NUMBER;
	}
	//mptrTF -= LDA_CLASS_NUMBER;
	mptrTF = mptrTF - LDA_CLASS_NUMBER * MYO_TUNNEL_NUMBER;
	LDA_WithinScatterMatrix(mptrTF, vMeanVector, mWithinScatterMatrix);
	LDA_ProjectionVector(&mWithinScatterMatrix[0], vMeanVector, vptrPVo);
	for(i=0;i<LDA_CLASS_NUMBER-1;i++)
	{
		for(j=i+1;j<LDA_CLASS_NUMBER;j++)
		{
			arm_dot_prod_f32(vptrPVo, &vMeanVector[i * LDA_SAMPLE_DIMENSION], LDA_SAMPLE_DIMENSION, &Projection[0]);
			arm_dot_prod_f32(vptrPVo, &vMeanVector[j * LDA_SAMPLE_DIMENSION], LDA_SAMPLE_DIMENSION, &Projection[1]);
/*#ifdef LDA_UNDEFINED_CLASS
			Boundary = (float32_t)0.5 * (Projection[0] + Projection[1]);
			* vptrBo = (float32_t)0.5 * (Projection[0] + Boundary);
			vptrBo++;
			* vptrBo = (float32_t)0.5 * (Projection[0] - Boundary) + Projection[0];
			vptrBo++;
			* vptrBo = (float32_t)0.5 * (Projection[1] + Boundary);
			vptrBo++;
			* vptrBo = (float32_t)0.5 * (Boundary - Projection[1]) + Projection[1];
			vptrBo++;
			num++;
#else*/
			* vptrBo = (float32_t)0.5 * (Projection[0] + Projection[1]);
			vptrPVo += LDA_SAMPLE_DIMENSION;
			num++;
			vptrBo++;
		}
	}
}

/* @brief				Classify the sample feature.
 * 						Only being invoked after training.
 * @param *mptrRF		Sample feature vector
 * 						Dimension:	[LDA_SAMPLE_DIMENSION, 1]
 * @param *vptrPV		Projection vector.
 * 						Dimension:	[1, LDA_SAMPLE_DIMENSION]
 * @param *ptrB			Decision boundary.
 */
int LDA_MC_Classify(arm_matrix_instance_f32 * mptrRF, float32_t * vptrPV, float32_t * vptrB)
{
	float32_t vProjection[LDA_COMBO_NUMBER];
	float32_t vVote[LDA_CLASS_NUMBER]={0};
	uint32_t Index;
	float32_t Votes;
	int i,j,num=0;
	for(i=0;i<LDA_COMBO_NUMBER;i++)
	{
		arm_dot_prod_f32(mptrRF->pData, &vptrPV[i*LDA_SAMPLE_DIMENSION], LDA_SAMPLE_DIMENSION, &vProjection[i]);
	}
	for(i=0;i<LDA_CLASS_NUMBER-1;i++)
		for(j=i+1;j<LDA_CLASS_NUMBER;j++)
		{
			if(vProjection[num] > (*vptrB))
				vVote[i]++;
			else vVote[j]++;
			num++;
			vptrB++;
		}
	arm_max_f32 (vVote, LDA_CLASS_NUMBER, &Votes, &Index);
	return Index+1;
}

int LDA_MC_UD_Classify(arm_matrix_instance_f32 * mptrRF, float32_t * vptrPV, float32_t * vptrB)
{
	float32_t vProjection[LDA_COMBO_NUMBER];
	float32_t vVote[LDA_CLASS_NUMBER]={0};
	uint32_t Index;
	float32_t Votes;
	int i,j,num=0;
	for(i=0;i<LDA_COMBO_NUMBER;i++)
	{
		arm_dot_prod_f32(mptrRF->pData, &vptrPV[i*LDA_SAMPLE_DIMENSION], LDA_SAMPLE_DIMENSION, &vProjection[i]);
	}
	for(i=0;i<LDA_CLASS_NUMBER-1;i++)
		for(j=i+1;j<LDA_CLASS_NUMBER;j++)
		{
			if(vProjection[num] > (*vptrB))
			{
				vptrB++;
				if(vProjection[num] < (*vptrB))
				{
					vptrB++;
					vVote[i]++;
				}
				else
					vptrB++;
			}
			else
			{
				vptrB++;
				vptrB++;
				if(vProjection[num] < (*vptrB))
				{
					vptrB++;
					if(vProjection[num] > (*vptrB))
					{
						vptrB++;
						vVote[j]++;
					}
				}
			}
			num++;
		}
	arm_max_f32 (vVote, LDA_CLASS_NUMBER, &Votes, &Index);
	if((Index+1) == LDA_CLASS_NUMBER - 1)
		return Index+1;
	else
		return 0;
}

int LDA_Compare_Float(float f1, float f2)
{
 float precision = 0.000000001;
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

// ----------------------------------------------------------------------------
