/*===============================================================================//
//== 文 件 名：main.c
//== author	 ：huyx@cashwaytech.com
//== 说    明：STM32F103VCT6平台
//== 修改日志：
//== 恒银金融有限公司
//===============================================================================*/

#include "function.h"
#include "usb_lib.h"
#include "IFD.H"
#include "ccid.h"
#include "../spi/spi.h"
#include "stm32f10x_iwdg.h"
#include "usb_pwr.h"


extern volatile struct _golbal_var	gVar;		//==public.c
extern volatile struct _sys_param sys_param;	//==pubic.c 
extern void cmd_process(void);					//==command.c
extern unsigned int set_pps(unsigned char flag, unsigned char *pps); 	//==command.c 20110527
extern RESULT PowerOff(void);
extern void MagcardTTLGPIOInit(void);
void led1_on(void);
void led1_off(void);
void led2_on(void);
void led2_off(void);
void led3_on(void);
void led3_off(void);
void led4_on(void);
void led4_off(void);
void led5_on(void);
void led5_off(void);
unsigned char get_key(void);
unsigned char IC_inset(void);
extern void init_usb(void);						//==usb_hid.c
extern void Time5Config(void);
extern unsigned int smartcardpoweron(unsigned char slot); //==easywaycommand.c
void SAM1_init(void);
void SAM2_init(void);

//===============================================================================//
//== @brief  Configures the different system clocks.
//== @param  None
//== @retval : None
//===============================================================================//
void RCC_Configuration(void)	      
{
	SysTick_Config(SystemFrequency / 1000);		//== 1ms 系统滴答
	NVIC_SetPriority(SysTick_IRQn, 0x0);
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	#ifdef  VECT_TAB_RAM  
  		/* Set the Vector Table base location at 0x20000000 */ 
  		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
  		/* Set the Vector Table base location at 0x08000000 */ 
 	 	
	#ifndef  IAPLOAD 	
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
	#else	
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
	#endif  
//		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);  //==IAP 启动0x08000000 应用程序启动0x08002000 
		//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);  //==IAP_US 启动0x08000000 应用程序启动0x08004000   
	#endif
}




//===============================================================================//
//== 函 数 名 : IWDG_Configuration
//== 功    能 : 独立看门狗的配置
//== 说    明 :
//== 入口参数 : 无
//== 出口参数 : 无
//== 返 回 值 : 无
//===============================================================================//
void IWDG_Configuration(void)
{
  	IWDG_WriteAccessCmd(0x5555);
  	IWDG_SetPrescaler(0x03);
	IWDG_SetReload(0xf00);//(0xff0); 		  //0xea6   6S
  	IWDG_ReloadCounter();
  	IWDG_Enable(); 
}
											 
 void IWDG_Configuration_FingerPrint(void)
{
  	IWDG_WriteAccessCmd(0x5555);
  	IWDG_SetPrescaler(0x06);
	IWDG_SetReload(0xF00);//(0xff0); 		  //0xea6   6S
  	IWDG_ReloadCounter();
  	IWDG_Enable(); 
}	


//===============================================================================//
//== 函 数 名：init_usb_ctl
//== 功    能：初始化usb控制管脚
//== 说    明：
//== 时    间：20110317
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
void init_usb_ctl(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(USB_CTL_PIN_GPIO_CLK , ENABLE);
																	//== Configure the GPIO_LED pin
	GPIO_InitStructure.GPIO_Pin 	= USB_CTL_PIN;						//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 			//== 指定输出模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度
	GPIO_Init(USB_CTL_PIN_GPIO, &GPIO_InitStructure);
}

//===============================================================================//
//== 函 数 名：usb_ctl_on
//== 功    能：usb的DP脚上拉
//== 说    明：
//== 时    间：20110317
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
void usb_ctl_on(void)
{
	init_usb_ctl();
	GPIO_WriteBit(USB_CTL_PIN_GPIO, USB_CTL_PIN, Bit_SET);
}

