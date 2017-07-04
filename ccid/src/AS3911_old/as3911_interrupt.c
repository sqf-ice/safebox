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

/*! \file as3911_interrupt.c
 *
 * \author Oliver Regenfelder
 *
 * \brief AS3911 interrupt handling and ISR
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

#include "as3911_interrupt.h"
#include "stm32f10x.h"
#include "as3911.h"
//#include "as3192hal/portingtype.h"
//#include "../config/CCM3310/as3192hal/timer_driver.h"
#include "errno.h"
#include "platform_config.h"
#include "../emv/emv_picc.h"
//#include "IS32U320A.h"
//#include "IS32U320A_typesmacro.h"
#include "stdbool.h"
//typedef  bool bool_t;
//#define FALSE false 
//#define TRUE  true 


extern void AS3911_IRQ_CLR(void); 
extern bool timerIsRunning(void);
extern s8 as3911ContinuousRead1(u8 address, u8 *data, u8 length); 
/*
******************************************************************************
* DEFINES
******************************************************************************
*/

/*
******************************************************************************
* MACROS
******************************************************************************
*/

/*!
 *****************************************************************************
 * \brief Clear the interrupt flag associated with the as3911 interrupt.
 *****************************************************************************
 */
 
//#define AS3911_IRQ_CLR()   	    




/*!
 *****************************************************************************
 * \brief Evaluates to true if there is a pending interrupt request from the
 * AS3911.
 *****************************************************************************
 */
     
//#define AS3911_IRQ_IS_SET()  1 

unsigned char  AS3911_IRQ_IS_SET(void)
{
	 
	 if(GPIO_ReadInputDataBit(AS3911_IRQ_IN_GPIO, AS3911_IRQ_IN_PIN))
	    return 1;
	 else
	    return 0;


}

//extern void disable_AS3911_ISR(unsigned char flag);

void AS3911_IRQ_OFF(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
//	GPIO_EXTILineConfig( GPIO_PORT_SOURCE_RF_IRQ, GPIO_PIN_SOURCE_RF_IRQ );
    EXTI_InitStructure.EXTI_Line = EXTI_LINE_RF_IRQ;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init( &EXTI_InitStructure );
}

void AS3911_IRQ_ON(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
//	unsigned int ii;
	AS3911_IRQ_CLR();
//	for(ii=0;ii>1000;ii++);
//	GPIO_EXTILineConfig( GPIO_PORT_SOURCE_RF_IRQ, GPIO_PIN_SOURCE_RF_IRQ );
    EXTI_InitStructure.EXTI_Line = EXTI_LINE_RF_IRQ;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );
}







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

/*! AS3911 interrutp mask. */
static volatile u32 as3911InterruptMask = 0;
/*! Accumulated AS3911 interrupt status. */
static volatile u32 as3911InterruptStatus = 0;

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

s8 as3911EnableInterrupts(u32 mask)
{
    s8 error = ERR_NONE;
    u32 irqMask = 0;
	
    u8 	 irqMask_temp[3];
	
    AS3911_IRQ_OFF();
	
   error |= as3911ContinuousRead(AS3911_REG_IRQ_MASK_MAIN, (u8*) &irqMask_temp[0], 3);
   //error |= as3911ContinuousRead(AS3911_REG_IRQ_MASK_MAIN, (u8*) &irqMask, 3);
   irqMask =  (((unsigned long)(irqMask_temp[2])<<16)|((unsigned long)(irqMask_temp[1])<<8)|(unsigned long)irqMask_temp[0]);//(irqMask_temp[2]<<16)|(irqMask_temp[1]<<8)|irqMask_temp[0];
   
    irqMask &= ~mask;
    as3911InterruptMask |= mask;
	
	irqMask_temp[0]=(u8)irqMask;
	irqMask_temp[1]=(u8)(irqMask>>8);
	irqMask_temp[2]=(u8)(irqMask>>16);  
	error |= as3911ContinuousWrite(AS3911_REG_IRQ_MASK_MAIN, (u8*) &irqMask_temp[0], 3);
    //error |= as3911ContinuousWrite(AS3911_REG_IRQ_MASK_MAIN, (u8*) &irqMask, 3);
	
/*	regtmp = u32READ(GPIO_IE);
	regtmp |= 0x40000000;
	u32WRITE(GPIO_IE,regtmp); */
    AS3911_IRQ_ON();

    if (ERR_NONE == error)
        return ERR_NONE;
    else
        return ERR_IO;
}

