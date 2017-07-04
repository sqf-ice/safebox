#ifndef __MAGCARD_DRV_H
#define __MAGCARD_DRV_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

extern int RCP2Count; 

#define CLS_MAGCARD_PORT             GPIOD
#define CLS_MAGCARD_CLK              RCC_APB2Periph_GPIOD
#define CLS_MAGCARD_PIN              GPIO_Pin_6


#define CLS3_MAGCARD_PORT             GPIOD
#define CLS3_MAGCARD_CLK              RCC_APB2Periph_GPIOD
#define CLS3_MAGCARD_PIN              GPIO_Pin_5
/*
#define CLS_MAGCARD_PORT             GPIOE
#define CLS_MAGCARD_CLK              RCC_APB2Periph_GPIOE
#define CLS_MAGCARD_PIN              GPIO_Pin_4

#define CLS3_MAGCARD_PORT             GPIOE
#define CLS3_MAGCARD_CLK              RCC_APB2Periph_GPIOE
#define CLS3_MAGCARD_PIN              GPIO_Pin_8  */

#define RDP1_MAGCARD_PORT             GPIOD
#define RDP1_MAGCARD_CLK              RCC_APB2Periph_GPIOD
#define RDP1_MAGCARD_PIN              GPIO_Pin_0
/*
#define RCP1_MAGCARD_PORT             GPIOE
#define RCP1_MAGCARD_CLK              RCC_APB2Periph_GPIOE
#define RCP1_MAGCARD_PIN              GPIO_Pin_0
#define RCP1_MAGCARD_EXTI_LINE        EXTI_Line0
#define RCP1_MAGCARD_PORT_SOURCE      GPIO_PortSourceGPIOE
#define RCP1_MAGCARD_PIN_SOURCE       GPIO_PinSource0
#define RCP1_MAGCARD_IRQn             EXTI0_IRQn  */

#define RCP1_MAGCARD_PORT             GPIOD
#define RCP1_MAGCARD_CLK              RCC_APB2Periph_GPIOD
#define RCP1_MAGCARD_PIN              GPIO_Pin_5
#define RCP1_MAGCARD_EXTI_LINE        EXTI_Line5
#define RCP1_MAGCARD_PORT_SOURCE      GPIO_PortSourceGPIOD
#define RCP1_MAGCARD_PIN_SOURCE       GPIO_PinSource5                    
#define RCP1_MAGCARD_IRQn             EXTI9_5_IRQn 

#define RDP2_MAGCARD_PORT             GPIOD
#define RDP2_MAGCARD_CLK              RCC_APB2Periph_GPIOD
#define RDP2_MAGCARD_PIN              GPIO_Pin_0
/*
#define RCP2_MAGCARD_PORT             GPIOE
#define RCP2_MAGCARD_CLK              RCC_APB2Periph_GPIOE
#define RCP2_MAGCARD_PIN              GPIO_Pin_5
#define RCP2_MAGCARD_EXTI_LINE        EXTI_Line5
#define RCP2_MAGCARD_PORT_SOURCE      GPIO_PortSourceGPIOE
#define RCP2_MAGCARD_PIN_SOURCE       GPIO_PinSource5
#define RCP2_MAGCARD_IRQn             EXTI9_5_IRQn
*/
#define RDP3_MAGCARD_PORT             GPIOD
#define RDP3_MAGCARD_CLK              RCC_APB2Periph_GPIOD
#define RDP3_MAGCARD_PIN              GPIO_Pin_3

#define RCP3_MAGCARD_PORT             GPIOC
#define RCP3_MAGCARD_CLK              RCC_APB2Periph_GPIOC
#define RCP3_MAGCARD_PIN              GPIO_Pin_1
#define RCP3_MAGCARD_EXTI_LINE        EXTI_Line1
#define RCP3_MAGCARD_PORT_SOURCE      GPIO_PortSourceGPIOC
#define RCP3_MAGCARD_PIN_SOURCE       GPIO_PinSource1
#define RCP3_MAGCARD_IRQn             EXTI2_IRQn

/*Magcard Deal Define*/
/****************************************************** 
* Track Buffer Size
******************************************************/  
#define READ_TRACK1BUFSIZE	85
#define READ_TRACK2BUFSIZE	45
#define	READ_TRACK3BUFSIZE	115
 
/****************************************************** 
* Track Start Code & End Code Define
******************************************************/ 
#define READ_TRACK1_STARTCODE	0x45 
#define READ_TRACK1_ENDCODE	    0x7C 
#define READ_TRACK23_STARTCODE	0x0B 
#define READ_TRACK23_ENDCODE	0x1F 

