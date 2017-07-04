/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V3.0.0
* Date               : 04/06/2009
* Description        : All processings related to Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "usb_bot.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  

#include <string.h>	
#include "global.h"	



extern Bulk_Only_CBW  CBW;
extern uint8_t Bot_State;


volatile unsigned char wReport[7] = {0x02,0x00,0x02,0xaa,0x55,0xff,0x03}; //== get report 错误处理码
volatile unsigned int tOffset = 0;	//==发送偏移量
extern volatile unsigned int hid_set_report_flag;	//==hid set report /usb_hid.c
//extern volatile unsigned int hid_report_process;	//==hid report process /usb_hid.c
extern struct _UsbMod usbMsg;	   					//== usb_hid.c
//unsigned char flag_rate=0;
uint8_t *Get_Clock_Frequencies(uint16_t Length);
uint8_t *Get_Rats(uint16_t Length);
volatile unsigned int rCount = 0;	//==接收字节计数（用于续传）guohaibo 20110118
//===============================================================================//
uint32_t ProtocolValue;
/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */
extern ONE_DESCRIPTOR Device_Descriptor;
extern ONE_DESCRIPTOR Config_Descriptor;
extern ONE_DESCRIPTOR String_Descriptor[6];

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    CustomCCID_init,
	CustomCCID_Reset,
    CustomHID_Status_In,
    CustomHID_Status_Out,
	USB_Class_Data_Setup,
    CustomHID_NoData_Setup,
    CustomHID_Get_Interface_Setting,
    CustomHID_GetDeviceDescriptor,
    CustomHID_GetConfigDescriptor,
    CustomHID_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    CustomHID_GetConfiguration,
    CustomHID_SetConfiguration,
    CustomHID_GetInterface,
    CustomHID_SetInterface,
    CustomHID_GetStatus,
    CustomHID_ClearFeature,
    CustomHID_SetEndPointFeature,
    CustomHID_SetDeviceFeature,
    CustomHID_SetDeviceAddress
  };


/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CustomHID_init.
* Description    : Custom HID init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomCCID_init(void)
{
	pInformation->Current_Configuration = 0;
	/* Connect the device */
	PowerOn();
	/* USB interrupts initialization */
	_SetISTR(0);               /* clear pending interrupts */
	wInterrupt_Mask = IMR_MSK;
	_SetCNTR(wInterrupt_Mask); /* set interrupts mask */
	
	bDeviceState = UNCONNECTED;
}


/**************************************************************************
 ****函数名称:	USB_Hal_BusReset
 ****入口参数:	void
 ****出口参数: 	void
 ****说    明: 	USB总线复位
 **************************************************************************/
void CustomCCID_Reset(void)			 
{
    pInformation->Current_Configuration = 0;
	pInformation->Current_Feature = Config_Descriptor.Descriptor[7];
	pInformation->Current_Interface = 0;
	SetBTABLE(BTABLE_ADDRESS);

	/* Initialize Endpoint 0 */
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxStatus(ENDP0, EP_TX_STALL);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	Clear_Status_Out(ENDP0);
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	SetEPRxValid(ENDP0);

	/* Initialize Endpoint 1 */
   SetEPType(ENDP1, EP_BULK);
   SetEPTxAddr(ENDP1, ENDP1_TXADDR);
   //SetEPTxCount(ENDP1, VIRTUAL_COM_PORT_DATA_SIZE);
   SetEPTxStatus(ENDP1, EP_TX_NAK);
   SetEPRxStatus(ENDP1, EP_RX_DIS);


	/* Initialize Endpoint 2 */
/*	SetEPType(ENDP2, EP_BULK);
	SetEPTxAddr(ENDP2, ENDP2_TXADDR);
	SetEPRxAddr(ENDP2, ENDP2_RXADDR);
	Clear_Status_Out(ENDP2);
	SetEPTxCount(ENDP2, Device_Property.MaxPacketSize);
	SetEPRxCount(ENDP2, 64);
	SetEPRxStatus(ENDP2, EP_RX_VALID);
	SetEPTxStatus(ENDP2, EP_TX_NAK);  */


    /* Initialize Endpoint 3 */
  SetEPType(ENDP3, EP_BULK);
  SetEPRxAddr(ENDP3, ENDP3_RXADDR);
  SetEPRxCount(ENDP3, 64);
  SetEPRxStatus(ENDP3, EP_RX_VALID);
  SetEPTxStatus(ENDP3, EP_TX_DIS);

  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);
	/* Initialize Endpoint 4 */
