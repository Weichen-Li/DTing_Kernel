/* 	@file		myo_datasim.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 * 	@brief		Sinwave generation benchmark.
 */
#include <myo_datasim.h>

float32_t Datasim_freq[DATASIM_CLASS_NUMBER] = {50, 100, 200};
float32_t Datasim_mag[DATASIM_CLASS_NUMBER] = {1, 1.2, 1.5};

void MYO_SinWaveSample(int Freq, float32_t Mag, float32_t * So)
{
	int i;
	float32_t AU;
	float32_t Angle;
	AU = (Freq / 1000.0) * 2 * 3.1415926;
	for(i=0; i<DATASIM_SIM_LENGTH; i++)
	{
		Angle = i * AU;
		*So = Mag * arm_sin_f32(Angle);
		So++;
	}
}

float MYO_Function_Test(void)
{
	float a;
	a = arm_sin_f32(3.1415927/2.0);
	a = arm_sin_f32(3.1415927);
	a = arm_sin_f32(0);
	a = arm_sin_f32(1);
	a = arm_sin_f32(360);
	return a;
}
