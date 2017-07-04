/*===============================================================================//
//== 文 件 名：usb_desc.c
//== author	 ：huyuxin
//== 说    明：STM32F103平台
//== 清华同方|同方锐安科技有限公司
//===============================================================================*/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h" 	

#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */  
const device_descriptor_t Desc_Device[] =
{
	sizeof(device_descriptor_t), // bLength
	USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
	0x10, //0x00,                       /*bcdUSB */					//== USB版本号
	0x01, //0x02,
	0x00,                       /*bDeviceClass*/
	0x00,                       /*bDeviceSubClass*/
	0x00,                       /*bDeviceProtocol*/
	0x40,                       /*bMaxPacketSize40*/		//== 端点0的最大包大小,这里设置64字节
	0xff,                       /*idVendor (0x0483)*/		//== 厂商编号VID，这里只是作为测试用，请使用自己的VID
	0x29,
	0x07,                       /*idProduct = 0x5750*/	   	//== 产品编号PID，这里只是作为测试用，请使用自己的PID 0001
	0x10,
	0x00,                       /*bcdDevice rel. 2.00*/		//== 设备出厂编号
	0x02,					    /*Device release number in BCD format*/
	0x01,                          /*Index of string descriptor describing
	                                          manufacturer */
	0x02,                          /*Index of string descriptor describing
	                                         product*/
	0x00,                          /*Index of string descriptor describing the
	                                         device serial number */
	0x01                        /*bNumConfigurations*/		//== 可能的配置数量
}; /* CustomCCID_DeviceDescriptor */

/*********************************************************************************************************
 * 配置描述符
 *********************************************************************************************************/

const ccid_configuration_descriptor_t Desc_Ccid_Config[] =
{

    0x09,   /* bLength: Configuation Descriptor size */
    0x02,   /* bDescriptorType: Configuration */
    32,

    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: */
    /*      Configuration value */
    0x00,   /* iConfiguration: */
    /*      Index of string descriptor */
    /*      describing the configuration */
    0xC0,   /* bmAttributes: */
    /*      bus powered */
    0x32,   /* MaxPower 100 mA */

    /******************** Descriptor of Mass Storage interface ********************/
    /* 09 */
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
    0x08,   /* bInterfaceClass: MASS STORAGE Class */
    0x06,   /* bInterfaceSubClass : SCSI transparent*/
    0x50,   /* nInterfaceProtocol */
    4,          /* iInterface: */
    /* 18 */
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    0x81,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */
    0x40,   //0x00  /*Maximum packet size (64 bytes) */
    0x00,	//0x01
    0x00,   /*Polling interval in milliseconds */
    /* 25 */
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x03,   /*Endpoint address (OUT, address 2) */
    0x02,   /*Bulk endpoint type */
    0x40,   // 0x00 /*Maximum packet size (64 bytes) */
    0x00,	// 0x01;
    0x00     /*Polling interval in milliseconds*/
    /*32*/

};



//==1.语言
/* USB String Descriptors (optional) */
struct
{
	uint8_t 	bLength;
	uint8_t		bDescriptorType;
	uint8_t		string[2];
}

const String_LangID =
{
		sizeof(String_LangID),										// Length of String descriptor
		USB_STRING_DESCRIPTOR_TYPE,									// DescriptorType
		0x09,0x04													// US English
}; 

//==2.厂商
#define STRING_VENDOR   "Cashway"
struct
{
	uint8_t 	bLength;
	uint8_t		bDescriptorType;
	uint8_t		string[(sizeof(STRING_VENDOR)-1)*2];
}const String_Vendor =
{
		(sizeof(String_Vendor)),
		USB_STRING_DESCRIPTOR_TYPE,
		'C',0x00,
		'a',0x00,
		's',0x00,
		'h',0x00,
		'w',0x00,
		'a',0x00,
		'y',0x00
};
//== 3.	产品
//#define STRING_PROCDUCT     "CWCCID Reader"
#define STRING_PROCDUCT     "UMASS SafeBox"

struct
{
	uint8_t 	bLength;
	uint8_t		bDescriptorType;
	uint8_t		string[(sizeof(STRING_PROCDUCT)-1)*2];
}const String_Product =
{
		sizeof(String_Product),
		USB_STRING_DESCRIPTOR_TYPE,
		'U',0x00,
		'M',0x00,
		'A',0x00,
		'S',0x00,
		'S',0x00,
		' ',0x00,
		'S',0x00,
		'a',0x00,
		'f',0x00,
		'e',0x00,
		'B',0x00,
		'o',0x00,
		'x',0x00	
};


