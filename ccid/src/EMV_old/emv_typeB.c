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

/*! \file emv_typeB.c
 *
 * \author Oliver Regenfelder
 *
 * \brief Type B specific PICC functions.
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

#include <stddef.h>

#include "emv_hal.h"
#include "emv_prelayer4.h"
#include "emv_poll.h"
#include "emv_main.h"
#include "emv_typeB.h"
#include "emv_standard.h"
#include "emv_error_codes.h"

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

extern unsigned char EMVTestUsed;
extern unsigned short int EMVSendLength;

s16 emvTypeBCardPresent()
{
    u8 wupb[3];
    u8 atqb[13+2];
    s8 error = EMV_HAL_ERR_OK;
    size_t responseLength = 0;

    emvHalSetStandard(EMV_HAL_TYPE_B);
    emvHalSetErrorHandling(EMV_HAL_PREACTIVATION_ERROR_HANDLING);

    /* Setup WUPB command according to section 6.3.1 of the EMV standard. */
    wupb[0] = 0x05;
    wupb[1] = 0x00;
    wupb[2] = 0x08;

    error = emvHalTransceive(wupb, sizeof(wupb), atqb, sizeof(atqb), &responseLength, EMV_FWT_ATQB_PCD, EMV_HAL_TRANSCEIVE_WITH_CRC);

    /* Any response shall be taken as a card presence indication. */
	/*
    if (EMV_HAL_ERR_TIMEOUT == error)
        return 0;
    else
        return 1;
		*/
	if(EMVTestUsed==3)
	{
		if (EMV_HAL_ERR_TIMEOUT == error)
		      return 0;
		else
		      return 1;

	}
	else
	{
		if(error==-3)
		{
			error=2;
		
		}
	}
	return error;

}
extern unsigned char buf_typeAB[20];
unsigned int VSAM_len=0;
s16 emvTypeBAnticollision1(EmvPicc_t *picc,unsigned char *buf,unsigned int *len)
{
    u8 wupb[3],i,i_len;
    u8 atqb[13+2];
    s8 error = EMV_HAL_ERR_OK;
    size_t responseLength = 0;

    emvHalSetStandard(EMV_HAL_TYPE_B);
    emvHalSetErrorHandling(EMV_HAL_PREACTIVATION_ERROR_HANDLING);

    /* Setup WUPB command according to section 6.3.1 of the EMV standard. */
	if((buf[0] == 0x05)&&(buf[1] == 0x00)&&(buf[2] == 0x00))
	{
	   for(i=0;i<3;i++)
		{
			if(i%2){
				wupb[0] = 0x05;wupb[1] = 0x00;wupb[2] = 0x08;  	//==WUPB				
			}else{
				wupb[0] = 0x05;wupb[1] = 0x00;wupb[2] = 0x00; 		//==REQB	
			}
//	    wupb[0] = 0x05;
//	    wupb[1] = 0x00;
//	    wupb[2] = 0x08;
	
	
	    error = emvPrelayer4Transceive(wupb, sizeof(wupb), atqb, sizeof(atqb), &responseLength,
                EMV_FWT_ATQB_PCD, EMV_HAL_TRANSCEIVE_WITH_CRC);
		if(error==0)
		{	
				VSAM_len = responseLength;
//		TypeB_flag =1;
		for(i_len=0;i_len<14;i_len++)
			buf_typeAB[i_len]=atqb[i_len];
			break;
			}
	}
	}
    /* Any response shall be taken as a card presence indication. */
	/*
    if (EMV_HAL_ERR_TIMEOUT == error)
        return 0;
    else
        return 1;
		*/
	if(EMVTestUsed==3)
	{
		if (EMV_HAL_ERR_TIMEOUT == error)
		      return 0;
		else
		      return 1;

	}
	else
	{
		if(error==-3)
		{
			error=2;
		
		}
	}
	return error;

}