s8 as3911DisableInterrupts(u32 mask)
{
    s8 error = ERR_NONE;
    u32 irqMask = 0;

	u8 irqMask_temp[3];
	
    AS3911_IRQ_OFF();

    //error |= as3911ContinuousRead(AS3911_REG_IRQ_MASK_MAIN, (u8*) &irqMask, 3);
	error |= as3911ContinuousRead(AS3911_REG_IRQ_MASK_MAIN, (u8*) &irqMask_temp[0], 3);
    irqMask = (((unsigned long)(irqMask_temp[2])<<16)|((unsigned long)(irqMask_temp[1])<<8)|(unsigned long)irqMask_temp[0]);//(irqMask_temp[2]<<16)|(irqMask_temp[1]<<8)|irqMask_temp[0];
	
	
    irqMask |= mask;
    as3911InterruptMask &=  ~mask;
	
	
	irqMask_temp[0]=(u8)irqMask;
	irqMask_temp[1]=(u8)(irqMask>>8);
	irqMask_temp[2]=(u8)(irqMask>>16);  		
    error |= as3911ContinuousWrite(AS3911_REG_IRQ_MASK_MAIN, (u8*) &irqMask_temp[0], 3);

    AS3911_IRQ_ON();

   // if (ERR_NONE == error)
    //    return ERR_NONE;
    //else
    //    return ERR_IO;
   
   return 0;
}

s8 as3911ClearInterrupts(u32 mask)
{
    s8 error = ERR_NONE;
    u32 irqStatus = 0;
    
	u8 irqStatus_temp[3];
	
    AS3911_IRQ_OFF();
	
    error |= as3911ContinuousRead(AS3911_REG_IRQ_MAIN, (u8*) &irqStatus_temp[0], 3);
	irqStatus =(((unsigned long)(irqStatus_temp[2])<<16)|((unsigned long)(irqStatus_temp[1])<<8)|(unsigned long)irqStatus_temp[0]);// (irqStatus_temp[2]<<16)|(irqStatus_temp[1]<<8)|irqStatus_temp[0];
	
	
    as3911InterruptStatus |= irqStatus & as3911InterruptMask;
    as3911InterruptStatus &= ~mask;

    AS3911_IRQ_ON();

        return ERR_NONE;
    //else
     //   return ERR_IO;
}
extern s8 timer1Running;
extern void timerStart( u16 mSec );
//unsigned int flag_irq=0;
s8 as3911WaitForInterruptTimed(u32 mask, u16 timeout, u32 *irqs)
{
    bool_t timerExpired = FALSE;
    u32 irqStatus = 0,irqStatus1=0;		  // ,regtmp=0
//	u8 irqStatus_temp[3];
//	timeout=timeout/10;

	
    if (timeout > 0)
        timerStart(timeout);
	
    do
    {
		 irqStatus1++;
		 if(irqStatus1>50000)  break;
		irqStatus = as3911InterruptStatus & mask;
        if (timeout > 0)
        {
        //   irqStatus1++;
		//   if(irqStatus1>10000)  break;
		   if (!timerIsRunning())
                timerExpired = TRUE;
        }
    } while (!irqStatus && !timerExpired);

    AS3911_IRQ_OFF();
    as3911InterruptStatus &= ~irqStatus;
    AS3911_IRQ_ON();
    *irqs = irqStatus;
	
    return ERR_NONE;
}

