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

/*! \file emv_hal.c
 *
 * \author Oliver Regenfelder
 *
 * \brief EMV module RFID hardware abstraction layer.
 */

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

#include "emv_hal.h"

#include "../AS3911/errno.h"

#include "emv_standard.h"
#include "../AS3911/as3911.h"
//#include "../config/CCM3310/as3192hal/sleep.h"


//#define FALSE 0    //false 
//#define TRUE  1 //true    
void SoftCloRf(void);
void SoftOpenRf(void);
/*
******************************************************************************
* DEFINES
******************************************************************************
*/
s8 emvHalResetField(void);
/*! AS3911 operation control register transmit enable bit mask. */
#define EMV_HAL_REG_OPCONTROL_TXEN_BIT    0x08

/*!
 *****************************************************************************
 * Receiver dead time (in carrier cycles) for the ISO14443A protocol.
 * See [CCP v2.01, PCD 4.8.1.3].
 * Calculation:
 *
 * 1108: Deaftime if the last transmitted data bis was logic '1'
 *
 *  276: Time from the rising pulse of the pause of the logic '1'
 *       (i.e. the timepoint to measure the deaftime from), to the
 *       actual end of the EOF sequence (the point where the MRT starts).
 *       Please note that the AS3911 uses the ISO14443-2 definition
 *       where the EOF consists of logic '0' followed by sequence Y.
 * 
 *   64: Adjustment for the MRT timer jitter. Currently the MRT timer
 *       will have any timeout between the set timeout and the set timout
 *       + 64 cycles.
 *****************************************************************************
 */
#define EMV_HAL_ISO14443A_RECEIVER_DEADTIME     (1108 - 276 - 64)

/*!
 *****************************************************************************
 * Receiver dead time (in carrier cycles) for the ISO14443B protocol.
 * See [CCP v2.01, PCD 4.8.1.3].
 *
 * 1024: TR0_MIN
 *
 *  340: Time from the rising edge of the EoS to the starting point
 *       of the MRT timer (sometime after the final high part of the
 *       EoS is completed).
 * 
 *   64: Adjustment for the MRT timer jitter. Currently the MRT timer
 *       will have any timeout between the set timeout and the set timout
 *       + 64 cycles.
 *****************************************************************************
 */
//#define EMV_HAL_ISO14443B_RECEIVER_DEADTIME     (1024 - 340 - 64)
#define EMV_HAL_ISO14443B_RECEIVER_DEADTIME     (1008 - 340 - 64)    //AS3911_1526.patch

/*!
 *****************************************************************************
 * Adjustment from the timeout requested by the caller to the timeout
 * value which has to be used for the AS3911.
 *****************************************************************************
 */
#define EMV_HAL_TYPE_A_TIMEOUT_ADJUSTMENT    (1024+1024)  // 512

/*!
 *****************************************************************************
 * Adjustment from the timeout requested by the caller to the timeout
 * value which has to be used for the AS3911.
 *****************************************************************************
 */
#define EMV_HAL_TYPE_B_TIMEOUT_ADJUSTMENT   6360      //(6360*8*4)测试B通过的  //(6360*4)00--12都测过了  // (6360*10)

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

/*! Stores the currently active standard. */
static EmvHalStandard_t emvioActiveStandard;




//u8 emvioIso14443bModulationLevelX[] = { 105, 163, 198, 214, 224 };
//u8 emvioIso14443bModulationLevelY[] = { 116, 126, 140, 146, 150 };


//u8 emvioIso14443bModulationLevelX[] = { 87, 107, 123, 134, 137 };
//u8 emvioIso14443bModulationLevelY[] = { 224, 225, 228, 229, 232 };


//仿真器参数 at BOTC
//u8 emvioIso14443bModulationLevelX[] = { 135, 151, 155, 176, 189 };
//u8 emvioIso14443bModulationLevelY[] = { 235, 236, 236, 237, 238 };



//板子参数 at Cashway
//u8 emvioIso14443bModulationLevelX[] = { 78, 79, 85, 87, 110 };
//u8 emvioIso14443bModulationLevelY[] = { 186, 186, 187, 187, 189 };

//国芯实验板
//u8 emvioIso14443bModulationLevelX[] = { 0x4B, 0x4B, 0x4C, 0x4D, 0x4E };
//u8 emvioIso14443bModulationLevelY[] = { 0xA8, 0xA8, 0xAA, 0xAE, 0xAE };


