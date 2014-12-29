/* @file				MYO_lda.h
 * @auther				Weichen Li
 * @date
 * @brief				Global definitions.
 */
#ifndef _MYO_DEF_H
#define _MYO_DEF_H

#include <Timer.h>
#include "stm32f4xx_nucleo.h"
#include "stm32f4xx_hal_conf.h"
#include <arm_math.h>


/****************** variable prefix and suffix definition************************/
/* The prefix and suffix of variables are define in the myo algorithm
 * for the benefit of understanding and reusability.
 *
 * Define:
 * 			m-			Matrix
 * 			ptr-		Pointer
 *  		d-			Data
 *  		v-			Vector
 *			-o			Output
 *			-[none]		Input
 * Example:
 *  		arm_matrix_instance_f32 *		mptrScatterMatrix;
 *  		float32_t 						mdScatterMatrix;
 *
 * Notice:
 * 			There are non-prefix defined variables in the algorithm as well.
 * 			They could be any datatype define in standard C99 definition.
 * 			Please see the details in the specific function description.
 */

/********************* global MYO definition  ****************************/
/* Function switch */
//#define MYO_DATASIM
//#define MYO_READ_FLASH_DATA
//#define MYO_WRITE_FLASH_DATA
//#define MYO_UNDEFINED_CLASS

#define MYO_MCU_MAINFREQ			84000000//SystemCoreClock
#define MYO_SAMPLE_FREQUENCY		1000
#define MYO_SEGMENT_LENGTH_MS		30
#define MYO_SEGMENT_LENGTH			30
#define MYO_TRAINING_LENGTH			3
#define MYO_FEATURE_NUMBER			4
#define MYO_TUNNEL_NUMBER			8
#define MYO_GESTURE_NUMBER			3
//#define MYO_DATASIM

#define MYO_GESTURE_1				0x01
#define MYO_GESTURE_2				0x02
#define MYO_GESTURE_3				0x03
#define MYO_GESTURE_4				0x04

#endif
