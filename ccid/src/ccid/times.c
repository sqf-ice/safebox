#include "stm32f10x.h" 		//== STM32F103VC 库头文件
#include "stm32f10x_conf.h"	
#include "platform_config.h"


#define TIMER_MAX_VALUE  8000 // 2000   /* real max is 1048 */
volatile u32 timerValue;
volatile s8 timer1Running;
unsigned int gSet_delay_ms=0;
void beep_on(void);
void  beep_con(void);
void beep_off(void);
bool timerIsRunning(void);
unsigned char count_start=0;
void tim1_init(unsigned int frequency)
{
	unsigned int temp;
//	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;					//== 定时器初始化结构
	TIM_OCInitTypeDef  TIM_OCInitStructure;							//== 通道输出初始化结构

	temp = 720000/frequency; 										//== 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			//== TIM1 clock enable

	//== TIM1 初始化
	TIM_TimeBaseStructure.TIM_Prescaler 	= 99;					//== 时钟分频 此值+1为分频除数
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;	//== 模式
	TIM_TimeBaseStructure.TIM_Period = temp;						//== 周期0～FFFF
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;					//== 时钟分割
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					//== 基本初始化
//	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);							//== 打开中断，中断需要这行代码
  
	//== TIM1 通道初始化
	TIM_OCStructInit(& TIM_OCInitStructure);						//== 默认参数
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1;			//== 工作状态
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//== 设定为输出，需要PWM输出才需要这行代码
	TIM_OCInitStructure.TIM_Pulse 		= temp >> 1;				//== 占空长度
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		//== 高电平
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);						//== 通道初始化，TIM1的第一通道

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);								//== 使能自动重装

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}



void tim3_init(unsigned int num)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;					//== 定时器初始化结构
	TIM_OCInitTypeDef  TIM_OCInitStructure;							//== 通道输出初始化结构

	TIM_Cmd(TIM3, DISABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				   	//== 中断组
	NVIC_InitStructure.NVIC_IRQChannel 				= TIM3_IRQn;	//== TIM3全局中断3.0的
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;		//== 占先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 	= 0;			//== 响应级
	NVIC_InitStructure.NVIC_IRQChannelCmd 			= ENABLE;		//== 启动
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//== TIM3 clock enable

	//== TIM3初始化
	TIM_TimeBaseStructure.TIM_Period 		= 65535;				//== 60000 后溢出
	TIM_TimeBaseStructure.TIM_Prescaler 	= 20-1;					//== 时钟分频  3.6MHz  此值+1为分频除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//== 时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;	//== 模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);					//== 基本初始化
  
	//== TIM3通道初始化
	TIM_OCStructInit(& TIM_OCInitStructure);						//== 默认参数
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_Inactive;		//== 工作模式，输出比较非主动模式
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//== 设定为输出，需要PWM输出才需要这行代码
	TIM_OCInitStructure.TIM_Pulse 		= num;						//== 占空长度，装入捕获比较寄存的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		//== 高电平
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);						//== 通道初始化，TIM3的第四通道

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);								//== 使能自动重装
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update);		//== 预先清除所有中断位
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);							//== 打开中断，中断需要这行代码
	
//	TIM_SetCounter(TIM3, 0);
	TIM_Cmd(TIM3, ENABLE);											//== 启动TIM3
	//GPIO_WriteBit(LED3_GPIO, LED3_PIN, Bit_SET);
}





void TIM3_IRQHandler(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) !=RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);					//== 必须清空标志位/*/
	}

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel 					= TIM3_IRQn;//== TIM3全局中断3.0的
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;		//== 占先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		= 0;		//== 响应级
	NVIC_InitStructure.NVIC_IRQChannelCmd 				= DISABLE;	//== 启动
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3, DISABLE);											//== TIM3 enable counter
//	gVar.psam_clock_flag = 1;
}


bool timerIsRunning(void)
{
    return ( timer1Running == 1 );
}

void timerStart( u16 mSec )
{
	timer1Running = 1;    /* set running flag */

    if ( mSec > TIMER_MAX_VALUE ) /* cannot handle values bigger than 1048 */
   {
        mSec = TIMER_MAX_VALUE;
    }
   timerValue = mSec;
   count_start =1;
}


void timerStop( )
{
 	count_start =0;
 	timer1Running = 0; /* timer is not running */
} 

void sleepMilliseconds(unsigned short int milliseconds)
{
    timerStart(milliseconds);
    while(timerIsRunning())
	;   
}


void delay_ms(unsigned int delay_nsm)
{
	gSet_delay_ms = delay_nsm;

	while ( gSet_delay_ms );
}


void beep_gpio_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE); 											
	GPIO_InitStructure.GPIO_Pin 	= BEEP_PIN;			//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP; 	//== 指定输出模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	//== 指定速度
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);
	
}


void disable_beep_gpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
																	
	GPIO_InitStructure.GPIO_Pin 	= BEEP_PIN;						//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;				//== 指定速度
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);
	GPIO_WriteBit(BEEP_GPIO, BEEP_PIN, Bit_RESET);					//== 下拉 停止蜂鸣	
}


void beep_tcf(unsigned int beep_time, unsigned int beep_conuter, unsigned int beep_frequency)
{

	if (beep_frequency < 20) beep_frequency = 20;
	else if (beep_frequency > 20000) beep_frequency = 20000;	

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);	//== Tim1管脚重定位 PE9
	tim1_init(beep_frequency);

	for (; beep_conuter>0; beep_conuter--)
	{
		beep_gpio_config();
		TIM_Cmd(TIM1, ENABLE);
		delay_ms(beep_time);

		if (beep_conuter <= 1) break;

		TIM_Cmd(TIM1, DISABLE);
		disable_beep_gpio();
		delay_ms(beep_time>>1);			//== 蜂鸣时间递减
	}

	TIM_Cmd(TIM1, DISABLE);
	disable_beep_gpio();
}																																 

void beep_on(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin 	= BEEP_PIN;  	//== beep
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 
  	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;  	
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure); 
 	
	BeepOn();
}


void beep_off(void)
{
	BeepOff();
}


void  beep_con(void)
{
	unsigned long i;
	beep_on();
	for(i=0;i<25000;i++);
	for(i=0;i<25000;i++);
	beep_off();

}