void IC_solt_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_APB2PeriphClockCmd( SLOT_ICC_IN_RCC | SLOT_ICC1_IN_RCC|SLOT_ICC2_IN_RCC|RCC_APB2Periph_AFIO, ENABLE );


    /* 配置 SC_CARD0_IN_PIN */
    GPIO_InitStructure.GPIO_Pin = SLOT_ICC_IN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SLOT_ICC_IN_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SLOT_ICC1_IN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SLOT_ICC1_IN_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SLOT_ICC2_IN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SLOT_ICC2_IN_GPIO, &GPIO_InitStructure);

//	RCC_APB2PeriphClockCmd( SLOT_CD4053_SET1_RCC | SLOT_CD4053_SET2_RCC|RCC_APB2Periph_AFIO, ENABLE );

 //   GPIO_InitStructure.GPIO_Pin = SLOT_CD4053_SET1_PIN;
  //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //  GPIO_Init(SLOT_CD4053_SET1_GPIO, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = SLOT_CD4053_SET2_PIN;
  //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //  GPIO_Init(SLOT_CD4053_SET2_GPIO, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd( SLOT_CD4053_SET3_RCC | SLOT_CD4053_SET4_RCC|RCC_APB2Periph_AFIO, ENABLE );

    GPIO_InitStructure.GPIO_Pin = SLOT_CD4053_SET3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SLOT_CD4053_SET3_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SLOT_CD4053_SET4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SLOT_CD4053_SET4_GPIO, &GPIO_InitStructure);

 //	 GPIO_InitStructure.GPIO_Pin = SLOT_CD4053_SET6_PIN;
 //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //  GPIO_Init(SLOT_CD4053_SET6_GPIO, &GPIO_InitStructure);

 //	 GPIO_InitStructure.GPIO_Pin = SLOT_CD4053_SET5_PIN;
 //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //  GPIO_Init(SLOT_CD4053_SET5_GPIO, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd( VCC_ICC1_OUT_RCC | VCC_ICC2_OUT_RCC|RCC_APB2Periph_AFIO, ENABLE );

    GPIO_InitStructure.GPIO_Pin = VCC_ICC1_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VCC_ICC1_OUT_GPIO, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = VCC_ICC2_OUT_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(VCC_ICC2_OUT_GPIO, &GPIO_InitStructure);


}

void SAM1_init(void)
{
//	GPIO_WriteBit(SLOT_CD4053_SET1_GPIO, SLOT_CD4053_SET1_PIN, Bit_SET);
//	GPIO_WriteBit(SLOT_CD4053_SET2_GPIO, SLOT_CD4053_SET2_PIN, Bit_SET);
//	GPIO_WriteBit(VCC_ICC1_OUT_GPIO, VCC_ICC1_OUT_PIN, Bit_RESET);
}

void SAM2_init(void)
{
//	GPIO_WriteBit(SLOT_CD4053_SET1_GPIO, SLOT_CD4053_SET1_PIN, Bit_RESET);
//	GPIO_WriteBit(SLOT_CD4053_SET2_GPIO, SLOT_CD4053_SET2_PIN, Bit_RESET);
//	GPIO_WriteBit(VCC_ICC1_OUT_GPIO, VCC_ICC1_OUT_PIN, Bit_RESET);
}

void SOLT_init(void)
{
	GPIO_WriteBit(SLOT_CD4053_SET3_GPIO, SLOT_CD4053_SET3_PIN, Bit_RESET);
	GPIO_WriteBit(SLOT_CD4053_SET4_GPIO, SLOT_CD4053_SET4_PIN, Bit_RESET);
//	GPIO_WriteBit(SLOT_CD4053_SET3_GPIO, SLOT_CD4053_SET3_PIN, Bit_SET);
//	GPIO_WriteBit(SLOT_CD4053_SET4_GPIO, SLOT_CD4053_SET4_PIN, Bit_SET);
//	GPIO_WriteBit(VCC_ICC1_OUT_GPIO, VCC_ICC1_OUT_PIN, Bit_RESET);
}

