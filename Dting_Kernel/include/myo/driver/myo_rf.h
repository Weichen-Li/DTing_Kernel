/*	@file		myo_rf.h
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 *
 * 	@brief		nRF24L01 driver header file.
 */
#ifndef _NFR24L01P_H_
#define _NFR24L01P_H_

#include <stm32f4xx_hal_conf.h>
#include <Timer.h>
#include <stdio.h>
#include "diag/Trace.h"

/****************** Hardware fixed definition ***********************/
//Define RF power value
#define P0dBm   0
#define Pm6dBm  1
#define Pm12dBm 2
#define Pm18dBm 3

//#define RF rate
#define R2mbps   0
#define R1mbps   1
#define R250kbps 3

//Define the commands for operate the nRF24L01P
#define READ_nRF_REG    0x00  // Command for read register
#define WRITE_nRF_REG   0x20 	// Command for write register
#define RD_RX_PLOAD     0x61  // Command for read Rx payload
#define WR_TX_PLOAD     0xA0  // Command for write Tx payload
#define FLUSH_TX        0xE1 	// Command for flush Tx FIFO
#define FLUSH_RX        0xE2  // Command for flush Rx FIFO
#define REUSE_TX_PL     0xE3  // Command for reuse Tx payload
#define NOP             0xFF  // Reserve

//Define the register address for nRF24L01P
#define CONFIG          0x00  //  Configurate the status of transceiver, mode of CRC and the replay of transceiver status
#define EN_AA           0x01  //  Enable the atuo-ack in all channels
#define EN_RXADDR       0x02  //  Enable Rx Address
#define SETUP_AW        0x03  // Configurate the address width
#define SETUP_RETR      0x04  //  setup the retransmit
#define RF_CH           0x05  // Configurate the RF frequency
#define RF_SETUP        0x06  // Setup the rate of data, and transmit power
#define NRFRegSTATUS    0x07  //
#define OBSERVE_TX      0x08  //
#define CD              0x09  // Carrier detect
#define RX_ADDR_P0      0x0A  // Receive address of channel 0
#define RX_ADDR_P1      0x0B  // Receive address of channel 1
#define RX_ADDR_P2      0x0C  // Receive address of channel 2
#define RX_ADDR_P3      0x0D  // Receive address of channel 3
#define RX_ADDR_P4      0x0E  // Receive address of channel 4
#define RX_ADDR_P5      0x0F  // Receive address of channel 5
#define TX_ADDR         0x10  //       Transmit address
#define RX_PW_P0        0x11  //  Size of receive data in channel 0
#define RX_PW_P1        0x12  //  Size of receive data in channel 1
#define RX_PW_P2        0x13  //  Size of receive data in channel 2
#define RX_PW_P3        0x14  //  Size of receive data in channel 3
#define RX_PW_P4        0x15  // Size of receive data in channel 4
#define RX_PW_P5        0x16  //  Size of receive data in channel 5
#define FIFO_STATUS     0x17  // FIFO Status

/******************* Local definition *******************************/
#define ADR_WIDTH 5

#define RX_PLOAD_WIDTH 1
#define TX_PLOAD_WIDTH 1
#define Buffer_Size 32
// define  GPIO for SPI
#define RF_MYO_SPI							SPI1
#define RF_MYO_SPI_GPIO						GPIOA
#define RF_MYO_CECS_GPIO					GPIOA
#define RF_MYO_CE							GPIO_PIN_1
#define RF_MYO_CS							GPIO_PIN_4
#define RF_MYO_SPI_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE()

#define RF_MYO_SPI_SCK						GPIO_PIN_5
#define RF_MYO_SPI_MISO						GPIO_PIN_6
#define RF_MYO_SPI_MOSI						GPIO_PIN_7
#define RF_MYO_SPI_CLK_ENABLE()				__SPI1_CLK_ENABLE()
#define RF_MYO_CECS_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE()

#define RF_MYO_SPI_TIMEOUT					800

/******************** Functions prototype ***************************/
void RX_Mode(void);
void TX_Mode(void);
uint8_t nRF24L01_TxPacket(uint8_t * tx_buf);
uint8_t nRF24L01_RxPacket(uint8_t* rx_buf);
uint8_t SPI_RD_Reg(uint8_t reg);
uint8_t SPI_WR_Reg(uint8_t reg, uint8_t value);
void nRF24L01_Delay_us(uint32_t n);

//Define the layer1:HW operation
uint8_t nRF24L01_SPI_Send_Byte(uint8_t dat);
void nRF24L01_HW_Init(void);
void nRF24L01_SPI_NSS_L(void);
void nRF24L01_SPI_NSS_H(void);
void nRF24L01_CE_L(void);
void nRF24L01_CE_H(void);
//Define the layer2:Reg operation
uint8_t SPI_WR_Reg(uint8_t reg, uint8_t value);
uint8_t SPI_RD_Reg(uint8_t reg);
uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t Len);
uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t Len);


#endif
