#include "stm32f10x_adc.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC2_DR_Address    ((uint32_t)0x4001284C)
unsigned short int ADCConvertedValue;

void DMA_ad_init(void)
{
	  ADC_InitTypeDef ADC_InitStructure;
	  DMA_InitTypeDef DMA_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2|RCC_APB2Periph_GPIOA, ENABLE);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //GPIO设置为模拟输入
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	  DMA_DeInit(DMA1_Channel1);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC2_DR_Address;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	  DMA_InitStructure.DMA_BufferSize = 1;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	  
	  /* Enable DMA1 channel1 */
	  DMA_Cmd(DMA1_Channel1, ENABLE);

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);   
	  /* ADC1 configuration ------------------------------------------------------*/
      ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_NbrOfChannel = 1;
	  ADC_Init(ADC2, &ADC_InitStructure);
	
	  /* ADC1 regular channel14 configuration */ 
	  ADC_RegularChannelConfig(ADC2, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	  /* Enable ADC1 DMA */
	  ADC_DMACmd(ADC2, ENABLE);
	  
	  /* Enable ADC1 */
	
}


extern void delay_ms(unsigned int delay_nsm);


void start_adc(void)
{
      uint16_t Loop_Waring=0;
	  unsigned int ad_delay;

	  DMA_ad_init();

	  
	  ADC_Cmd(ADC2, ENABLE);
	 
	  ADC_ResetCalibration(ADC2);
	  /* Check the end of ADC1 reset calibration register */
	  //while(ADC_GetResetCalibrationStatus(ADC2));

	  Loop_Waring=0;
	  while(ADC_GetResetCalibrationStatus(ADC2) && Loop_Waring < 1000)
	  {
	  	 Loop_Waring++;

		 IWDG_ReloadCounter();
		 //delay_ms(1);
		 ad_delay=1000;
		 while(ad_delay--);

		 IWDG_ReloadCounter();
		 //delay_ms(1);
		 ad_delay=1000;
		 while(ad_delay--);
	  
	  }


	  /* Start ADC1 calibaration */
	  ADC_StartCalibration(ADC2);
	  /* Check the end of ADC1 calibration */
	 // while(ADC_GetCalibrationStatus(ADC2));

	  Loop_Waring=0;
	  while(ADC_GetCalibrationStatus(ADC2) && Loop_Waring < 1000)
	  {
	  	Loop_Waring++;
		IWDG_ReloadCounter();
		//delay_ms(1);
		ad_delay=1000;
		while(ad_delay--);

		IWDG_ReloadCounter();
		 //delay_ms(1);
		 ad_delay=1000;
		 while(ad_delay--);

	  }


	  ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}


int GetAD(void)
{
	
	int ret;
	ret = ADC_GetConversionValue(ADC2);
	
	return ret;


}

void end_adc(void)
{
   
    ADC_Cmd(ADC2, DISABLE);
	DMA_Cmd(DMA1_Channel1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, DISABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, DISABLE);
}
