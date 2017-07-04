#include "usart.h"
#include <stdio.h>
#include <string.h>	

static unsigned int i =0; 
unsigned int Count=0,rec_count=0;
unsigned char RC_flag =0;
extern unsigned char UsbBulkMsgBuf[271];	 //与USB通信的缓存buf
extern unsigned char UsbBulkInMsgBuf[271];	 //与USB通信的缓存buf
//===============================================================================//
//== 函 数 名：init_usartx_nvic
//== 功   能 ：初始化串口x中断
//== 说   明 ：
//== 时   间 ：
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
unsigned int init_usartx_nvic(USART_TypeDef* USARTx)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//== Configure the NVIC Preemption Priority Bits  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	//== Enable the USARTx Interrupt
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//== 占先级	  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	if(USARTx == USART1){

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	}else if(USARTx == USART2){

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	}else if(USARTx == USART3){

		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	}else if(USARTx == UART4){

		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	}else if(USARTx == UART5){

		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	}else 
		return 1;

	NVIC_Init(&NVIC_InitStructure);
	return 0;
}

//===============================================================================//
//== 函 数 名：init_usartx_gpio
//== 功   能 ：初始化串口x gpio
//== 说   明 ：
//== 时   间 ：
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
unsigned int init_usartx_gpio(USART_TypeDef* USARTx)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	if(USARTx == USART1){ 

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA , ENABLE);
	  	//==Configure USART1 Tx (PA.09) as alternate function push-pull
	  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	    //==Configure USART1 Rx (PA.10) as input floating
	  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	  	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING; 
	  	GPIO_Init(GPIOA, &GPIO_InitStructure);				
	}else if(USARTx == USART2){

	  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
		//==Configure USART2 Tx (PA.02) as alternate function push-pull
		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	    //==Configure USART2 Rx (PA.03) as input floating
	  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	}else if(USARTx == USART3){

	  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		//==Configure USART3 Tx (PB.10) as alternate function push-pull	
		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	    //==Configure USART3 Rx (PB.11) as input floating
	  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;	
	  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	}else if(USARTx == UART4){

	  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

		//==Configure USART4 Tx (PC.10) as alternate function push-pull	
		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	    //==Configure USART4 Rx (PC.11) as input floating
	  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;	
	  	GPIO_Init(GPIOC, &GPIO_InitStructure);
		
	}else if(USARTx == UART5){

	  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

		//==Configure USART5 Tx (PC.12) as alternate function push-pull	
		GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	    //==Configure USART5 Rx (PD.02) as input floating
	  	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;	
	  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
	}else
		return 1;

	return 0;

}
//===============================================================================//
//== 函 数 名：init_usartx_config
//== 功   能 ：初始化串口 x 配置
//== 说   明 ：
//== 时   间 ：
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
void init_usartx_config(USART_TypeDef* USARTx, unsigned int BaudRate)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate 		= BaudRate;
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	USART_InitStructure.USART_Parity 		= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 			= USART_Mode_Rx | USART_Mode_Tx;

	//== Configure USARTx
	USART_Init(USARTx, &USART_InitStructure);

	//== Enable USART1 Receive and Transmit interrupts
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);		//== 允许接收中断
//	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);		//== 允许发送中断

	//== Enable the USARTx
	USART_Cmd(USARTx, ENABLE);
}

void init_usartx_config_c(USART_TypeDef* USARTx, unsigned int BaudRate)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate 		= BaudRate;
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
	USART_InitStructure.USART_Parity 		= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 			= USART_Mode_Rx | USART_Mode_Tx;

	//== Configure USARTx
	USART_Init(USARTx, &USART_InitStructure);

	//== Enable USART1 Receive and Transmit interrupts
	//USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);		//== 允许接收中断
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);		//== 允许发送中断

	//== Enable the USARTx
	USART_Cmd(USARTx, ENABLE);
}

 void init_usartx_config_c_2stopbit(USART_TypeDef* USARTx, unsigned int BaudRate)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate 		= BaudRate;
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		= USART_StopBits_2;
	USART_InitStructure.USART_Parity 		= USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 			= USART_Mode_Rx | USART_Mode_Tx;

	//== Configure USARTx
	USART_Init(USARTx, &USART_InitStructure);

	//== Enable USART1 Receive and Transmit interrupts
	//USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);		//== 允许接收中断
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);		//== 允许发送中断

	//== Enable the USARTx
	USART_Cmd(USARTx, ENABLE);
}


//===============================================================================//
//== 函 数 名：init_usartx
//== 功   能 ：初始化串口x
//== 说   明 ：与pc通信
//== 时   间 ：20101102创建函数
//== 入口参数：USARTx：串口标识USART1、USART2、USART3、UART4、UART5；
//== 		   BaudRate:波特率9600,19200,38400,57600,115200...
//== 出口参数：
//== 返 回 值：无
//===============================================================================//
void init_usartx(USART_TypeDef* USARTx, unsigned int BaudRate)
{
	init_usartx_nvic(USARTx);
	init_usartx_gpio(USARTx);
	init_usartx_config(USARTx, BaudRate);
}

void init_usartx_c(USART_TypeDef* USARTx, unsigned int BaudRate)
{
//	init_usartx_nvic(USARTx);
	init_usartx_gpio(USARTx);
	init_usartx_config_c(USARTx, BaudRate);
}



extern void init_usartx_config_c_2stopbit(USART_TypeDef* USARTx, unsigned int BaudRate);

void init_usartx_cc(USART_TypeDef* USARTx, unsigned int BaudRate)
{
//	init_usartx_nvic(USARTx);
	init_usartx_gpio(USARTx);
	init_usartx_config_c_2stopbit(USARTx, BaudRate);
}



