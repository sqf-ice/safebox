#include "stm32f10x.h"
#include "SPI_Flash.h"
#include "stm32f10x_spi.h"

void SPI_Flash_Init(void);
u8 SPI_Flash_ReadByte(void);
u8 SPI_Flash_SendByte(u8 byte);

void wip(void);
void wen(void);
void wdis(void);
void wsr(void);
unsigned char rdsr(void);
void FlashReadID(void);
void sect_clr(unsigned long a1);
unsigned char SST25_buffer[4096],fac_id,dev_id;

/* Select SPI FLASH: ChipSelect pin low  */
#define Select_Flash()     GPIO_ResetBits(GPIOC, GPIO_Pin_6)
/* Deselect SPI FLASH: ChipSelect pin high */
#define NotSelect_Flash()    GPIO_SetBits(GPIOC, GPIO_Pin_6)


//#define  SPI_SOLFT	1



void  spi_delay(void)
{

  unsigned short temp,i;


  for(i=0;i<3;i++)
  {	
	temp=1000;
	
	while(temp)
	{
	  temp--;
	
	}
  }

}

/*
//++++++++++++写使能 +++++++++++++++++

void wen(void){
	Select_Flash();
	SPI_Flash_SendByte(0x06);
	NotSelect_Flash();
}

//+++++ +++++++ 写禁止 ++++++++++++++++++
void wdis(void){

	Select_Flash();
	SPI_Flash_SendByte(0x04); 
	NotSelect_Flash();
	wip();
	
}	
//+++++++++++++++++++++++++++++++
void wsr(void){	
	Select_Flash();
	SPI_Flash_SendByte(0x50);
	NotSelect_Flash();
	//Delay(0xff);
	Select_Flash();
	SPI_Flash_SendByte(0x01);
	SPI_Flash_SendByte(0x00); 
	NotSelect_Flash();
    wip();
	
}

//++++++++++ 忙检测 +++++++++
void wip(void){
	unsigned char a=1;
	while((a&0x01)==1) a=rdsr();	

}
//++++++++++++++++++++++++++++++++
unsigned char rdsr(void){
	unsigned char busy;
	Select_Flash();
	SPI_Flash_SendByte(0x05);
	busy = SPI_Flash_ReadByte();
	NotSelect_Flash();
	return(busy);
	
}


//+++++++++++++字节写入+++++++++++++++++++++++
void bw1(unsigned long a1,unsigned char a2){

	
	wsr();	  
    wen();  
	Select_Flash();   
	SPI_Flash_SendByte(0x02);
	SPI_Flash_SendByte((a1&0xffffff)>>16);
	SPI_Flash_SendByte((a1&0xffff)>>8);
	SPI_Flash_SendByte(a1&0xff);
	SPI_Flash_SendByte(a2);	
	NotSelect_Flash();
	wip();				   	
	wdis();
	
}

//+++++++++字节读++++++++++++++++++++++++++++
unsigned char br1(unsigned long a1){
	unsigned char i;
	Select_Flash();
    SPI_Flash_SendByte(0x03);
	SPI_Flash_SendByte((a1&0xffffff)>>16);
	SPI_Flash_SendByte((a1&0xffff)>>8);
	SPI_Flash_SendByte(a1&0xff);
	i=SPI_Flash_ReadByte();	
	NotSelect_Flash(); 
	return(i);
}


//++++++++++连续读++++++++++++++++++++++++++++
void r_sect(unsigned long a){
	unsigned int i;
	Select_Flash();
	SPI_Flash_SendByte(0x03);
	SPI_Flash_SendByte((a&0xffffff)>>16);
	SPI_Flash_SendByte((a&0xffff)>>8);
	SPI_Flash_SendByte(a&0xff);
	for(i=0; i<816; i++){	
		SST25_buffer[i]=SPI_Flash_ReadByte();
	}
	NotSelect_Flash();
}

//+++++++++++++连续写入+++++++++++++++++++++++
void w_sect(unsigned long a1){
	unsigned int i=0,a2;
	
	wsr();
  	wen(); 
	Select_Flash();    
	SPI_Flash_SendByte(0xad);
	SPI_Flash_SendByte((a1&0xffffff)>>16);
	SPI_Flash_SendByte((a1&0xffff)>>8);
	SPI_Flash_SendByte(a1&0xff);
  	SPI_Flash_SendByte(SST25_buffer[0]);
	SPI_Flash_SendByte(SST25_buffer[1]);
	NotSelect_Flash();
	i=2;
	while(i<4096){
		a2=130;
		while(a2>0) a2--;
		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(SST25_buffer[i++]);
		SPI_Flash_SendByte(SST25_buffer[i++]);
		NotSelect_Flash();
	}
	
	a2=130;
	while(a2>0) a2--;
	wdis();	
	Select_Flash();  
	wip();	
	
}
//++++++++++块读++++++++++++++++++++++++++++
void SST25_R_BLOCK(uint32_t addr, u8 *readbuff, uint16_t BlockSize){
	unsigned int i=0;
	Select_Flash();
	SPI_Flash_SendByte(0x0b);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(0);
	while(i<BlockSize){	
		readbuff[i++]=SPI_Flash_ReadByte();
	}
	NotSelect_Flash();
}
//+++++++++++++块写入+++++++++++++++++++++++
void SST25_W_BLOCK(uint32_t addr, u8 *readbuff, uint16_t BlockSize){
	unsigned int i=0,a2;
	sect_clr(addr);    //清除addr开始的4096个字节的块
	wsr();
  	wen(); 
	Select_Flash();    
	SPI_Flash_SendByte(0xad);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
  	SPI_Flash_SendByte(readbuff[0]);
	SPI_Flash_SendByte(readbuff[1]);
	NotSelect_Flash();
	i=2;
	while(i<BlockSize){
		a2=60;
		while(a2>0) a2--;
		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(readbuff[i++]);
		SPI_Flash_SendByte(readbuff[i++]);
		NotSelect_Flash();
	}
	
	a2=100;
	while(a2>0) a2--;
	wdis();	
	Select_Flash();  
	wip();	
	
}
//++++++++++++++++++++++++++++++++
void sect_clr(unsigned long a1){
	//wen();
	
	wsr();
	wen();     
	Select_Flash();

	SPI_Flash_SendByte(0x20);
	SPI_Flash_SendByte((a1&0xffffff)>>16);          //addh
	SPI_Flash_SendByte((a1&0xffff)>>8);          //addl 
	SPI_Flash_SendByte(a1&0xff);          //wtt
	NotSelect_Flash();
	wip();
	
}


void FlashReadID(void)
{

	Select_Flash();	
  	SPI_Flash_SendByte(0x90);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
  	fac_id= SPI_Flash_ReadByte();		 //BFH: SST
	dev_id= SPI_Flash_ReadByte();	     //41H:SST25VF016B
  
  	NotSelect_Flash();	
}
*/
/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