/*	SetEPType(ENDP4, EP_BULK);
	SetEPTxAddr(ENDP4, ENDP4_TXADDR);
	SetEPRxAddr(ENDP4, ENDP4_RXADDR);
	Clear_Status_Out(ENDP4);
	SetEPTxCount(ENDP4, Device_Property.MaxPacketSize);
	SetEPRxCount(ENDP4, 64);
	SetEPRxStatus(ENDP4, EP_RX_VALID);
	SetEPTxStatus(ENDP4, EP_TX_NAK);


 */

 /*   SetEPType(ENDP5, EP_INTERRUPT);
    SetEPTxAddr(ENDP5, ENDP5_TXADDR);
    Clear_Status_Out(ENDP5);
    SetEPTxCount(ENDP5, 4);
	SetEPTxStatus(ENDP5, EP_TX_NAK);


	bDeviceState = ATTACHED;
	/* Set this device to response on default address */
	SetDeviceAddress(0);

	  CBW.dSignature = BOT_CBW_SIGNATURE;
  Bot_State = BOT_IDLE;

  bDeviceState = ATTACHED;

} /* End of USB_Hal_BusReset	*/

/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Udpade the device state to configured and command the ADC 
*                  conversion.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
unsigned char bDeviceState_time=0;
void CustomHID_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0)
  {
    /* Device configured */
	bDeviceState_time++;
	if(bDeviceState_time==1)
   		 bDeviceState = CONFIGURED;
    
    /* Start ADC1 Software Conversion */ 
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  }
}

/*******************************************************************************
* Function Name  : CustomHID_SetDeviceAddress.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : CustomHID_Status_In.
* Description    : Joystick status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Status_In(void)
{
}

/*******************************************************************************
* Function Name  : CustomHID_Status_Out
* Description    : Joystick status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Status_Out (void)
{
}

/**************************************************************************
 ****函数名称:	USB_Class_Data_Setup
 ****入口参数:	uint8_t ReruestNo
 ****出口参数: 	RESULT
 ** 				USB_UNSUPPORT						不支持
 ** 				USB_SUCCESS
 ****说    明: 	CCID 类请求 Setup 包处理
 ** 			根据CCID规范 类请求有三种：
 ** 			ABORT										01
 ** 			GET_CLOCK_FREQUENCIES						02
 **************************************************************************/
RESULT USB_Class_Data_Setup(uint8_t RequestNo)
{
	uint8_t *(*CopyRoutine)(uint16_t);

	CopyRoutine = NULL;

	/*增加CCID 设备的3个控制端点请求*/ /*CCID GET_CLOCK_FREQUENCIES*/
	if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
	        && (RequestNo == GET_CLOCK_FREQUENCIES) && (pInformation->USBwValue == 0)
	       && (pInformation->USBwLength == 0x04))
	{

	    CopyRoutine = Get_Clock_Frequencies;
	}


	if (CopyRoutine == NULL)
	{
	    return USB_UNSUPPORT;
	}
	pInformation->Ctrl_Info.CopyData = CopyRoutine;
	pInformation->Ctrl_Info.Usb_wOffset = 0;
	(*CopyRoutine)(0);
	return USB_SUCCESS;
} 