//CW Dev Broad  Room21
//u8 emvioIso14443bModulationLevelX[] = { 0x4c, 0x4c, 0x4c, 0x4c, 0x4c };
//u8 emvioIso14443bModulationLevelY[] = { 0xAC, 0xAC, 0xAC, 0xAC, 0xAC };

//CW Dev Broad  room3
//u8 emvioIso14443bModulationLevelX[] = { 0x56, 0x58, 0x58, 0x58, 0x58 };
//u8 emvioIso14443bModulationLevelY[] = { 0xA0, 0xA2, 0xA4, 0xA4, 0xA4 };

//u8 emvioIso14443bModulationLevelX[5] = { 0x5A, 0x5A, 0x5A, 0x5A, 0x5A };
//u8 emvioIso14443bModulationLevelY[5] = { 0xAE, 0xAE, 0xAE, 0xAE, 0xAE };


//u8 emvioIso14443bModulationLevelX[5] = { 0x8B, 0x8B, 0x8B, 0x8B, 0x8B };
//u8 emvioIso14443bModulationLevelY[5] = { 0x88, 0x88, 0x88, 0x88, 0x88 };

// M3# 
//u8 emvioIso14443bModulationLevelX[] = { 0xCE, 0xCE, 0xCE, 0xCE, 0xCE };
//u8 emvioIso14443bModulationLevelY[] = { 0xB8, 0xB8, 0xB8, 0xB8, 0xB8 };


// M4#
//u8 emvioIso14443bModulationLevelX[] = { 0x48, 0x48, 0x48, 0x48, 0x48 };
//u8 emvioIso14443bModulationLevelY[] = { 0xAC, 0xAC, 0xAC, 0xAC, 0xAC };


//板子参数 at BOTC
u8 emvioIso14443bModulationLevelX[] = { 0xd8, 0xd8, 0xd9, 0xd8, 0xd8 };
u8 emvioIso14443bModulationLevelY[] = { 0xc6, 0xc6, 0xc7, 0xc7, 0xc6 };

static AS3911ModulationLevelMode_t emvHalTypeBModulationLevelMode = AS3911_MODULATION_LEVEL_FIXED;
static const void *emvHalTypeBModulationLevelModeData = NULL;


AS3911ModulationLevelTable_t emvioIso14443bModulationLevelTable
        = { sizeof(emvioIso14443bModulationLevelX),
            &emvioIso14443bModulationLevelX[0],
            &emvioIso14443bModulationLevelY[0]
          };



/**@}*/

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
 * \brief Convert AS3911 module error codes to EMV IO module error codes.
 *
 * \param as3911errorCode AS3911 module error code.
 * \return EMV IO module error code.
 *****************************************************************************
 */
static s16 emvioConvertErrorCode(s16 as3911ErrorCode);

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

s8 emvHalSetAs3911TypeBModulationMode(AS3911ModulationLevelMode_t modulationLevelMode, const void *modulationLevelModeData)
{
	emvHalTypeBModulationLevelMode = AS3911_MODULATION_LEVEL_FROM_AMPLITUDE;//2;  //modulationLevelMode;
	emvHalTypeBModulationLevelModeData = modulationLevelModeData;
	
	return ERR_NONE;
}


//extern unsigned char ctrl_flag;
extern void sleepMilliseconds(unsigned short int milliseconds);

void emvHalSleepMilliseconds(u16 milliseconds)
{
    sleepMilliseconds(milliseconds);
}

