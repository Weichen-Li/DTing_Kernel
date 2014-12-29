/*
 * myo_adc.h
 *
 *  Created on: Jul 16, 2014
 *      Author: li_we
 */

#ifndef MYO_ADC_H_
#define MYO_ADC_H_

#include <stdio.h>
#include "diag/Trace.h"

#include <Timer.h>
#include "stm32f4xx_hal_conf.h"
#include <MYO_def.h>

/****************** Global definition loading **********************/
#define ADC_CONVERSION_TIME_US			(12 + 3) / (MYO_MCU_MAINFREQ / 1000000) // See the datasheet
#define ADC_SAMPLE_DELAY_US				(1000000 / MYO_SAMPLE_FREQUENCY)
#define ADC_TUNNEL_NUMBER				MYO_TUNNEL_NUMBER
#define ADC_SAMPLE_LENGTH				MYO_SEGMENT_LENGTH
#define ADC_SAMPLE_TIMES				MYO_TRAINING_LENGTH

/****************** Hardware fixed definition ***********************/
#define ADC_MAX_NUM						4096
#define ADC_REF_VOLTAGE					3.3
#define ADC_LSB							(ADC_REF_VOLTAGE / ADC_MAX_NUM)

/******************* Local definition *******************************/
#define ADC_MYO_GPIO					GPIOA
#define ADC_MYO_GPIO_PIN				GPIO_PIN_0
#define ADC_MYO_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE()
#define ADC_MYO_INSTANCE				ADC1
#define ADC_MYO_CHANNEL					ADC_CHANNEL_0
#define ADC_MYO_CLK_ENABLE()			__ADC1_CLK_ENABLE()
#define ADC_MYO_DMA_CLK_ENABLE()		__DMA2_CLK_ENABLE()
#define ADC_MYO_DMA_STREAM				DMA2_Stream0
#define ADC_MYO_DMA_CHANNEL				DMA_CHANNEL_0
#define ADC_MYO_DMA_IRQn				DMA2_Stream0_IRQn

/******************** Functions prototype ***************************/
void MYO_ADC_DMA_Init(void);
void MYO_ADC_Init(void);
void MYO_RawSampleInterpret(uint16_t * RS, uint16_t L, arm_matrix_instance_f32 * So);
void MYO_ADC_TrainingConvert(arm_matrix_instance_f32 * So);
void MYO_ADC_RecogConvert(arm_matrix_instance_f32 * So);

#endif /* MYO_ADC_H_ */
