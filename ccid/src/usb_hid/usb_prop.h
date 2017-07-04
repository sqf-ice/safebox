/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V3.0.0
* Date               : 04/06/2009
* Description        : All processings related to Custom HID demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_PROP_H
#define __USB_PROP_H


#define ABORT  0x01
#define GET_CLOCK_FREQUENCIES  0x02
#define GET_DATA_RATES  0x03
//extern unsigned char flag_rate;
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _HID_REQUESTS
{
  GET_REPORT = 1,
  GET_IDLE,
  GET_PROTOCOL,

  SET_REPORT = 9,
  SET_IDLE,
  SET_PROTOCOL
} HID_REQUESTS;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CustomCCID_init(void);
void CustomHID_Reset(void);
void CustomCCID_Reset(void);
void CustomHID_SetConfiguration(void);

void CustomHID_GetConfiguration(void);
void CustomHID_GetInterface(void);
void CustomHID_SetInterface(void);
void CustomHID_GetStatus(void);
void CustomHID_ClearFeature(void);
void CustomHID_SetEndPointFeature(void);
void CustomHID_SetDeviceFeature(void);

void CustomHID_SetDeviceAddress (void);
void CustomHID_Status_In (void);
void CustomHID_Status_Out (void);
RESULT USB_Class_Data_Setup(uint8_t RequestNo);
RESULT CustomHID_Data_Setup(uint8_t);
RESULT CustomHID_NoData_Setup(uint8_t);
RESULT CustomHID_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *CustomHID_GetDeviceDescriptor(uint16_t );
uint8_t *CustomHID_GetConfigDescriptor(uint16_t);
uint8_t *CustomHID_GetStringDescriptor(uint16_t);
RESULT CustomHID_SetProtocol(void);
uint8_t *CustomHID_GetProtocolValue(uint16_t Length);
RESULT CustomHID_SetProtocol(void);
uint8_t *CustomHID_GetReportDescriptor(uint16_t Length);
uint8_t *CustomHID_GetHIDDescriptor(uint16_t Length);


/* Exported define -----------------------------------------------------------*/
//#define CustomHID_GetConfiguration          NOP_Process
////#define CustomHID_SetConfiguration          NOP_Process
//#define CustomHID_GetInterface              NOP_Process
//#define CustomHID_SetInterface              NOP_Process
//#define CustomHID_GetStatus                 NOP_Process
//#define CustomHID_ClearFeature              NOP_Process
//#define CustomHID_SetEndPointFeature        NOP_Process
//#define CustomHID_SetDeviceFeature          NOP_Process
////#define CustomHID_SetDeviceAddress          NOP_Process

#define REPORT_DESCRIPTOR                  0x22

#endif /* __USB_PROP_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
