#include <stm32f10x_lib.h>               
#include "SPI_SD_driver.h"

unsigned char  SD_Type=0;
void SPI_GPIO(void);
void SPI_Configuration(void);
unsigned char SD_Init(void);
unsigned long SD_GetCapacity(void);
unsigned char SD_WriteSingleBlock(unsigned long sector, const unsigned char *data);
unsigned char SD_ReadSingleBlock(unsigned long sector, unsigned char *buffer);
void SPI_Configuration_SD(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
	return;
}


void SPI_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;


    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO  , ENABLE);
  
  
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   

}


void SPI_SetSpeed(unsigned char SpeedSet)
{
    SPI_InitTypeDef SPI_InitStructure;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  
    if(SpeedSet==SPI_SPEED_LOW)
    {
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    }
    else
    {
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    }
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
    return;
}



unsigned char SPI_ReadWriteByte(unsigned char TxData)
{
    unsigned char RxData = 0;
    
  
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    
    SPI_I2S_SendData(SPI1, TxData);

 
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    RxData = SPI_I2S_ReceiveData(SPI1);

    return (u8)RxData;
}


unsigned char SD_WaitReady(void)
{
    unsigned char r1;
    unsigned short retry;
    retry = 0;
    do
    {
        r1 = SPI_ReadWriteByte(0xFF);
        if(retry==0xfffe)
        {
            return 0;
        }
	//	retry++;
    }while(r1!=0xFF);

    return 0;
}


unsigned char SD_SendCommand(unsigned char cmd, unsigned long arg, unsigned char crc)
{
    unsigned char r1;
    unsigned char Retry = 0;


    SPI_ReadWriteByte(0xff);

    SD_CS_ENABLE();

   
    SPI_ReadWriteByte(cmd | 0x40);                       
    SPI_ReadWriteByte(arg >> 24);
    SPI_ReadWriteByte(arg >> 16);
    SPI_ReadWriteByte(arg >> 8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);
    
   
    while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }
    

    
    SD_CS_DISABLE();
 
    SPI_ReadWriteByte(0xFF);

    return r1;
}



unsigned char SD_SendCommand_NoDeassert(unsigned char cmd, unsigned long arg, unsigned char crc)
{
    unsigned char r1;
    unsigned char Retry = 0;
 
    SPI_ReadWriteByte(0xff);
 
    SD_CS_ENABLE();
 
    SPI_ReadWriteByte(cmd | 0x40);                         //分别写入命令
    SPI_ReadWriteByte(arg >> 24);
    SPI_ReadWriteByte(arg >> 16);
    SPI_ReadWriteByte(arg >> 8);
    SPI_ReadWriteByte(arg);
    SPI_ReadWriteByte(crc);

    while((r1 = SPI_ReadWriteByte(0xFF))==0xFF)
    {
        Retry++;
        if(Retry > 200)
        {
            break;
        }
    }

    return r1;
}