/*Marcard T1 Struct Define*/
typedef struct _MagcardTTLT1	
{
	 unsigned int 	nReadTrack1Len;			/*receive data length;not contain LRC*/ 
	 unsigned int 	fTrack1HaveData;		/*flag of having data*/
	 unsigned char  chTrack1Buf1;           /* Track2 Data Buffer1 for recieve data; For get startcode and endcode */    
     unsigned char  chTrack1Buf2;           /* Track2 Data Buffer2 for recieve data; For get startcode and endcode */
	 unsigned char  chTrackIndex;          /* Track2 Data Buffer index for recieve data */ 
	 unsigned char  chTrack1Counter;        /* Track2 Data bit counter for recieve data  */  
	 unsigned char  fTrack1Start;           /* Track2 Start recieve data , it be set by Track2 Receive  */  
	 unsigned char  fTrack1Direction;       /* Track2 Swipe direction: 1:Forward / 0:Reserve , it be set by Track2Interrupt */     
	 unsigned char  fTrack1StartEnd; 	
	 unsigned char 	chTrack1Buffer[READ_TRACK1BUFSIZE];	/*receive data buffer*/	

}MagcardTTLT1;
/*Marcard T2 Struct Define*/
typedef struct _MagcardTTLT2	
{
	 unsigned int 	nReadTrack2Len;			/*receive data length;not contain LRC*/ 
	 unsigned int 	fTrack2HaveData;		/*flag of having data*/
	 unsigned char  chTrack2Buf1;           /* Track2 Data Buffer1 for recieve data; For get startcode and endcode */    
     unsigned char  chTrack2Buf2;           /* Track2 Data Buffer2 for recieve data; For get startcode and endcode */
	 unsigned char  chTrack2Index;          /* Track2 Data Buffer index for recieve data */ 
	 unsigned char  chTrack2Counter;        /* Track2 Data bit counter for recieve data  */  
	 unsigned char  fTrack2Start;           /* Track2 Start recieve data , it be set by Track2 Receive  */  
	 unsigned char  fTrack2Direction;       /* Track2 Swipe direction: 1:Forward / 0:Reserve , it be set by Track2Interrupt */     
	 unsigned char  fTrack2StartEnd; 	
	 unsigned char 	chTrack2Buffer[READ_TRACK2BUFSIZE];	/*receive data buffer*/	

}MagcardTTLT2;
/*Marcard T3 Struct Define*/
typedef struct _MagcardTTLT3	
{
	 unsigned int 	nReadTrack3Len;			/*receive data length;not contain LRC*/ 
	 unsigned int 	fTrack3HaveData;		/*flag of having data*/
	 unsigned char  chTrack3Buf1;           /* Track2 Data Buffer1 for recieve data; For get startcode and endcode */    
     unsigned char  chTrack3Buf2;           /* Track2 Data Buffer2 for recieve data; For get startcode and endcode */
	 unsigned char  chTrack3Index;          /* Track2 Data Buffer index for recieve data */ 
	 unsigned char  chTrack3Counter;        /* Track2 Data bit counter for recieve data  */  
	 unsigned char  fTrack3Start;           /* Track2 Start recieve data , it be set by Track2 Receive  */  
	 unsigned char  fTrack3Direction;       /* Track2 Swipe direction: 1:Forward / 0:Reserve , it be set by Track2Interrupt */     
	 unsigned char  fTrack3StartEnd; 	
	 unsigned char 	chTrack3Buffer[READ_TRACK3BUFSIZE];	/*receive data buffer*/	

}MagcardTTLT3;

extern MagcardTTLT1 magcardTTLT1;
extern MagcardTTLT2 magcardTTLT2;
extern MagcardTTLT3 magcardTTLT3;
 
/*
*	Used  by application layer
*	application layer :magcardTTL.c
*/

/*
*	Used these by drive layer 
*	application layer :magcard_drv.c
*/
extern void MagcardCLS_GPIOInit(void);
extern void MagcardCLS3_GPIOInit(void);
extern unsigned char ReadMagcardCLSDataBit(void);

/* Track 2 Functions */
extern void MagcardRDP2_GPIOInit(void);
extern void MagcardRCP2_GPIOInit(void);
extern void MagcardRCP2_IRQHandler(void);
extern unsigned char ReadMagcardRDP2DataBit(void);
extern void SetRCP2Interrupt(unsigned char status);	/*1 enable irq;  0 disable irq*/
/* Track 3 Functions */
extern void MagcardRDP3_GPIOInit(void);
extern void MagcardRCP3_GPIOInit(void);
extern void MagcardRCP3_IRQHandler(void);
extern unsigned char ReadMagcardRDP3DataBit(void);
extern void SetRCP3Interrupt(unsigned char status);	/*1 enable irq;  0 disable irq*/

#ifdef __cplusplus
}
#endif

#endif /* __MAGCARD_DRV_H */

