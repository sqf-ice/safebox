#include "SPI_Flash.h"
//#include "Gpio.h"
#include "..\INC\headfile.h"
#include "..\umass\SPI_SD_driver.h"
//#include "spi/spi.h" 
//#include "Declare_SRC.h"
//#include "Declare_API.h"
#include "stm32f10x_gpio.h"

#define xdata     


unsigned char Flashcmd[5];


unsigned char  Select_SD_SPIFlash  = 0;
void Select_Flash()  
{
	 if(Select_SD_SPIFlash==0)
	 {
	 	  GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	 }
	 else
	 {
	 
	 	    GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	 }
}  


/* Deselect SPI FLASH: ChipSelect pin high */
void NotSelect_Flash()   
{
	 if(Select_SD_SPIFlash==0)
	 {
	 	  GPIO_SetBits(GPIOC, GPIO_Pin_6);
	 }
	 else
	 {
	 
	 	  GPIO_SetBits(GPIOC, GPIO_Pin_4);
	 }
}


#define SD_CS_ENABLE()     Select_Flash()  // GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET)
#define SD_CS_DISABLE()     NotSelect_Flash()    //(GPIOA, GPIO_Pin_1, Bit_SET)    //不选中SD卡

extern void  spi_delay(void);

///////////////////////////////////////////////////////////////////////////////////////////////////



void spi_ReadStatusRegister(void)
{

    unsigned char Flashcmd1[5];
 	  
	  SD_CS_ENABLE();
    
	  do
    {
        Flashcmd1[0] = 0x05;	
			  SPI_sendNByte(Flashcmd1,1);
			  SPI_recieveNByte(Flashcmd1,1);
			
    }while((Flashcmd1[0]&0x01) == 0x01);
    
		SD_CS_DISABLE();
	
	
	
}


void spi_WriteEnable(void)
{
    spi_ReadStatusRegister();

    SD_CS_ENABLE();	  

    Flashcmd[0] = 0x06;
    SPI_sendNByte(&Flashcmd[0], 1);

  	SD_CS_DISABLE();
}

void APL_ExFlash_EraseBlock(unsigned long BlockAddr)
{
    BlockAddr &= 0x00FF0000;

    spi_ReadStatusRegister();

    spi_WriteEnable();

    SD_CS_ENABLE();	 	 
	
    Flashcmd[0] = 0xD8;
    Flashcmd[1] = (unsigned char)(BlockAddr>>16);
    Flashcmd[2] = (unsigned char)(BlockAddr>>8);
    Flashcmd[3] = (unsigned char) BlockAddr;
  
    SPI_sendNByte(Flashcmd, 0x04);
	   
    SD_CS_DISABLE();
}


BYTE APL_ExFlash_GetCapcity(void)
{
    spi_ReadStatusRegister();

    SD_CS_ENABLE();	  
    
    Flashcmd[0] = 0x9F;
    Flashcmd[1] = 0x00;
    Flashcmd[2] = 0x00;
    Flashcmd[3] = 0x00;

    SPI_sendNByte(Flashcmd, 1);
    SPI_recieveNByte(Flashcmd, 3);

    SD_CS_DISABLE();	  

    return Flashcmd[2];
}


   void Delayspi(unsigned int num)
   {
   
       unsigned int numt;
		numt = num;
	   while(numt--);
   
   }

//发送读指令,地址，字节长度n, 字节存放地址，读取n bytes长度
void APL_ExFlash_ReadData (unsigned long ReadAddr,unsigned char xdata* DataAddr,unsigned int n) 
{
//    unsigned  int i;
    
    spi_ReadStatusRegister();

    SD_CS_ENABLE();	  
  
    Flashcmd[0] = 0x0B;
    Flashcmd[1] = (unsigned char)(ReadAddr>>16);
    Flashcmd[2] = (unsigned char)(ReadAddr>>8);
    Flashcmd[3] = (unsigned char) ReadAddr;
    Flashcmd[4] = 0x00;

    SPI_sendNByte(Flashcmd, 0x05);
	//Delayspi(1000);
    SPI_recieveNByte(DataAddr, n);

    SD_CS_DISABLE();	   
}

BYTE APL_ExFlash_WriteData(unsigned long PageAddr, BYTE xdata *pbString, UINT16 Len)
{
//    unsigned  int i;
    //spi_delay();
        //读写保护的状态	 
	spi_ReadStatusRegister();
		
	spi_WriteEnable();

   SD_CS_ENABLE();	 	  
        
	Flashcmd[0] = 0x02;
	Flashcmd[1] = (unsigned char)(PageAddr>>16);
	Flashcmd[2] = (unsigned char)(PageAddr>>8);
	Flashcmd[3] = (unsigned char) PageAddr;

	SPI_sendNByte(Flashcmd, 0x04);

	SPI_sendNByte(pbString, Len);
 

	SD_CS_DISABLE();
	
	//spi_delay();	 
		
	return 0;
}



void Spi_Chip_Erase(void)
{
    spi_ReadStatusRegister();

    spi_WriteEnable();

    SD_CS_ENABLE();	  

    Flashcmd[0] = 0xC7;
    SPI_sendNByte(Flashcmd, 1);

    SD_CS_DISABLE();	   
    
    spi_ReadStatusRegister();
}


//////////////////////////////////////////////////////