void SOLT_init_sam(void)
{
	GPIO_WriteBit(SLOT_CD4053_SET3_GPIO, SLOT_CD4053_SET3_PIN, Bit_SET);
	GPIO_WriteBit(SLOT_CD4053_SET4_GPIO, SLOT_CD4053_SET4_PIN, Bit_SET);
//	GPIO_WriteBit(SLOT_CD4053_SET3_GPIO, SLOT_CD4053_SET3_PIN, Bit_SET);
//	GPIO_WriteBit(SLOT_CD4053_SET4_GPIO, SLOT_CD4053_SET4_PIN, Bit_SET);
//	GPIO_WriteBit(VCC_ICC1_OUT_GPIO, VCC_ICC1_OUT_PIN, Bit_RESET);
}
//===============================================================================//
//== 函 数 名：init_led
//== 功    能：LED init
//== 说    明：
//== 时    间：20110317
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无		   LED1_OUT_PIN
//===============================================================================//
void init_led(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIO时钟 */
    //RCC_APB2PeriphClockCmd( LED1_OUT_RCC | LED2_OUT_RCC|LED3_OUT_RCC|LED4_OUT_RCC|LED5_OUT_RCC|LED6_OUT_RCC|RCC_APB2Periph_AFIO, ENABLE );


    /* 配置 SC_CARD0_IN_PIN */
 //   GPIO_InitStructure.GPIO_Pin = LED1_OUT_PIN;
 //   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //   GPIO_Init(LED1_OUT_GPIO, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = LED2_OUT_PIN;
 //   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //   GPIO_Init(LED2_OUT_GPIO, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = LED3_OUT_PIN;
 //   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //   GPIO_Init(LED3_OUT_GPIO, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = LED4_OUT_PIN;
 //   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //   GPIO_Init(LED4_OUT_GPIO, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = LED5_OUT_PIN;
 //   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //  GPIO_Init(LED5_OUT_GPIO, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = LED6_OUT_PIN;
  //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 //   GPIO_Init(LED6_OUT_GPIO, &GPIO_InitStructure);


}

void led1_on(void)
{
//	GPIO_WriteBit(LED1_OUT_GPIO, LED1_OUT_PIN, Bit_SET);
}


void led1_off(void)
{
//	GPIO_WriteBit(LED1_OUT_GPIO, LED1_OUT_PIN, Bit_RESET);
}



void led2_on(void)
{
//	GPIO_WriteBit(LED2_OUT_GPIO, LED2_OUT_PIN, Bit_SET);
}


void led2_off(void)
{
//	GPIO_WriteBit(LED2_OUT_GPIO, LED2_OUT_PIN, Bit_RESET);
}

void led3_on(void)
{
//	GPIO_WriteBit(LED3_OUT_GPIO, LED3_OUT_PIN, Bit_SET);
}


void led3_off(void)
{
//	GPIO_WriteBit(LED3_OUT_GPIO, LED3_OUT_PIN, Bit_RESET);
}

void led4_on(void)
{
//	GPIO_WriteBit(LED4_OUT_GPIO, LED4_OUT_PIN, Bit_SET);
}


void led4_off(void)
{
//	GPIO_WriteBit(LED4_OUT_GPIO, LED4_OUT_PIN, Bit_RESET);
}

//读卡
void led5_on(void)
{
//	GPIO_WriteBit(LED5_OUT_GPIO, LED5_OUT_PIN, Bit_RESET);
}


void led5_off(void)
{
//	GPIO_WriteBit(LED5_OUT_GPIO, LED5_OUT_PIN, Bit_SET);
}


void led6_on(void)
{
//	GPIO_WriteBit(LED6_OUT_GPIO, LED6_OUT_PIN, Bit_RESET);
}


void led6_off(void)
{
//	GPIO_WriteBit(LED6_OUT_GPIO, LED6_OUT_PIN, Bit_SET);
}






