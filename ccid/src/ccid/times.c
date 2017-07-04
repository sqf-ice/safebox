#include "stm32f10x.h" 		//== STM32F103VC ��ͷ�ļ�
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
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;					//== ��ʱ����ʼ���ṹ
	TIM_OCInitTypeDef  TIM_OCInitStructure;							//== ͨ�������ʼ���ṹ

	temp = 720000/frequency; 										//== 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			//== TIM1 clock enable

	//== TIM1 ��ʼ��
	TIM_TimeBaseStructure.TIM_Prescaler 	= 99;					//== ʱ�ӷ�Ƶ ��ֵ+1Ϊ��Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;	//== ģʽ
	TIM_TimeBaseStructure.TIM_Period = temp;						//== ����0��FFFF
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;					//== ʱ�ӷָ�
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					//== ������ʼ��
//	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);							//== ���жϣ��ж���Ҫ���д���
  
	//== TIM1 ͨ����ʼ��
	TIM_OCStructInit(& TIM_OCInitStructure);						//== Ĭ�ϲ���
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1;			//== ����״̬
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//== �趨Ϊ�������ҪPWM�������Ҫ���д���
	TIM_OCInitStructure.TIM_Pulse 		= temp >> 1;				//== ռ�ճ���
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		//== �ߵ�ƽ
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);						//== ͨ����ʼ����TIM1�ĵ�һͨ��

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);								//== ʹ���Զ���װ

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}



void tim3_init(unsigned int num)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;					//== ��ʱ����ʼ���ṹ
	TIM_OCInitTypeDef  TIM_OCInitStructure;							//== ͨ�������ʼ���ṹ

	TIM_Cmd(TIM3, DISABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				   	//== �ж���
	NVIC_InitStructure.NVIC_IRQChannel 				= TIM3_IRQn;	//== TIM3ȫ���ж�3.0��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;		//== ռ�ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 	= 0;			//== ��Ӧ��
	NVIC_InitStructure.NVIC_IRQChannelCmd 			= ENABLE;		//== ����
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//== TIM3 clock enable

	//== TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period 		= 65535;				//== 60000 �����
	TIM_TimeBaseStructure.TIM_Prescaler 	= 20-1;					//== ʱ�ӷ�Ƶ  3.6MHz  ��ֵ+1Ϊ��Ƶ����
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//== ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;	//== ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);					//== ������ʼ��
  
	//== TIM3ͨ����ʼ��
	TIM_OCStructInit(& TIM_OCInitStructure);						//== Ĭ�ϲ���
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_Inactive;		//== ����ģʽ������ȽϷ�����ģʽ
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//== �趨Ϊ�������ҪPWM�������Ҫ���д���
	TIM_OCInitStructure.TIM_Pulse 		= num;						//== ռ�ճ��ȣ�װ�벶��ȽϼĴ������ֵ
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		//== �ߵ�ƽ
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);						//== ͨ����ʼ����TIM3�ĵ���ͨ��

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);								//== ʹ���Զ���װ
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update);		//== Ԥ����������ж�λ
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);							//== ���жϣ��ж���Ҫ���д���
	
//	TIM_SetCounter(TIM3, 0);
	TIM_Cmd(TIM3, ENABLE);											//== ����TIM3
	//GPIO_WriteBit(LED3_GPIO, LED3_PIN, Bit_SET);
}





void TIM3_IRQHandler(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) !=RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);					//== ������ձ�־λ/*/
	}

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel 					= TIM3_IRQn;//== TIM3ȫ���ж�3.0��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;		//== ռ�ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		= 0;		//== ��Ӧ��
	NVIC_InitStructure.NVIC_IRQChannelCmd 				= DISABLE;	//== ����
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
	GPIO_InitStructure.GPIO_Pin 	= BEEP_PIN;			//== ָ������
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP; 	//== ָ�����ģʽ
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	//== ָ���ٶ�
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);
	
}


void disable_beep_gpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
																	
	GPIO_InitStructure.GPIO_Pin 	= BEEP_PIN;						//== ָ������
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;				//== ָ���ٶ�
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);
	GPIO_WriteBit(BEEP_GPIO, BEEP_PIN, Bit_RESET);					//== ���� ֹͣ����	
}


void beep_tcf(unsigned int beep_time, unsigned int beep_conuter, unsigned int beep_frequency)
{

	if (beep_frequency < 20) beep_frequency = 20;
	else if (beep_frequency > 20000) beep_frequency = 20000;	

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);	//== Tim1�ܽ��ض�λ PE9
	tim1_init(beep_frequency);

	for (; beep_conuter>0; beep_conuter--)
	{
		beep_gpio_config();
		TIM_Cmd(TIM1, ENABLE);
		delay_ms(beep_time);

		if (beep_conuter <= 1) break;

		TIM_Cmd(TIM1, DISABLE);
		disable_beep_gpio();
		delay_ms(beep_time>>1);			//== ����ʱ��ݼ�
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