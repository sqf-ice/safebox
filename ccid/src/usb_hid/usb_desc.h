/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V3.0.0
* Date               : 04/06/2009
* Description        : Descriptor Header for Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define CUSTOMHID_SIZ_HID_DESC                  0x09
#define CUSTOMHID_OFF_HID_DESC                  0x12

#define CUSTOMCCID_SIZ_DEVICE_DESC              18
#define CUSTOMCCID_SIZ_CONFIG_DESC               177
//#define CUSTOMHID_SIZ_REPORT_DESC               162
//#define CUSTOMCCID_SIZ_REPORT_DESC               27
#define CUSTOMCCID_SIZ_STRING_LANGID             4
#define CUSTOMCCID_SIZ_STRING_VENDOR             10
#define CUSTOMCCID_SIZ_STRING_PRODUCT            32
#define CUSTOMCCID_SIZ_STRING_SERIAL             26

//#define STANDARD_ENDPOINT_DESC_SIZE             0x09
/*************************************************************************
 * …Ë±∏√Ë ˆ∑˚
 */
typedef struct _device_descriptor_t
{
	uint8_t		bLength;              							// Size of this Descriptor in Bytes
	uint8_t		bDescriptorType;      							// Descriptor Type (=1)
	uint8_t		_bcdUSB[2];                        				// USB Spec Release Number in BCD
	uint8_t		bDeviceClass;         							// Device Class Code
	uint8_t		bDeviceSubClass;      							// Device Subclass Code
	uint8_t		bDeviceProtocol;      							// Device Protocol Code
	uint8_t		bMaxPacketSize0;      							// Maximum Packet Size for EP0
	uint8_t		_idVendor[2];                      				// Vendor ID
	uint8_t		_idProduct[2];                     				// Product ID
	uint8_t		_bcdDevice[2];                     				// Device Release Number in BCD
	uint8_t		iManufacturer;        							// Index of String Desc for Manufacturer
	uint8_t		iProduct;             							// Index of String Desc for Product
	uint8_t		iSerialNumber;        							// Index of String Desc for SerNo
	uint8_t		bNumConfigurations;   							// Number of possible Configurations
} device_descriptor_t;              // End of Device Descriptor Type


/*********************************************************************************************************
 * ≈‰÷√√Ë ˆ∑˚
 *********************************************************************************************************/
typedef struct _configuration_descriptor_t
{
	uint8_t		bLength;              							// Size of this Descriptor in Bytes
	uint8_t		bDescriptorType;     	 						// Descriptor Type (=2)
	uint8_t 	_wTotalLength[2];                  				// Total Length of Data for this Conf
	uint8_t		bNumInterfaces;       							// No of Interfaces supported by this Conf
	uint8_t		bConfigurationValue;  							// Designator Value for *this*  Configuration
	uint8_t		iConfiguration;       							// Index of String Desc for this Conf
	uint8_t		bmAttributes;         							// Configuration Characteristics (see below)
	uint8_t		bMaxPower;            							// Max. Power Consumption in this Conf (*2mA)
} configuration_descriptor_t;            // End of Configuration Descriptor Type
/*********************************************************************************************************
 * Ω”ø⁄√Ë ˆ∑˚
 *********************************************************************************************************/
typedef struct _interface_descriptor_t
{
	uint8_t		bLength;              							// Size of this Descriptor in Bytes
	uint8_t		bDescriptorType;      							// Descriptor Type (=4)
	uint8_t		bInterfaceNumber;     							// Number of *this* Interface (0..)
	uint8_t		bAlternateSetting;    							// Alternative for this Interface (if any)
	uint8_t		bNumEndpoints;        							// No of EPs used by this IF (excl. EP0)
	uint8_t		bInterfaceClass;      							// Interface Class Code
	uint8_t		bInterfaceSubClass;   							// Interface Subclass Code
	uint8_t		bInterfaceProtocol;   							// Interface Protocol Code
	uint8_t		iInterface;           							// Index of String Desc for this Interface
} interface_descriptor_t;                // End of Interface Descriptor Type


/*********************************************************************************************************
 * CCID ±Í◊º¿‡–Õ√Ë ˆ∑˚
 *********************************************************************************************************/
