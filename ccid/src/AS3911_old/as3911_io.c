/*
 *****************************************************************************
 * Copyright @ 2009 by austriamicrosystems AG                                *
 * All rights are reserved.                                                  *
 *                                                                           *
 * Reproduction in whole or in part is prohibited without the written consent*
 * of the copyright owner. Austriamicrosystems reserves the right to make    *
 * changes without notice at any time. The software is provided as is and    *
 * Austriamicrosystems makes no warranty, expressed, implied or statutory,   *
 * including but not limited to any implied warranty of merchantability or   *
 * fitness for any particular purpose, or that the use will not infringe any *
 * third party patent, copyright or trademark. Austriamicrosystems should    *
 * not be liable for any loss or damage arising from its use.                *
 *****************************************************************************
 */

/*
 * PROJECT: AS3911 firmware
 * $Revision: $
 * LANGUAGE: ANSI C
 */

/*! \file as3911_io.c
 *
 * \author Oliver Regenfelder
 *
 * \brief AS3911 SPI communication.
 *
 * Implementation of the AS3911 SPI communication. The PIC is set to IPL 7 to disable
 * interrupts while accessing the SPI.
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
//#include <p24FJ64GB002.h>

//#include "stm32f10x.h"

#include "as3911_io.h"
#include "as3911_def.h"
#include "errno.h"
#include "../spi/spi.h"



extern AS3911_SS(unsigned char flag);
extern void SpiSendByte(unsigned char    val);
extern void spim_send_bytes(unsigned char    *pBuf, unsigned int len);
extern void spim_rcv_bytes(unsigned char    *pBuf, unsigned int len);
extern unsigned char    SpiRcvByte(void);
extern void AS3911_IRQ_OFF(void);
extern void AS3911_IRQ_ON(void);
s8 as3911ContinuousRead1(u8 address, u8 *data, u8 length);
//#include "../Inc/IS32U320A_gpio.h"

//extern void SPI1ReceiveData2(UINT8 *pbuff,UINT32 datalen);
extern void TimeDelay_us(unsigned int Time);



/*
******************************************************************************
* DEFINES
******************************************************************************
*/

#define AS3911_SPI_ADDRESS_MASK         (0x3F)
#define AS3911_SPI_CMD_READ_REGISTER    (0x40)
#define AS3911_SPI_CMD_WRITE_REGISTER   (0x00)
#define AS3911_SPI_CMD_READ_FIFO        (0xBF)
#define AS3911_SPI_CMD_WRITE_FIFO       (0x80)
#define AS3911_SPI_CMD_DIREC_CMD        (0xC0)

#define AS3911_REG_AM_MOD_DEPTH_CONTROL_mod_8percent       (0xb<<1)	
#define AS3911_REG_AM_MOD_DEPTH_CONTROL_mod_10percent      (0xe<<1)	
#define AS3911_REG_AM_MOD_DEPTH_CONTROL_mod_14percent      (0x14<<1)	
#define AS3911_REG_AM_MOD_DEPTH_CONTROL_mod_20percent      (0x20<<1)	
#define AS3911_REG_AM_MOD_DEPTH_CONTROL_mod_30percent      (0x37<<1)	
#define AS3911_REG_AM_MOD_DEPTH_CONTROL_mod_33percent      (0x3f<<1)	


/*
******************************************************************************
* MACROS
******************************************************************************
*/

//#define AS3911_SEN_ON() { _LATB8 = 0; }
//#define AS3911_SEN_OFF() { _LATB8 = 1; }

/*
******************************************************************************
* LOCAL DATA TYPES
******************************************************************************
*/

/*
******************************************************************************
* LOCAL VARIABLES
******************************************************************************
*/

/*
******************************************************************************
* LOCAL TABLES
******************************************************************************
*/

/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/

/*
******************************************************************************
* GLOBAL VARIABLE DEFINITIONS
******************************************************************************
*/

/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/



void write_data(unsigned char* sbuf, unsigned int len)
{
	unsigned int i;	 //,j
//	AS3911_IRQ_OFF();
	AS3911_SEN_ON();

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	for (i=0; i<len; i++)
	{
		SPI_I2S_SendData(SPI2, sbuf[i]);
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		SPI_I2S_ReceiveData(SPI2);										//== 读出本次发送同时接收到的数据
//		for(j=0;j<1000;j++);
	}

	AS3911_SEN_OFF();
//	AS3911_IRQ_ON();
}


