

/*********************************************************************************************************
 * ∂®“Â ø®≤€ƒ⁄ ICC ◊¥Ã¨
*********************************************************************************************************/
#define ICC_PRESENT_ACTIVE					0x00				//ø®∆¨≤Â»Î «“ º§ªÓ
#define ICC_PRESENT_INACTIVE				0x01				//ø®∆¨≤Â»Î Œ¥º§ªÓ
#define ICC_NO_PRESENT						0x02				//ø®∆¨Œ¥≤Â»Î

/*********************************************************************************************************
 * ∂®“Âø®≤€ƒ⁄ ICC ≤Â»Î◊¥Ã¨
*********************************************************************************************************/
#define SLOT_ICC_IN_STATE					1					//ø®≤Â»Î◊¥Ã¨
#define SLOT_ICC_OUT_STATE					0					//ø®∞Œ≥ˆ◊¥Ã¨

/*********************************************************************************************************
 * ø®≤€1ºÏ≤‚ GPIO ≈‰÷√
*********************************************************************************************************/
#define SLOT_ICC_IN_PIN                     GPIO_Pin_8
#define SLOT_ICC_IN_GPIO                    GPIOB
#define SLOT_ICC_IN_RCC                     RCC_APB2Periph_GPIOB

#define SLOT_ICC1_IN_PIN                     GPIO_Pin_14
#define SLOT_ICC1_IN_GPIO                    GPIOE
#define SLOT_ICC1_IN_RCC                     RCC_APB2Periph_GPIOE

#define SLOT_ICC2_IN_PIN                     GPIO_Pin_10
#define SLOT_ICC2_IN_GPIO                    GPIOE
#define SLOT_ICC2_IN_RCC                     RCC_APB2Periph_GPIOE

#define SLOT_CD4053_SET1_PIN                  //GPIO_Pin_1		     //A 
#define SLOT_CD4053_SET1_GPIO                // GPIOB						
#define SLOT_CD4053_SET1_RCC                 // RCC_APB2Periph_GPIOB

#define SLOT_CD4053_SET2_PIN                 // GPIO_Pin_0			   //B
#define SLOT_CD4053_SET2_GPIO                // GPIOB
#define SLOT_CD4053_SET2_RCC                 // RCC_APB2Periph_GPIOB

#define SLOT_CD4053_SET3_PIN                  GPIO_Pin_0				//E
#define SLOT_CD4053_SET3_GPIO                 GPIOB
#define SLOT_CD4053_SET3_RCC                  RCC_APB2Periph_GPIOB

#define SLOT_CD4053_SET4_PIN                  GPIO_Pin_1			    //F
#define SLOT_CD4053_SET4_GPIO                 GPIOB
#define SLOT_CD4053_SET4_RCC                  RCC_APB2Periph_GPIOB

#define SLOT_CD4053_SET5_PIN                  GPIO_Pin_6				//C
#define SLOT_CD4053_SET5_GPIO                 GPIOD
#define SLOT_CD4053_SET5_RCC                  RCC_APB2Periph_GPIOD

#define SLOT_CD4053_SET6_PIN                  GPIO_Pin_5				//D
#define SLOT_CD4053_SET6_GPIO                 GPIOD
#define SLOT_CD4053_SET6_RCC                  RCC_APB2Periph_GPIOD

#define VCC_ICC1_OUT_PIN                     GPIO_Pin_9				//S1VCC
#define VCC_ICC1_OUT_GPIO                    GPIOE
#define VCC_ICC1_OUT_RCC                     RCC_APB2Periph_GPIOE

#define VCC_ICC2_OUT_PIN                     GPIO_Pin_11			   //S2VCC
#define VCC_ICC2_OUT_GPIO                    GPIOE
#define VCC_ICC2_OUT_RCC                     RCC_APB2Periph_GPIOE


#define LCD_BACKLIGHT_OUT_PIN                         GPIO_Pin_15				//led1
#define LCD_BACKLIGHT_OUT_GPIO                    	 GPIOE
#define LCD_BACKLIGHT_OUT_RCC                    	 RCC_APB2Periph_GPIOE

