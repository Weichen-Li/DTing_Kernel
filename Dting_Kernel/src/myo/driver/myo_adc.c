/*
 * myo_adc.c
 *
 *  Created on: Jul 16, 2014
 *      Author: li_we
 */

#include <myo_adc.h>
#include <myo_mux.h>

/****************** Global variable prototype ********************/
ADC_HandleTypeDef ADC_Handle;
DMA_HandleTypeDef DMA_Handle;
ADC_ChannelConfTypeDef ADCChannel_Handle;
__IO uint16_t ADCValueDMA;

/*********************** Private functions **********************/

/* @brief			Initialization function of ADC DMA driver.
 * 					This function uses the global variable @DMA_Handle.
 */
void MYO_ADC_DMA_Init(void)
{
	/* Enable DMA2 clock */
	ADC_MYO_DMA_CLK_ENABLE();
	DMA_Handle.Instance 					= ADC_MYO_DMA_STREAM;
	DMA_Handle.Init.Channel 				= ADC_MYO_DMA_CHANNEL;
	DMA_Handle.Init.Direction 				= DMA_PERIPH_TO_MEMORY;
	DMA_Handle.Init.PeriphInc 				= DMA_PINC_DISABLE;
	DMA_Handle.Init.MemInc 					= DMA_MINC_ENABLE;
	DMA_Handle.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_HALFWORD;
	DMA_Handle.Init.MemDataAlignment 		= DMA_MDATAALIGN_HALFWORD;
	DMA_Handle.Init.Mode 					= DMA_CIRCULAR;
	DMA_Handle.Init.Priority 				= DMA_PRIORITY_HIGH;
	DMA_Handle.Init.FIFOMode 				= DMA_FIFOMODE_DISABLE;
	DMA_Handle.Init.FIFOThreshold 			= DMA_FIFO_THRESHOLD_HALFFULL;
	DMA_Handle.Init.MemBurst 				= DMA_MBURST_SINGLE;
	DMA_Handle.Init.PeriphBurst 			= DMA_PBURST_SINGLE;
	if(HAL_DMA_Init(&DMA_Handle) != HAL_OK)
		trace_printf("DMA initialization Error.\n");

	/* Associate the initialized DMA handle to the the ADC handle */
	__HAL_LINKDMA(&ADC_Handle, DMA_Handle, DMA_Handle);

	/* NVIC configuration for DMA transfer complete interrupt */
	//HAL_NVIC_SetPriority(ADC_MYO_DMA_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(ADC_MYO_DMA_IRQn);
}

/* @brief			Initialization function of ADC driver.
 * 					This function uses the global variable @ADC_Handle.
 */
void MYO_ADC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	ADC_MYO_CLK_ENABLE();
	ADC_MYO_GPIO_CLK_ENABLE();

	/* Configure GPIO pin corresponds to ADC */
	GPIO_InitStruct.Pin 					= ADC_MYO_GPIO_PIN;
	GPIO_InitStruct.Mode 					= GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull 					= GPIO_PULLUP;
	GPIO_InitStruct.Speed 					= GPIO_SPEED_HIGH;
	HAL_GPIO_Init(ADC_MYO_GPIO, &GPIO_InitStruct);

	/* Configure ADC */
	ADC_Handle.Instance 					= ADC_MYO_INSTANCE;
	ADC_Handle.Init.ClockPrescaler 			= ADC_CLOCKPRESCALER_PCLK_DIV2;
	ADC_Handle.Init.Resolution 				= ADC_RESOLUTION12b;
	ADC_Handle.Init.ScanConvMode 			= DISABLE;
	ADC_Handle.Init.ContinuousConvMode 		= ENABLE;
	ADC_Handle.Init.DiscontinuousConvMode 	= DISABLE;
	ADC_Handle.Init.NbrOfDiscConversion 	= 0;
	ADC_Handle.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC_Handle.Init.ExternalTrigConv 		= ADC_EXTERNALTRIGCONV_T1_CC1;
	ADC_Handle.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	ADC_Handle.Init.NbrOfConversion 		= 1;
	ADC_Handle.Init.DMAContinuousRequests 	= ENABLE;
	ADC_Handle.Init.EOCSelection 			= DISABLE;
	if(HAL_ADC_Init(&ADC_Handle) != HAL_OK)
		trace_printf("ADC initialization Error.\n");

	/* Configure ADC channel */
	ADCChannel_Handle.Channel 				= ADC_MYO_CHANNEL;
	ADCChannel_Handle.Rank 					= 1;
	ADCChannel_Handle.SamplingTime 			= ADC_SAMPLETIME_3CYCLES;
	ADCChannel_Handle.Offset 				= 0;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADCChannel_Handle) != HAL_OK)
		trace_printf("ADC configuration Error.\n");
}