void read_data(unsigned char* rbuf, unsigned int len)
{
	unsigned int i;	 // ,j
//	AS3911_IRQ_OFF();
	AS3911_SEN_ON();

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	for (i=0; i<len; i++)
	{
		SPI_I2S_SendData(SPI2, 0xFF);										//== 输出数据，提供输入数据的时钟
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		rbuf[i] = (unsigned char)SPI_I2S_ReceiveData(SPI2);
//		for(j=0;j<1000;j++);
	}

	AS3911_SEN_OFF();
//	AS3911_IRQ_ON();
}


s8 as3911WriteRegister(u8 address, u8 data)
{
	u8 as3911WriteCommand[2];
	AS3911_IRQ_OFF();
    as3911WriteCommand[0] = address & 0x3f;
	as3911WriteCommand[1] = data;
	
	 write_data(as3911WriteCommand, 2);
	AS3911_IRQ_ON();
	return 0;
}




s8 as3911ReadRegister(u8 address, u8 *data)
{	

// unsigned char Flashcmd[5];
	u8 as3911ReadCommand = AS3911_SPI_CMD_READ_REGISTER | (address & AS3911_SPI_ADDRESS_MASK);
	AS3911_IRQ_OFF();
//	disable_AS3911_ISR(0);
	AS3911_SEN_ON();
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	SPI_I2S_SendData(SPI2, as3911ReadCommand);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI2);
		
	SPI_I2S_SendData(SPI2,0xff);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	*data = (unsigned char)SPI_I2S_ReceiveData(SPI2);		 
    AS3911_SEN_OFF();
	AS3911_IRQ_ON();	
//	disable_AS3911_ISR(1);
	
		return 0;
}

s8 as3911WriteTestRegister(u8 address, u8 data)
{

	u8 as3911WriteCommand[3];
	AS3911_IRQ_OFF();
    as3911WriteCommand[0] = AS3911_SPI_CMD_DIREC_CMD | AS3911_CMD_TEST_ACCESS;
	as3911WriteCommand[1] = address & AS3911_SPI_ADDRESS_MASK;
	as3911WriteCommand[2] = data;
	write_data(as3911WriteCommand, 3);
	AS3911_IRQ_ON();
	return 0;
}

s8 as3911ReadTestRegister(u8 address, u8 *data)
{


	u8 as3911ReadCommand[2],i; 
	AS3911_IRQ_OFF();
	as3911ReadCommand[0]= AS3911_SPI_CMD_DIREC_CMD | AS3911_CMD_TEST_ACCESS;
	as3911ReadCommand[1] = AS3911_SPI_CMD_READ_REGISTER | (address & AS3911_SPI_ADDRESS_MASK);

//	 disable_AS3911_ISR(0);
	 
	AS3911_SEN_ON();
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);
	for(i=0;i<2;i++)
	{
		SPI_I2S_SendData(SPI2, as3911ReadCommand[i]);
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		SPI_I2S_ReceiveData(SPI2);
	}	
	SPI_I2S_SendData(SPI2,0xff);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	*data = (unsigned char)SPI_I2S_ReceiveData(SPI2);		 
			 
    AS3911_SEN_OFF();
	AS3911_IRQ_ON();
	return 0;
}

s8 as3911ModifyRegister(u8 address, u8 mask, u8 data)
{
    s8 error = ERR_NONE;
//	unsigned int j;
    u8 registerValue = 0;
	AS3911_IRQ_OFF();
    error |= as3911ReadRegister(address, &registerValue);
    registerValue = (registerValue & ~mask) | data;
//	for(j=0;j<1000;j++);
    error |= as3911WriteRegister(address, registerValue);
	AS3911_IRQ_ON();
 //   error |= as3911ReadRegister(address, &registerValue);
    if (ERR_NONE == error)
        return ERR_NONE;
    else
        return ERR_IO;	
}

s8 as3911ContinuousWrite(u8 address, const u8 *data, u8 length)
{
    u8 as3911WriteCommand[500];
	u8  i;
	AS3911_IRQ_OFF();
	if (length == 0)
        return ERR_NONE;

    as3911WriteCommand[0] = address & 0x3f;

	for(i=0;i<length;i++)
		as3911WriteCommand[1+i] = data[i];
	write_data(as3911WriteCommand, length+1);
	AS3911_IRQ_ON();
	return 0;
}

//unsigned char testnum1=0;
//unsigned char testnum2=0;
unsigned char buff1[300];
s8 as3911ContinuousRead(u8 address, u8 *data, u8 length)
{
	u8 as3911ReadCommand[2],i;
//	u16 j;
	 AS3911_IRQ_OFF();
	as3911ReadCommand[0] = AS3911_SPI_CMD_READ_REGISTER | (address & AS3911_SPI_ADDRESS_MASK);

//	 disable_AS3911_ISR(0);
	 
	AS3911_SEN_ON();
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	SPI_I2S_SendData(SPI2, as3911ReadCommand[0]);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	for(i=0;i<length;i++)
	{	
		SPI_I2S_SendData(SPI2,0xff);
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		*data = (unsigned char)SPI_I2S_ReceiveData(SPI2);	 
	}
			 
    AS3911_SEN_OFF();
	AS3911_IRQ_ON();	
	return 0;
}