s8 emvHalSetStandard(EmvHalStandard_t standard)
{
    unsigned char temp;
	if (EMV_HAL_TYPE_A == standard)
    {
        emvioActiveStandard = standard;

        /* Set the AS3911 to ISO14443-A, 106kBit/s rx/tx datarate. */
		as3911WriteRegister(AS3911_REG_MODE, 0x08);
        as3911WriteRegister(AS3911_REG_BIT_RATE, 0x00);

        /* Disable transmit without parity, disable receive without parity&crc, set
         * to default pulse width, and disable special anticollision mode.
         */
        as3911WriteRegister(AS3911_REG_ISO14443A_NFC, 0x00);

        /* OOK modulation, no tolerant processing of the first byte. */
        as3911WriteRegister(AS3911_REG_AUX, 0x00);

        /* Set the receiver deadtime. */
        as3911SetReceiverDeadtime(EMV_HAL_ISO14443A_RECEIVER_DEADTIME);

        /* Set the frame delay time to the minimum allowed PCD frame delay time. */
        as3911SetFrameDelayTime(EMV_FDT_PCD_MIN);

        /* Disable dynamic adjustment of the modulation level. */
        as3911SetModulationLevelMode(AS3911_MODULATION_LEVEL_FIXED, NULL);
    }
    else if (EMV_HAL_TYPE_B == standard)
    {
        emvioActiveStandard = standard;
	//	mvHalSetAs3911TypeBModulationMode(1, (void *)0);
        /* Set the AS3911 to ISO14443-B, 106kBit/s rx/tx datarate. */
        as3911WriteRegister(AS3911_REG_MODE, 0x10);
        as3911WriteRegister(AS3911_REG_BIT_RATE, 0x00);

        /* Configure AM modulation for ISO14443B. */
        as3911WriteRegister(AS3911_REG_AUX, 0x20);

		as3911WriteRegister(AS3911_REG_AM_MOD_DEPTH_CONF, 0x1c);
		as3911ExecuteCommand(0xD7);
	   	as3911ReadRegister(0x25,&temp);
	/*    as3911ReadRegister(AS3911_REG_AM_MOD_DEPTH_CONF,&temp);
		temp|=0x80;
		as3911WriteRegister(AS3911_REG_AM_MOD_DEPTH_CONF, temp);
        as3911WriteRegister(AS3911_REG_RFO_AM_ON_LEVEL, emvioIso14443bModulationLevelY[0]);
	    //as3911WriteRegister(AS3911_REG_RFO_AM_ON_LEVEL, 0xE9); */
	
	
	
	//if((ctrl_flag&0x20)==0)
	//{
     //   as3911WriteRegister(AS3911_REG_RFO_AM_OFF_LEVEL, 0x80);
	//}
	//else
	//{
	    as3911WriteRegister(AS3911_REG_RFO_AM_OFF_LEVEL, 0x00);
	
	//}
	



        /* Set the receiver deadtime. */
        as3911SetReceiverDeadtime(EMV_HAL_ISO14443B_RECEIVER_DEADTIME);

        /* Set the frame delay time to the minimum allowed PCD frame delay time. */
        as3911SetFrameDelayTime(EMV_FDT_PCD_MIN);

        /* Enable dynamic adjustment of the modulation level. */
       // as3911SetModulationLevelMode(emvHalTypeBModulationLevelMode, emvHalTypeBModulationLevelModeData);
		as3911SetModulationLevelMode(emvHalTypeBModulationLevelMode, &emvioIso14443bModulationLevelTable);
    }
    else
    {
        /* Handle unkown parameter error here. */
        return EMV_HAL_ERR_PARAM;
    }

    return EMV_HAL_ERR_OK;
}

s8 emvHalSetErrorHandling(EmvHalErrorHandling_t errorHandling)
{
    if (EMV_HAL_PREACTIVATION_ERROR_HANDLING == errorHandling)
    {
        /* Disable higher layer EMV exception processing. */
        as3911EnableEmvExceptionProcessing(FALSE);
        as3911SetTransmissionErrorThreshold(0);

        /* Enable detailed anticollision detection. */
        as3911ModifyRegister(AS3911_REG_ISO14443A_NFC, 0x01, 0x01);
    }
    else if (EMV_HAL_LAYER4_ERROR_HANDLING == errorHandling)
    {
        /* Errornous frames <= 4 bytes in length shall not be considered
         * transmission errors.
         */
        as3911SetTransmissionErrorThreshold(4);
        as3911EnableEmvExceptionProcessing(TRUE);

        /* Disable detailed anticollision detction.
         * This autmatically enables the enhanced SOF detection.
         */ 
        // as3911ModifyRegister(AS3911_REG_ISO14443A_NFC, 0x01, 0x00);
    }
    else
    {
        /* Handle unkown parameter error here. */
        return EMV_HAL_ERR_PARAM;
    }

    return EMV_HAL_ERR_OK;
}

