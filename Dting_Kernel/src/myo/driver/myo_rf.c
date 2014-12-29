/*	@file		myo_rf.c
 * 	@author		Weichen Li
 * 	@date		20141217
 * 	@project	DTing_Kernel
 * 	@version	v0.0.1
 *
 * 	@brief		nRF24L01 driver.
 */

#include "myo_rf.h"
///**************************************************************************************

//define the private constants in this library
//#define TX_ADR_WIDTH 5
//#define RX_ADR_WIDTH 5

/******************************* Global variables ************************/
SPI_HandleTypeDef  SPI_Handle;
uint8_t nRF24L01_Freq = 0;
uint8_t nRF24L01_power_rate = 0;

//define the initial Address
uint8_t  TX_ADDRESS[ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t  ADDRESS_VER[ADR_WIDTH]= {0};
uint8_t  RX_ADDRESS[ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};

/******************************* Private functions *****************************/
void nRF24L01_Delay_us(uint32_t n)
{
	uint32_t i;
	while(n--)  // delay n us
	{
 	   i=100;
 	   while(i--); // delay 1 us
  }
}

uint8_t SPI_RD_Reg(uint8_t reg)
{
	uint8_t reg_val;

	nRF24L01_SPI_NSS_L();                // CSN low, initialize SPI communication...
	nRF24L01_SPI_Send_Byte(reg);            // Select register to read from..
	reg_val = nRF24L01_SPI_Send_Byte(0);    // ..then read register value
	nRF24L01_SPI_NSS_H();                // CSN high, terminate SPI communication

	return(reg_val);        // return register value
}

uint8_t SPI_WR_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;

	nRF24L01_SPI_NSS_L();
    status = nRF24L01_SPI_Send_Byte(reg);	// select register
	nRF24L01_SPI_Send_Byte(value);          // ..and write value to it..
	nRF24L01_SPI_NSS_H();                   // CSN high again
	return(status);            // return nRF24L01 status uint8_t
}

uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t Len)
{
	unsigned int status,i;

	nRF24L01_SPI_NSS_L();                  // Set CSN low, init SPI tranaction
	status = nRF24L01_SPI_Send_Byte(reg);  // Select register to write to and read status uint8_t

  for(i=0;i<Len;i++)
  {
     pBuf[i] = nRF24L01_SPI_Send_Byte(0);
  }
	nRF24L01_SPI_NSS_H();

	return(status);                    // return nRF24L01 status uint8_t
}

uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t Len)
{
	unsigned int status,i;

	nRF24L01_SPI_NSS_L();
	status = nRF24L01_SPI_Send_Byte(reg);
	for(i=0; i<Len; i++) //
	{
		nRF24L01_SPI_Send_Byte(*pBuf);
		pBuf ++;
	}
	nRF24L01_SPI_NSS_H();
	return(status);
}

uint8_t nRF24L01_SPI_Send_Byte(uint8_t dat)
{
	uint8_t Data;
  if(HAL_SPI_TransmitReceive(&SPI_Handle, &dat, &Data, 1, RF_MYO_SPI_TIMEOUT) != HAL_OK)
  	  trace_printf("SPI tranceive error.\n");

  return Data;
}

void nRF24L01_SPI_NSS_H(void)
{
	HAL_GPIO_WritePin(RF_MYO_CECS_GPIO, RF_MYO_CS, SET);
}

void nRF24L01_SPI_NSS_L(void)
{
	HAL_GPIO_WritePin(RF_MYO_CECS_GPIO, RF_MYO_CS, RESET);
}

void nRF24L01_CE_L(void)
{
	HAL_GPIO_WritePin(RF_MYO_CECS_GPIO, RF_MYO_CE, RESET);
}

void nRF24L01_CE_H(void)
{
	HAL_GPIO_WritePin(RF_MYO_CECS_GPIO, RF_MYO_CE, SET);
}

/*	@brief		Set the chip to transmission mode (TX).
 * 				Please manipulate the parameters according to different situation.
 */
