/*===============================================================================//
//== 文 件 名：ifd.c
//== author	 ：huyx@cashwaytech.com
//== 说    明：STM32F103VC8平台
//== 恒银金融科技有限公司
//===============================================================================*/
#include "IFD.H"
#include "STM32F10x.H"
#include "function.h"


void IFD1_Change_ISR(void);
char icc_in_level=0;
unsigned char IFD_Slot_GetStatus(void);
unsigned char IFD_nContact_Slot_GetStatus(void);
/**************************************************************************
 ****函数名称:	IFD_Hal_Slot_dect
 ****入口参数:	uint8_t bSlot
 ****出口参数: 	uint8_t
 ****说    明: 	IFD卡槽检测
 **************************************************************************/

unsigned char IFD_Hal_Slot_dect(void)
{
	
	unsigned char i;
	icc_in_level = GPIO_ReadInputDataBit(SLOT_ICC_IN_GPIO,SLOT_ICC_IN_PIN);

	/* 连续检测 卡槽插入检测io 直到检测到的电平相同 */
	do
	{
	 for(i=0;i<50;i++);
		/* 如果连续两次读取的结果相同 则跳出循环 */
		if(icc_in_level == GPIO_ReadInputDataBit(SLOT_ICC_IN_GPIO,SLOT_ICC_IN_PIN))
		{
			break;
		}
		else
		{
			/* 读取结果不同，保存当前值 */
			icc_in_level = GPIO_ReadInputDataBit(SLOT_ICC_IN_GPIO,SLOT_ICC_IN_PIN);
		}
	}while(1);


	if(icc_in_level == 0)
	{
		return SLOT_ICC_IN_STATE;
	}
	else
	{
		return SLOT_ICC_OUT_STATE;
	}
} 

extern unsigned char Detect_14443_4_Card(void);		
extern unsigned char Detect_14443_3_Card(void);	
extern unsigned char bSak;


void Time5Config(void)
{

   NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

 
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 

    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_Prescaler = 7200-10;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);


   // TIM_ITConfig(TIM5, TIM_IT_Update,ENABLE);

  //  TIM_ARRPreloadConfig(TIM5, ENABLE);

    TIM_ClearITPendingBit( TIM5, TIM_IT_Update );

    TIM_Cmd(TIM5, DISABLE);	  

}  


//===============================================================================//
//== 函 数 名：IFD_nContact_Slot_GetStatus
//== 功    能：检测非接触卡槽状态
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
unsigned char ifd_icc_active_sate=1;
unsigned char beep_flag=0;									  
unsigned char IFD_nContact_Slot_GetStatus(void)
{
    int status=0,i;
	//	beep_on();	
	
//	unsigned char temp_Buffer[2],len;

	    status=Detect_14443_3_Card();	//没有卡片处于激活状态，发送寻卡命令

		 if (status==0)
	    {
			if(beep_flag==1)
			{
				beep_on();
				for(i=0;i<25000;i++);											
				for(i=0;i<25000;i++);
				beep_off();	 
			}	  
		//	beep_tcf(100,3,2000);
			return ICC_PRESENT_ACTIVE;
	    }
	    else
	    {
			return ICC_NO_PRESENT;
	    }
}											       
 

void close_time5_isr(void)
{	
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM5, DISABLE);
}

void open_time5_isr(void)
{		
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}


//===============================================================================//
//== 函 数 名：TIM5_IRQHandler
//== 功    能：TIM5 中断处理
//== 说    明：
//== 时    间：
//== 入口参数：无
//== 出口参数：无
//== 返 回 值：无
//===============================================================================//
extern void IFD_14443_dec_Process( void );
unsigned char timesss=0;
//extern unsigned char bDevice_flag;




extern unsigned char buf_typeAB[20];
extern unsigned char search_card(unsigned char *buf_atr,unsigned int *rlen);
extern unsigned char g_configflag;

void TIM5_IRQHandler(void)
{

   unsigned char ret;
   unsigned int len,i;

//	NVIC_InitTypeDef NVIC_InitStructure;
	if(TIM_GetITStatus( TIM5, TIM_IT_Update ) != RESET )
   {
   	//	timesss	++;
        TIM_ClearITPendingBit( TIM5, TIM_IT_Update );
       //	if(bDevice_flag == 1)//&&(timesss==200))
 		{
		   //IFD_14443_dec_Process();

		   if(g_configflag&0x01)
		   {
				 ret =  search_card(buf_typeAB,&len);
				 if(ret==0)
				 {
				 	  beep_on();
				      for(i=0;i<25000;i++);											
				      for(i=0;i<25000;i++);
				      beep_off();
					  for(i=0;i<25000;i++);
					  for(i=0;i<25000;i++);
					  beep_on();
				      for(i=0;i<25000;i++);											
				      for(i=0;i<25000;i++);
				      beep_off();	 
				 }
				 else
				 {
				 
				 
				 }	
			}
				 	
		   //timesss=0;
		}
   }
}



unsigned char IFD_Slot_GetStatus(void)
{
        /* 首先检测 卡槽内是否有卡 */
    if( IFD_Hal_Slot_dect() == SLOT_ICC_OUT_STATE )
      {
            /* 无卡 返回卡片拔出 */
          return ICC_NO_PRESENT;
      }
    else
     {
          return ICC_PRESENT_ACTIVE;				        			
     }
 
} 


void IFD1_Change_ISR(void)
{

}	