u8 SD_Init(void)
{



    unsigned short i;     
    unsigned char r1;    
    unsigned short retry;  
    unsigned char buff[6];

  
    if(!SD_DET())
    {
               
        return  99 ;  
    }


    for(i=0;i<0xf00;i++);

  
    for(i=0;i<10;i++)
    {
        SPI_ReadWriteByte(0xFF);
    }


    retry = 0;
    do
    {
  
        r1 = SD_SendCommand(CMD0, 0, 0x95);
        retry++;
    }while((r1 != 0x01) && (retry<200));
 
    if(retry==200)
    {
        return 1;   
    }
    

    r1 = SD_SendCommand_NoDeassert(8, 0x1aa, 0x87);

   
    if(r1 == 0x05)
    {
       
        SD_Type = SD_TYPE_V1;

        SD_CS_DISABLE();
     
        SPI_ReadWriteByte(0xFF);

        retry = 0;
        do
        {
    
            r1 = SD_SendCommand(CMD55, 0, 0);
            if(r1 != 0x01)
            {
                return r1;  
            }
           
            r1 = SD_SendCommand(ACMD41, 0, 0);
            retry++;
        }while((r1!=0x00) && (retry<400));

        
        if(retry==400)
        {
            retry = 0;
            
            do
            {
                r1 = SD_SendCommand(1, 0, 0);
                retry++;
            }while((r1!=0x00)&& (retry<400));
            if(retry==400)
            {
                return 1; 
            }
          
            SD_Type = SD_TYPE_MMC;
        }

        SPI_SetSpeed(1);

		SPI_ReadWriteByte(0xFF);
        
        /*
		r1 = SD_SendCommand(CMD59, 0, 0x01);
        if(r1 != 0x00)
        {
            return r1;  //?üá?′í?ó￡?·μ??r1
        }
        */    
        
        r1 = SD_SendCommand(CMD16, 512, 0xff);
		//r1 = SD_SendCommand(CMD16, 128, 0xff);
        if(r1 != 0x00)
        {
            return r1;  
        }
        

    }
    


    else if(r1 == 0x01)
    {
       
        buff[0] = SPI_ReadWriteByte(0xFF);  //should be 0x00
        buff[1] = SPI_ReadWriteByte(0xFF);  //should be 0x00
        buff[2] = SPI_ReadWriteByte(0xFF);  //should be 0x01
        buff[3] = SPI_ReadWriteByte(0xFF);  //should be 0xAA
     
        SD_CS_DISABLE();
        //the next 8 clocks
        SPI_ReadWriteByte(0xFF);
        
   
        if(buff[2]==0x01 && buff[3]==0xAA)
        {
            
            retry = 0;
       
    		do
    		{
    			r1 = SD_SendCommand(CMD55, 0, 0);
    			if(r1!=0x01)
    			{
    				return r1;
    			}
    			r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
                if(retry>200)   
                {
                    return r1;  
                }
            }while(r1!=0);
          
         

      
            r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
            if(r1!=0x00)
            {
                return r1;  
            }

            buff[0] = SPI_ReadWriteByte(0xFF);
            buff[1] = SPI_ReadWriteByte(0xFF); 
            buff[2] = SPI_ReadWriteByte(0xFF);
            buff[3] = SPI_ReadWriteByte(0xFF);

          
            SD_CS_DISABLE();
            SPI_ReadWriteByte(0xFF);


            if(buff[0]&0x40)    //?ì2éCCS
            {
                SD_Type = SD_TYPE_V2HC;
            }							   
            else
            {
                SD_Type = SD_TYPE_V2;
            }
           

            
     
            SPI_SetSpeed(1);  
        }

    }
    return r1;




}


unsigned char SD_ReceiveData(unsigned char *data, unsigned short len, unsigned char release,unsigned char mod)
{
    unsigned short retry;
    unsigned char r1;


    SD_CS_ENABLE();

    retry = 0;
  
   if(mod==1)			                                                  
   {
	    do
	    {
	        r1 = SPI_ReadWriteByte(0xFF);
	        retry++;
	        if(retry>2000)  //2000次等待后没有应答，退出报错
	        {
	            SD_CS_DISABLE();
	            return 1;
	        }
	    }while(r1 != 0xFE);		
   }

    while(len--)
    {
        *data = SPI_ReadWriteByte(0xFF);
        data++;
    }

    SPI_ReadWriteByte(0xFF);
    SPI_ReadWriteByte(0xFF);

    if(release == RELEASE)
    {
       
        SD_CS_DISABLE();
        SPI_ReadWriteByte(0xFF);
    }

    return 0;
}




unsigned char SD_GetCID(unsigned char *cid_data)
{
    unsigned char r1;

    
    r1 = SD_SendCommand(CMD10, 0, 0xFF);
    if(r1 != 0x00)
    {
        return r1; 
    }

    SD_ReceiveData(cid_data, 16, RELEASE,0);

    return 0;
}


unsigned char SD_GetCSD(unsigned char *csd_data)
{
    unsigned char r1;


    r1 = SD_SendCommand(CMD9, 0, 0xFF);
	 SD_ReceiveData(csd_data, 16, RELEASE,0);
    if(r1 != 0x00)
    {
        return r1;  
    }

   

    return 0;
}



unsigned long SD_GetCapacity(void)
{
    unsigned char csd[16];
    unsigned long Capacity;
    unsigned char r1;
    unsigned short i;
	unsigned short temp;

    
    if(SD_GetCSD(csd)!=0)
    {
        return 0;
    }
       
    
    if((csd[0]&0xC0)==0x40)
    {
        Capacity =  ((((u32)csd[8]*0x100) +  (u32)csd[9])+1)*512; //*1024;
    }
    else
    {

        //C_SIZE
    	i = csd[6]&0x03;
    	i<<=8;
    	i += csd[7];
    	i<<=2;
    	i += ((csd[8]&0xc0)>>6);
    
        //C_SIZE_MULT
    	r1 = csd[9]&0x03;
    	r1<<=1;
    	r1 += ((csd[10]&0x80)>>7);
    
        //BLOCKNR
    	r1+=2;
    	temp = 1;
    	while(r1)
    	{
    		temp*=2;
    		r1--;
    	}
    	Capacity = ((u32)(i+1))*((u32)temp);
    
        // READ_BL_LEN
    	i = csd[5]&0x0f;
    
    	temp = 1;
    	while(i)
    	{
    		temp*=2;
    		i--;
    	}
    
    	Capacity *= (u32)temp; 
   
    }
    return (u32)Capacity;
}