void TX_Mode(void)
{
	uint8_t status;
	uint8_t Data;
	nRF24L01_CE_L();

	/* Reset PRIM_RX and set PTX */
	while(1)
	{
	status = SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x0A);
	timer_sleep(1500);
	Data = SPI_RD_Reg(CONFIG);
	if(Data == 0x0A)
			break;
	}
	trace_printf("CONFIG is set to 0x%x.\n", Data);

	/* Write Tx address */
	SPI_Write_Buf(WRITE_nRF_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH);
	SPI_Read_Buf(READ_nRF_REG + TX_ADDR, ADDRESS_VER, ADR_WIDTH);
	trace_printf("TX address is written into TX_ADDR:  0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",ADDRESS_VER[0], ADDRESS_VER[1], ADDRESS_VER[2], ADDRESS_VER[3], ADDRESS_VER[4]);


	/* write RX_P0 address */
	SPI_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH);
	SPI_Read_Buf(READ_nRF_REG + TX_ADDR, ADDRESS_VER, ADR_WIDTH);
	trace_printf("RX address is written into RX_ADDR_P0:  0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",ADDRESS_VER[0], ADDRESS_VER[1], ADDRESS_VER[2], ADDRESS_VER[3], ADDRESS_VER[4]);

	/* Enable auto ACK P0 */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + EN_AA, 0x01);
	Data = SPI_RD_Reg(EN_AA);
	if(Data == 0x01)
		break;
	}
	trace_printf("EN_AA is set to 0x%x.\n", Data);

	/* Enable P0 */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + EN_RXADDR, 0x01);
	Data = SPI_RD_Reg(EN_RXADDR);
	if(Data == 0x01)
		break;
	}
	trace_printf("EN_RXADDR is set to 0x%x.\n", Data);

	/* Set auto retransmit configuration */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + SETUP_RETR, 0x1F);//Auto Retransmit Delay: 500 us, Auto Retransmit Count: Up to 2 Re-Transmit
	Data = SPI_RD_Reg(SETUP_RETR);
	if(Data == 0x1F)
		break;
	}
	trace_printf("SETUP_RETR is set to 0x%x.\n", Data);

	/* Set RF frequency */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + RF_CH, 0x02);
	Data = SPI_RD_Reg(RF_CH);
	if(Data == 0x02)
		break;
	}
	trace_printf("RF_CH is set to 0x%x.\n", Data);

	/* Set address field with */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + SETUP_AW, 0x03); // setup add width to 5 bytes
	Data = SPI_RD_Reg(SETUP_AW);
	if(Data == 0x03)
		break;
	}
	trace_printf("SETUP_AW is set to 0x%x.\n", Data);

	/* Set transmitting configuration */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + RF_SETUP ,0x07); //setup power 0dbm, rate 1Mbps, continuous carrier transmit
	Data = SPI_RD_Reg(RF_SETUP);
	if(Data == 0x07)
		break;
	}
	trace_printf("RF_SETUP is set to 0x%x.\n", Data);

	/* Set size of RX payload in data pipe 0 */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + RX_PW_P0, 0x01);
	Data = SPI_RD_Reg(RX_PW_P0);
	if(Data == 0x01)
		break;
	}
	trace_printf("RX_PW_P0 is set to 0x%x.\n", Data);
}

/*	@brief		Set the chip to receiving mode (RX).
 * 				Please manipulate the parameters according to different situation.
 */
void RX_Mode(void)
{
	uint8_t status;
	uint8_t Data;
	nRF24L01_CE_L();
	/* Set PRIM_RX and set PTX */
	while(1)
	{
	status = SPI_WR_Reg(WRITE_nRF_REG + CONFIG, 0x0B);
	Data = SPI_RD_Reg(CONFIG);
	if(Data == 0x0B)
			break;
	}
	trace_printf("CONFIG is set to 0x%x.\n", Data);

	/* Write Tx address */
	SPI_Write_Buf(WRITE_nRF_REG + TX_ADDR, TX_ADDRESS, ADR_WIDTH);
	trace_printf("TX address is written into TX_ADDR.\n");

	/* write RX_P0 address */
	SPI_Write_Buf(WRITE_nRF_REG + RX_ADDR_P0, RX_ADDRESS, ADR_WIDTH);
	trace_printf("RXP0 address is written into RX_ADDR_P0.\n");

	/* Enable auto ACK P0 */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + EN_AA, 0x01);
	Data = SPI_RD_Reg(EN_AA);
	if(Data == 0x01)
		break;
	}
	trace_printf("EN_AA is set to 0x%x.\n", Data);

	/* Enable P0 */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + EN_RXADDR, 0x01);
	Data = SPI_RD_Reg(EN_RXADDR);
	if(Data == 0x01)
		break;
	}
	trace_printf("EN_RXADDR is set to 0x%x.\n", Data);

	/* Set auto retransmit configuration */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + SETUP_RETR, 0x1F);//Auto Retransmit Delay: 500 us, Auto Retransmit Count: Up to 2 Re-Transmit
	Data = SPI_RD_Reg(SETUP_RETR);
	if(Data == 0x1F)
		break;
	}
	trace_printf("SETUP_RETR is set to 0x%x.\n", Data);

	/* Set RF frequency */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + RF_CH, 0x02);
	Data = SPI_RD_Reg(RF_CH);
	if(Data == 0x02)
		break;
	}
	trace_printf("RF_CH is set to 0x%x.\n", Data);

	/* Set address field with */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + SETUP_AW, 0x03); // setup add width to 5 bytes
	Data = SPI_RD_Reg(SETUP_AW);
	if(Data == 0x03)
		break;
	}
	trace_printf("SETUP_AW is set to 0x%x.\n", Data);

	/* Set transmitting configuration */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + RF_SETUP ,0x07); //setup power 0dbm, rate 1Mbps, continuous carrier transmit
	Data = SPI_RD_Reg(RF_SETUP);
	if(Data == 0x07)
		break;
	}
	trace_printf("RF_SETUP is set to 0x%x.\n", Data);

	/* Set size of RX payload in data pipe 0 */
	while(1)
	{
	SPI_WR_Reg(WRITE_nRF_REG + RX_PW_P0, 0x20);
	Data = SPI_RD_Reg(RX_PW_P0);
	if(Data == 0x20)
		break;
	}
	trace_printf("RX_PW_P0 is set to 0x%x.\n", Data);
}

