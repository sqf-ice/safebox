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
 * PROJECT: AS911 firmware
 * $Revision: $
 * LANGUAGE: ANSI C
 */

/*! \file emv_main.c
 *
 * \author Oliver Regenfelder
 *
 * \brief EMV terminal application.
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

#include <stddef.h>

//#include "usb_hid_stream_appl_handler.h"

#include "emv_hal.h"
#include "emv_standard.h"
#include "emv_poll.h"
#include "emv_layer4.h"
#include "emv_display.h"
//#include  "../config/CCM3310/as3192hal/sleep.h"

//#define FALSE 0    //false 
//#define TRUE  1   //true    
//extern void ExternalClk_60M();


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

/*! Indicates whether a stop request has been received from the GUI or not. */
static volatile bool_t emvStopRequestReceivedFlag = FALSE;

void Set_emvStopRequestReceivedFlag(unsigned char flag)
{
	if(flag==1)
	{
	
		 emvStopRequestReceivedFlag= TRUE;
	}
	else
	{
		  emvStopRequestReceivedFlag= FALSE;
	
	}


}


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

/*!
 *****************************************************************************
 * \brief Convert 13.56 MHz carrier cycle numbers to milliseconds.
 *
 * \note \a numCarrierCycles must be <= 888720133 (65535 ms).
 *
 * \param numCarrierCycles Number of carrier cycles.

 * \return \a numCarrierCycles converted to milliseconds.
 *****************************************************************************
 */
u16 emvConvertCarrierCyclesToMilliseconds(u32 numCarrierCycles);

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

extern void sleepMilliseconds(unsigned short int milliseconds);
extern int emvTypeA;
//extern unsigned char ctrl_flag; 


extern  void   PC_UART_SendData(unsigned char *h, unsigned short int len);
s16 emvStartTerminalApplication(s16 (*application)(void))
{
    s16 error = EMV_ERR_OK;
 //   unsigned char temp[10];
    u32 sfgtCycles;
	u16 sfgtMilliseconds;
	/*temp[0]=0x56;
	temp[1]=0x03;
	temp[2]=0x23;
	temp[3]=0x99;
	temp[4]=0x58;
	temp[5]=0x21;  */

    /* Reset stop request received flag. */
    emvStopRequestReceivedFlag = FALSE;

    /* Implementation:
     * Error checking is done at the beginning of the while loop because
     * error handling is the same for all stages of the terminal main loop.
     * If any stage encounters an error it uses a continue statement to fall
     * through the rest of the while loop and reach the error handling code.
     */
    while(1)
    {
        EmvPicc_t picc;

//		ctrl_flag&=0x7F;
		
		
        if (EMV_ERR_STOPPED == error)
        {
            /* Received stop request, stop terminal main loop. */
            return EMV_ERR_STOPPED;
        }
        if (EMV_ERR_OK != error)
        {
            emvDisplayError(error);

            /* Reset field and continue with polling. */
            emvHalResetField();
        }

        /* Polling. */
        emvDisplayMessage(EMV_M_POLLING);

		
		//PC_UART_SendData(temp,5);
		
///	if((ctrl_flag&0x10)==0)
	{
   //     ExternalClk_40M();
	}
        error = emvPoll();
		
//	if((ctrl_flag&0x10)==0)
	{
//		ExternalClk_60M();
	}
	
        if (EMV_ERR_OK != error)
            continue;

        /* Anticollision. */
       sleepMilliseconds(EMV_T_P);
        error = emvCollisionDetection(&picc);
        if (EMV_ERR_OK != error)
            continue;

        /* Activation. */
        error = emvActivate(&picc);
        if (EMV_ERR_OK != error)
            continue;

        /* Wait for SFGT. */
        if(picc.sfgi > 0)
        {
			
			
	 if(emvTypeA==1)   //BCTC3
	 {
		 //if( picc.sfgi==14)
		// {
		//	 sfgtCycles =  (4096UL+1024UL+ 384) << picc.sfgi; 
		//	 sleepMilliseconds(sfgtMilliseconds);
		 //}
		 //else
		 //{
             sfgtCycles =  (4096UL + 384) << picc.sfgi;  // V1 (2048UL + 384) << picc.sfgi;  //(4096UL + 384) << picc.sfgi;
		 //}
	 }
	 else
	 {
	         sfgtCycles =  (128UL + 384) << picc.sfgi; 
	 }
	 
            sfgtMilliseconds  = emvConvertCarrierCyclesToMilliseconds(sfgtCycles);
            sleepMilliseconds(sfgtMilliseconds);
			
			
			 if(( picc.sfgi==14)&&(emvTypeA==1))
			 {
				  sleepMilliseconds(sfgtMilliseconds/2); 
			 }
        }

        /* Initialize layer 4. */
        error = emvInitLayer4(&picc);
        if (EMV_ERR_OK != error)
            continue;

        /* Start terminal application. */
        if (application != NULL)
            error = application();
        else
            error = EMV_ERR_OK;

        if (EMV_ERR_OK != error)
            continue;

        /* Card removal. */
        //emvDisplayMessage(EMV_M_REMOVE_CARD);
        error = emvRemove(&picc);
        if (EMV_ERR_OK != error)
            continue;
    }
}

bool_t emvStopRequestReceived()
{
    //ProcessIO();

    if (emvStopRequestReceivedFlag)
    {
        emvStopRequestReceivedFlag = FALSE;
        return TRUE;
    }
	
    return FALSE;
}

void emvStopTerminalApplication()
{
    emvStopRequestReceivedFlag = TRUE;
}


/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/

u16 emvConvertCarrierCyclesToMilliseconds(u32 num_cycles)
{
    return (num_cycles / 13560) + 1;
}








//extern s16 emvPrevalidationApplication(void);
extern s16 emvDigitalApplication(void);
unsigned char 	EMVTestUsed;
extern void	apdu_loop(void);
extern void Re_Download(void);
extern unsigned char  PC_UART_RecieveByte(void);
extern void PC_UART_SendByte (unsigned char ucData);
extern unsigned char 	T_tvar;
extern void SetAS3911_REG_RX_CONF3Register(void);

void   EMVLoopTest(void)
{


			        if(EMVTestUsed==0x01)
					 {
					 	//VICEnable(INTTIMER3,DISABLE);
					    //EMVTestUsed = 0;
						//apdu_loop();
						//VICEnable(INTTIMER3,ENABLE);
					   
					 }
					 else if(EMVTestUsed==0x02)
					 {
					 
					     /*while(1)
						  {
						     EMVTestUsed = PC_UART_RecieveByte();
							 if(EMVTestUsed==0x5A)
							 {
							     T_tvar=0x20;
							 	 PC_UART_SendByte(0x5A);
							 }
							 else if(EMVTestUsed==0xA5)
							 {
							     T_tvar=0x22;
							 	 PC_UART_SendByte(0xA5);
							 }
						   	 
							 Delay(300);
						   }
					 	 //EMVTestUsed = 0;
		                 //emvPoll();
					     */
						 
					 }
					 else if(EMVTestUsed==0x03)
					 {
						 
//						 	if((ctrl_flag&0x10)==0)
							{
//					           ExternalClk_60M();
							}
							
                            // SetAS3911_REG_RX_CONF3Register();
							
						  	//VICEnable(INTTIMER3,DISABLE);
						    emvStartTerminalApplication(emvDigitalApplication);
							 EMVTestUsed=0;
					 	    //VICEnable(INTTIMER3,ENABLE);
					 }
					 else if(EMVTestUsed==0xFE)
					 {
					 	//Re_Download();
					 
					 } 
			  

}


