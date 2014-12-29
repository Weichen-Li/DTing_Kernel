/*	@file		myo_fla.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 *
 * 	@brief		stmf4xx flash driver.
 */

#include <myo_fla.h>

/*********************** Private functions **********************/

/* @brief			Write data in a fixed region in flash
 * @param *ptrData	Source data pointer.
 * @param Size		the size of data to write.
 * @note			The fixed sector will be erased before writing.
 * 					Please change the writing address in header file.
 */
void MYO_FLASH_Write(uint32_t * ptrData, uint32_t Size)
{
	uint32_t Addr, MemVerify;
	uint32_t MemData;
	uint32_t SectorError;
	FLASH_EraseInitTypeDef EraseInitStruct;

	HAL_FLASH_Unlock();
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase 				= TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange 			= VOLTAGE_RANGE_3;
	EraseInitStruct.Sector 					= FLASH_MYO_SECTOR;
	EraseInitStruct.NbSectors 				= 1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
		trace_printf("Flash erasing error.\n");
	Addr = FLASH_MYO_START_ADDR;
	if(Size > (FLASH_MYO_END_ADDR - FLASH_MYO_START_ADDR))
		trace_printf("Flash overflow error.\n");
	while(Addr < (FLASH_MYO_START_ADDR + Size))
	{
		if(HAL_FLASH_Program(TYPEPROGRAM_WORD, Addr, *ptrData) == HAL_OK)
		{
			Addr = Addr + 4;
			ptrData++;
		}
		else trace_printf("Flash writing error.\n");
	}
	HAL_FLASH_Lock();

	MemVerify = 0;
	Addr = FLASH_MYO_START_ADDR;
	ptrData = ptrData - Size / 4;
	while(Addr < (FLASH_MYO_START_ADDR + Size))
	{
		MemData = *(uint32_t*)Addr;
		if(MemData != *ptrData)
			MemVerify++;
		Addr = Addr + 4;
		ptrData++;
	}
	if(MemVerify != 0)
		trace_printf("Flash verification error.\n");
}

/* @brief			Read data in a fixed region in flash
 * @param *ptrData	Source data pointer.
 * @param Size		the size of data to read.
 * @note			Please change the reading address in header file.
 */
void MYO_FLASH_Read(uint32_t * ptrData, uint32_t Size)
{
	uint32_t Addr;
	Addr = FLASH_MYO_START_ADDR;
	while(Addr < (FLASH_MYO_START_ADDR + Size))
	{
		*ptrData = *(uint32_t*)Addr;
		Addr = Addr + 4;
		ptrData++;
	}
}
/******************************** End of file *********************************/
