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

/*! \file emv_display.c
 *
 * \author Oliver Regenfelder
 *
 * \brief Methods to display EMV data on the GUI.
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

#include "emv_display.h"

#include <stddef.h>

//#include "logger.h"

#include "emv_layer4.h"
#include "emv_response_buffer.h"

//#include "../config/PCuart.h"


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

//#if USE_LOGGER == LOGGER_ON
//#define EMV_LOG dbgLog
//#else
//#define EMV_LOG(...)
unsigned short int EMVSendLength;

void   EMV_LOG(unsigned  char *h)
{
   unsigned short int i;
   i=0;
   while(1)
   {
   	  
//   	  PC_UART_SendString(&h[i],1);
   	  i++;
	  if(i==EMVSendLength) break;
   }
  
   EMVSendLength=0;
}

//#endif

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

void emvDisplayString(const char *string)
{
    EMV_LOG((unsigned  char *)string);

}

void emvDisplayByteArray(const u8 *array, size_t length)
{
   // size_t index = 0;
   // for (index = 0; index < length; index++)
   //     EMV_LOG("%hhx", array[index]);
   //unsigned short int i;

   //for(i=0;i<length;i++)
   //{
//      PC_UART_SendData((unsigned char *)array,length);

	  EMVSendLength=6;
      emvDisplayString("  \r\n");
   //}
}

void emvDisplayUid(const u8 *uid, size_t length)
{
    /*size_t index = length - 1;

    do
    {
        //EMV_LOG("%hhx", uid[index]);
		PC_UART_SendData((unsigned char *)&uid[index],1);

        index--;
    } while (index > 0);*/ 
}

void emvDisplayError(s16 errorCode)
{
    switch (errorCode)
    {
    case EMV_ERR_OK:
	    EMVSendLength=17;
        EMV_LOG((unsigned char *)"EMV: no error\r\n");
        break;
    case EMV_ERR_COLLISION:
	    EMVSendLength=24;
        EMV_LOG((unsigned char *)"EMV: collision error\r\n");
        break;
    case EMV_ERR_PROTOCOL:
	    EMVSendLength=23;
        EMV_LOG((unsigned char *)"EMV: protocol error\r\n");
        break;
    case EMV_ERR_TRANSMISSION:
	    EMVSendLength=27;
        EMV_LOG((unsigned char *)"EMV: transmission error\r\n");
        break;
    case EMV_ERR_TIMEOUT:
	    EMVSendLength=22;
        EMV_LOG((unsigned char *)"EMV: timeout error\r\n");
        break;
    case EMV_ERR_INTERNAL:
	    EMVSendLength=23;
        EMV_LOG((unsigned char *)"EMV: internal error\r\n");
        break;
    case EMV_ERR_STOPPED:
	    EMVSendLength=22;
        EMV_LOG((unsigned char *)"EMV: stopped error\r\n");
        break;
    default:
     	EMVSendLength=26;
        EMV_LOG((unsigned char *)"EMV: unkown error code\r\n");
    }
}

void emvDisplayMessage(s16 messageCode)
{
    switch (messageCode)
    {
    case EMV_M_POLLING:
	    EMVSendLength=20;
        EMV_LOG((unsigned char *)"EMV: Polling ...\r\n");
        break;
    case EMV_M_REMOVE_CARD:
	    EMVSendLength=24;
        EMV_LOG((unsigned char *)"EMV: Remove card ...\r\n");
        break;
    default:
	    EMVSendLength=28;
        EMV_LOG((unsigned char *)"EMV: unkown message code\r\n");
    }
}

void emvDisplayCAPDU(const u8 *apdu, size_t length)
{
   	EMVSendLength=16;
    emvDisplayString((const char *)"EMV: C-APDU \r\n");
    emvDisplayByteArray(apdu, length);
    //emvDisplayString("\n");
}

void emvDisplayRAPDU(const u8 *apdu, size_t length)
{
    EMVSendLength=16;
    emvDisplayString((const char *)"EMV: R-APDU \r\n");
    emvDisplayByteArray(apdu, length);
    //emvDisplayString("\n");
}

/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/