/*	@brief		Data packet transmission.
 * 	@param		*tx_buf		data ready to be transmitted.
 * 	@retval		status code.
 */
uint8_t nRF24L01_TxPacket(uint8_t * tx_buf)
{
	uint8_t status;
	uint8_t Data;
	status = SPI_WR_Reg(WRITE_nRF_REG + NRFRegSTATUS, 0x7E); // Write 1 to clear bit
	Data = SPI_RD_Reg(FIFO_STATUS);
	SPI_WR_Reg(FLUSH_TX, 0x00);
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);
	nRF24L01_CE_H();
	nRF24L01_Delay_us(20);
	nRF24L01_CE_L();
	nRF24L01_Delay_us(2000);
	Data = SPI_RD_Reg(NRFRegSTATUS);
	if(status & (uint8_t)0x20)
		return 0;
	else
		return 1;
}

/*	@brief		Data packet receiving.
 * 	@param		*rx_buf		data buffer saving receiving data.
 * 	@retval		status code.
 */
uint8_t nRF24L01_RxPacket(uint8_t* rx_buf)
{
	uint8_t status;

  	status =  SPI_WR_Reg(WRITE_nRF_REG + NRFRegSTATUS, 0x7E); // Write 1 to clear bit
  	SPI_WR_Reg(FLUSH_RX, 0x00);
  	nRF24L01_CE_H();
  	nRF24L01_Delay_us(20);
  	status=SPI_RD_Reg(NRFRegSTATUS);
  	while(!(status & (uint8_t)0x40))
  	{
  		status=SPI_RD_Reg(NRFRegSTATUS);
  		trace_printf("STATUS is 0x%x.\n", status);
  	}
	SPI_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);
	nRF24L01_CE_L();
	SPI_WR_Reg(WRITE_nRF_REG+NRFRegSTATUS, status); // Write 1 to clear bit
	return 0;
}

/*	@brief		Initialization function of RF driver.
 * 				This function uses the global variable @SPI_Handle.
 */
void nRF24L01_HW_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable GPIO clocks */
	RF_MYO_SPI_GPIO_CLK_ENABLE();
	RF_MYO_CECS_GPIO_CLK_ENABLE();
	/* SPI Periph clock enable */
	RF_MYO_SPI_CLK_ENABLE();

	/* Configure SPI pins:  SCK ,MOSI, MISO*/
    GPIO_InitStruct.Pin       = RF_MYO_SPI_SCK | RF_MYO_SPI_MOSI | RF_MYO_SPI_MISO;
  	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  	GPIO_InitStruct.Pull      = GPIO_PULLUP;
  	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	
  	HAL_GPIO_Init(RF_MYO_SPI_GPIO, &GPIO_InitStruct);

  	GPIO_InitStruct.Pin       = RF_MYO_CS | RF_MYO_CE;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;

	HAL_GPIO_Init(RF_MYO_CECS_GPIO, &GPIO_InitStruct);

	/* SPI configuration */
	SPI_Handle.Instance 			= RF_MYO_SPI;
	SPI_Handle.Init.Direction 		= SPI_DIRECTION_2LINES;
	SPI_Handle.Init.Mode 			= SPI_MODE_MASTER;
	SPI_Handle.Init.DataSize 		= SPI_DATASIZE_8BIT;
	SPI_Handle.Init.CLKPolarity 	= SPI_POLARITY_LOW;
	SPI_Handle.Init.CLKPhase 		= SPI_PHASE_1EDGE;
	SPI_Handle.Init.NSS 			= SPI_NSS_SOFT;
	SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; // 84000kHz/256=328kHz < 400kHz
	SPI_Handle.Init.FirstBit 		= SPI_FIRSTBIT_MSB;
	SPI_Handle.Init.CRCCalculation 	= SPI_CRCCALCULATION_DISABLED;
	SPI_Handle.Init.CRCPolynomial 	= 7;
	SPI_Handle.Init.TIMode           = SPI_TIMODE_DISABLED;

	if(HAL_SPI_Init(&SPI_Handle) != HAL_OK)
		trace_printf("SPI initialization error.\n");
}
/******************************** End of file *********************************/
