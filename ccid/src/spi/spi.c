/*===============================================================================//
//== 文 件 名：spi.c
//== author	 ：huyx@cashwaytech.com
//== 说    明：SPI配置
//== 修改日志：
//== 
//===============================================================================*/

#include "stm32f10x_spi.h"
#include "platform_config.h"
#include "spi.h"
#include "stm32f10x_exti.h"
//#include "../AS3911/as3911_def.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

void SPI_Init_GPIO(void);
//void AS3911_IRQ_CLR(void);  
//===============================================================================//
//== 函 数 名：init_spix_gpio
//== 功   能 ：初始化 SPIx gpio
//== 说   明 ：
//== 时   间 ：创建函数
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
unsigned int init_spix_gpio(SPI_TypeDef* SPIx)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if((unsigned int)SPIx == (unsigned int)SPI1){

		RCC_APB2PeriphClockCmd(SPI1_GPIO_CLK | SPI1_CLK, ENABLE);
		//== Configure SPIx pins: SCK, MISO and MOSI
		GPIO_InitStructure.GPIO_Pin 	= SPI1_PIN_CKL | SPI1_PIN_MISO | SPI1_PIN_MOSI;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP; 		
		GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);

	}else if((unsigned int)SPIx == (unsigned int)SPI2){

		RCC_APB2PeriphClockCmd(SPI2_GPIO_CLK , ENABLE);	
		RCC_APB1PeriphClockCmd(SPI2_CLK, ENABLE); 	
		//== Configure SPIx pins: SCK, MISO and MOSI
		GPIO_InitStructure.GPIO_Pin 	= SPI2_PIN_CKL | SPI2_PIN_MISO | SPI2_PIN_MOSI;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;			
		GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(SPI2_GPIO, &GPIO_InitStructure);
	}else 
		return 1;

	return 0;  													   
}

//===============================================================================//
//== 函 数 名：init_spix_config
//== 功   能 ：初始化 spix 配置
//== 说   明 ：
//== 时   间 ：20101103
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
void init_spix_config(SPI_TypeDef* SPIx)
{
	SPI_InitTypeDef  SPI_InitStructure;

	//== SPIx configuration
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode 		= SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize 	= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 		= SPI_CPOL_Low;		//== 时钟悬空时为低
	SPI_InitStructure.SPI_CPHA		= SPI_CPHA_1Edge;	//== 数据捕获于第一个时钟沿
	SPI_InitStructure.SPI_NSS 		= SPI_NSS_Soft;		//== NSS 由软件管理

	if(SPIx == SPI1){
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;	//== 72M/32
	}else
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//== 36M/16	   SPI_BaudRatePrescaler_8

	SPI_InitStructure.SPI_FirstBit 	= SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPIx, &SPI_InitStructure);	 	
	//== Enable SPIx CRC calculation
	//==SPI_CalculateCRC(SPIx, ENABLE); 	
	//== Enable SPI2
	SPI_SSOutputCmd(SPIx, ENABLE);		//== PB12 作为其他功能时，要使能NSS输出
	SPI_Cmd(SPIx, ENABLE);
}

//===============================================================================//
//== 函 数 名：init_spi
//== 功   能 ：初始化 spi
//== 说   明 ：
//== 时   间 ：20101102 创建函数
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
void init_spix(SPI_TypeDef* SPIx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	init_spix_gpio(SPIx);
	init_spix_config(SPIx);

	THM_Reset();
}

void AS3911_SEN_ON(void)
{	
	GPIO_WriteBit(SPI2_PIN_NSS_GPIO, SPI2_PIN_NSS, Bit_RESET);	
}


void AS3911_SEN_OFF(void)
{
	GPIO_WriteBit(SPI2_PIN_NSS_GPIO, SPI2_PIN_NSS, Bit_SET);
}

void AS3911_IRQ_Init(void)
{
/*    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
//	unsigned char tdata;

	as3911ExecuteCommand(AS3911_CMD_SET_DEFAULT);
//	as3911ReadRegister(AS3911_REG_IO_CONF1, &tdata);
	as3911WriteRegister(AS3911_REG_IO_CONF1, 0x0F);
	as3911WriteRegister(AS3911_REG_IO_CONF2, 0x18);
//	as3911ReadRegister(AS3911_REG_IO_CONF1, &tdata);
	as3911WriteRegister(AS3911_REG_OP_CONTROL, 0xC8);
//	as3911ReadRegister(AS3911_REG_OP_CONTROL, &tdata);
	
    as3911WriteRegister(AS3911_REG_RX_CONF3, 0x60);
   // as3911WriteRegister(AS3911_REG_RX_CONF3, 0x6c);

	as3911WriteTestRegister(0x01, 0x04);   
//	as3911DisableInterrupts(0xFFFFFF);
//	as3911ClearInterrupts(0xFFFFFF);

//	 as3911WriteRegister(AS3911_REG_ANT_CAL_TARGET, 0x80);
 //   as3911WriteRegister(AS3911_REG_ANT_CAL_CONTROL, 0x30);

    /* 使能GPIO时钟 */
/*  	 RCC_APB2PeriphClockCmd( AS3911_IRQ_IN_RCC | RCC_APB2Periph_AFIO, ENABLE );
	
    GPIO_InitStructure.GPIO_Pin = AS3911_IRQ_IN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(AS3911_IRQ_IN_GPIO, &GPIO_InitStructure);

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    /****************************************************************************
     * 配置外部中断
     */
 /*   GPIO_EXTILineConfig( GPIO_PORT_SOURCE_RF_IRQ, GPIO_PIN_SOURCE_RF_IRQ );
    EXTI_InitStructure.EXTI_Line = EXTI_LINE_RF_IRQ;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Rising;// EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure ); 	*/
} 
/*
extern void as3911Isr(void);
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_LINE_RF_IRQ) != RESET)
	  {
	     as3911Isr();
	     EXTI_ClearITPendingBit(EXTI_LINE_RF_IRQ);
	  }
}

void AS3911_IRQ_CLR(void)  
{
 	EXTI_ClearITPendingBit(EXTI_LINE_RF_IRQ);	
}

  */