//extern unsigned char TypeB_flag;
s16 emvTypeBAnticollision(EmvPicc_t *picc)
{
    u8 wupb[3],i_len;
    u8 atqb[13 + 2];
    s8 error = EMV_ERR_OK;
    size_t index = 0;
    size_t responseLength = 0;

    emvHalSetStandard(EMV_HAL_TYPE_B);
    emvHalSetErrorHandling(EMV_HAL_PREACTIVATION_ERROR_HANDLING);

    /* Setup WUPB command according to section 6.3.1 of the EMV standard. */
    wupb[0] = 0x05;
    wupb[1] = 0x00;
    wupb[2] = 0x08;

    error = emvPrelayer4Transceive(wupb, sizeof(wupb), atqb, sizeof(atqb), &responseLength,
                EMV_FWT_ATQB_PCD, EMV_HAL_TRANSCEIVE_WITH_CRC);

	if (error == EMV_ERR_OK)
	{
		VSAM_len = responseLength;
//		TypeB_flag =1;
		for(i_len=0;i_len<14;i_len++)
			buf_typeAB[i_len]=atqb[i_len];
	}
	if(error == EMV_ERR_OK)
	{
		if((buf_typeAB[1]==0)&&(buf_typeAB[2]==0)&&(buf_typeAB[3]==0)&&(buf_typeAB[4]==0))
			return 0x95;
	}
	else
	{
//		TypeB_flag=0;
	}
    if (error == EMV_ERR_TIMEOUT)
        return EMV_ERR_TIMEOUT;
    else if (error != EMV_ERR_OK)
        return EMV_ERR_COLLISION;
    else if (responseLength != (12 + 2))
        return EMV_ERR_PROTOCOL;

    if (atqb[0] != 0x50)
        return EMV_ERR_PROTOCOL;

    /* Copy the received PUPI into the EMVPicc uid field. */
    for (index = 0; index < 4; index++)
        picc->uid[index] = atqb[1 + index];
    picc->uidLength = 4;

    /* Parse bitrate definition fields. */
    picc->dPiccPcd = (atqb[9] & 0x70) >> 4;
    picc->dPcdPicc = (atqb[9] & 0x07);

    /* Parse and check FSC field. */
    picc->fsci = atqb[10] >> 4;

    //if (picc->fsci < EMV_FSCI_MIN_PCD)
    //    return EMV_ERR_PROTOCOL;
     if (picc->fsci > 8)
        picc->fsci = 8;

    /* Parse and check ISO14443-4 conformance bit  and b4 bit . */
//    if ((atqb[10] & 0x01) != 0x01)
//	 if ((atqb[10] & 0x09) != 0x01)   //AS3911_1526.patch
	
 //       return EMV_ERR_PROTOCOL;		//		 ����֤�޸�

    /* Parse and check FWI. */
    picc->fwi = atqb[11] >> 4;
    if (picc->fwi > EMV_FWI_MAX_PCD)
        //return EMV_ERR_PROTOCOL;
	    picc->fwi = EMV_FWI_DEFAULT;    //AS3911_1559.patch

    /* Parse and check SFGI. */
    if (responseLength == (13 + 2))
    {
        /* Received extented ATQB. */
        picc->sfgi = atqb[12] >> 4;

        if (picc->sfgi > EMV_SFGI_MAX_PCD)
            //return EMV_ERR_PROTOCOL;
			picc->sfgi = EMV_SFGI_DEFAULT;   //AS3911_1557.patch
    }
    else
        picc->sfgi = EMV_SFGI_DEFAULT;

    picc->activate = emvTypeBActivation;
    picc->remove = emvTypeBRemove;

    return 0;
}


//extern unsigned char ctrl_flag; 

short int emvTypeBActivation(EmvPicc_t *picc)
{
    u8 attrib[9];
    u8 response[1+2];
    s8 error = EMV_ERR_OK;
    size_t responseLength = 0;
    u32 timeoutInCarrierCycles = 0;
    size_t index = 0;

#ifdef EVM_DETAILEDCOMPRINT_DEBUG
	    EMVSendLength=29;
        emvDisplayString("EMV: TypeB Activation... \r\n");
#endif

    
    emvHalSetErrorHandling(EMV_HAL_LAYER4_ERROR_HANDLING);

    attrib[0] = 0x1D;

    for (index = 0; index < 4; index++)
        attrib[1 + index] = picc->uid[index];

    attrib[5] = 0x00;

    /* Set FSDI and select 106 kBit/s datarate in both directions. */
    attrib[6] = EMV_FSDI_MIN_PCD;
    attrib[7] = 0x01;
    attrib[8] = 0x00;

    /* Calculate timeout from FWI in milliseconds. */
	
	if(picc->fwi==8)
	{
      timeoutInCarrierCycles = (U32_C(2048L*2) + 384) << picc->fwi;
	}
	else if(picc->fwi==7)
	{
	
	   timeoutInCarrierCycles = (U32_C(2048L*2) + 384*5) << picc->fwi;
	}
	else
	{
	
	   timeoutInCarrierCycles = (U32_C(2048L*4) + 384*5) << picc->fwi;
	}
	
	
	if(picc->fwi==0x0E)
	{
	
//    	ctrl_flag|=0x40;
	}
	

	response[0]=0xff;
	response[1]=0xff;
	response[2]=0xff;

    error = emvPrelayer4Transceive(attrib, 9, response, sizeof(response), &responseLength,
                timeoutInCarrierCycles, EMV_HAL_TRANSCEIVE_WITH_CRC);

	
//    ctrl_flag &=0xBF;
		
    if (error != EMV_ERR_OK)
        return error;

    /* The Attrib response must have at least 1 databyte and 2 crc bytes. Any
     * shorter response without transmission errors (but with possible CRC errors)
     * must be considered a protocl error accroding to FIME testcase TB 306.12
     * failure description.
     */
    if (responseLength < 3)		  
        return EMV_ERR_PROTOCOL;

    /* The CID must be 0000b. */
    if ((response[0] & 0x0F) != 0x00)
        return EMV_ERR_PROTOCOL;

	
//	ctrl_flag|=0x80;
	
    return EMV_ERR_OK;
}