void CustomHID_GetConfiguration(void)
{
	
}
void CustomHID_GetInterface(void)
{
	
}
void CustomHID_SetInterface(void)
{
	
}
void CustomHID_GetStatus(void)
{						  
	
}
void CustomHID_ClearFeature(void)
{
	
}
void CustomHID_SetEndPointFeature(void)
{
		
}
void CustomHID_SetDeviceFeature(void)
{
	
}
//===============================================================================//
//== 函 数 名：CustomHID_SetReport
//== 功    能：
//== 说    明：
//== 时    间：20101230
//== 入口参数：Length
//== 出口参数：
//== 返 回 值：
//===============================================================================//
uint8_t *CustomHID_SetReport(uint16_t Length) 
{ 
	uint32_t  wOffset; 
	
	wOffset = pInformation->Ctrl_Info.Usb_wOffset; 
	if (Length == 0) 
	{ 
		pInformation->Ctrl_Info.Usb_wLength = 1024+256+20+5-wOffset;   //==1300+5
		return 0; 
	} 
//======================单包发所有数据模式====================
//==usbMsg.rCount = (pInformation->Ctrl_Info.Usb_wLength);
//	usbMsg.rCount = (pInformation->Ctrl_Info.Usb_wOffset)+64; //==ok
//	return (usbMsg.rBuf + wOffset); 

//======================多包续传模式=========================
	if (usbMsg.rCount == 0)
	{	
		rCount = 0;
		usbMsg.rCount = 0;
	}else{		
		rCount= usbMsg.rCount;					
	}
	usbMsg.rCount += 64;
	rCount += wOffset; 
	return (usbMsg.rBuf + rCount);
}

//===============================================================================//
//== 函 数 名：CustomHID_GetReport
//== 功    能：
//== 说    明：
//== 时    间：20101230
//== 入口参数：Length
//== 出口参数：
//== 返 回 值：
//===============================================================================//
uint8_t *CustomHID_GetReport(uint16_t Length) 
{ 
	uint32_t  wOffset; 

	wOffset = pInformation->Ctrl_Info.Usb_wOffset; 
	if (Length == 0) 
	{ 
		pInformation->Ctrl_Info.Usb_wLength = 1024+256+20+5-wOffset;	//==1300+5 - wOffset; 
		return 0; 
	} 

	if(usbMsg.tCount > pInformation->USBwLength)
	{
		(pInformation->Ctrl_Info.Usb_wLength) = pInformation->USBwLength;
		usbMsg.tCount -= pInformation->USBwLength;		
//		hid_report_process = 1;					//==还有数据
	}else
	{
		(pInformation->Ctrl_Info.Usb_wLength) = usbMsg.tCount;
		tOffset = 0;	//==数据偏移量清零
	} 

	tOffset += (pInformation->Ctrl_Info.Usb_wLength);		//==数据偏移
	return (usbMsg.tBuf + wOffset);
}
//===============================================================================//
//== 函 数 名：CustomHID_GetReport_Waiting
//== 功    能：GetReport 数据没处理完或者没有发set report命令时 处理函数
//== 说    明：
//== 时    间：20101231
//== 入口参数：Length
//== 出口参数：
//== 返 回 值：
//===============================================================================//
uint8_t *CustomHID_GetReport_Waiting(uint16_t Length) 
{ 
	uint32_t  wOffset; 
	
	wOffset = pInformation->Ctrl_Info.Usb_wOffset; 
	if (Length == 0) 
	{ 
		pInformation->Ctrl_Info.Usb_wLength = 512-wOffset;//256 - wOffset; 
		return 0; 
	} 
	(pInformation->Ctrl_Info.Usb_wLength) = 7;
	return (uint8_t *)(wReport + wOffset);
}
//===============================================================================//