/*******************************************************************************
* Function    : SPI_FRAME_START
* Description : SSN Control  as LOW
* Arguments   :  			
* Return      :	
*******************************************************************************/ 
void SPI_FRAME_START(void)  
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);                // Set PA4 as SPI_SSN and set SSN LOW
}

/*******************************************************************************
* Function    : SPI_FRAME_END
* Description : SSN Control  as HIGH
* Arguments   :  			
* Return      :	
*******************************************************************************/ 
void SPI_FRAME_END(void)  
{
	GPIO_SetBits(GPIOD,GPIO_Pin_10);                   // Set PA4 as SPI_SSN and set SSN High
}

/*******************************************************************************
* Function    : _send_byte
* Description : Send  one Byte
* Arguments   : dat to be send  			
* Return      :	
*******************************************************************************/ 
void _send_byte(unsigned char dat)
{
   unsigned char i;
   GPIO_ResetBits(GPIOB,GPIO_Pin_15);
   for (i =0;i<8;i++)
   {
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);               // Set PA5 as SPI_CLK and set LOW
		if (dat & 0x80)
			GPIO_SetBits(GPIOB,GPIO_Pin_15);             // Set PA7 as SPI_MOSI and set high
		 else
		 	GPIO_ResetBits(GPIOB,GPIO_Pin_15);           // Set PA7 as SPI_MOSI and set  LOW
		 dat = dat << 1;
	     GPIO_SetBits(GPIOB,GPIO_Pin_13);                // Set PA5 as SPI_CLK and set high
   }
   GPIO_ResetBits(GPIOB,GPIO_Pin_13);                    // Set PA5 as SPI_CLK and set  LOW
}

/*******************************************************************************
* Function    : _recv_byte
* Description : Recieve one Byte
* Arguments   :  			
* Return      :	dat recieved
*******************************************************************************/ 
static u8 _recv_byte()
{
    unsigned char i,dat,temp;
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);                     // Set PA5 as SPI_CLK and set  LOW
	dat =0; temp =0x80;
	for (i=0;i<8;i++)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);                    // Set PA5 as SPI_CLK and set high
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)&0x01)   // Set PA6 as SPI_MISO and Read 
			dat|= temp;					    
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);                  // Set PA5 as SPI_CLK and set  LOW
		temp >>= 1;		
	}
	return dat;
}

/*******************************************************************************
* Function    : SPI_SendBuff
* Description : Send  num Byte  Data
* Arguments   : buf  -- send buffer
				num  -- data number 			
* Return      :	
*******************************************************************************/ 
void SPI_SendBuff(unsigned char *buf,unsigned int num)
{
 	unsigned int i=0;
	if ((buf== NULL)||(num == 0)) return;
/*	while( num--)
	{
	 	_send_byte(*buf++);
	} */ 
	 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	for (i=0; i<num; i++)
	{
		SPI_I2S_SendData(SPI2, buf[i]);
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		SPI_I2S_ReceiveData(SPI2);										//== 读出本次发送同时接收到的数据
	}


}	   

/*******************************************************************************
* Function    : SPI_RecvBuff
* Description : Recieve  num Byte  Data
* Arguments   : buf  -- Rec buffer
				num  -- data number 			
* Return      :	
*******************************************************************************/ 
void SPI_RecvBuff(unsigned char *buf,unsigned int num)
{
	unsigned int i=0;
/*	if (num == 0) 		
		num= 0x100;
	while(num--)
	{
		*buf++ = _recv_byte();	
	 } */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_ReceiveData(SPI2);

	for (i=0; i<num; i++)
	{
		SPI_I2S_SendData(SPI2, 0xFF);										//== 输出数据，提供输入数据的时钟
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
		buf[i] = (unsigned char)SPI_I2S_ReceiveData(SPI2);

	}
}

/*******************************************************************************
* Function    : SPI_Init_GPIO
* Description : GPIO init  as SPI
* Arguments   :  			
* Return      :
*******************************************************************************/ 
void SPI_Init_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd( T3070_RST_RCC|BLUE_P20_RCC|BLUE_RST_RCC, ENABLE );

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);	      // Enable GPIOA_CLOCK
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	      // Enable GPIOB_CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
//	GPIOA->CRL = 0x38334444;                                        // Set GPIOa 0~7 as IIIIOOIO   (SSN AS OUT  MOSI as OUT  CLK AS OUT   and  MISO AS IN)                                        // Set  
//	GPIOE->CRH = 0x44434444;
//	GPIOD->CRH = 0x44444343;
//	RCC_APB2PeriphClockCmd( T3070_RST_RCC ENABLE );
	GPIO_InitStructure.GPIO_Pin = T3070_RST_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(T3070_RST_GPIO, &GPIO_InitStructure);
																												  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_IN_FLOATING
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOD, GPIO_Pin_8, Bit_RESET);
//	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	delay_ms1(5);   
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
	delay_ms1(5); 
	GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET);
	delay_ms1(10); 
	GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_SET);  							  // Set PA4 as SPI_SSN and set SSN High
	GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_SET);
	delay_ms1(10); 
	GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET);
	delay_ms1(10); 
	GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_SET); 
  						  
  						 
}

/*
void THM_Reset(void)
{
	SPI_Init_GPIO();
	THM_En_2Tx();		            //Config init
}
*/

