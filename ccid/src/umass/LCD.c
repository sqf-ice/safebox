//#include "Z8D256U.h"
//#include "IomDrv.h"

//#include "Declare_SRC.h"
//#include "Declare_API.h"

#include "lcd.h"
//#include "..\spi\spi.h"

//#define SPIMODE  1

void Delay(unsigned int times)
{
    while(times--);
}

void APL_LCD_Test(unsigned char x)
{

    if(x==1)
	{
	  CS_LOW;
	}
	else if(x==2)
	{
		
		CS_HIGH;
		
	}
	else if(x==3)
	{
		
		RST_LOW;
		
	}	
	else if(x==4)
	{
		RST_HIGH;
		
		
	}
	else if(x==5)
	{
		
		A0_LOW;
		
	}
	else if(x==6)
{
	
	A0_HIGH;
	
}
else if(x==7)
{
	
	SCL_LOW;
	
}
else if(x==8)
{
	
	SCL_HIGH;
	
}
else if(x==9)
{
	SI_LOW;
	
	
}
else if(x==10)
{
	
	SI_HIGH;
	
}





}
//#define SPIMODE
#define LCD_YEEBO


void TimeDelay(unsigned char Time)
{
	int i;
	while(Time > 0)
	{
		for(i = 0;i < 1000;i++)
		{
		//	i_nop;
		}
		Time --;
	}
}

void LCD_IO_Init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能GPIO时钟 RCC_APB2Periph_GPIOA */
    RCC_APB2PeriphClockCmd( LCD_BACKLIGHT_OUT_RCC , ENABLE );


    /* 配置 SC_CARD0_IN_PIN */
    GPIO_InitStructure.GPIO_Pin = LCD_MOSI_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_MOSI_OUT_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_SCK_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_SCK_OUT_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_CD_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_CD_OUT_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_RET_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_RET_OUT_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_CS_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_CS_OUT_GPIO, &GPIO_InitStructure);


	    /* 配置 SC_CARD0_IN_PIN */
    GPIO_InitStructure.GPIO_Pin = LCD_BACKLIGHT_OUT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_BACKLIGHT_OUT_GPIO, &GPIO_InitStructure);



		/*
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		*/
}




void  LCD_BackLight_On()
{

  GPIO_WriteBit(LCD_BACKLIGHT_OUT_GPIO, LCD_BACKLIGHT_OUT_PIN, Bit_RESET);
}



void  LCD_BackLight_Off()
{

  GPIO_WriteBit(LCD_BACKLIGHT_OUT_GPIO, LCD_BACKLIGHT_OUT_PIN, Bit_SET);
}




void APL_LCD_Reset(void)
{
		
	LCD_IO_Init();
//	LCD_BackLight_Off();
	//SPI_Flash_Init();
	RST_LOW;
	Delay(1000);
	RST_HIGH;
	Delay(100);



}



//void APL_LCD_Test(void)
//{
	
	
//}


void test_gpio(unsigned char i)
{
	if(i==1)
	{
	CS_LOW;
	}
	else
	{
	CS_HIGH;
	
	}

}

void APL_LCD_WriteCmd(unsigned char bVar)
{

#ifdef SPIMODE

    unsigned char temp[2];
    
    temp[0] = bVar;
    
    CS_LOW;
    A0_LOW;

    //spim_send_bytes(&temp[0], 1);
	 // HED_SPIM_SendNBytes(&temp[0], 1);
	 SPI_sendNByte(&temp[0], 1);

    CS_HIGH;
    
#else

    unsigned char i;

    CS_LOW;
    SCL_LOW;
    A0_LOW;

    for(i=0;i<8;i++)
    {	
        if(bVar & 0x80)
        {
            SI_HIGH;
        }
        else
        {
            SI_LOW;
        }

        SCL_LOW;
        SCL_HIGH;
        bVar <<= 1;              
    }

    CS_HIGH;
    
#endif

}

void APL_LCD_WriteByte(unsigned char bVar)
{
#ifdef SPIMODE
    unsigned char temp[2];
    
    temp[0] = bVar;

    CS_LOW;
    A0_HIGH;

   // spim_send_bytes(&temp[0], 1);
	// HED_SPIM_SendNBytes(&temp[0], 1);
	SPI_sendNByte(&temp[0], 1);

    CS_HIGH;
#else
    unsigned char i;

    CS_LOW;
    SCL_LOW;
    A0_HIGH;

    for(i=0;i<8;i++)
    {	
        if(bVar & 0x80)
        {
            SI_HIGH;
        }
        else
        {
            SI_LOW;
        }

        SCL_LOW;
        SCL_HIGH;
        bVar <<= 1;              
    }

    CS_HIGH;
    
#endif
}

