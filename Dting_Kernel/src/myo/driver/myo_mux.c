/*	@file		myo_rf.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 *
 * 	@brief		ISL84051IBZ driver.
 */
#include <myo_mux.h>

/****************** Global variable prototype ********************/
int CurrentTunnel;

/*********************** Private functions **********************/
/*	@brief	Initialization of multiplexer.
 * 			This function uses the global variable @CuttentTunnel.
 */
void MYO_MUX_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	MUX_MYO_GPIO_CLK_ENABLE();

	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Pin       = MUX_MYO_DATA0 | MUX_MYO_DATA1| MUX_MYO_DATA2;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	HAL_GPIO_Init(MUX_MYO_GPIO, &GPIO_InitStruct);

	/* select the first tunnel */
	CurrentTunnel = 1;
	MYO_MUX_SelectTunnel(CurrentTunnel);
}

/*	@brief	Tunnel selection function.
 * 	@param	Num		Tunnel number. (This number is counted from 1, not 0)
 */
void MYO_MUX_SelectTunnel(int Num)
{
	if(Num > 8)
		trace_printf("Wrong tunnel number !.\n");
	Num--;
	if(Num & 0b001)
		HAL_GPIO_WritePin(MUX_MYO_GPIO, MUX_MYO_DATA0, SET);
	else
		HAL_GPIO_WritePin(MUX_MYO_GPIO, MUX_MYO_DATA0, RESET);
	if(Num & 0b010)
		HAL_GPIO_WritePin(MUX_MYO_GPIO, MUX_MYO_DATA1, SET);
	else
		HAL_GPIO_WritePin(MUX_MYO_GPIO, MUX_MYO_DATA1, RESET);
	if(Num & 0b100)
		HAL_GPIO_WritePin(MUX_MYO_GPIO, MUX_MYO_DATA2, SET);
	else
		HAL_GPIO_WritePin(MUX_MYO_GPIO, MUX_MYO_DATA2, RESET);
}

/* @brief	Tunnel changing function.
 * 			Function accumulates the tunnel number.
 */
void MYO_MUX_ChangeTunnel(void)
{
	if(CurrentTunnel == 8)
		CurrentTunnel = 1;
	else
		CurrentTunnel++;
	MYO_MUX_SelectTunnel(CurrentTunnel); // 9us delay in total
}
/******************************** End of file *********************************/
