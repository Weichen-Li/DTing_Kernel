/*	@file		myo_rf.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 *
 * 	@brief		ISL84051IBZ driver header file.
 */
#ifndef MYO_MUX_H_
#define MYO_MUX_H_

#include <stdio.h>
#include "diag/Trace.h"

#include <Timer.h>
#include "stm32f4xx_hal_conf.h"
#include <MYO_def.h>

/******************* Local definition *******************************/
#define MUX_MYO_GPIO						GPIOC
#define MUX_MYO_DATA0						GPIO_PIN_5
#define MUX_MYO_DATA1						GPIO_PIN_6
#define MUX_MYO_DATA2						GPIO_PIN_8
#define MUX_MYO_GPIO_CLK_ENABLE()			__GPIOC_CLK_ENABLE()
#define MUX_MYO_OFFON_DELAY					1 //us, typical off-on delay is 180 + 100 ns

/******************** Functions prototype ***************************/
void MYO_MUX_Init(void);
void MYO_MUX_SelectTunnel(int Num);
void MYO_MUX_ChangeTunnel(void);

#endif /* MYO_MUX_H_ */
