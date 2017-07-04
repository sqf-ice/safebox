/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_conf.h
* Author             : MCD Application Team
* Version            : V3.0.0
* Date               : 04/06/2009
* Description        : Custom HID demo configuration file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF_H
#define __USB_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/
#define EP_NUM     (8)

/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */
/* buffer table base address */
#define BTABLE_ADDRESS      (0x00)

//== EP0  rx/tx buffer base address 
#define ENDP0_RXADDR        (0x40)
#define ENDP0_TXADDR        (0x80)

//== EP1  tx buffer base address 
#define ENDP1_TXADDR        (0xC0)
//#define ENDP1_RXADDR        (0x100)
//== EP2  tx buffer base address 
//#define ENDP2_TXADDR        (0xC0)//(0x140)	
//#define ENDP2_RXADDR        (0x100)//(0x180) 
//== EP3  tx buffer base address 	
//#define ENDP3_TXADDR        (0x140)//(0x1C0)	
#define ENDP3_RXADDR        (0x110) 
//== EP4  tx buffer base address 	
//#define ENDP4_TXADDR        (0x144)	
//#define ENDP4_RXADDR        (0x184) 
//== EP5  tx buffer base address 	
//#define ENDP5_TXADDR        (0x1C4)	
//#define ENDP5_RXADDR        (0x300) 
//== EP6  tx buffer base address 	
//#define ENDP6_TXADDR        (0x340)	
//#define ENDP6_RXADDR        (0x380) 
//== EP7  tx buffer base address 	
//#define ENDP7_TXADDR        (0x3C0)	
//#define ENDP7_RXADDR        (0x400)

/*-------------------------------------------------------------*/
/* -------------------   ISTR events  -------------------------*/
/*-------------------------------------------------------------*/
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )

/* CTR service routines */
/* associated to defined endpoints */
//#define  EP1_IN_Callback   NOP_Process
//#define  EP2_IN_Callback   NOP_Process
//#define  EP3_IN_Callback   NOP_Process
//#define  EP4_IN_Callback   NOP_Process
//#define  EP5_IN_Callback   NOP_Process
//#define  EP6_IN_Callback   NOP_Process
//#define  EP7_IN_Callback   NOP_Process
//
//#define  EP1_OUT_Callback   NOP_Process
//#define  EP2_OUT_Callback   NOP_Process
//#define  EP3_OUT_Callback   NOP_Process
//#define  EP4_OUT_Callback   NOP_Process
//#define  EP5_OUT_Callback   NOP_Process
//#define  EP6_OUT_Callback   NOP_Process
//#define  EP7_OUT_Callback   NOP_Process

#endif /*__USB_CONF_H*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