//========================================================================
//== 函 数 名：RxByte
//== 功   能 ：USARTx收一Byte,查询模式
//== 时   间 ：201007
//== 入口参数：USARTx：串口标识；TimeOut超时时间(ms)
//== 出口参数：*rxData接收到数据；	 
//== 返 回 值：0:成功，1：超时
//========================================================================
unsigned int RxByte(USART_TypeDef *USARTx, unsigned char *rxData,unsigned int TimeOut)
{

	unsigned int i = 0;

	for(i=0;i< TimeOut;i++)
	{ 		
		if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == !RESET) 
		{
			rxData[0] = (USART_ReceiveData(USARTx));
			USART_ClearFlag(USARTx, USART_FLAG_RXNE);
			return 0;
		} 					
	}
	return 1;




	
}




extern unsigned int waittimeout;
extern unsigned char CheckCancleByteReceive(void);


unsigned int RxByte_FingerPrint(USART_TypeDef *USARTx, unsigned char *rxData,unsigned int TimeOut)
{

	unsigned int i = 0;


   if(TimeOut==0xFFFFFFFF)
   {

		    while(waittimeout)
			{
			    if(CheckCancleByteReceive())  return 2;
				if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == !RESET) 
				{
					rxData[0] = (USART_ReceiveData(USARTx));
					USART_ClearFlag(USARTx, USART_FLAG_RXNE);
					return 0;
				} 
			}
	}
	else
	{

		//	for(i=0;i< TimeOut;i++)
		  while(1)
			{ 		
				if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == !RESET) 
				{
					rxData[0] = (USART_ReceiveData(USARTx));
					USART_ClearFlag(USARTx, USART_FLAG_RXNE);
					return 0;
				} 					
			}
	}

	return 1;
	
}




unsigned int RxByte_0(USART_TypeDef *USARTx, unsigned char *rxData,unsigned int TimeOut)
{

//	unsigned int i = 0;

		
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET) 
	{
	/*	 TimeOut--;
		 if(TimeOut == 0)
		 {
		 return 1;
		 }	  */
			
				   
	}
//	if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == !RESET) 
	{
		rxData[0] = (USART_ReceiveData(USARTx));
		USART_ClearFlag(USARTx, USART_FLAG_RXNE);
		return 0;
	} 	
	 					
	
}
//========================================================================
//== 函 数 名：RxData
//== 功   能 ：USARTx收数据,查询模式
//== 时   间 ：201007
//== 入口参数：USARTx：串口标识；rxLen收数据的长度;TimeOut超时时间 
//== 出口参数：*rxBuf接收到数据； 
//== 返 回 值：0:成功，1：超时
//========================================================================
unsigned int RxData(USART_TypeDef* USARTx, unsigned char* rxBuf,unsigned int rxLen,unsigned int TimeOut)
{
	unsigned int i;
	
	for(i=0;i<rxLen;i++)
	{
		if(RxByte(USARTx,&rxBuf[i],TimeOut)) return 1;
	}
	return 0;
}


void RxDataCOM( unsigned char* rxBuf,unsigned int rxLen,unsigned int TimeOut)
{

  RxData(USART3, rxBuf,rxLen,TimeOut);

}



//========================================================================
//== 函 数 名：TxByte
//== 功   能 ：USARTx发送一Byte,
//== 时   间 ：201007
//== 入口参数：USARTx：串口标识；*txData发送到数据(Byte)
//== 出口参数：无	 
//== 返 回 值：无 
//========================================================================
void TxByte(USART_TypeDef *USARTx, unsigned char *txData)
{	
	//==20100709 Disable the USARTx Transmit interrupt
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE); 
	USART_SendData(USARTx, *txData); 
	//==等待数据发送完
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
    {
    }
			
}
//========================================================================
//== 函 数 名：TxData
//== 功   能 ：USARTx发送数据
//== 时   间 ：201007
//== 入口参数：USARTx：串口标识；*txBuf发送到数据;txLen要发送数据的长度
//== 出口参数：无 
//== 返 回 值：无 
//========================================================================
void TxData(USART_TypeDef* USARTx, unsigned char *txBuf, unsigned int txLen)
{
	unsigned int i=0;

	for(i=0;i<txLen;i++){
		TxByte(USARTx, txBuf);
		txBuf++;	
	} 
}

void TxDataCOM(unsigned char *txBuf, unsigned int txLen)
{

	TxData(USART3,txBuf,txLen);

}

void TxSendByte(USART_TypeDef *USARTx, unsigned char txData)
{

	uint16_t tD;
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE); 
	tD = 	txData;
  	USART_SendData(USARTx, tD); 
	//==等待数据发送完
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
    {
    }

		//==20100709 Disable the USARTx Transmit interrupt



}


//===============================================================================//
//== 函 数 名：USARTx_IRQProcess
//== 功   能 ：usartx通信中断服务
//== 说   明 ：
//== 时   间 ：20101116
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
void USARTx_IRQProcess(USART_TypeDef* USARTx)
{
	//== 接收中断
	if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET) 
	{
		UsbBulkInMsgBuf[rec_count]=USART_ReceiveData(USARTx);
		rec_count++;
		if(rec_count==0x05)
		{
			Count = (UsbBulkInMsgBuf[2]<<8)|UsbBulkInMsgBuf[1]+11;
		}
		
		if(rec_count==Count)
		{
			RC_flag =1;
			rec_count=0;
		}	
	}
			
	//==发送中断
	if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
	{
		for (i= 0; i< Count; i++)
		{
			USART_SendData(USARTx, UsbBulkMsgBuf[i]);
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){ }
			USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
		}		
		if(i >= Count)
		{
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
		
		}		
	}	
}

void process_usart_tmsg(USART_TypeDef* USARTx,unsigned int len)
{

		Count =len;

		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);  
}
