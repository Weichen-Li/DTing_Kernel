/*	@file		myo_dp.h
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 *
 * 	@brief		AD5142BRUZ100 driver header file.
 */
#ifndef MYO_DP_H_
#define MYO_DP_H_

#include <stdio.h>
#include "diag/Trace.h"

#include <Timer.h>
#include "stm32f4xx_hal_conf.h"
#include <MYO_def.h>

/****************** Hardware fixed definition ***********************/
/* Hardware fixed parameters */
#define DP_V_MID							3.3 / 2
#define DP_V_AUX							3.3
#define DP_SETTLE_TIME_US					2
#define DP_R_W								200 //typical 130
#define DP_R_AB								100000
#define DP_RADC_DATA(_R_AW_)				(uint8_t)(255-(_R_AW_-DP_R_W)*256/DP_R_AB)

/* reduced commands operation */
#define DP_WRITE_RADC						0x1000
#define DP_WRITE_INPUT_REGISTER				0x2000
#define DP_READ_EEPROM						0x3001
#define DP_READ_RDAC						0x3003
#define DP_COPY_RDAC_EEPROM					0x7001
#define DP_COPY_EEPROM_RDAC					0x7000
#define DP_RESET							0xB000
#define DP_SHUTDOWN_NORMAL					0xC000
#define DP_SHUTDOWN_DOWN					0xC001

/* reduced address bits */
#define DP_ADDR_ALL_CHANNELS				0x0800
#define DP_ADDR_RDAC1						0x0000
#define DP_ADDR_RDAC2						0x0100
#define DP_ADDR_RDAC1_READ					0x0000 // use this address when reading
#define DP_ADDR_RDAC2_READ					0x0200 // use this address when reading

/* general macro */
#define DP_DATA_DONTCARE					0x0000
#define DP_COMMAND_LENGTH					2
#define DP_COMMAND(_COMMAND_, _ADD_, _DATA_)			(uint16_t)(_COMMAND_ | _ADD_ | _DATA_)

/******************* Local definition *******************************/
#define DP_MYO_SPI_SCK						GPIO_PIN_13
#define DP_MYO_SPI_MISO						GPIO_PIN_14
#define DP_MYO_SPI_MOSI						GPIO_PIN_15
#define DP_MYO_SPI_NSS						GPIO_PIN_12
#define DP_MYO_SPI_SYNC						GPIO_PIN_2
#define DP_MYO_SPI_GPIO						GPIOB
#define DP_MYO_GPIO_CLK_ENABLE()			__GPIOB_CLK_ENABLE()
#define DP_MYO_SPI_CLK_ENABLE()				__SPI2_CLK_ENABLE()
#define DP_MYO_SPI_INSTANCE					SPI2
#define DP_MYO_SPI_TIMEOUT					0x1000

/******************** Functions prototype ***************************/
void MYO_DP_Init(void);
void MYO_DP_DeInit(void);
void MYO_DP_Command(uint16_t C);
uint16_t MYO_EndianConvert(uint16_t Data);

#endif /* MYO_DP_H_ */