s8 as3911GetInterrupts(u32 mask, u32 *irqs)
{
	
    AS3911_IRQ_OFF();

    *irqs = as3911InterruptStatus & mask;
    as3911InterruptStatus &= ~mask;

    AS3911_IRQ_ON();


    return ERR_NONE;
}

extern unsigned char buff1[300];
unsigned int temp_status=0;
//LeonTodo     
//void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void)
 void as3911Isr(void)
{
	u8 irqStatus_temp[3];
	u32 timeout_M1=0;
    do
    {
        u32 irqStatus = 0;
        		
        AS3911_IRQ_CLR();
		
        as3911ContinuousRead1(AS3911_REG_IRQ_MAIN, (u8*) &irqStatus_temp[0], 3);
		irqStatus = buff1[0];//|((unsigned int)(buff1[1])<<8);
		temp_status = buff1[1];
		temp_status =(temp_status<<8);
		irqStatus=irqStatus+temp_status;

		//	AS3911_IRQ_CLR();
		as3911InterruptStatus |= irqStatus & as3911InterruptMask;
		timeout_M1++;
		if(timeout_M1>80000)	break;

		
    } while (AS3911_IRQ_IS_SET());
}

/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/

EmvPicc_t picca;
EmvPicc_t piccb;
extern signed int emvTypeACardPresent(void);
extern signed int emvTypeBCardPresent(void);
extern signed short int  emvCollisionDetection(EmvPicc_t *picc);
extern signed short int emvTypeAAnticollision(EmvPicc_t *picc);
extern signed short int emvActivate(EmvPicc_t *picc);
extern signed short int emvTypeBAnticollision(EmvPicc_t *picc);
extern  unsigned short int emvConvertCarrierCyclesToMilliseconds(unsigned long  num_cycles);
//extern void sleepMilliseconds(unsigned short int milliseconds);
extern signed short int emvInitLayer4(EmvPicc_t *picc);
extern short int emvTypeBActivation(EmvPicc_t *picc);
extern void sleepMilliseconds(unsigned short int milliseconds);
//extern void SendString(unsigned char * p_u8String);


#define EMV_T_P                 2


unsigned int TypeArearch(void)
{
   	
		unsigned long sfgtCycles;
		unsigned short int  sfgtMilliseconds; 
	//	unsigned char uid1[10],i;	
									
		as3911InterruptStatus=0;
		
		//SendString((unsigned char *)"TypeA search...\r\n");
	    emvTypeACardPresent();

        sleepMilliseconds(EMV_T_P);
	    emvTypeAAnticollision(&picca);

//        SendString("uid=");
        
      /*  for(i=0;i<10;i++)
        {
           uid1[i]=picca.uid[i];
        
        } */
        
		//print_buf(uid1,10);
//		SendString("\r\n");
			   
	    emvActivate(&picca);

			   		if(picca.sfgi > 0)
	        {
	            sfgtCycles = (4096UL + 384) << picca.sfgi;
	            sfgtMilliseconds = emvConvertCarrierCyclesToMilliseconds(sfgtCycles);
	            sleepMilliseconds(sfgtMilliseconds);
	        }


	   emvInitLayer4(&picca);
			  

	   return 0;
}

unsigned int TypeBrearch(void)
{
   	
	unsigned long sfgtCycles;
	unsigned short int  sfgtMilliseconds; 
	emvTypeBAnticollision(&piccb);
	sleepMilliseconds(EMV_T_P);
	emvTypeBActivation(&piccb);
	

	if(piccb.sfgi > 0)
	{
		sfgtCycles = (4096UL + 384) << piccb.sfgi;
		sfgtMilliseconds = emvConvertCarrierCyclesToMilliseconds(sfgtCycles);
		sleepMilliseconds(sfgtMilliseconds);
	}


	emvInitLayer4(&piccb);
	
	return 0;

}