#define PORT_SPI_CS               GPIOC
#define PORT_SPI_SCK              GPIOA
#define PORT_SPI_MISO             GPIOA
#define PORT_SPI_MOSI             GPIOA

#define PIN_SPI_CS                GPIO_Pin_6
#define PIN_SPI_SCK               GPIO_Pin_5
#define PIN_SPI_MISO              GPIO_Pin_6
#define PIN_SPI_MOSI              GPIO_Pin_7


#define SPI_CS_ENABLE             (PORT_SPI_CS->BRR  = PIN_SPI_CS)
#define SPI_CS_DISABLE            (PORT_SPI_CS->BSRR = PIN_SPI_CS)

#define SPI_SCK_LOW               (PORT_SPI_SCK->BRR  = PIN_SPI_SCK)
#define SPI_SCK_HIGH              (PORT_SPI_SCK->BSRR = PIN_SPI_SCK)

#define SPI_MISO_READ             (PORT_SPI_MISO->IDR & PIN_SPI_MISO)

#define SPI_MOSI_LOW              (PORT_SPI_MOSI->BRR  = PIN_SPI_MOSI)
#define SPI_MOSI_HIGH             (PORT_SPI_MOSI->BSRR = PIN_SPI_MOSI)

/*
void SPI_Delay_emu(void)
{
  uint16_t cnt = 80;

  while(cnt--);
}
  */