//===============================================================================//
//== 函 数 名：usb_ctl_off
//== 功    能：usb的DP脚下电
//== 说    明：
//== 时    间：20110317
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
void usb_ctl_off(void)
{
 	PowerOff();	//==usb_pwr.c  usb下电，清除所有中断寄存器等
	GPIO_WriteBit(USB_CTL_PIN_GPIO, USB_CTL_PIN, Bit_RESET);
}

//===============================================================================//
//== 函 数 名：init
//== 功    能：平台初始化函数
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
void init(void)
{
	init_sam_v();
	init_led();	
	//IWDG_Configuration();
}

 void Bluetooth_status_init(void)
 {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_APB2PeriphClockCmd( BLUE_ICC_IN_RCC|BLUE_P20_RCC|BLUE_RST_RCC, ENABLE );


    /* 配置 SC_CARD0_IN_PIN */
    GPIO_InitStructure.GPIO_Pin = BLUE_ICC_IN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BLUE_ICC_IN_GPIO, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = BLUE_P20_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BLUE_P20_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BLUE_RST_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BLUE_RST_GPIO, &GPIO_InitStructure);
	GPIO_WriteBit(BLUE_RST_GPIO, BLUE_RST_PIN, Bit_SET);
	GPIO_WriteBit(BLUE_P20_GPIO, BLUE_P20_PIN, Bit_SET);
 }

void BluesetM(void)
{
	unsigned long i;
	GPIO_WriteBit(BLUE_P20_GPIO, BLUE_P20_PIN, Bit_RESET);
	GPIO_WriteBit(BLUE_RST_GPIO, BLUE_RST_PIN, Bit_RESET);
	for(i=0;i<0x1ffff8;i++);
	GPIO_WriteBit(BLUE_RST_GPIO, BLUE_RST_PIN, Bit_SET);
	for(i=0;i<0x1fffff;i++);
	for(i=0;i<0x1fffff;i++);
	for(i=0;i<0xfffff;i++);
//	GPIO_WriteBit(BLUE_P20_GPIO, BLUE_P20_PIN, Bit_SET);
}

void BluesetM_end(void)
{
	GPIO_WriteBit(BLUE_P20_GPIO, BLUE_P20_PIN, Bit_SET);
}


 unsigned char  IsBlueTooth(void)
 {
 
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );


    /* 配置 SC_CARD0_IN_PIN */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


 	 if((1==GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)))
	 {
	 	 return 1;
	 
	 }
	 else
	 {
	 
	    return 0;
	 }

 
 }

void RF_init(void)
{
	init_spix(SPI2);	
   		 AS3911_IRQ_Init();
 }

unsigned char IC_inset(void)
{
   if(0==GPIO_ReadInputDataBit(SLOT_ICC_IN_GPIO,SLOT_ICC_IN_PIN))
   	{	
		return 1;
	}
	else
	{
	 	return 0;
	}
}

void COM1_init(void)
{
	GPIO_WriteBit(SLOT_CD4053_SET5_GPIO, SLOT_CD4053_SET5_PIN, Bit_RESET);
	GPIO_WriteBit(SLOT_CD4053_SET6_GPIO, SLOT_CD4053_SET6_PIN, Bit_RESET);
}


void COM2_init(void)
{
	GPIO_WriteBit(SLOT_CD4053_SET5_GPIO, SLOT_CD4053_SET5_PIN, Bit_SET);
	GPIO_WriteBit(SLOT_CD4053_SET6_GPIO, SLOT_CD4053_SET6_PIN, Bit_SET);
}


void key_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIO时钟 */
    RCC_APB2PeriphClockCmd( KEY1_RCC | KEY2_RCC|KEY3_RCC|KEY4_RCC|KEY5_RCC|KEY6_RCC|KEY7_RCC|KEY8_RCC|RCC_APB2Periph_AFIO, ENABLE );


    /* 配置 SC_CARD0_IN_PIN */
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY1_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY2_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY3_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY4_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY5_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY5_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY6_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY6_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY7_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY7_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY8_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY8_GPIO, &GPIO_InitStructure);
}