typedef struct _ccid_class_descriptor_t
{
	uint8_t		bLength;										// Size of this descriptor in bytes.
	uint8_t		bDescriptorType;								// Functional Descriptor type.
	uint8_t		_bcdCCID[2];									// Integrated Circuit(s) Cards Interface Devices (CCID)
																// Specification Release Number in Binary-Coded decimal (i.e., 2.10 is 0210h).
	uint8_t		bMaxSlotIndex;									// The index of the highest available slot on this device.
																// All slots are consecutive starting at 00h.
	uint8_t		bVlotageSupprot;								// This value indicates what voltages the CCID can supply to its slots.
																//		01h 5.0V
																//		02h 3.0V
																//		04h 1.8V
	uint8_t		_dwProtocols[4];								// Encodes the supported protocol types
																// 		00000001h = Protocol T=0
																//		00000002h = Protocol T=1
	uint8_t		_dwDefaultClock[4];								// Default ICC clock frequency in KHz
	uint8_t		_dwMaximumClock[4];								// Maximum supported ICC clock frequency in KHz.
	uint8_t		bNumClockSupported;								// The number of clock frequencies that are supported by the CCID.
	uint8_t		_dwDataRate[4];									// Default ICC I/O data rate in bps.
	uint8_t		_dwMaxDataRate[4];								// Maximum supported ICC I/O data rate in bps
	uint8_t		bNumDataRatesSupported;							// The number of data rates that are supported by the CCID
	uint8_t		_dwMaxIFSD[4];									// Indicates the maximum IFSD supported by CCID for protocol T=1.
	uint8_t		_dwSynchProtocols[4];							// encodes the supported protocol types.
																// 		0001h indicates support for the 2-wire protocol 1
																//		0002h indicates support for the 3-wire protocol 1
																//		0004h indicates support for the I2C protocol
	uint8_t		_dwMechanical[4];								// The value is a bitwise OR operation performed on the following values:
																//		00000000h No special characteristics
																//		00000001h Card accept mechanism
																//		00000002h Card ejection mechanism
																//		00000004h Card capture mechanism
																//		00000008h Card lock/unlock mechanism
	uint8_t		_dwFeatures[4];									// This value indicates what intelligent features the CCID has.
																// The value is a bitwise OR operation performed on the following values:
																//		00000000h No special characteristics
																//		00000002h Automatic parameter configuration based on ATR data
																//		00000004h Automatic activation of ICC on inserting
																//		00000008h Automatic ICC voltage selection
																//		00000010h Automatic ICC clock frequency change according to active parameters provided by the Host or self determined 3
																//		00000020h 3 Automatic baud rate change according to active parameters provided by the Host or self determined
																//		00000040h 4 Automatic parameters negotiation made by the CCID (use of warm or cold resets or PPS according to a manufacturer proprietary algorithm to select the communication parameters with the ICC)
																//		00000080h 4 Automatic PPS made by the CCID according to the active parameters
																//		00000100h CCID can set ICC in clock stop mode
																//		00000200h NAD value other than 00 accepted (T=1 protocol in use)
																//		00000400h Automatic IFSD exchange as first exchange (T=1 protocol in use)
																// Only one of the following values may be present to select a level of exhange:
																// 		00010000h TPDU level exchanges with CCID
																// 		00020000h Short APDU level exchange with CCID
																// 		00040000h Short and Extended APDU level exchange with CCID
																// 		If none of those values is indicated the level of exchange is character.
	uint8_t		_dwMaxCCIDMessageLength[4];						// For extended APDU level the value shall be between 261 + 10 (header) and 65544 +10,
																// otherwise the minimum value is the wMaxPacketSize of the Bulk-OUT endpoint.
	uint8_t		bClassGetResponse;								// Significant only for CCID that offers an APDU level for exchanges.
	uint8_t		bClassEnvelop;									// Significant only for CCID that offers an extended APDU level for exchanges.
	uint8_t		_wLcdLayout[2];									// Number of lines and characters for the LCD display used to send messages for PIN entry.
																//		XX: number of lines
																//		YY: number of characters per line.
																//		XXYY=0000h no LCD.
	uint8_t		bPINSupport;									// This value indicates what PIN support features the CCID has.
	uint8_t		bMaxCCIDBusySlots;								// Maximum number of slots which can be simultaneously busy.
}ccid_class_descriptor_t;
/*********************************************************************************************************
 * ∂Àµ„√Ë ˆ∑˚
 *********************************************************************************************************/
typedef struct _endpoint_descriptor_t
{
	uint8_t 	bLength;              							// Size of this Descriptor in Bytes
	uint8_t 	bDescriptorType;      							// Descriptor Type (=5)
	uint8_t 	bEndpointAddress;     							// Endpoint Address (Number + Direction)
	uint8_t 	bmAttributes;         							// Endpoint Attributes (Transfer Type)
	uint8_t 	_wMaxPacketSize[2];	               				// Max. Endpoint Packet Size
	uint8_t 	bInterval;            							// Polling Interval (Interrupt) ms
} endpoint_descriptor_t;                 // End of Endpoint Descriptor Type

/*********************************************************************************************************
 * CCID …Ë±∏≈‰÷√√Ë ˆ∑˚
 *********************************************************************************************************/
typedef struct _ccid_configuration_descriptor_t
{
	configuration_descriptor_t 	ccid_configuration_descriptor;
	//Contactless CARD configuration

	interface_descriptor_t 		ccid_interface_descriptor1;
	ccid_class_descriptor_t 	ccid_class_descriptor1;
	endpoint_descriptor_t 		ccid_endpoint_bulk_in_descriptor1;
	endpoint_descriptor_t 		ccid_endpoint_bulk_out_descriptor1;
	endpoint_descriptor_t 		ccid_endpoint_int_in_descriptor1;

    interface_descriptor_t      ccid_interface_descriptor2;
    ccid_class_descriptor_t     ccid_class_descriptor2;
    endpoint_descriptor_t       ccid_endpoint_bulk_in_descriptor2;
    endpoint_descriptor_t       ccid_endpoint_bulk_out_descriptor2;
    endpoint_descriptor_t       ccid_endpoint_int_in_descriptor2;

}ccid_configuration_descriptor_t;

#define _CCID_CONFIG_DESC_LEN				sizeof(ccid_configuration_descriptor_t)

/* Exported functions ------------------------------------------------------- */
//extern const uint8_t Desc_Device[CUSTOMCCID_SIZ_DEVICE_DESC];
//extern const uint8_t Desc_Ccid_Config[CUSTOMCCID_SIZ_CONFIG_DESC];
////extern const uint8_t CustomCCID_ReportDescriptor[CUSTOMCCID_SIZ_REPORT_DESC];
//extern const  String_LangID[CUSTOMCCID_SIZ_STRING_LANGID];
//extern const  String_Vendor[CUSTOMCCID_SIZ_STRING_VENDOR];
//extern const  String_Product[CUSTOMCCID_SIZ_STRING_PRODUCT];
//extern const  String_Serial[CUSTOMCCID_SIZ_STRING_SERIAL];


#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