void SPI_Flash_Init(void)
{


#ifdef SPI_SOLFT


   
  GPIO_InitTypeDef GPIO_InitStructure;

  // CS //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PIN_SPI_CS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // SCK //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // MISO //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // MOSI //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_CS_DISABLE;
  SPI_SCK_HIGH;
  SPI_MOSI_HIGH;
    SPI_Delay_emu();
    SPI_Delay_emu();
	 SPI_Delay_emu();	 



#else

  SPI_InitTypeDef  SPI_InitStructure;
 GPIO_InitTypeDef GPIO_InitStructure;
   

  // Enable SPI2 GPIOB clocks //
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
 /* // Configure SPI2 pins: SCK, MISO and MOSI //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // Configure PB.12 as Output push-pull, used as Flash Chip select //

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		  
  //GPIO_Init(GPIOB, &GPIO_InitStructure);	   */

  // Deselect the FLASH: Chip Select high //
  //NotSelect_Flash();
  //;


		   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PIN_SPI_CS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // SCK //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // MISO //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // MOSI //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // SPI2 configuration // 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  // Enable SPI2  //
  SPI_Cmd(SPI1, ENABLE);   
  NotSelect_Flash();
  GPIO_SetBits(GPIOC, GPIO_Pin_6);
  //GPIO_SetBits(GPIOB, GPIO_Pin_7);
  //GPIO_SetBits(GPIOA, GPIO_Pin_7);

#endif  


}


void SPI_Flash_Init_ISU256(void)
{


#ifdef SPI_SOLFT


   
  GPIO_InitTypeDef GPIO_InitStructure;

  // CS //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PIN_SPI_CS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // SCK //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // MISO //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // MOSI //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_CS_DISABLE;
  SPI_SCK_HIGH;
  SPI_MOSI_HIGH;
    SPI_Delay_emu();
    SPI_Delay_emu();
	 SPI_Delay_emu();	 



#else

  SPI_InitTypeDef  SPI_InitStructure;
 GPIO_InitTypeDef GPIO_InitStructure;
   

  // Enable SPI2 GPIOB clocks //
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOC, ENABLE);
 /* // Configure SPI2 pins: SCK, MISO and MOSI //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  // Configure PB.12 as Output push-pull, used as Flash Chip select //

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		  
  //GPIO_Init(GPIOB, &GPIO_InitStructure);	   */

  // Deselect the FLASH: Chip Select high //
  //NotSelect_Flash();
  //;


		   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PIN_SPI_CS;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // SCK //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // MISO //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // MOSI //
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  	   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  // SPI2 configuration // 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  
  // Enable SPI2  //
  SPI_Cmd(SPI1, ENABLE);   
  //NotSelect_Flash();
  GPIO_SetBits(GPIOC, GPIO_Pin_7);
    GPIO_SetBits(GPIOE, GPIO_Pin_8);
  //GPIO_SetBits(GPIOB, GPIO_Pin_7);
  //GPIO_SetBits(GPIOA, GPIO_Pin_7);

#endif  


}


/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u8 SPI_Flash_ReadByte(void)
{


 #ifdef SPI_SOLFT
   
    uint8_t cnt;
  uint8_t RxData = 0;

  for(cnt=0; cnt<8; cnt++)
  {
    SPI_SCK_LOW;                                 //时钟 - 低
    SPI_Delay_emu();

    RxData <<= 1;
    if(SPI_MISO_READ)                            //读取数据
    {
      RxData |= 0x01;
    }

    SPI_SCK_HIGH;                                //时钟 - 高
    SPI_Delay_emu();
  }

  return RxData;	

 #else


	  return (SPI_Flash_SendByte(0xFF));

 #endif

}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/


unsigned int g_SPI_srdelay = 0;
u8 SPI_Flash_SendByte(u8 byte)
{
  
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  //NotSelect_Flash();  while(1);
  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}


