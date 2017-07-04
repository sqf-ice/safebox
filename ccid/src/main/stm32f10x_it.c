/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V3.0.0
* Date               : 04/06/2009
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "platform_config.h"
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "../MAG_TTL/magcardTTL.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//uint8_t Send_Buffer[2];
//extern uint32_t ADC_ConvertedValueX;
//extern uint32_t ADC_ConvertedValueX_1;
extern void SysTick_Process(void);						//==public
extern void USARTx_IRQProcess(USART_TypeDef* USARTx);	//==usart.c
extern void SC_USARTx_IRQProcess(USART_TypeDef* USARTx);//==iso7816.c
extern volatile u32 timerValue;
extern volatile s8 timer1Running;
extern unsigned int gSet_delay_ms;
extern unsigned char count_start;
extern void sam_uartIRQProcess(USART_TypeDef* USARTx);	//==sam_v.c 20110307
//extern 	MagcardGlobalIOTTL magcardGlobalIOTTL;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern unsigned int gVar_v_delay_ms;
extern unsigned int gVar_ms;
extern unsigned int waittimeout;
extern unsigned char waitflag;
unsigned char bat_falg1=0;
unsigned int bat_falg=0;

extern unsigned int showBLdelay;

void SysTick_Handler(void)
{



    if(showBLdelay)
	{
	
		 showBLdelay--;

		 if(showBLdelay==0)
		 {
		 		LCD_BackLight_Off();
		 }
	
	
	} 

	if(count_start)
	{
		if(timerValue==0)
		{		
			timer1Running=0;		
		}
		else
		{		
			if(timerValue!=0)
			timerValue-=1;		
		}  
	}
	if (gSet_delay_ms)		
	{
		gSet_delay_ms--;		
	}

	if (gVar_v_delay_ms >0xffffff)	
		gVar_v_delay_ms = 0;
	else 
		gVar_v_delay_ms++;
			
	if (gVar_ms >0xffffff)		
		gVar_ms = 0;
	else 
		gVar_ms++;
	bat_falg++;	
	if(bat_falg>0xfff0)
	{
		ad_get();
		bat_falg =0;
	}
/*	if(magcardGlobalIOTTL.magcardRevcTimeFlag == 1)				
	{
		magcardGlobalIOTTL.magcardRevcTimeCount++;
	}
	else{
		magcardGlobalIOTTL.magcardRevcTimeCount = 0;
	} */
	if(waitflag==1)
	{
	   waittimeout--;
	}
	if (waittimeout ==1)
	{		
		waittimeout = 0;
		waitflag =0;
	}		
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval : None
  */

void USART1_IRQHandler(void)
{
	USARTx_IRQProcess(USART1);	
}

/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */ 
void USART2_IRQHandler(void)
{
	USARTx_IRQProcess(USART2);
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */
void USART3_IRQHandler(void)
{
//	SC_USARTx_IRQProcess(USART3);
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */
void UART4_IRQHandler(void)
{
	//USARTx_IRQProcess(UART4);	
}
/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval : None
  */
void UART5_IRQHandler(void)
{
   sam_uartIRQProcess(UART5);
}

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN_TX_IRQHandler(void)
{
}
/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{


    USB_Istr();
}
/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char test12;

void ADC1_2_IRQHandler(void)
{
   test12++;
}


void WWDG_IRQHandler(void)
{
  test12++;

}



/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void EXTI9_5_IRQHandler(void)
//{
	
//}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void EXTI15_10_IRQHandler(void)
//{

//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