s8 emvHalActivateField(bool_t activateField)
{
    s8 error = ERR_NONE;

    if (activateField)
        error |= as3911ModifyRegister(AS3911_REG_OP_CONTROL, EMV_HAL_REG_OPCONTROL_TXEN_BIT, EMV_HAL_REG_OPCONTROL_TXEN_BIT);
    else
        error |= as3911ModifyRegister(AS3911_REG_OP_CONTROL, EMV_HAL_REG_OPCONTROL_TXEN_BIT, 0x00);

    if (ERR_NONE == error)
        return ERR_NONE;
    else
        return ERR_IO;
}


void SoftCloRf(void)
{

  emvHalActivateField(FALSE);

}

void SoftOpenRf(void)
{

  emvHalActivateField(TRUE);

}


bool_t emvHalFieldIsActivated()
{
    u8 regOpcontrol = 0;

    if (ERR_NONE != as3911ReadRegister(AS3911_REG_OP_CONTROL, &regOpcontrol))
        return FALSE;

    if (regOpcontrol & EMV_HAL_REG_OPCONTROL_TXEN_BIT)
        return TRUE;
    else
        return FALSE;
}


extern int emvTypeA;
extern int emvTypeB;

s8 emvHalResetField()
{
    s8 error = ERR_NONE;



    emvHalSleepMilliseconds(EMV_T_RESETDELAY);   //AS3911_1558.patch
    error |= emvHalActivateField(FALSE);
    emvHalSleepMilliseconds(EMV_T_RESET);
    error |= emvHalActivateField(TRUE);

	emvTypeA=0;
	emvTypeB=0;

    if (ERR_NONE == error)
        return ERR_NONE;
    else
        return ERR_IO;
}


unsigned long EMV_HAL_TYPE_B_TIMEOUT_ADJUSTMENT_value=0;
unsigned char testboct;


//extern unsigned char ctrl_flag; 

s8 emvHalTransceive(const u8 *request, size_t requestLength, u8 *response
    , size_t maxResponseLength, size_t *responseLength, u32 timeout, EmvHalTransceiveMode_t transceiveMode)
{
    s16 error = AS3911_NO_ERROR;

    /* Adjust timeout by the delay between the start of card modulation
     * and the SoR interrupt of the AS3911.
     */
    if (EMV_HAL_TYPE_A == emvioActiveStandard)
        timeout += EMV_HAL_TYPE_A_TIMEOUT_ADJUSTMENT;
    else if (EMV_HAL_TYPE_B == emvioActiveStandard)
	{	
//		 if(ctrl_flag&0x80)
		 {
			 timeout +=  EMV_HAL_TYPE_B_TIMEOUT_ADJUSTMENT*8*4; 
		 }
	//	 else
		 {
          //  timeout +=  EMV_HAL_TYPE_B_TIMEOUT_ADJUSTMENT;
		 }	
	}

    if (EMV_HAL_TRANSCEIVE_WITH_CRC == transceiveMode)
    {
        error = as3911Transceive(request, requestLength, response, maxResponseLength,
                    responseLength, timeout, AS3911_CRC_TO_FIFO);
                    //SendString((unsigned char *)"-w \r\n");
    }
    else if (EMV_HAL_TRANSCEIVE_WITHOUT_CRC == transceiveMode)
    {
        error = as3911Transceive(request, requestLength, response, maxResponseLength,
                    responseLength, timeout,
                    AS3911_IGNORE_CRC | AS3911_TRANSMIT_WITHOUT_CRC);
         
    }
    else if (EMV_HAL_TRANSCEIVE_WUPA == transceiveMode)
    {
        error = as3911Transceive(NULL, 0, response, maxResponseLength,
                    responseLength, timeout,
                    AS3911_TRANSMIT_WUPA);
    }
    else
    {


	//if(testboct==1 )
	//{
	
	//	 testboct=2;
	
	//}
        /* Handle unkown parameter error here. */
        return EMV_HAL_ERR_PARAM;
    }


//		if(testboct==1 )
	{
	
//		 testboct=2;
	
	}

    return emvioConvertErrorCode(error);
}

/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/

static s16 emvioConvertErrorCode(s16 as3911ErrorCode)
{
    if (AS3911_NO_ERROR == as3911ErrorCode)
        return EMV_HAL_ERR_OK;
    else if (AS3911_TIMEOUT_ERROR == as3911ErrorCode)
        return EMV_HAL_ERR_TIMEOUT;
    else if (AS3911_OVERFLOW_ERROR == as3911ErrorCode)
        return EMV_HAL_ERR_OVERFLOW;
    else
        return EMV_HAL_ERR_ENCODING;
}