short int emvTypeBActivation1(EmvPicc_t *picc,unsigned char *attrib,unsigned int responseLength)
{
//    u8 attrib[9];
    u8 response[13];
    s8 error = EMV_ERR_OK,i;
  //  size_t responseLength = 0;
    u32 timeoutInCarrierCycles = 0;
//    size_t index = 0;

#ifdef EVM_DETAILEDCOMPRINT_DEBUG
	    EMVSendLength=29;
        emvDisplayString("EMV: TypeB Activation... \r\n");
#endif

    
    emvHalSetErrorHandling(EMV_HAL_LAYER4_ERROR_HANDLING);


    /* Calculate timeout from FWI in milliseconds. */
	
	if(picc->fwi==8)
	{
      timeoutInCarrierCycles = (U32_C(2048L*2) + 384) << picc->fwi;
	}
	else if(picc->fwi==7)
	{
	
	   timeoutInCarrierCycles = (U32_C(2048L*2) + 384*5) << picc->fwi;
	}
	else
	{
	
	   timeoutInCarrierCycles = (U32_C(2048L*4) + 384*5) << picc->fwi;
	}
	
	
	if(picc->fwi==0x0E)
	{
	
//    	ctrl_flag|=0x40;
	}
	

	response[0]=0xff;
	response[1]=0xff;
	response[2]=0xff;

    error = emvPrelayer4Transceive(attrib, 9, response, sizeof(response), &responseLength,
                timeoutInCarrierCycles, EMV_HAL_TRANSCEIVE_WITH_CRC);

	for(i=0;i<responseLength;i++)
		attrib[i]= response[i];
//    ctrl_flag &=0xBF;
		
    if (error != EMV_ERR_OK)
        return error;

    /* The Attrib response must have at least 1 databyte and 2 crc bytes. Any
     * shorter response without transmission errors (but with possible CRC errors)
     * must be considered a protocl error accroding to FIME testcase TB 306.12
     * failure description.
     */
 //   if (responseLength < 3)		  
 //       return EMV_ERR_PROTOCOL;

    /* The CID must be 0000b. */
    if ((response[0] & 0x0F) != 0x00)
        return EMV_ERR_PROTOCOL;

	
//	ctrl_flag|=0x80;
	
    return EMV_ERR_OK;
}

s16 emvTypeBRemove(EmvPicc_t *picc)
{
    u8 numWupbWithoutResponse;

    /* Do not ignore any transmission errors. */
    emvHalSetErrorHandling(EMV_HAL_PREACTIVATION_ERROR_HANDLING);

    /* Reset operating field. */
    emvHalResetField();

    /* Wait t_p milliseconds. */
    emvHalSleepMilliseconds(EMV_T_P);
    
    numWupbWithoutResponse = 0;
    while (numWupbWithoutResponse < 3)
    {
        if (emvStopRequestReceived())
            return EMV_ERR_STOPPED;

        if (emvTypeBCardPresent())
        {
            /* Wait t_p milliseconds. */
            emvHalSleepMilliseconds(EMV_T_P);

            numWupbWithoutResponse = 0;
        }
        else
            numWupbWithoutResponse++;
    }

    return EMV_ERR_OK;
}



/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/