unsigned char SD_ReadSingleBlock(unsigned long sector, unsigned char *buffer)
{
	u8 r1;


    SPI_SetSpeed(SPI_SPEED_HIGH);
    

  //  sector = sector<<9;
      if(SD_Type!=SD_TYPE_V2HC)
    {
        sector = sector<<9;
    }


	r1 = SD_SendCommand(CMD17, sector, 0);
	
	if(r1 != 0x00)
    {
        return r1;
    }
    
    r1 = SD_ReceiveData(buffer, 512, RELEASE,1);
    if(r1 != 0)
    {
        return r1;   
    }
    else
    {
        return 0;
    }
}



unsigned char SD_WriteSingleBlock(unsigned long sector, const unsigned char *data)
{
    unsigned char r1;
    unsigned short i;
    unsigned short retry;

 
    SPI_SetSpeed(SPI_SPEED_HIGH);


    if(SD_Type!=SD_TYPE_V2HC)
    {
        sector = sector<<9;
    }

    r1 = SD_SendCommand(CMD24, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1; 
    }
    

    SD_CS_ENABLE();

    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
 
    SPI_ReadWriteByte(0xFE);

    for(i=0;i<512;i++)
    {
        SPI_ReadWriteByte(*data++);
    }

    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);
    
    r1 = SPI_ReadWriteByte(0xff);
    if((r1&0x1F)!=0x05)
    {
        SD_CS_DISABLE();
        return r1;
    }
    

    retry = 0;
    while(!SPI_ReadWriteByte(0xff))
    {
        retry++;
        if(retry>0xfffe)       
        {
            SD_CS_DISABLE();
            return 1;          
        }
    }

    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    return 0;
}


/*
unsigned char SD_ReadMultiBlock( unsigned long sector, unsigned char *buffer, unsigned char count)
{
    unsigned char r1;


    SPI_SetSpeed(SPI_SPEED_HIGH);
    
 
    sector = sector << 9;
    //SD_WaitReady();

	r1 = SD_SendCommand(CMD18, sector, 0);
	if(r1 != 0x00)
    {
        return r1;
    }

    do
    {
        if(SD_ReceiveData(buffer, 128, NO_RELEASE) != 0x00)
        {
            break;
        }
        buffer += 512;
    } while(--count);


    SD_SendCommand(CMD12, 0, 0);

    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xFF);
    
    if(count != 0)
    {
        return count;   
    }
    else
    {
        return 0;
    }
}



unsigned char SD_WriteMultiBlock(unsigned long sector, const unsigned char *data, unsigned char count)
{
    unsigned char r1;
    unsigned short i;

   
    SPI_SetSpeed(SPI_SPEED_HIGH);

   
    if(SD_Type != SD_TYPE_V2HC)
    {
        sector = sector<<9;
    }
 
    if(SD_Type != SD_TYPE_MMC)
    {
        r1 = SD_SendCommand(ACMD23, count, 0x00);
    }
 
    r1 = SD_SendCommand(CMD25, sector, 0x00);
    if(r1 != 0x00)
    {
        return r1; 
    }
    
  
    SD_CS_ENABLE();
    SPI_ReadWriteByte(0xff);
    SPI_ReadWriteByte(0xff);

    
    do
    {
       
        SPI_ReadWriteByte(0xFC);
    
        
        for(i=0;i<512;i++)
        {
            SPI_ReadWriteByte(*data++);
        }
       
        SPI_ReadWriteByte(0xff);
        SPI_ReadWriteByte(0xff);
        
        
        r1 = SPI_ReadWriteByte(0xff);
        if((r1&0x1F)!=0x05)
        {
            SD_CS_DISABLE();    
            return r1;
        }

    
        if(SD_WaitReady()==1)
        {
            SD_CS_DISABLE();   
            return 1;
        }


    }while(--count);
    
   
    r1 = SPI_ReadWriteByte(0xFD);
    if(r1==0x00)
    {
        count =  0xfe;
    }

    if(SD_WaitReady())
    {
        while(1)
        {
        }
    }
    
    
    SD_CS_DISABLE();
    SPI_ReadWriteByte(0xff);

    return count;  
}
 */