/*******************************************************************************
* Function Name  : CustomHID_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT CustomHID_Data_Setup(uint8_t RequestNo)
{
	uint8_t *(*CopyRoutine)(uint16_t);
	
	CopyRoutine = NULL;	   
	
	if ((RequestNo == GET_DESCRIPTOR)
	  && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
	  && (pInformation->USBwIndex0 == 0))
	{
		tOffset = 0;	//==数据偏移量清零
		if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
		{
//		  	CopyRoutine = CustomHID_GetReportDescriptor;
		}
		else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
		{
//		  	CopyRoutine = CustomHID_GetHIDDescriptor;
		}
	
	} /* End of GET_DESCRIPTOR */
	
	/*** GET_PROTOCOL ***/
	else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
	       && RequestNo == GET_PROTOCOL)
	{
		tOffset = 0;	//==数据偏移量清零
		CopyRoutine = CustomHID_GetProtocolValue;
	}

//== SET_REPORT	   21 09 00 03 00 00 20 00
  	else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))&& RequestNo == SET_REPORT)
	{
//		hid_set_report_flag = 1;
//		hid_report_process = 0;
		tOffset = 0;	//==数据偏移量清零
		CopyRoutine = CustomHID_SetReport;		 		
	}
//== GET_REPORT	 a1 01 00 03 00 00 20 00
	else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))&& RequestNo == GET_REPORT)					
	{ 
//	    if (hid_report_process)						//==数据处理完
//		{
//			hid_set_report_flag =0;		//==ep0 control
//			hid_report_process =0;
			CopyRoutine = CustomHID_GetReport;			
//	    }else
//		{ 
//			return USB_NOT_READY;
//			hid_report_process =0;
			CopyRoutine = CustomHID_GetReport_Waiting;
//		}
					
	}	
//===============================================================================//
  	if (CopyRoutine == NULL)
  	{
    	return USB_UNSUPPORT;
  	}

  	pInformation->Ctrl_Info.CopyData = CopyRoutine;
//  pInformation->Ctrl_Info.Usb_wOffset = 0;  
	pInformation->Ctrl_Info.Usb_wOffset = tOffset; //==数据偏移量
  	(*CopyRoutine)(0); 	
  	return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_NoData_Setup
* Description    : handle the no data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT CustomHID_NoData_Setup(uint8_t RequestNo)
{
// if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
 //     && (RequestNo == SET_PROTOCOL))
 // {
 //   return CustomHID_SetProtocol();
//  }

//  else
 // {
    return USB_UNSUPPORT;
 // }
}

/*******************************************************************************
* Function Name  : CustomHID_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : CustomHID_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : CustomHID_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *CustomHID_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 > 6)
  {
    return NULL;
  }
  else 
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}
/*******************************************************************************
* Function Name  : Get_Clock_Frequencies
* Description    : Handle the Get Max Lun request.
* Input          : uint16_t Length.
* Output         : None.
* Return         : None.
*******************************************************************************/
extern const uint8_t clock_frequencies[4];
uint8_t *Get_Clock_Frequencies(uint16_t Length)
{
    if (Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = sizeof(clock_frequencies);
        return 0;
    }
    else
    {
        return((uint8_t*)clock_frequencies);
    }
}



/*******************************************************************************
* Function Name  : CustomHID_GetReportDescriptor.
* Description    : Gets the HID report descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
//uint8_t *CustomHID_GetReportDescriptor(uint16_t Length)
//{
//  return Standard_GetDescriptorData(Length, &CustomHID_Report_Descriptor);
//}

/*******************************************************************************
* Function Name  : CustomHID_GetHIDDescriptor.
* Description    : Gets the HID descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
//uint8_t *CustomHID_GetHIDDescriptor(uint16_t Length)
//{
//  return Standard_GetDescriptorData(Length, &CustomHID_Hid_Descriptor);
//}

/*******************************************************************************
* Function Name  : CustomHID_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT CustomHID_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_SetProtocol
* Description    : Joystick Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT CustomHID_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protcol value.
*******************************************************************************/
uint8_t *CustomHID_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