/* @brief:	Function interpret the ADC register value to the voltage value
 * 			and shift it down half ADV_REF_VOLTAGE, 1.5V for the fixed
 * 			internal reference voltage.
 * 			The output ranges from -1.5 to 1.5.
 * @param	*RS		Raw sample ready to be interpreted.
 * @param	L		The length of sample.
 * @param	*So		Interpreted sample.
 */
void MYO_RawSampleInterpret(uint16_t * RS, uint16_t L, arm_matrix_instance_f32 * So)
{
	int i;
	float * ptrVoltage;
	ptrVoltage = So->pData;
	for(i=0;i<L;i++)
	{
		*(ptrVoltage) = *RS * ADC_LSB;
		*(ptrVoltage) -= (ADC_REF_VOLTAGE / 2.0);
		ptrVoltage++;
		RS++;
	}
}

/* @brief:	Function convert the analog voltage in a fixed period.
 * 			It is for generating the training data for one class.
 * 			This function should be invoked n times for n-class problem.
 * @param	*So		Converted sample.
 */
void MYO_ADC_TrainingConvert(arm_matrix_instance_f32 * So)
{
	int i,j;
	uint16_t RawSample[ADC_SAMPLE_LENGTH * ADC_SAMPLE_TIMES * ADC_TUNNEL_NUMBER];

	/* Enable ADC DMA request */
	if(HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADCValueDMA, 1) != HAL_OK)
		trace_printf("ADC conversion Error (training).\n");

	/* Tunnel changing and sampling */
	for(i=0;i<ADC_TUNNEL_NUMBER;i++)
	{
	for(j=0;j<(ADC_SAMPLE_LENGTH* ADC_SAMPLE_TIMES);j++)
	{
		RawSample[i*ADC_SAMPLE_LENGTH* ADC_SAMPLE_TIMES +j] = ADCValueDMA;
			//MYO_MUX_ChangeTunnel(); // Tunnel Changing;
			//timer_sleep(100); //Delay for changing the tunnel;
		timer_sleep(1000); // minus time compensation when multi-tunnel
		//timer_sleep(ADC_SAMPLE_DELAY_US - MUX_MYO_OFFON_DELAY -100);
	}
		MYO_MUX_ChangeTunnel();
		timer_sleep(100000); //Delay for changing the tunnel. This value shall be changed if Tunnel-changing problem has been fixed.
	}
	MYO_RawSampleInterpret(RawSample,ADC_SAMPLE_LENGTH * ADC_SAMPLE_TIMES * ADC_TUNNEL_NUMBER, So);

	/* Disable ADC DMA request */
	if(HAL_ADC_Stop_DMA(&ADC_Handle) != HAL_OK)
		trace_printf("ADC termination Error (training).\n");
}

/* @brief:	Function convert the analog voltage in a fixed period.
 * 			It is for generating the recognizing data.
 * 			This function should be invoked in the main routine.
 * @param	*So		converted sample.
 */
void MYO_ADC_RecogConvert(arm_matrix_instance_f32 * So)
{
	int i,j;
	uint16_t RawSample[ADC_SAMPLE_LENGTH * ADC_TUNNEL_NUMBER];

	/* Enable ADC DMA request */
	if(HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t*)&ADCValueDMA, 1) != HAL_OK)
		trace_printf("ADC conversion Error (recognizing).\n");

	/* Tunnel changing and sampling */
	for(i=0;i<ADC_TUNNEL_NUMBER;i++)
	{
		for(j=0;j<ADC_SAMPLE_LENGTH;j++)
		{
			RawSample[i*ADC_SAMPLE_LENGTH+j] = ADCValueDMA;
			timer_sleep(1000);
		}
		MYO_MUX_ChangeTunnel();
		timer_sleep(100000); //Delay for changing the tunnel. This value shall be changed if Tunnel-changing problem has been fixed.
	}
	MYO_RawSampleInterpret(RawSample,ADC_SAMPLE_LENGTH * ADC_TUNNEL_NUMBER, So);

	/* Disable ADC DMA request */
	if(HAL_ADC_Stop_DMA(&ADC_Handle) != HAL_OK)
		trace_printf("ADC termination Error (recognizing).\n");
}