//==4.Serial Number
#define STRING_SERIAL     "G2 V0.1"
struct
{
	uint8_t 	bLength;
	uint8_t		bDescriptorType;
	uint8_t		string[(sizeof(STRING_SERIAL)-1)*2];
}const String_Serial =
{
		sizeof(String_Serial),									// Length of Product String descriptor
		USB_STRING_DESCRIPTOR_TYPE,								// DescriptorType
		'G',0x00,												// Manufacturer
		'2',0x00,
		' ',0x00,
		'V',0x00,
		'0',0x00,
		'.',0x00,
		'1',0x00
};
/*********************************************************************************************************
 * 接口 字符串描述符
 * Interface = "FTsafe Smartcard Reader"
 *********************************************************************************************************/
#define STRING_INTERFACE_USER  "CWContact-C"     //"Key Reader"
struct
{
	uint8_t 	bLength;
	uint8_t		bDescriptorType;
	uint8_t		string[(sizeof(STRING_INTERFACE_USER)-1)*2];
}const String_Interface0 =
{
		sizeof(String_Interface0),
		USB_STRING_DESCRIPTOR_TYPE,
		'C',0x00,
		'W',0x00,
		'C',0x00,
		'o',0x00,
		'n',0x00,
		't',0x00,
		'a',0x00,
		'c',0x00,
        't',0x00,
        '-',0x00,
        'C',0x00
};

#define STRING_INTERFACE_CONTACTLESS  "RF1"//"CWContactless"  //"RF1 Reader"
struct
{
    uint8_t     bLength;
    uint8_t     bDescriptorType;
    uint8_t     string[(sizeof(STRING_INTERFACE_CONTACTLESS)-1)*2];
}const String_Interface1 =
{
        sizeof(String_Interface1),
        USB_STRING_DESCRIPTOR_TYPE,
		'R',0x00,
		'F',0x00,
		'1',0x00
  /*      'o',0x00,
        'n',0x00,
        't',0x00,
        'a',0x00,
        'c',0x00,
        't',0x00,
        'l',0x00,
        'e',0x00,
        's',0x00,
        's',0x00   */
};

ONE_DESCRIPTOR Device_Descriptor =
  {
  	(uint8_t *)Desc_Device,
	sizeof(device_descriptor_t)
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
  	(uint8_t *)Desc_Ccid_Config,
	sizeof(ccid_configuration_descriptor_t)
  };


ONE_DESCRIPTOR String_Descriptor[6] =
  {
  	(uint8_t *)&String_LangID,sizeof(String_LangID),
	(uint8_t *)&String_Vendor,sizeof(String_Vendor),
	(uint8_t *)&String_Product,sizeof(String_Product),
	(uint8_t *)&String_Serial,sizeof(String_Serial),
	(uint8_t *)&String_Interface0,sizeof(String_Interface0),
	(uint8_t *)&String_Interface1,sizeof(String_Interface1),
  };


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

const uint8_t clock_frequencies[4]={ 0xa0,0x0f,0x00,0x00};

//const uint8_t data_rates[160]={  0x01,0x3f,0x00,0x00,0x01,0x54,0x00,0x00,0x8d,0x5b,0x00,0x00,0x06,0x65,0x00,0x00,0x12,0x7a,0x00,0x00,
//							     0x02,0x7e,0x00,0x00,0xde,0x89,0x00,0x00,0x35,0x97,0x00,0x00,0x9D,0xA8,0x00,0x00,0x1B,0xb7,0x00,0x00,
//							     0x03,0xbd,0x00,0x00,0x0c,0xca,0x00,0x00,0xc7,0xe5,0x00,0x00,0x24,0xf4,0x00,0x00,0x04,0xfc,0x00,0x00,
//							     0xbc,0x13,0x01,0x00,0x63,0x34,0x01,0x00,0xb9,0x3c,0x01,0x00,0xac,0x53,0x01,0x00,0x36,0x6e,0x01,0x00,
//							     0x06,0x7a,0x01,0x00,0x18,0x94,0x01,0x00,0x22,0xaa,0x01,0x00,0x8f,0xcb,0x01,0x00,0x48,0xe8,0x01,0x00,
//							     0x82,0xfd,0x01,0x00,0x09,0x2e,0x02,0x00,0xc6,0x68,0x02,0x00,0x72,0x79,0x02,0x00,0x56,0xb1,0x02,0x00,
//							     0x6c,0xdc,0x02,0x00,0x0c,0xf4,0x02,0x00,0x0e,0x64,0x03,0x00,0x80,0xa9,0x03,0x00,0x90,0xd0,0x03,0x00,
//							     0x05,0xfb,0x03,0x00,0x15,0x16,0x05,0x00,0xad,0x62,0x05,0x00,0xd8,0xb8,0x05,0x00,0xae,0x52,0x08,0x00};



