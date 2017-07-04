#include "IFD.H"
#include "stm32f10x_iwdg.h"



//CS-I2C_SDA,  RST-I2C_SCL,  A0-ISODAT, CLK-ISOCLK, DATA-ISORST
/*#define  CS_LOW  I2C_PDR &= 0xFD;	//CS=0
#define  CS_HIGH I2C_PDR |= 0x02;	//CS=0

#define  RST_LOW  I2C_PDR &= 0xFE;	//RST=0
#define  RST_HIGH I2C_PDR |= 0x01;	//RST=1

#define  A0_LOW  USIPDR &= 0xFB;	//A0=0
#define  A0_HIGH USIPDR |= 0x04;	//A0=1

#define  SCL_LOW  USIPDR &= 0xFD;	//SCL=0
#define  SCL_HIGH USIPDR |= 0x02;	//SCL=1

#define  SI_LOW  USIPDR &= 0xFE;	//SI=0
#define  SI_HIGH USIPDR |= 0x01;	//SI=1*/

//CS- GPIO17,  RST-GPIO19 ,  A0-GPIO13, CLK-GPIO7, DATA-GPIO8

	




	
	

#define  CS_LOW    GPIO_WriteBit(LCD_CS_OUT_GPIO, LCD_CS_OUT_PIN, Bit_RESET);
#define  CS_HIGH   GPIO_WriteBit(LCD_CS_OUT_GPIO, LCD_CS_OUT_PIN, Bit_SET);

#define  RST_LOW  GPIO_WriteBit(LCD_RET_OUT_GPIO, LCD_RET_OUT_PIN, Bit_RESET);//(GPIODATA &= (~(1 << 19)))  	//RST=0
#define  RST_HIGH GPIO_WriteBit(LCD_RET_OUT_GPIO, LCD_RET_OUT_PIN, Bit_SET);//(GPIODATA |= (1 << 19))	//RST=1

#define  A0_LOW  GPIO_WriteBit(LCD_CD_OUT_GPIO, LCD_CD_OUT_PIN, Bit_RESET);//(GPIODATA &= (~(1 << 13)))  //A0=0
#define  A0_HIGH GPIO_WriteBit(LCD_CD_OUT_GPIO, LCD_CD_OUT_PIN, Bit_SET);//(GPIODATA |= (1 << 13))	//A0=1

#define  SCL_LOW  GPIO_WriteBit(LCD_SCK_OUT_GPIO, LCD_SCK_OUT_PIN, Bit_RESET);//(GPIODATA &= (~(1 << 7)))  	//SCL=0
#define  SCL_HIGH GPIO_WriteBit(LCD_SCK_OUT_GPIO, LCD_SCK_OUT_PIN, Bit_SET);//(GPIODATA |= (1 << 7))	//SCL=1

#define  SI_LOW  GPIO_WriteBit(LCD_MOSI_OUT_GPIO, LCD_MOSI_OUT_PIN, Bit_RESET);//(GPIODATA &= (~(1 << 8)))	//SI=0
#define  SI_HIGH GPIO_WriteBit(LCD_MOSI_OUT_GPIO, LCD_MOSI_OUT_PIN, Bit_SET);//(GPIODATA |= (1 << 8))	//SI=1
/*

#define  SCL_LOW  GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);//(GPIODATA &= (~(1 << 7)))  	//SCL=0
#define  SCL_HIGH GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);//(GPIODATA |= (1 << 7))	//SCL=1

#define  SI_LOW  GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);//(GPIODATA &= (~(1 << 8)))	//SI=0
#define  SI_HIGH GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);//(GPIODATA |= (1 << 8))	//SI=1
*/