unsigned char RRByte(unsigned char var)
{
  unsigned char temp;
	temp=var;
	
	temp = (temp>>1);
	
	if(var&0x01)
	{
	  temp|=0x80;
	}
	else
	{
	   temp&=0x7F;
	
	}
  return temp;
}



	  #define Select_IS8()     GPIO_ResetBits(GPIOC, GPIO_Pin_7)

#define NotSelect_IS8()    GPIO_SetBits(GPIOC, GPIO_Pin_7)


u8 SPI_Flash_SendByte_w(u8 byte)
{

 #ifdef SPI_SOLFT

 
   uint8_t cnt;

  for(cnt=0; cnt<8; cnt++)
  {
    SPI_SCK_LOW;                                 //时钟 - 低
    SPI_Delay_emu();

    if(byte & 0x80)                            //发送数据
      SPI_MOSI_HIGH;
    else
      SPI_MOSI_LOW;
    byte <<= 1;

    SPI_Delay_emu();
    SPI_SCK_HIGH;                                //时钟 - 高
    SPI_Delay_emu();
  }
		  


 #else
// unsigned char temp;
 unsigned int i,j;

  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  //NotSelect_Flash();  while(1);
  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI1, byte);


   for(i=0;i<g_SPI_srdelay;i++)
  {
  	 j=10;
	 while(j--);
  
  }

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
 //return 0;
  /* Return the byte read from the SPI bus */
  return(SPI_I2S_ReceiveData(SPI1));


 #endif
 

}

unsigned char SPI_Delay_Time=200;

void  ISU_SPI_Delay(void)
{
  unsigned short temp,i;


  for(i=0;i<SPI_Delay_Time;i++)
  {	
	temp=100;
	
	while(temp)
	{
	  temp--;
	
	}
  }


}


void  SPI_Delay(void)
{
  unsigned short temp,i;


  for(i=0;i<100;i++)
  {	
	temp=10000;
	
	while(temp)
	{
	  temp--;
	
	}
  }


}



void SPI_sendNByte(unsigned char * buf, unsigned int len)
{

   unsigned int i;

   for(i=0;i<len;i++)
    {
       SPI_Flash_SendByte( buf[i]);
       //SPI_Delay();
       //wip();
    }


}

void ISU_SPI_sendNByte(unsigned char * buf, unsigned int len)
{

   unsigned int i;

   for(i=0;i<len;i++)
    {
	  Select_IS8();
	  ISU_SPI_Delay();
       SPI_Flash_SendByte_w( buf[i]);
       		ISU_SPI_Delay();

	      NotSelect_IS8() ;
		  ISU_SPI_Delay();
		  ISU_SPI_Delay();
       //wip();
    }


}

void SPI_recieveNByte(unsigned char * buf, unsigned int len)
{
	    unsigned int i;

   for(i=0;i<len;i++)
    {
       buf[i] = SPI_Flash_ReadByte();
		 //buf[i] = RRByte(buf[i]);
			
    }

}

void ISU_SPI_recieveNByte(unsigned char * buf, unsigned int len)
{

   unsigned int i;
	 
   for(i=0;i<len;i++)
    {

	 Select_IS8();
	 ISU_SPI_Delay();
       buf[i] = SPI_Flash_SendByte_w(0xFF);
	  		 
		 //buf[i] = RRByte(buf[i]);
			ISU_SPI_Delay();
		    NotSelect_IS8() ;
		    ISU_SPI_Delay();
			ISU_SPI_Delay();
			
    }
	  

}


 
 unsigned char ISU_SPI_recieveOneByte(void)
 {
 	 unsigned char temp;
 	 Select_IS8();
	 ISU_SPI_Delay();
       temp = SPI_Flash_ReadByte();
	  
		 //buf[i] = RRByte(buf[i]);
			 ISU_SPI_Delay();
		  NotSelect_IS8() ;
		    ISU_SPI_Delay();
			ISU_SPI_Delay();

			return temp;
 
 }
   

