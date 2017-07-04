/*===============================================================================//
//== 文 件 名：platform_config.h
//== author	 ：huyx@cashwaytech.com
//== 说    明：STM32F103VCT6平台IO资源定义
//== 修改日志：
//== 同恒银金融有限公司
//===============================================================================*/

#ifndef	__PLATFORM_CONFIG_H__
#define	__PLATFORM_CONFIG_H__

//============================ 定义 STM32F103VCT6 管脚连接的设备 ============================//
//====== LED 接口定义 ======
/*#define	LED1_PIN						GPIO_Pin_0				//== TIM2 CH1
#define	LED1_GPIO						GPIOA
#define	LED1_GPIO_CLK					RCC_APB2Periph_GPIOA

#define	LED2_PIN						GPIO_Pin_1				//== TIM3 CH4 Remap
#define	LED2_GPIO						GPIOB
#define	LED2_GPIO_CLK					RCC_APB2Periph_GPIOB

#define	LED3_PIN						GPIO_Pin_8				//== TIM4 CH3
#define	LED3_GPIO						GPIOB
#define	LED3_GPIO_CLK					RCC_APB2Periph_GPIOB

#define	LED0_PIN						GPIO_Pin_0				//== 板级LED指示灯
#define	LED0_GPIO						GPIOB
#define	LED0_GPIO_CLK					RCC_APB2Periph_GPIOB	  */
//====== END LED ======//

//====== BEEP 接口定义 ======
#define	BEEP_PIN						GPIO_Pin_4				//== 
#define	BEEP_GPIO						GPIOD
#define	BEEP_GPIO_CLK					RCC_APB2Periph_GPIOD
#define BeepOn()						GPIO_WriteBit(BEEP_GPIO, BEEP_PIN, Bit_SET)  		//==PB.5
#define BeepOff()						GPIO_WriteBit(BEEP_GPIO, BEEP_PIN, Bit_RESET)
//====== END BEEP ======

//====== SPI接口定义 ======
#define	SPI1_CLK						RCC_APB2Periph_SPI1
#define	SPI1_GPIO						GPIOA
#define	SPI1_GPIO_CLK					RCC_APB2Periph_GPIOA
#define	SPI1_PIN_CKL					GPIO_Pin_5
#define	SPI1_PIN_MISO					GPIO_Pin_6
#define	SPI1_PIN_MOSI					GPIO_Pin_7

#define	SPI2_CLK						RCC_APB1Periph_SPI2
#define	SPI2_GPIO						GPIOB
#define	SPI2_GPIO_CLK					RCC_APB2Periph_GPIOB
#define	SPI2_PIN_CKL					GPIO_Pin_13
#define	SPI2_PIN_MISO					GPIO_Pin_14
#define	SPI2_PIN_MOSI					GPIO_Pin_15
//====== END SPI ======//


//====== I2C接口定义 ======
#define	I2C1_CLK						RCC_APB1Periph_I2C1
#define	I2C1_GPIO						GPIOB
#define	I2C1_GPIO_CLK					RCC_APB2Periph_GPIOB
#define	I2C1_PIN_CKL					GPIO_Pin_6
#define	I2C1_PIN_SDA					GPIO_Pin_7

#define	I2C2_CLK						RCC_APB1Periph_I2C2
#define	I2C2_GPIO						GPIOB
#define	I2C2_GPIO_CLK					RCC_APB2Periph_GPIOB
#define	I2C2_PIN_CKL					GPIO_Pin_10
#define	I2C2_PIN_SDA					GPIO_Pin_11
//====== END LCD ======//

//====== LCD (I2C1)接口定义 ======
#define	LCD_PIN_CS						GPIO_Pin_0				//== LCD片选
#define	LCD_PIN_CS_GPIO					GPIOE
#define	LCD_PIN_CS_GPIO_CLK				RCC_APB2Periph_GPIOE

#define	LCD_PIN_LEDK					GPIO_Pin_1				//== LCD_LEDK
#define	LCD_PIN_LEDK_GPIO				GPIOE
#define	LCD_PIN_LEDK_GPIO_CLK			RCC_APB2Periph_GPIOE

//== IO 模拟串行通信==//
#define	LCD_PIN_CKL						GPIO_Pin_6
#define	LCD_PIN_CKL_GPIO				GPIOB
#define	LCD_PIN_CKL_GPIO_CLK			RCC_APB2Periph_GPIOB
#define	LCD_PIN_SDA						GPIO_Pin_7
#define	LCD_PIN_SDA_GPIO				GPIOB
#define	LCD_PIN_SDA_GPIO_CLK			RCC_APB2Periph_GPIOB

//====== END LCD ======//




//====== USB 接口定义 ======
#define	USB_CTL_PIN						GPIO_Pin_1				//== USB_CTL_PIN
#define	USB_CTL_PIN_GPIO				GPIOA
#define	USB_CTL_PIN_GPIO_CLK			RCC_APB2Periph_GPIOA
//====== END USB ======


#define	SPI2_PIN_NSS					GPIO_Pin_11
#define	SPI2_PIN_NSS_GPIO				GPIOD
#define	SPI2_PIN_NSS_GPIO_CLK			RCC_APB2Periph_GPIOD


#define AS3911_IRQ_IN_PIN                     GPIO_Pin_10
#define AS3911_IRQ_IN_GPIO                    GPIOD
#define AS3911_IRQ_IN_RCC                     RCC_APB2Periph_GPIOD

#define EXTI_LINE_RF_IRQ				EXTI_Line10
#define GPIO_PORT_SOURCE_RF_IRQ			GPIO_PortSourceGPIOD
#define GPIO_PIN_SOURCE_RF_IRQ			GPIO_PinSource10
//======================== END 定义 STM32F103VCT6 管脚连接的设备 ============================//

#endif