#define LCD_MOSI_OUT_PIN                         GPIO_Pin_14			   //led2
#define LCD_MOSI_OUT_GPIO                   	 GPIOE
#define LCD_MOSI_OUT_RCC                    	 RCC_APB2Periph_GPIOE

#define LCD_SCK_OUT_PIN                    	 GPIO_Pin_13			   //led3
#define LCD_SCK_OUT_GPIO                   	 GPIOE
#define LCD_SCK_OUT_RCC                    	 RCC_APB2Periph_GPIOE

#define LCD_CD_OUT_PIN                     	 GPIO_Pin_12				//led4
#define LCD_CD_OUT_GPIO                   	 GPIOE
#define LCD_CD_OUT_RCC                      	 RCC_APB2Periph_GPIOE


#define LCD_RET_OUT_PIN                     	 GPIO_Pin_11				//led4
#define LCD_RET_OUT_GPIO                   	 GPIOE
#define LCD_RET_OUT_RCC                      	 RCC_APB2Periph_GPIOE


#define LCD_CS_OUT_PIN                     	 GPIO_Pin_10				//led4
#define LCD_CS_OUT_GPIO                   	 GPIOE
#define LCD_CS_OUT_RCC                      	 RCC_APB2Periph_GPIOE





#define LED5_OUT_PIN                     	 GPIO_Pin_7				   //led5
#define LED5_OUT_GPIO                   	 GPIOD
#define LED5_OUT_RCC                      	 RCC_APB2Periph_GPIOD

#define LED6_OUT_PIN                     	 GPIO_Pin_12				   //led6
#define LED6_OUT_GPIO                   	 GPIOE
#define LED6_OUT_RCC                      	 RCC_APB2Periph_GPIOE

#define KEY1_PIN                     	 	 GPIO_Pin_6				  
#define KEY1_GPIO                   	 	 GPIOE
#define KEY1_RCC                      	 	 RCC_APB2Periph_GPIOE

#define KEY2_PIN                     	 	 GPIO_Pin_5			   
#define KEY2_GPIO                   	 	 GPIOE
#define KEY2_RCC                      	 	 RCC_APB2Periph_GPIOE

#define KEY3_PIN                     	 	 GPIO_Pin_4				   
#define KEY3_GPIO                   	 	 GPIOE
#define KEY3_RCC                      	 	 RCC_APB2Periph_GPIOE

#define KEY4_PIN                     	 	 GPIO_Pin_3				   
#define KEY4_GPIO                   	 	 GPIOE
#define KEY4_RCC                      	 	 RCC_APB2Periph_GPIOE

#define KEY5_PIN                     	 	 GPIO_Pin_2				   
#define KEY5_GPIO                   	 	 GPIOE
#define KEY5_RCC                      	 	 RCC_APB2Periph_GPIOE

#define KEY6_PIN                     	 	 GPIO_Pin_1			  
#define KEY6_GPIO                   	 	 GPIOE
#define KEY6_RCC                      	 	 RCC_APB2Periph_GPIOE

#define KEY7_PIN                     	 	 GPIO_Pin_0				   
#define KEY7_GPIO                   	 	 GPIOE
#define KEY7_RCC                      	 	 RCC_APB2Periph_GPIOE

#define KEY8_PIN                     	 	 GPIO_Pin_9				   
#define KEY8_GPIO                   	 	 GPIOB
#define KEY8_RCC                      	 	 RCC_APB2Periph_GPIOB


extern void IFD1_Change_ISR(void);
extern unsigned char IFD_Slot_GetStatus(void);
extern unsigned char IFD_nContact_Slot_GetStatus(void);


//¿∂—¿◊¥Ã¨ºÏ≤‚
#define BLUE_ICC_IN_PIN                     GPIO_Pin_7			   //Status
#define BLUE_ICC_IN_GPIO                    GPIOB
#define BLUE_ICC_IN_RCC                     RCC_APB2Periph_GPIOB

#define BLUE_P20_PIN                     GPIO_Pin_6
#define BLUE_P20_GPIO                    GPIOB
#define BLUE_P20_RCC                     RCC_APB2Periph_GPIOB

#define BLUE_RST_PIN                     GPIO_Pin_5
#define BLUE_RST_GPIO                    GPIOB
#define BLUE_RST_RCC                     RCC_APB2Periph_GPIOB