s8 as3911ContinuousRead1(u8 address, u8 *data, u8 length)
{
	u8 as3911ReadCommand[2],i;
//	u16 j;
	 AS3911_IRQ_OFF();
	as3911ReadCommand[0] = AS3911_SPI_CMD_READ_REGISTER | (address & AS3911_SPI_ADDRESS_MASK);

//	 disable_AS3911_ISR(0);
	 
	AS3911_SEN_ON();
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	SPI_I2S_SendData(SPI2, as3911ReadCommand[0]);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	for(i=0;i<length;i++)
	{	
		SPI_I2S_SendData(SPI2,0xff);
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		buff1[i] = (unsigned char)SPI_I2S_ReceiveData(SPI2);
	//	for(j=0;j<200;j++);	
//		if(data[i-1] == 0x40)
//			j++;	 
	}
			 
    AS3911_SEN_OFF();
	AS3911_IRQ_ON();	
	return 0;
} 


s8 as3911WriteFifo(const u8 *data, u8 length)
{

    u8 as3911WriteFifoCommand[500],i;
	AS3911_IRQ_OFF();
    if (0 == length)
        return ERR_NONE;

//    disable_AS3911_ISR(0);
	
	as3911WriteFifoCommand[0] = AS3911_SPI_CMD_WRITE_FIFO;


	for(i=0;i<length;i++)
		as3911WriteFifoCommand[1+i] = data[i];
	write_data(as3911WriteFifoCommand, length+1);
	AS3911_IRQ_ON();
	return 0;
	
}
//unsigned char buff1[300];
s8 as3911ReadFifo(u8 *data,u8 length)
{


	u8 as3911ReadCommand[2],i; 
//	u16 j;
	as3911ReadCommand[0] = AS3911_SPI_CMD_READ_FIFO;
	AS3911_IRQ_OFF();

    if (length == 0)
        return ERR_NONE;

//     disable_AS3911_ISR(0);
	 
	AS3911_SEN_ON();
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	SPI_I2S_SendData(SPI2, as3911ReadCommand[0]);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	for(i=0;i<length;i++)
	{	
		SPI_I2S_SendData(SPI2,0xff);
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		buff1[i]=(unsigned char)SPI_I2S_ReceiveData(SPI2);
	//	*data = (unsigned char)SPI_I2S_ReceiveData(SPI2);	
//		for(j=0;j<1000;j++);	 
	}		 
    AS3911_SEN_OFF();
	AS3911_IRQ_ON();	
	return 0;		
}

//u8 temp=0;
s8 as3911ExecuteCommand(u8 directCommand)
{
    u8 as3911DirectCommand = AS3911_SPI_CMD_DIREC_CMD | (directCommand & AS3911_SPI_ADDRESS_MASK);
	AS3911_IRQ_OFF();
	AS3911_SEN_ON();

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	
	SPI_I2S_SendData(SPI2, as3911DirectCommand);
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI2);										//== 读出本次发送同时接收到的数据
//	for(j=0;j<1000;j++)

	AS3911_SEN_OFF();
//	write_data(&as3911DirectCommand,1);
	AS3911_IRQ_ON();
	return 0;
}

/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/
extern int emvTypeB;

void AS3911_Init(void)
{
// 	u32 regtmp =0;

	u8  tdata=0;

	as3911ExecuteCommand(AS3911_CMD_SET_DEFAULT);
	as3911WriteRegister(AS3911_REG_IO_CONF1, 0x0F);
//	as3911WriteRegister(AS3911_REG_IO_CONF2, 0x18);
	as3911WriteRegister(AS3911_REG_OP_CONTROL, 0xC8);



	tdata=0;
	as3911ReadRegister(AS3911_REG_OP_CONTROL, &tdata);

	as3911WriteRegister(AS3911_REG_OP_CONTROL, 0x12);

 	tdata=0;
	as3911ReadRegister(AS3911_REG_OP_CONTROL, &tdata);



 	as3911WriteRegister(AS3911_REG_OP_CONTROL, 0xC8);  



  if(emvTypeB==1)
  {

    as3911WriteRegister(AS3911_REG_AM_MOD_DEPTH_CONF,AS3911_REG_AM_MOD_DEPTH_CONTROL_mod_10percent);
    as3911WriteRegister(AS3911_REG_BIT_RATE,0x00);
  
  }

}