void Set_outkey(void)
{
	GPIO_WriteBit(KEY5_GPIO, KEY5_PIN, Bit_SET);
	GPIO_WriteBit(KEY6_GPIO, KEY6_PIN, Bit_SET);
	GPIO_WriteBit(KEY7_GPIO, KEY7_PIN, Bit_SET);
	GPIO_WriteBit(KEY8_GPIO, KEY8_PIN, Bit_SET);
}

void Reset_outkey(void)
{
	GPIO_WriteBit(KEY5_GPIO, KEY5_PIN, Bit_RESET);
	GPIO_WriteBit(KEY6_GPIO, KEY6_PIN, Bit_RESET);
	GPIO_WriteBit(KEY7_GPIO, KEY7_PIN, Bit_RESET);
	GPIO_WriteBit(KEY8_GPIO, KEY8_PIN, Bit_RESET);
}


unsigned char   resetbebug(void)
{
	unsigned char 	temp_value;

 	GPIO_WriteBit(KEY5_GPIO, KEY5_PIN, Bit_RESET);
	temp_value = GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN);
	if(temp_value==0)
	{
		
		return 	1;
	}
	else
	{
		 	return 	0;
	
	}

}

unsigned char get_key(void)
{
	unsigned char j;
	unsigned char temp_value=0xff;


	GPIO_WriteBit(KEY5_GPIO, KEY5_PIN, Bit_RESET);
	temp_value = GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN);
	if(temp_value==0)
	{


		temp_value =1; //13;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY2_GPIO, KEY2_PIN);
	if(temp_value==0)
	{
		temp_value =5;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY3_GPIO, KEY3_PIN);
	if(temp_value==0)
	{
		temp_value =9; //15;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY4_GPIO, KEY4_PIN);
	if(temp_value==0)
	{
		temp_value =14;  //16;
		return 	temp_value;
	}

	GPIO_WriteBit(KEY5_GPIO, KEY5_PIN, Bit_SET);
	GPIO_WriteBit(KEY6_GPIO, KEY6_PIN, Bit_RESET);
	temp_value = GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN);
	if(temp_value==0)
	{
		temp_value =2; //3;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY2_GPIO, KEY2_PIN);
	if(temp_value==0)
	{
		temp_value =6;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY3_GPIO, KEY3_PIN);
	if(temp_value==0)
	{
		temp_value =0x3;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY4_GPIO, KEY4_PIN);
	if(temp_value==0)
	{
		temp_value =11;
		return 	temp_value;
	}

	GPIO_WriteBit(KEY6_GPIO, KEY6_PIN, Bit_SET);
	GPIO_WriteBit(KEY7_GPIO, KEY7_PIN, Bit_RESET);
	temp_value = GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN);
	if(temp_value==0)
	{
		temp_value =3; //2;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY2_GPIO, KEY2_PIN);
	if(temp_value==0)
	{
		temp_value =7; //5;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY3_GPIO, KEY3_PIN);
	if(temp_value==0)
	{
		temp_value =0; //8;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY4_GPIO, KEY4_PIN);
	if(temp_value==0)
	{
		temp_value =0x10;  //11;
		return 	temp_value;
	}

	GPIO_WriteBit(KEY7_GPIO, KEY7_PIN, Bit_SET);
	GPIO_WriteBit(KEY8_GPIO, KEY8_PIN, Bit_RESET);
	temp_value = GPIO_ReadInputDataBit(KEY1_GPIO, KEY1_PIN);
	if(temp_value==0)
	{
		temp_value =4; //1;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY2_GPIO, KEY2_PIN);
	if(temp_value==0)
	{
		temp_value =8;  //4;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY3_GPIO, KEY3_PIN);
	if(temp_value==0)
	{
		temp_value =15;  //7;
		return 	temp_value;
	}
	temp_value = GPIO_ReadInputDataBit(KEY4_GPIO, KEY4_PIN);
	if(temp_value==0)
	{
		temp_value =10;
		return 	temp_value;
	}
	return 	0xff;
}

extern unsigned short int ADCConvertedValue;
extern unsigned char bat_falg1;
void ad_get(void)
{
    	unsigned char i=0;
		unsigned int AD_v =0;
 		 start_adc();
		 for(i=0;i<0xf0;i++);
		 AD_v = ADCConvertedValue;
		 
		 end_adc();
		 if(AD_v>=0x800)
		 {
			 led5_off(); 
		 }else if(AD_v<0x800)
		 {		 
			 led5_on(); 
		 }
}

//===============================================================================//
//== 函 数 名：main
//== 功    能：
//== 说    明：
//== 时    间：201012
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
extern void USBConnectinit(void);

extern unsigned char CommandReceive(void) ;
extern void MakeDataback(void)  ;
extern  void MakeDataback_Error(unsigned char flag);
extern unsigned   int Mass_Block_Size[2];
extern unsigned   int Mass_Block_Count[2];
extern unsigned char CWBotState;
extern void init_usartx_c(USART_TypeDef* USARTx, unsigned int BaudRate) ;
extern  void USB_Interrupts_DisConfig(void);




extern void GetConfigPara(void);




void InitCOM(unsigned int baut )
{

 	init_usartx_c(USART3,baut);  


}

unsigned char g_configflag=0;
extern unsigned char  Select_SD_SPIFlash;


void CheckResetMode(void)
{
  //g_configflag |= 0x2;
  if(RCC->CSR & 0x10000000)
  {//软件复位
    g_configflag |= 0x2;
  }
  RCC->CSR |= 0x01000000;//RCC->CSR 寄存器清零
}

//unsigned long haha=0;
//unsigned long hoho=0;

int main(void)
{
	unsigned char *buf,len;
	unsigned char bluedata_flag = 0;

#ifdef IAPLOAD

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
	
#endif
	Select_SD_SPIFlash=0;
	//CheckResetMode();

	SystemInit();
	  /*使能PC口的外设时钟 */
  	Mass_Block_Size[0]=512;				  
 	Mass_Block_Size[1]=512; 

	RCC_Configuration(); 
	
 			     
  	init_usartx_c(UART4,115200);
	init_usartx_c(USART3,115200);

	init_usartx(USART2,38400);  
	//init_usartx(UART4,38400);  
	init();	
	IC_solt_init();
	//SPI_Init_GPIO();
    init_spix(SPI2);	
  //  AS3911_IRQ_Init();
	//DMA_ad_init();						 
	ISO14443_carrier_off();

//	beep_con(); 
 	Bluetooth_status_init();
	MakeDataback_Error(4);
	led5_off(); 
	led6_off(); 
	key_init();	 


	SPI_Flash_Init();
    LCD_Init();	 
	 ShowDefault();

  

	GetConfigPara();

	if(resetbebug())
	{
	   ResetContactlessDebug();
	}



	if(g_configflag&0x01)
	{
	   // g_configflag|=0x01;
	 	 Time5Config();
	     open_time5_isr();
	}


	
	//IWDG_ReloadCounter();

  if(IsBlueTooth())		//
   // if(1)
   {


	   	ShowUSB();
  	    usb_ctl_off();					//== USB 下电 	 	
		usb_ctl_on();
		init_usb();	
	//	 while(bDeviceState != CONFIGURED);

		  CWBotState=0;

		  while(1)
		  {
		//  	Detect_14443_3_Card();
			//search_typea(buf,len);
			IWDG_ReloadCounter();
		  }	

   }
   else
   {

      		ShowBT();
		while (1)
		{
		//	if(0 == GPIO_ReadInputDataBit(BLUE_ICC_IN_GPIO, BLUE_ICC_IN_PIN))
			{						   	
			 	 
				 bluedata_flag = CommandReceive(); 
				 if(0==bluedata_flag)
				  {
					 MakeDataback();
	 				 IWDG_ReloadCounter();
				  } 
				  else
				  	{
				  		MakeDataback_Error(bluedata_flag);
					
				  	}	 
			
			} 
     }	
  }	   
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
