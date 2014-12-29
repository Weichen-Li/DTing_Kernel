/*
 * myo_fla.h
 *
 *  Created on: Jul 16, 2014
 *      Author: li_we
 */

#ifndef MYO_FLA_H_
#define MYO_FLA_H_

#include <stdio.h>
#include "diag/Trace.h"

#include <Timer.h>
#include "stm32f4xx_hal_conf.h"
#include <MYO_def.h>

/****************** Hardware fixed definition ***********************/
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     			((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     			((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     			((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     			((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     			((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     			((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     			((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     			((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */

/******************* Local definition *******************************/
#define FLASH_MYO_SECTOR					FLASH_SECTOR_6
#define FLASH_MYO_START_ADDR				ADDR_FLASH_SECTOR_6
#define FLASH_MYO_END_ADDR					ADDR_FLASH_SECTOR_7

/******************** Functions prototype ***************************/
void MYO_FLASH_Write(uint32_t * ptrData, uint32_t Length);
void MYO_FLASH_Read(uint32_t * ptrData, uint32_t Length);

#endif /* MYO_FLA_H_ */
