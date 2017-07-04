#ifndef __SPI_H__
#define __SPI_H__


#define T3070_RST_PIN                        GPIO_Pin_9				//led1
#define T3070_RST_GPIO                    	 GPIOD
#define T3070_RST_RCC                    	 RCC_APB2Periph_GPIOD



void init_spix(SPI_TypeDef* SPIx);
void AS3911_SEN_ON(void);
void AS3911_SEN_OFF(void);
void AS3911_IRQ_Init(void);
extern void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);

#endif