void APL_LCD_Init(void)
{
    APL_LCD_Reset();

//#ifdef DEBUG_EVB

    APL_LCD_WriteCmd(0x40);			       //设置显示起始行对应RAM
    APL_LCD_WriteCmd(0xa2);				   //LCD偏压设置，V3时选
    APL_LCD_WriteCmd(0xa0);                //ADC  select  ,a0  normal,a1 reverse

    APL_LCD_WriteCmd(0xc8);				   //Com 扫描方式设置
    APL_LCD_WriteCmd(0x25);				   //V5内部电压调节电阻设置

    APL_LCD_WriteCmd(0x81);				   //电量设置模式（显示亮度）
    APL_LCD_WriteCmd(0x17);					//指令数据0x0000~0x003f

    APL_LCD_WriteCmd(0x2f);				   //设置上电控制模式
    APL_LCD_WriteCmd(0xF8);							   //指令数据
    APL_LCD_WriteCmd(0x00);							//booster 4x

    APL_LCD_WriteCmd(0xaf);						   //LCD On  */
	
//#else	
/*	
	APL_LCD_WriteCmd(0xAE);     //Set Display Off 
	
	APL_LCD_WriteCmd(0xd5);     //display divide ratio/osc. freq. mode	
	APL_LCD_WriteCmd(0x80);     //
	
	APL_LCD_WriteCmd(0xA8);     //multiplex ration mode:63 
	APL_LCD_WriteCmd(0x3F);
	
	APL_LCD_WriteCmd(0xD3);     //Set Display Offset   
	APL_LCD_WriteCmd(0x00);
	//APL_LCD_WriteCmd(0x3F);
	
	APL_LCD_WriteCmd(0x40);     //Set Display Start Line 
	
	APL_LCD_WriteCmd(0xAD);     //DC-DC Control Mode Set 
	//APL_LCD_WriteCmd(0x8a);     //DC-DC ON/OFF Mode Set 
	APL_LCD_WriteCmd(0x8B);     //DC-DC ON/OFF Mode Set 
	
	//APL_LCD_WriteCmd(0x32);     //Set Pump voltage value 
	
	//APL_LCD_WriteCmd(0xA1);     //Segment Remap
	APL_LCD_WriteCmd(0xA0); 	 
	
	//APL_LCD_WriteCmd(0xC8);     //Sst COM Output Scan Direction
	APL_LCD_WriteCmd(0xC0);
   	
	
	APL_LCD_WriteCmd(0xDA);     //common pads hardware: alternative	
	APL_LCD_WriteCmd(0x12);
	
	APL_LCD_WriteCmd(0x81);     //contrast control 
	//APL_LCD_WriteCmd(0x40);
	APL_LCD_WriteCmd(0x50);
	
	APL_LCD_WriteCmd(0xD9);	    //set pre-charge period	  
	APL_LCD_WriteCmd(0x1F);
	
	APL_LCD_WriteCmd(0xDB);     //VCOM deselect level mode 
	APL_LCD_WriteCmd(0x40);	    //
	
	APL_LCD_WriteCmd(0xA4);     //Set Entire Display On/Off	
	
	APL_LCD_WriteCmd(0xA6);     //Set Normal Display 
	
	APL_LCD_WriteCmd(0xAF);     //Set Display On  
	
*/
//#endif
	
}

void APL_LCD_Init_192(void)
{
    APL_LCD_Reset();

    APL_LCD_WriteCmd(0xe2);
    APL_LCD_WriteCmd(0xa0);
    APL_LCD_WriteCmd(0xeb);

    APL_LCD_WriteCmd(0x2f);				   //设置上电控制模式Internal VLCD
    APL_LCD_WriteCmd(0xc6);							//Bias Ratio

    APL_LCD_WriteCmd(0x81);							//Bias Ratio
    APL_LCD_WriteCmd(0xaa);							//Bias Ratio	

    APL_LCD_WriteCmd(0xaf);						   //LCD On  
	
    //APL_LCD_WriteCmd(0xa5);						   //LCD On  

    //APL_LCD_WriteByte(0x00);						   //LCD On  
}







