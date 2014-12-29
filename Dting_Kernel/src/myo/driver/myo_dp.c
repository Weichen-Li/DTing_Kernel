/*	@file		myo_dp.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 *
 * 	@brief		AD5142BRUZ100 driver.
 */

#include <myo_dp.h>
#include <Timer.h>

/****************** Global variable prototype ********************/
SPI_HandleTypeDef SPI_Handle;

/*********************** Private functions **********************/

/*	@brief		Initialization function of Digital potentiometer.
 * 				This function uses the global variable @SPI_Handle.
 */
void MYO_DP_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStruct;
	DP_MYO_GPIO_CLK_ENABLE();
	DP_MYO_SPI_CLK_ENABLE();

	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Pin       = DP_MYO_SPI_SCK;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(DP_MYO_SPI_GPIO, &GPIO_InitStruct);

	/* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.Pin       = DP_MYO_SPI_MISO;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(DP_MYO_SPI_GPIO, &GPIO_InitStruct);

	/* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.Pin       = DP_MYO_SPI_MOSI;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(DP_MYO_SPI_GPIO, &GPIO_InitStruct);

	/* SPI NSS GPIO pin configuration */
	GPIO_InitStruct.Pin       = DP_MYO_SPI_NSS;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(DP_MYO_SPI_GPIO, &GPIO_InitStruct);

	/* SPI SYNC GPIO pin configuration */
	GPIO_InitStruct.Pin       = DP_MYO_SPI_SYNC;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(DP_MYO_SPI_GPIO, &GPIO_InitStruct);

	/* Set the SPI parameters */
	SPI_Handle.Instance               	= DP_MYO_SPI_INSTANCE;
	SPI_Handle.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_256;
	SPI_Handle.Init.Direction         	= SPI_DIRECTION_2LINES;
	SPI_Handle.Init.CLKPhase          	= SPI_PHASE_1EDGE;
	SPI_Handle.Init.CLKPolarity       	= SPI_POLARITY_HIGH;
	SPI_Handle.Init.CRCCalculation    	= SPI_CRCCALCULATION_DISABLED;
	SPI_Handle.Init.CRCPolynomial     	= 7;
	SPI_Handle.Init.DataSize          	= SPI_DATASIZE_8BIT;
	SPI_Handle.Init.FirstBit          	= SPI_FIRSTBIT_MSB;
	SPI_Handle.Init.NSS               	= SPI_NSS_SOFT;
	SPI_Handle.Init.TIMode            	= SPI_TIMODE_DISABLED;
	SPI_Handle.Init.Mode 				= SPI_MODE_MASTER;
	if(HAL_SPI_Init(&SPI_Handle) != HAL_OK)
	{
		trace_printf("SPI initialization error !.\n");
	}
	HAL_GPIO_WritePin(DP_MYO_SPI_GPIO, DP_MYO_SPI_SYNC, 1);
}

/*	@brief		De-Initialization function of Digital potentiometer.
 * 				This function uses the global variable @SPI_Handle.
 */
void MYO_DP_DeInit(void)
{
	HAL_SPI_DeInit(&SPI_Handle);
}

/*	@brief		Endian convert for command transmission.
 * 	@param		Data	The data ready to be converted.
 *
 */
uint16_t MYO_EndianConvert(uint16_t Data)
{
	uint16_t CD;
	CD = (Data << 8)| (Data >> 8);
	return CD;
}

/* @brief:	Function gives the command to digital potentiometer.
 * 			Command should be selected from the definition in
 * 			header file.
 * 			Please use the macro DP_COMMAND(_COMMAND_, _ADD_, _DATA_).
 */
void MYO_DP_Command(uint16_t C)
{
	uint16_t Command;
	Command = MYO_EndianConvert(C);
	HAL_GPIO_WritePin(DP_MYO_SPI_GPIO, DP_MYO_SPI_SYNC, 0);
	if(HAL_SPI_Transmit(&SPI_Handle, (uint8_t *)&Command, DP_COMMAND_LENGTH, DP_MYO_SPI_TIMEOUT) != HAL_OK)
	{
		trace_printf("SPI transmission error !.\n");
	}
	HAL_GPIO_WritePin(DP_MYO_SPI_GPIO, DP_MYO_SPI_SYNC, 1);

	/* Check if the SPI is ready again */
	while (HAL_SPI_GetState(&SPI_Handle) != HAL_SPI_STATE_READY)
	{
	}
}
/******************************** End of file *********************************/
