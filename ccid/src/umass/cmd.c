#include "headfile.h"
#include "globle.h"
#include "TS36EBG.h"
#include "SPI_SD_driver.h"
#include "../usart/usart.h"	
#include "lcd_show.h"




extern unsigned char TS36ebg_Transbuf1[395];
extern void SetBootMode(unsigned char mode);
extern void  DES_Triple_Encrypt(unsigned char *m ,unsigned char * k, unsigned char * e,int  n,int * rn);
extern void  DES_Triple_Decrypt(unsigned char *m ,unsigned char * k, unsigned char * e,int  n,int * rn);
extern unsigned char GetKey(unsigned char * key,unsigned char flag,unsigned char index);
extern unsigned char SaveKey(unsigned char * key,unsigned char index,unsigned char flag);
extern unsigned char CheckKey(unsigned char * key1, unsigned char * key2, unsigned char keylen);
extern unsigned char IsRootKeyHas(unsigned char ctl);
extern void InitRootKey(void);
extern  unsigned char THM_TestReady(void);
extern unsigned char Test_ICCReady(void);
extern unsigned char buf_typeAB[20];
extern int GetAD(void);
unsigned char pinmod=0;
extern unsigned int g_SPI_srdelay;
extern unsigned char SPI_Delay_Time;
extern void  SPI_Delay(void);
extern unsigned char  Select_SD_SPIFlash;
extern unsigned  char SM_2B_SendFree(void);
extern unsigned char UsbBulkInMsgBuf[2300];
#define   TS36ebg_Transbuf  UsbBulkInMsgBuf 


extern unsigned char g_configflag;
void Reset_Device(void);

//----------------------------------------------------------版本号
#define VER1  ('H')
#define VER2  ('2')
#define VER3  ('0')
#define VER4  ('2')
#define VER5  ('8')

//---------------------------------------------------------------------


const unsigned char ImageOrder[30][11]	  =
{

	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x00,0x1F,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x01,0x1E,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x02,0x1D,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x03,0x1C,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x04,0x1B,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x05,0x1A,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x06,0x19,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x07,0x18,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x08,0x17,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x09,0x16,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x0A,0x15,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x0B,0x14,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x0C,0x13,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x0D,0x12,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x0E,0x11,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x0F,0x10,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x10,0x0F,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x11,0x0E,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x12,0x0D,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x13,0x0C,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x14,0x0B,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x15,0x0A,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x16,0x09,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x17,0x08,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x18,0x07,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x19,0x06,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x1A,0x05,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x1B,0x04,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x1C,0x03,0x03},
	{0x02,0x00,0x06,0x19,0x00,0x00,0x00,0x00,0x1D,0x02,0x03}

};
              




BYTE cmd_0000(void)
{



  				CMD_DATA[0]=0x3B;
				CMD_DATA[1]=0x7C;
				CMD_DATA[2]=0x94;
				CMD_DATA[3]=0x00;
				CMD_DATA[4]=0x00;
				CMD_DATA[5]=0x80;
				CMD_DATA[6]=0x64;
				CMD_DATA[7]=0x51;
				CMD_DATA[8]=0x4D;
				CMD_DATA[9]=0x86;
				CMD_DATA[10]=0x53;
				CMD_DATA[11]=0x85;
			    CMD_DATA[12]=VER2;
				CMD_DATA[13]=VER3;
				CMD_DATA[14]=VER4;
				CMD_DATA[15]=VER5;
				CMD_DATA[16]=0x48;

				gb_Le  = 0x11;	

				 ShowDefault();


	  return SW9000; 

}


extern unsigned char buffer[2050];

#define  InBuff  buffer 


extern  unsigned char  IS8_CheckStatus(unsigned char * buf);

BYTE cmd_0084(void) 
{

   unsigned short i,len;

   unsigned char DataTest[] ={0xE6, 0x98, 0xBE, 0xE7, 0xA4, 0xBA, 0xE5, 0xB1 ,0x8F ,0xE6 ,0xB5 ,0x8B ,0xE8 ,0xAF ,0x95};

   if(CMD_P1==0)
   {
	   for(i=0;i<CMD_P3;i++)
	   {
	
	    	CMD_DATA[i]=i;
				  
	   }
   }
   else if(CMD_P1==1)
   {
   
	   for(i=0;i<CMD_P3;i++)
	   {
	
	    	if(CMD_DATA[i]!=i)
			{
			
			     gb_Le  = CMD_P3;
				 return SW6E00;
			
			}
				  
	   }
   
   }	
   else if(CMD_P1==2)
   {
   	    LCD_Init();
        LCD_Fill(0x0F);
   
   }
   else if(CMD_P1==3)
   {
       		  // SetSysClockTo20();
		 SPI_Flash_Init();
		// SPI_Configuration();
		 
		 APL_ExFlash_GetCapcity();
		 
		 APL_ExFlash_EraseBlock(0);
		 
		 APL_ExFlash_ReadData(0,InBuff,256);
		 
		 for (i = 0; i < 256; i++)
        {
           InBuff[i] = i;
        }
		  	//SPI_Delay();
		 APL_ExFlash_WriteData(0,InBuff,256);
		 	SPI_Delay();
		 
		 for (i = 0; i < 256; i++)
         {
              CMD_DATA[i] = 0xEE;
         } 		 
		 
		 APL_ExFlash_ReadData(0,CMD_DATA,256);
		// APL_ExFlash_ReadData(1,InBuff,256);
		// APL_ExFlash_ReadData(2,InBuff,256);
		// APL_ExFlash_EraseBlock(0);
		 
		// APL_ExFlash_ReadData(0,InBuff,256);
		    //SetSysClockTo72();
        
		 gb_Le  = 255;

		 return SW9000;
   
   }
   else if(CMD_P1==4)
   {
   
        LCD_Init();
		 SPI_Flash_Init();
		//LCD_Fill(0xFF);
		//LCD_display_UTF8(strhz,18);
		LCD_display_UTF8Line(DataTest,15,0);
       // ipt_LCD_display(1, 2);
   
   
   
   }
    else if(CMD_P1==5)
	{

	 /* 
         
	*/	  
 

         
	}
      else if(CMD_P1==6)
	{
		  //SPI_Flash_Init_ISU256();


	}
   
   gb_Le  = CMD_P3;

   return   SW9000;

}
extern void TS36ebg_UartInit(void);
extern  unsigned char TS36EBG_CONNECT(unsigned  char *buf);
extern  unsigned char   TS36ebg_UartSendData(unsigned char * buf,unsigned short len );
extern  unsigned char   TS36ebg_UartRecieveData(unsigned char * buf,unsigned short * len );
extern void SPI_GPIO(void);
extern void SPI_Configuration(void);
extern unsigned char SD_Init(void);
extern unsigned long SD_GetCapacity(void);
extern unsigned char SD_WriteSingleBlock(unsigned long sector, const unsigned char *data);
extern unsigned char SD_ReadSingleBlock(unsigned long sector, unsigned char *buffer);
extern unsigned char UsbBulkInMsgBuf[2300];	 //与USB通信的缓存buf
extern unsigned char UsbBulkMsgBuf[271];
extern unsigned char MAG_XfrBlock(unsigned char *T2T3rbuf,unsigned int len);
extern unsigned char mag_flag(void);
extern void CLR_mag_flag(void);
extern unsigned char IFD_nContact_Slot_GetStatus(void);
extern void CCID_AnalyseBulkMessage(void);
extern void CCID_AnalyseBulkMessage1(void);
extern unsigned char IC_inset(void);
extern unsigned char search_card(unsigned char *buf_atr,unsigned int *rlen);
extern unsigned char search_typeb1(void);
extern void SAM1_init(void);
extern void SAM2_init(void);
extern void beep_on(void);
extern void beep_con(void);
extern void beep_off(void);
extern void led1_on(void);
extern void led1_off(void);
extern void led2_on(void);
extern void led2_off(void);
extern void led3_on(void);
extern void led3_off(void);
extern void led4_on(void);
extern void led4_off(void);
extern void led5_on(void);
extern void led5_off(void);
extern unsigned char get_key(void);
extern void ISO14443_carrier_off();
extern void ISO14443_carrier_on(void);
extern unsigned char   TS36ebg_UartSendData(unsigned char * buf,unsigned short len );
extern unsigned char   TS36ebg_UartRecieveData(unsigned char * buf,unsigned short * len );
extern unsigned char  slot1;
extern unsigned char  card_type;
extern unsigned char buf_typeAB[20];
unsigned int waittimeout=0;
unsigned char waitflag=0,less_flag=0;
unsigned char ASk_ATR[10]={0x62,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char times1=0;	
extern void ISO14443_reset(u8 t_reset);


extern unsigned char	pinmod;

void getPinBlock(unsigned char *  inbuf ,unsigned char * outbuf , unsigned char  len , unsigned char flag)
{
    unsigned char tlen,r,i,t;

		 if(len>12)  
		 {
		    tlen =12;
		 }
		 else
		 {
		 	 tlen = len;
		 }

	   if(flag==1)
	   {
	   	   for(i=0;i<8;i++)
           {	 
		     outbuf[i] = 0xFF;
    	   }
	   }
	   else
	   {
	   	   for(i=0;i<16;i++)
           {	 
		     outbuf[i] = 0xFF;
    	   }

	   }

		 outbuf[0]=len;

		 t = (len % 2);

         if(t == 0)
		 {
		 	r = (len / 2);
		 }
		 else
		 {
		    r = ((len-1) / 2);	 
		 }

	
    	for(i=0;i<r;i++)
	    {	 
		     outbuf[i+1] = 	 ((inbuf[2*i]<<4)|(inbuf[2*i+1]&0x0F));
	    }

		if(t)
		{ 
		 	 outbuf[i+1] = ((outbuf[i+1]&0x0F)|(inbuf[len-1]<<4));
		}



}

unsigned char CheckCancleByteReceive(void)
{

    unsigned int cancle;

			 if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == !RESET) 
			{
				cancle = (USART_ReceiveData(UART4));
				USART_ClearFlag(UART4, USART_FLAG_RXNE);
				if(cancle==0xFa)
				{
					//	waitflag =0;
					//	led2_off();
					  // led3_off();
					   //led1_off();
					   //SoftCloRf();
						return 1;
				}
			}

			return 0;
 }
 

unsigned char WEL_Image_Flag; 
unsigned int  image_index = 0; 
unsigned char imageflag	 =0;
unsigned char cmd_i=0;
unsigned char cmd_s=0;


 																							 
BYTE cmd_F000(void)
{
unsigned int ret=0,cancle;
unsigned char r;
unsigned char temp_rbuf[16],key_val=0xff,temp=0xff;
unsigned int res=0;
unsigned int i,t,len;				 //len_s=0 ,
unsigned short tl,rl,j;
unsigned char cmdflag;

unsigned char rootkey[24];
unsigned char usedkey[24];   

	 
	 if(CMD_P1==0x00)	//读磁条卡数据
	 {	
	 	for(j=0;j<120;j++)
	 	CMD_DATA[j]=0;
		 waittimeout = CMD_P2*1000;
		 waitflag =1;
		 less_flag =0;
		 ////////////////////////////////
		  mag_init();
		  CLR_mag_flag();
		  for(i=0;i<500;i++);
		 MagcardRCP1_GPIOInit();
		 //////////////////////////////////
	//	 CLR_mag_flag();
		 //led1_on();

		 ShowMagCard();

		 while(waittimeout)
		 {
			  if(mag_flag() ==1)
			 { 
			 	//led3_on(); 
			 	ret = GeneraldataTest_new(CMD_DATA,gb_Le);
//				CMD_DATA[0]=0x00;
				CLR_mag_flag();
				//led1_off();
				ShowDefault();
				if(ret ==0)
				  {
					 waitflag =0;
					 return SW9000;
				  }
				  else
				{
				    	//ShowDefault();
					 return SW6300;
				}
			 }
			if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == !RESET) 
			{
				cancle = (USART_ReceiveData(UART4));
				USART_ClearFlag(UART4, USART_FLAG_RXNE);
				if(cancle==0xFa)
				{
						waitflag =0;
						//led4_off();
					   //led3_off();
					   //led1_off();
					   ShowDefault();
					   MagcardRCP1_GPIOInitDesable();
						return SW6985;
				}
			} 
			IWDG_ReloadCounter();	
		 }
		if(waittimeout<=0)
		{	
			waitflag =0;
				ShowDefault();
			MagcardRCP1_GPIOInitDesable();
			return SW6F00;
		}
	 }
	 else if(CMD_P1==0x10)  //非接触卡通讯
	 {
	 	 waittimeout = CMD_P2*1000;
		 waitflag =1;
	//	 less_flag=0;
	//	 ISO14443_carrier_on();
		 if(times1==0)
		 {
		 	//led4_on();
			ShowContectlessCard();
			times1=1;
		}
		 while(waittimeout)
		 {
			 if(less_flag==0)
			 {
				 //ISO14443_carrier_on();
				 //ISO14443_reset(16);

				 ret =  search_card(buf_typeAB,&len);
				 if(ret==0)
				 {
				   less_flag =1;
				 }
				 
			 }
			 else 
			 {
				  
				  for(i=0;i<CMD_P3;i++)
				  UsbBulkMsgBuf[i]=gs_cmd.tmp2usb[10+i];
				  slot1 =1;
				  card_type =0x0a;
				//  led2_on();
			 	  CCID_AnalyseBulkMessage();
				  gb_Le = UsbBulkInMsgBuf[1]+(UsbBulkInMsgBuf[2]<<8)+10;
				//  led2_off();
				  for(i=0;i<gb_Le;i++)
				   {		
				    	CMD_DATA[i]=UsbBulkInMsgBuf[i];							  
				   }
				   break;
			 }

			 if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == !RESET) 
			{
				cancle = (USART_ReceiveData(UART4));
				USART_ClearFlag(UART4, USART_FLAG_RXNE);
				if(cancle==0xFa)
				{
						waitflag =0;
						//led4_off();
					   //led3_off();
					   //led1_off();
					   ShowDefault();
					   times1=0;
					   ISO14443_carrier_off();
						return SW6985;
				}
			} 
			IWDG_ReloadCounter();	
		  }	
		if(waittimeout<=0)
		{	
			waitflag =0;
				ShowDefault();
			ISO14443_carrier_off();
			return SW6F00;
		}
		else
		{
			waitflag =0;			
			return 	SW9000;
		} 
	 }
	 else if(CMD_P1==0x11)  //身份证安全模块透传
	 {
	 	 waittimeout = CMD_P2*1000;
		 waitflag =1;
		 times1=0;
    	 ISO14443_carrier_on();
		 //led4_on();
		 ShowIDCard();
		 less_flag =0;
		 while(waittimeout)
		 {
reader:
			  //ISO14443_reset(100);
			  ISO14443_carrier_on_off_on(30,50);
			  ret = ISO14443_WUPB(0,temp_rbuf);
			  if(ret ==0x95)
			  {
				//  led2_on();
			 	  ret = read_id_info(CMD_DATA,&gb_Le);
			 	  if(ret ==0)
				  {
					 //led4_off();
					 ShowDefault();
					 break;
				  }
				  else
				  {	   
				  	   times1++;
				  	   if(times1<5)
					   {
				  	   		goto  reader;
					   }
						else
						{
						   waitflag =0;
						   	ShowDefault();
						   return SW6300;
					    }
				  }
			  }
			 if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == !RESET) 
			{
				cancle = (USART_ReceiveData(UART4));
				USART_ClearFlag(UART4, USART_FLAG_RXNE);
				if(cancle==0xFa)
				{
						waitflag =0;
						//led3_off();
					   //led4_off();
					   //led1_off();
					   ShowDefault();
					   ISO14443_carrier_off();
						return SW6985;
				}
			} 
			IWDG_ReloadCounter();	
		}
		 if(waittimeout==0)
		{	
			waitflag =0;
				ShowDefault();
			ISO14443_carrier_off();
			return SW6F00;
		}
		else
		{
			waitflag =0;
			ISO14443_carrier_off();
		    ShowDefault();	
			return 	SW9000;
		}
	 }
	 else if(CMD_P1==0x20)	//接触卡大卡通讯
	 {						  
	 	 waittimeout = CMD_P2*1000;
		 waitflag =1;
		 SOLT_init();
		 less_flag =0;
		 if(times1==0)
		 {
		//	 led2_on();
			// led3_on();
			 ShowContectCard();
			 times1=1;
		 }
		 while(waittimeout)
		 {
			 if(1==IC_inset())
			 {
			 	  for(i=0;i<CMD_P3;i++)
				  	  UsbBulkMsgBuf[i]=gs_cmd.tmp2usb[10+i];
				  slot1 =0;				  
			 	  CCID_AnalyseBulkMessage();
				  gb_Le = UsbBulkInMsgBuf[1]+(UsbBulkInMsgBuf[2]<<8)+10;
				  for(i=0;i<gb_Le;i++)
				   {		
				    	CMD_DATA[i]=UsbBulkInMsgBuf[i];												  
				   }

				   break;
		 	 }
			 if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == !RESET) 
			{
				cancle = (USART_ReceiveData(UART4));
				USART_ClearFlag(UART4, USART_FLAG_RXNE);
				if(cancle==0xFa)
				{
						waitflag =0;
						//led4_off();
					   //led3_off();
					   //led1_off();
					   ShowDefault();
					   times1=0;
						return SW6985;
				}
			} 
			IWDG_ReloadCounter();	
		 }
		 if(waittimeout<=0)
		{	
			waitflag =0;
				ShowDefault();
			return SW6F00;
		}
		else
		{
			waitflag =0;
			//ShowDefault();	
			return 	SW9000;
		}
	 }
	 else if(CMD_P1 == 0xfe)
	 {
		 waittimeout = CMD_P2*1000;
		 waitflag =1;
		 //ISO14443_carrier_on();
		 less_flag =0;
		 ///////////////////////////////
	      mag_init();
		  CLR_mag_flag();
		  for(i=0;i<500;i++);
		 MagcardRCP1_GPIOInit();
		 ////////////////////////////////
		 if(times1==0)
		 {
			 //led1_on();
			 //led3_on(); 
			 ShowMagCardOrContectCard();
			 times1=1;
		 }
		 while(waittimeout)
		 {
			 SOLT_init();
			 if(1==IC_inset())
			 {
				  for(i=0;i<10;i++)
				    UsbBulkMsgBuf[i]=ASk_ATR[i];
				  slot1 =0;
			 	  CCID_AnalyseBulkMessage();
				  gb_Le = UsbBulkInMsgBuf[1]+(UsbBulkInMsgBuf[2]<<8);

				  for(i=0;i<gb_Le;i++)
				   {				
				    	CMD_DATA[1+i]=UsbBulkInMsgBuf[10+i];										  
				   }
				   CMD_DATA[0]=0x01;
				  gb_Le+=1;
				  break;
			 }
			 else if(mag_flag() ==1)
			 {
			//	led3_on(); 
				for(j=0;j<120;j++)
	 			CMD_DATA[j]=0;
			 //	led3_on(); 
			 	ret = GeneraldataTest_new(CMD_DATA+1,gb_Le);
				CMD_DATA[0]=0x00;
			//	CMD_DATA[gb_Le+1]=0x6a;
			//	CMD_DATA[gb_Le+2]=0x80;
				CLR_mag_flag();
				//led3_off();
				//led1_off();
				ShowDefault();
				times1=0;
				if(ret ==0)
				  {
					 gb_Le+=1;
					 waitflag =0;
					 return SW9000;
				  }
				  else
				{
			    	ShowDefault();
					 return SW6300;
				}
			 }
			if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == !RESET) 
			{
				cancle = (USART_ReceiveData(UART4));
				USART_ClearFlag(UART4, USART_FLAG_RXNE);
				if(cancle==0xFa)
				{
						waitflag =0;
						//led4_off();
					   //led3_off();
					   //led1_off();
					   ShowDefault();
					   times1=0;
					   MagcardRCP1_GPIOInitDesable();
						return SW6985;
				}
			} 
			IWDG_ReloadCounter();	
		}
		if(waittimeout<=0)
		{	
			waitflag =0;
			MagcardRCP1_GPIOInitDesable();
			ShowDefault();
			return SW6F00;
		}
		else
		{
			waitflag =0;
			MagcardRCP1_GPIOInitDesable();
			return 	SW9000;
		}
	 }
	 else if((CMD_P1 == 0xff)||(CMD_P1 == 0xfd))
	 {
		 waittimeout = CMD_P2*1000;
		 waitflag =1;
		 less_flag =0;
		 ///////////////////////////////

		 if(CMD_P1 == 0xff)
		 {
	        mag_init();
		    CLR_mag_flag();
		    for(i=0;i<500;i++);
		   MagcardRCP1_GPIOInit();
		 }
		 ////////////////////////////////
		  ISO14443_carrier_on();
		 //CLR_mag_flag();
		 if(times1==0)
		 {

		   if(CMD_P1 == 0xff)
		   {
			 ShowMagOrContectOrlessCard();
		   }
		   else
		   {
		   	  ShowContectOrlessCard();
		   }
			 //led3_on();
			 //led4_on();
			 times1=1;
		 }
		 while(waittimeout)
		 {
			 SOLT_init();
			 if(1==IC_inset())
			 {
				  for(i=0;i<10;i++)
				   UsbBulkMsgBuf[i]=ASk_ATR[i];
				   	slot1 =0;
			//	  led4_on();
			 	  CCID_AnalyseBulkMessage();
				  gb_Le = UsbBulkInMsgBuf[1]+(UsbBulkInMsgBuf[2]<<8);
			//	  led4_off();
				  for(i=0;i<gb_Le;i++)
				   {
				
				    	CMD_DATA[1+i]=UsbBulkInMsgBuf[10+i];										  
				   }
				   CMD_DATA[0]=0x01;
				 //  CMD_DATA[gb_Le+1]=0x6a;
				  // CMD_DATA[gb_Le+2]=0x82;
				  gb_Le+=1;
				  break;
			 }
			 else if((mag_flag() ==1)&&(CMD_P1 == 0xff))
			 {
			//	led3_on(); 
		    	for(j=0;j<120;j++)
	 			CMD_DATA[j]=0;
			// 	led3_on(); 
			 	ret = GeneraldataTest_new(CMD_DATA+1,gb_Le);
				CMD_DATA[0]=0x00;
			//	CMD_DATA[gb_Le+1]=0x6a;
			//	CMD_DATA[gb_Le+2]=0x80;
				CLR_mag_flag();
			//	led3_off();
				if(ret ==0)
				  {
					 gb_Le+=1;
					 waitflag =0;
					 	//ShowDefault();	
					 return SW9000;
				  }
				  else
				{
				     ShowDefault();
					 return SW6300;
				}
			 }
			 else
			 {
			 	 //ret= search_card();
				 ret =  search_card(buf_typeAB,&len);

				 if(ret==0)
				 {
				 	beep_con();
					less_flag =1;
					for(i=1;i<5;i++)
					{
						 CMD_DATA[i] =	buf_typeAB[i];
					}
					CMD_DATA[0]=0x02;
				//	CMD_DATA[5]=0x6a;
				//	CMD_DATA[6]=0x81;
				 	gb_Le=5;
					break;
				 }			 
			 } 
			if(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == !RESET) 
			{
				cancle = (USART_ReceiveData(UART4));
				USART_ClearFlag(UART4, USART_FLAG_RXNE);
				if(cancle==0xFa)
				{
						waitflag =0;
						//led4_off();
					   //led3_off();
					   //led1_off();
					   ShowDefault();
					   times1=0;
					   MagcardRCP1_GPIOInitDesable();
						return SW6985;
				}
			} 
			IWDG_ReloadCounter();	
		}
		if(waittimeout<=0)
		{	
			waitflag =0;
			MagcardRCP1_GPIOInitDesable();
			ShowDefault();
			return SW6F00;
		}
		else
		{
			waitflag =0;
			MagcardRCP1_GPIOInitDesable();
				//ShowDefault();	
			return 	SW9000;
		}
	 }
	 else if(CMD_P1==0x21)	//接触卡PSAM卡1通信
	 {
	 
	 	 for(i=0;i<CMD_P3;i++)
		   UsbBulkMsgBuf[i]=gs_cmd.tmp2usb[10+i];
		  slot1 =2;
		  led3_on();
		  //SAM1_init();
		  SOLT_init_sam();
	 	  CCID_AnalyseBulkMessage1();
		  gb_Le = UsbBulkInMsgBuf[1]+(UsbBulkInMsgBuf[2]<<8)+10;
		  led1_off();
		  for(i=0;i<gb_Le;i++)
		   {
		
		    	CMD_DATA[i]=UsbBulkInMsgBuf[i];				
			
					  
		   }
	 
	 }
	 else if(CMD_P1==0x22)	 //接触卡PSAM卡2通信
	 {
	 
	 	 /*for(i=0;i<CMD_P3;i++)
		   UsbBulkMsgBuf[i]=gs_cmd.tmp2usb[10+i];
		  slot1 =3;
		  led3_on();
		  SAM2_init();
	 	  CCID_AnalyseBulkMessage1();
		  gb_Le = UsbBulkInMsgBuf[1]+(UsbBulkInMsgBuf[2]<<8)+10;
		  led1_off();
		  for(i=0;i<gb_Le;i++)
		   {
		
		    	CMD_DATA[i]=UsbBulkInMsgBuf[i];				
			
					  
		   }*/
	 
	 }
	 else if((CMD_P1&0xF0)==0x30)	 //指纹仪数据透传
	 {

		  
		  waittimeout = CMD_P2*1000;

	      if((CMD_P1&0x0F)==0x00)
		  {
		  		AS606_FPC1020_UartInit();
		  		gb_Le = 0;
		  		return  SW9000;
		  
		  }
	 	  if((CMD_P1&0x0F)==0x01)
		  {


			   t= CMD_P3;
			   r = AS606_FPC1020_UartSendData(CMD_DATA,t);

			   r = AS606_FPC1020_UartRecieveData_OnePackage(CMD_DATA,&t);

			   if(r)
			   {
			      	return SW6F00;
			   }

			   gb_Le = (unsigned short)t;

		       return  SW9000;
		  
		  }
		  if((CMD_P1&0x0F)==0x02)
		  {
		  
		  	   t= CMD_P3;

			   r = AS606_FPC1020_UartSendData(CMD_DATA,t);

			   j = AS606_FPC1020_UartRecieveData_ContinuePackages(CMD_DATA+500,&t);

			   if(j==0)
			   {
			      	return SW6F00;
			   }

			   CMD_DATA[0]=(unsigned char)(j>>8);
			   CMD_DATA[1]=(unsigned char)(j);
			   FingerPrintDevice = 1;
			   gb_Le = 2;

		       return  SW9000;
		  	  
		  }
		  if((CMD_P1&0x0F)==0x03)
		  {
		     if(FingerPrintDevice==0)
		   	 {
			
				 return SW6F00;
			
			 }
		  	 if(FingerPrintDevice==1)
			 {	 
		       j= ((CMD_DATA[0]<<8)|(CMD_DATA[1]));

		       r =	AS606_FPC1020_GetRecievedPackage(CMD_DATA+500,j,&rl);
               
			   if(r)
			    return SW6F00;

				for(j=0;j<rl;j++)
				{
				
					CMD_DATA[j]=CMD_DATA[j+500+AS606_FPC1020_index];
				
				}

				gb_Le = rl;

				return  SW9000;
			  }
			 if(FingerPrintDevice==2)
			 {
			 
			 /*	t= ((CMD_DATA[0]<<8)|(CMD_DATA[1]));
			 
			    if(t==0)
				{
				
					for(j=0;j<0x80;j++)
				    {
					    CMD_DATA[j]=CMD_DATA[j+500];
				    }

					gb_Le = 0x80;
				}
				else if(t==1)
				{
					for(j=0;j<0x80;j++)
				    {
					    CMD_DATA[j]=CMD_DATA[j+500+128];
				    }

					gb_Le = 0x80;
				}
				else if(t==2)
				{
					for(j=0;j<102;j++)
				    {
					    CMD_DATA[j]=CMD_DATA[j+500+128+128];
				    }

					gb_Le = 102;
				
				}
				else if(t==3)
				{
				    gb_Le  =0;
					return SW6F00;
				}
			 
			 	*/

			 }
		  
		  }
		  if((CMD_P1&0x0F)==0x04)
		  {
		  
		  	   WEL_Init();
			   gb_Le =0;
			   WEL_Image_Flag=0;
			   ShowFinger();
			   return  SW9000;
		  
		  }
		  if((CMD_P1&0x0F)==0x05)
		  {

		   /*  //waittimeout = CMD_P2*1000;
		      waitflag =1;

		  	  t= (CMD_DATA[1]<<8)|CMD_DATA[2];
			  WEL_SendData(&CMD_DATA[3],t);
			  //WEL_RecieveData(CMD_DATA+500,358);

			  r =  WEL_RecieveData(CMD_DATA+500,3);

			  if(r==1)
			  {
			     waitflag =0;
			     return SW6F00;
			  
			  }

			  if(r==2)
			  {
			     waitflag =0;
			     return SW6985;
			  
			  }

			  if(CMD_DATA[502]==0x41)
			  {
			      WEL_RecieveData(CMD_DATA+503,15);
			      FingerPrintDevice=0;
				  waitflag =0;
				  return SW6F00;		  
			  }

			  WEL_RecieveData(CMD_DATA+503,355);

			  FingerPrintDevice = 2;
			  //gb_Le = 358;
			  CMD_DATA[0]=0;
			  CMD_DATA[1]=3;
			  gb_Le = 2;
			  waitflag =0;

			  return  SW9000;	*/


			cmdflag=0;

            if(CMD_DATA[0]==2)
			{
			  imageflag|=1;
			  cmdflag = 0;
			}
			else  if(CMD_DATA[0]==3)
			{
			   imageflag|=1;
				cmdflag = 1;
			}
			else
			{
				imageflag = 0;
			   	cmdflag = 0;
			}





            if(imageflag&1)
	        {
			   cmd_i = CMD_DATA[1];
			   cmd_s = CMD_DATA[2];
			   t = 11;
	        }
			else
			{
		       t= (CMD_DATA[1]<<8)|CMD_DATA[2];
			}


__LoopImage_Lable:

			 waitflag =1;
			 //led2_on();


			if(imageflag&1)
		    {
			  	 
			  if(cmd_i==cmd_s)   	 
			     goto Retrun_LABLE;
				 
				  
			    for(j=0;j<11;j++)	 
				   CMD_DATA[j+3]=ImageOrder[cmd_i][j];
				 
			   cmd_i++;	 
		    }



			 tl = WEL_ToTransforSequence(CMD_DATA+4,CMD_DATA+901,t-2);


			 CMD_DATA[900] = 0x02; //CMD_DATA[3];
			 CMD_DATA[tl+900+1] = 0x03;


			if(CMD_DATA[0]==1)
			{
				 r = WEL_CheckFingerExist();
				 if(r==1)
				 {
				 	waitflag =0;
					 //led2_off();
					 ShowDefault();
					 beep_con();
                    return SW6F00;
				 
				 }
				 else if(r==2)
				 {
				 
				 	 waitflag =0;
					 //led2_off();
					 ShowDefault();
					 beep_con();
                     return SW6985;
				 
				 }
			}

			 len = (tl+2);

			 WEL_SendData(CMD_DATA+900,len);


             WEL_RecieveData(CMD_DATA+900,5,0);
		     WEL_FromTransforSequence(CMD_DATA+901,TS36ebg_Transbuf+1,4);
													
			 TS36ebg_Transbuf[0] = CMD_DATA[900];

			 rl =  (TS36ebg_Transbuf[1]<<8)|TS36ebg_Transbuf[2];

			 len =   ((rl+1)*2+1);
			 r = WEL_RecieveData(CMD_DATA+905,len,0);


			  if(r==1)
			  {
			      waitflag =0;
					// led2_off();
					ShowDefault();
					 beep_con();					
                  return SW6F00;			  
			  }
			  if(r==2)
			  {
			     waitflag =0;
					 //led2_off();
					 ShowDefault();
					 beep_con();					
                  return SW6985;			  
			  }
			 IWDG_ReloadCounter();
	         WEL_FromTransforSequence(CMD_DATA+905,TS36ebg_Transbuf+3,(rl+1)*2);

			 temp = 0;

			 for(i=1;i<(rl+4-1);i++)
			 {	 
			 	  temp ^=  TS36ebg_Transbuf[i];
			 }

			 if(temp!=TS36ebg_Transbuf[rl+3])
			 {
					 //led2_off();
					 ShowDefault();
					 beep_con();				 
			 	return	 SW6A85;
			 
			 }
			
			  TS36ebg_Transbuf[rl+4]=0x03;
			  
			  
			if(WEL_Image_Flag==0)
 			{		  
				     for(i=0;i<((rl+1)*2+2);i++)
					 {
					 	   CMD_DATA[i]=	  CMD_DATA[i+905];
					 }
		
				 	  if(CMD_DATA[0]!=0x30)
					  {
							 //led2_off();
							 ShowDefault();
							 beep_con();					
					     return	 SW6300;
					  }
		
					// CMD_DATA[rl+4] = 0x03;
		
					 gb_Le = (rl+1)*2+2;


			 }
			 else
			 {
			 
			 	  	 for(i=0;i<((rl+4));i++)
					 {
					 	   CMD_DATA[i]=	  TS36ebg_Transbuf[i+3];
					 }
					  gb_Le = rl+2;
			 
			 }

             if(imageflag&1)
			 {
			  	
			  	  	 for(i=0;i<(gb_Le-4);i++)
					 {
					 	   CMD_DATA[3500 + i + image_index] = CMD_DATA[2+i];
					 }

			  		 image_index  += 	(gb_Le-4);



					Delay(60000);
					Delay(60000);
					Delay(60000);
					Delay(60000);
					Delay(60000);
					Delay(60000);
				//	Delay(60000);
					//Delay(60000);

			        goto  __LoopImage_Lable;
			 }


			  
Retrun_LABLE:
     

		    if(imageflag&1)
		    {
			     gb_Le =  image_index;
	
				 if(cmdflag==1)
				 {
				 	 imageflag &= 0xFE;
				 }

				 

			   	
				for(i=0;i<image_index;i++)
				{
					 	   CMD_DATA[i+2] = CMD_DATA[3500 + i];
				}

			   image_index = 0;
			   gb_Le+=4;
		    }


			 waitflag =0;
			 led2_off();
			 beep_con();	
			 
			 			
			 return  SW9000;  

		  
		  }
		  if((CMD_P1&0x0F)==0x06)
		  {
	
			   WEL_Init_Image();
				  gb_Le =0;
				  WEL_Image_Flag=1;
				  image_index=0;
			   return  SW9000;


		/*     waitflag =1;

		      t = (CMD_DATA[1]<<8)|CMD_DATA[2];
			  i = 0;
		  	  r = WEL_TransferData(&CMD_DATA[3],CMD_DATA+500,t,&i);
			  
			  if(r==1)
			  {
			      waitflag =0;
                  return SW6F00;			  
			  }
			  if(r==2)
			  {
			      waitflag =0;
                  return SW6985;			  
			  }

			  for(j=0;j<i;j++)
			  {
				  CMD_DATA[j]=*(CMD_DATA+500+j);
			  }


			  gb_Le = i;
			  waitflag =0;
			  return  SW9000;  
		   */	  
		  
		  }
		  if((CMD_P1&0x0F)==0x07)
		  {
			    //ShowDefault();
			//	g_configflag|=0x02;
				Reset_Device();

		  }
      	 if((CMD_P1&0x0F)==0x08)
		   {
		   	   WEL_Init();
			   ShowFinger();
			   gb_Le =0;
			   return  SW9000;
		   }
		   if((CMD_P1&0x0F)==0x09)
		   {

		        waitflag =1;
					 led2_on();
				 len = 	(CMD_DATA[3]<<24)|(CMD_DATA[4]<<16)|(CMD_DATA[5]<<8)|CMD_DATA[6];


			// for(i=0;i<10;i++)
				 WEL_SendData(CMD_DATA,len+8);

				 temp = WEL_RecieveData(TS36ebg_Transbuf,8,0);

			     if(temp==1) 
				 {
				 		     waitflag =0;
					 //led2_off();
					 ShowDefault();
					 beep_con();
			         return SW6F00;
				 }

				 len = (TS36ebg_Transbuf[4]<<24)|(TS36ebg_Transbuf[5]<<16)|(TS36ebg_Transbuf[6]<<8)|TS36ebg_Transbuf[7];

				 temp = WEL_RecieveData(TS36ebg_Transbuf+8,len+1,0);
				    waitflag =0;
			     if(temp==1) 
				 {
				   		     waitflag =0;
					 //led2_off();
					 ShowDefault();
					 beep_con();
				  
			         return SW6F00;
				 }
				 if(temp==2)
			     {
			     			     waitflag =0;
					 //led2_off();
					 ShowDefault();
					 beep_con(); 				
                    return SW6985;			  
			     }
				
				temp = 0;

				for(i=1;i<(len+8);i++)
			    {	 
			 	  temp ^=  TS36ebg_Transbuf[i];
			    }

			    if(temp!=TS36ebg_Transbuf[len+8])
			    {
				 			     waitflag =0;
					 //led2_off();
					 ShowDefault();
					 beep_con();		 
			 	   return	 SW6A85;
			 
			    }
			   
			   	for(i=0;i<(len+9);i++)
			    {	 
			 	  CMD_DATA[i]=  TS36ebg_Transbuf[i];
			    }
				  
			   	gb_Le = len+9;
					 		     waitflag =0;
					 //led2_off();
					 ShowDefault();
					 beep_con();
				return  SW9000;   	 
			
		   }
           if((CMD_P1&0x0F)==0x0A)
		   {


						 waitflag =1;
						 //led2_on();
			
						 t= (CMD_DATA[1]<<8)|CMD_DATA[2];
			
						 tl = WEL_ToTransforSequence(CMD_DATA+4,CMD_DATA+901,t-2);
			
			
						 CMD_DATA[900] = 0x02; //CMD_DATA[3];
						 CMD_DATA[tl+900+1] = 0x03;
			
						 len = (tl+2);
			
						 WEL_SendData(CMD_DATA+900,len);
			
			
			             WEL_RecieveData(CMD_DATA+900,5,0);
					     WEL_FromTransforSequence(CMD_DATA+901,TS36ebg_Transbuf+1,4);
																
						 TS36ebg_Transbuf[0] = CMD_DATA[900];
			
						 rl =  (TS36ebg_Transbuf[1]<<8)|TS36ebg_Transbuf[2];
			
						 len =   ((rl+1)*2+1);
						 r = WEL_RecieveData(CMD_DATA+905,len,0);
			
			
						  if(r==1)
						  {
						        waitflag =0;
								// led2_off();
								SM_2B_SendFree();
								ShowDefault();
								 beep_con();					
			                    return SW6F00;			  
						  }
						  if(r==2)
						  {
						         waitflag =0;
								 //led2_off();
								 SM_2B_SendFree();
								 ShowDefault();
								 beep_con();					
			                  return SW6985;			  
						  }
						 IWDG_ReloadCounter();
				         WEL_FromTransforSequence(CMD_DATA+905,TS36ebg_Transbuf+3,(rl+1)*2);
			
						 temp = 0;
			
						 for(i=1;i<(rl+4-1);i++)
						 {	 
						 	  temp ^=  TS36ebg_Transbuf[i];
						 }
			
						 if(temp!=TS36ebg_Transbuf[rl+3])
						 {
								 //led2_off();
								 ShowDefault();
								 beep_con();				 
						 	return	 SW6A85;
						 
						 }
											  
					     for(i=0;i<((rl+1)*2+2);i++)
						 {
						 	   CMD_DATA[i]=	  CMD_DATA[i+905];
						 }
			
					 	  if(CMD_DATA[0]!=0x30)
						  {
								 //led2_off();
								 ShowDefault();
								 beep_con();					
						         return	 SW6300;
						  }
			
						// CMD_DATA[rl+4] = 0x03;
			
						 gb_Le = (rl+1)*2+2;
						 waitflag =0;
								 led2_off();
								 beep_con();				
						 return  SW9000;  

			
		   }
         

 



	 
	 }
	 else if((CMD_P1&0xF0)==0x40)	 //TF卡数据透传
	 {
	 		
			
		  Select_SD_SPIFlash  =1;

		  if((CMD_P1&0x0F)==0x00)
		  {
			
				SPI_GPIO();
   		        SPI_Configuration_SD();

			    gb_Le = 0;
				Select_SD_SPIFlash =0;
			    return SW9000;
			
		   }  	 
		   if((CMD_P1&0x0F)==0x01)
		   {

   		  	   
			   CMD_DATA[0] = SD_Init();
	           gb_Le = 1;
		  	   Select_SD_SPIFlash =0;
		       return SW9000;
		   
		   }

		   if((CMD_P1&0x0F)==0x02)
		   {
				if(CMD_DATA[0]==0)
				{
			   	    t = SD_GetCapacity();
	   	            CMD_DATA[0]=(unsigned char)(t>>24);
		          	CMD_DATA[1]=(unsigned char)(t>>16);
			        CMD_DATA[2]=(unsigned char)(t>>8);
			        CMD_DATA[3]=(unsigned char)(t);
			        gb_Le = 4;
				}
				else
				{
				
					if(SD_GetCSD(CMD_DATA)!=0)
				    {
					       Select_SD_SPIFlash =0;
						   return SW6A86;
					}

				    gb_Le = 16;
				}

				Select_SD_SPIFlash =0;
                return SW9000;
		   
		   }

		   if((CMD_P1&0x0F)==0x03)
		   {
		   
		   	      	  t = (CMD_DATA[0]<<24)| (CMD_DATA[1]<<16)| (CMD_DATA[2]<<8)| (CMD_DATA[3]);

					//  for(i=0;i<512;i++)
					//  {								 
					//  	 TS36ebg_Transbuf[i]= CMD_DATA[4];
					//  }
				
				      
				
				   	  res = SD_WriteSingleBlock(t,&buffer[0]);
				
				
					  gb_Le = 0;
					  Select_SD_SPIFlash =0;
					  if(res==0)
					  {
					  	   	return SW9000;
					  }
					  else
					  {
					  		return SW6E00;
					  }
		   
		   
		   
		   }

		   if((CMD_P1&0x0F)==0x04)
		   {
		   
		   		  t = (CMD_DATA[0]<<24)| (CMD_DATA[1]<<16)| (CMD_DATA[2]<<8)| (CMD_DATA[3]);

				  for(i=0;i<512;i++)
				  {								 
				  	 buffer[i]= 0;
				  }
			
			   	  res = SD_ReadSingleBlock(t,&buffer[0]);
			
				  //for(i=0;i<128;i++)
				  //{								 
				  //	 CMD_DATA[i]= TS36ebg_Transbuf[i];
				  //}
				  Select_SD_SPIFlash =0;
				  if(res==0)
				  {
				        //gb_Le = 128;
						gb_Le = 0;
				  	   	return SW9000;
				  
				  }
				  else
				  {
				     	 gb_Le = 0;
				  		return SW6E00;
				  
				  }
		   
		   
		   
		   }
		    if((CMD_P1&0x0F)==0x05)
			{
			
				   t = ((CMD_DATA[0]<<8)| (CMD_DATA[1]));
				  // tl = ((CMD_DATA[2]<<8)| (CMD_DATA[3]));
				   rl = CMD_P3 - 2;

				   for(j=0;j<rl;j++)
				   {
				   		buffer[j+t] = CMD_DATA[2+j];
				   
				   }
				   
				   gb_Le =0;
				   Select_SD_SPIFlash =0;
				   return SW9000;	
			
			}
	   	  
		   if((CMD_P1&0x0F)==0x06)
			{
			
				   t = ((CMD_DATA[0]<<8)| (CMD_DATA[1]));

				   rl =((CMD_DATA[2]<<8)| (CMD_DATA[3]));

				   for(j=0;j<rl;j++)
				   {
				   		CMD_DATA[j] = buffer[j+t];
				   
				   }	

				   gb_Le =rl;
				   Select_SD_SPIFlash =0;
				   return SW9000;

			}

	 }
	 else if(CMD_P1==0x23)	 //密码键盘
	 {
	 	waittimeout = CMD_P2*1000;
		waitflag =1; 
		j=0;
		t=0;
		key_val =0xff;
		//led6_on();
		
		ShowKeyPad();

		beep_con();
//		Set_outkey();
//	for(j=0;j<50;j++);
        
		tl = CMD_DATA[0];
		r = CMD_DATA[1];

		while(waittimeout)
		{
			Set_outkey();
			temp= get_key();
			if((key_val==0xff)&&(temp!= 0xff))
				key_val= temp;
			t=0;
			if((temp!=0xff)&&(key_val== temp))
			{
			  	 //t=26000;
				    while(waittimeout)
                    {
                        IWDG_ReloadCounter();
                        Set_outkey();
                        temp	=  get_key();
                        t++;
                        if((t>10000)&&(t<60000))
                        {
                            if((key_val==0x10)||(key_val==14))
                            {
                                waitflag =0;
                                led6_off();
                                if(r==0)
                                {
                                    beep_con();
                                }
                                if(key_val==14)
                                {
                                    gb_Le =  0;
                                    return SW6985;									
                                }
                                if(tl==1)
                                {
                                    /*	 CMD_DATA[0]=0x31;
                                    CMD_DATA[1]=0x33;
                                    CMD_DATA[2]=0x32;
                                    CMD_DATA[3]=0x35;
                                    CMD_DATA[4]=0x36;
                                    CMD_DATA[5]=0x39;
                                    gb_Le = 6;	*/
                                    getPinBlock(CMD_DATA,CMD_DATA+512,gb_Le,pinmod);
                                    if(pinmod==1)
                                    {
                                        for(i=0;i<8;i++)
                                        {	
                                            CMD_DATA[i] = CMD_DATA[i+512]^CMD_DATA[i+600];
                                        }
                                        gb_Le = 8;
                                    }
                                    else
                                    {
                                        for(i=0;i<16;i++)
                                        {	
                                            CMD_DATA[i] = CMD_DATA[i+512]^CMD_DATA[i+600];
                                            //CMD_DATA[i] = temp_rbuf[i];
                                        }
                                        gb_Le = 16;
                                    }
                                    GetKey(rootkey,1,0);
                                    API_Encrypt(CMD_DATA,&rootkey[8],CMD_DATA+512,gb_Le,&len);
                                    // len  =8;
                                    if(len > (TAPDU_BUF_LEN-512)) //越界检测
                                    {
                                        len = 0;
                                        //return SW6985;
                                    }
                                    for(i=0;i<len;i++)
                                    {
                                        CMD_DATA[i]	  =   CMD_DATA[512+i];
                                    }
                                    gb_Le =  len;
                                }
                                ShowDefault();
                                return SW9000;
                            }
                            else
                            {
                                if(key_val==15)
                                {
                                    j=0;
                                    gb_Le =j;
                                    ShowKeyPad();
                                    t=60000;
                                }
                                else
                                {
                                    if(gb_Le<12)    //按照银联Pin规范只能是12位pin
                                    {  
                                        CMD_DATA[j]= (key_val|0x30);

                                        if(CMD_DATA[j]==0x3b)
                                        {
                                            CMD_DATA[j]=0x30;
                                        }

                                        ShowPinCode(j);

                                        j++;
                                        gb_Le =j;
                                    }

                                    t=60000;
                                }
                            }
                        }

                        if(temp==key_val)
                        {
                            continue;
                        }
                        else
                        {
                            key_val =0xff;
                            if(t>60000)
                            {
                                if(r==0)
                                {
                                    beep_con();
                                }
                            }
                            t=0;
                            temp = 0xff;
                            break;
                        }
                    }
			} 
			else
			{
				 t=0;
				 key_val =0xff;
				 temp =0xff;
			} 
	 /*		Set_outkey();
			temp= get_key();
			if(temp==0xff)
			  continue;
			for(i=0;i<500000;i++);
			Set_outkey();
			key_val= get_key();
		
		//	for(i=0;i<100000;i++);
			if(temp==key_val)
			{
				if(key_val!=0xff)
				{
					beep_con();
					if((key_val==0x10)||(key_val==15))
					{
					 	waitflag =0;
						led6_off();
					//	CMD_DATA[j]= temp_rbuf[0];
					//	gb_Le =1;
						return SW9000;
					}
					else
					{
						 CMD_DATA[j]= key_val;	 
						 j++;
						 gb_Le =j; 
						 
					}
				}
			} 
/*			if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == !RESET) 
			{
				cancle = (USART_ReceiveData(USART3));
				USART_ClearFlag(USART3, USART_FLAG_RXNE);
				if(cancle==0xFa)
				{
						waitflag =0;
						led2_off();
					   led3_off();
					   led1_off();
					   MagcardRCP1_GPIOInitDesable();
						return SW6985;
				}
			}  */
			IWDG_ReloadCounter();

	 	}
		if(waittimeout<=0)
		{	
			waitflag =0;
		//	MagcardRCP1_GPIOInitDesable();
		    ShowDefault();
			return SW6F00;
		}
	 }
	 else if(CMD_P1==0x25)
	 {
	     
		 if(CMD_P3==8)
		 {
		     for(i=0;i<8;i++)
			 {
		 		CMD_DATA[600+i]= CMD_DATA[i];
			 }

			pinmod = 1;
		 }
		 else  if(CMD_P3==16)
		 {
		 
		  	for(i=0;i<16;i++)
			 {
		 		CMD_DATA[600+i]= CMD_DATA[i];
			 }
		 	pinmod = 0;
		 }
		/* else if(CMD_P3==0)
		 {
		 	 for(i=0;i<16;i++)
			 {
		 		CMD_DATA[i] = temp_rbuf[i];
			 }
		 	 CMD_DATA[16]= pinmod;
			 	 gb_Le = 17;
				 return SW9000;
		 }	 */
		 else
		 {
		    return SW6700;
		 
		 }

		 gb_Le = 0;
		 return SW9000;
	 
	 }
	 else if(CMD_P1==0x51)	 //预留串口1数据透传
	 {
	    waittimeout = CMD_P2*0xFFFFFF;

		 InitCOM(9600);
		 CMD_DATA[0] = 0;
		 TxDataCOM(CMD_DATA,1);

		// COM1_init();
		 gb_Le = 0;
		 return SW9000;
	 }
 	 else if(CMD_P1==0x52)	 //预留串口2数据透传
	 {
		   //waittimeout = CMD_P2*0xFFFFFF;
	   	   len = (CMD_DATA[0]<<8)|(CMD_DATA[1]);
		   TxDataCOM(&CMD_DATA[2], CMD_P3-2);
           RxDataCOM(CMD_DATA,len,waittimeout);
         
		   gb_Le  =	 len;
		   return SW9000;

	 
	 }
	 else if((CMD_P1&0xF0)==0x60)
	 {
	 	  if((CMD_P1&0x0F)==0)
		 {
	      	//DES_Triple_Encrypt(CMD_DATA+17,CMD_DATA+1,CMD_DATA+1000,CMD_DATA[0],&len);
			API_Encrypt(CMD_DATA+17,CMD_DATA+1,CMD_DATA+1000,CMD_DATA[0],&len);
			for(i=0;i<len;i++)
			{
				 CMD_DATA[i]=CMD_DATA[1000+i];
			
			}
			 gb_Le = len;
			return SW9000;
		}
		else  if((CMD_P1&0x0F)==1)
		{
			//DES_Triple_Decrypt(CMD_DATA+17,CMD_DATA+1,CMD_DATA+1000,CMD_DATA[0],&len);
			API_Decrypt(CMD_DATA+17,CMD_DATA+1,CMD_DATA+1000,CMD_DATA[0],&len);

			for(i=0;i<len;i++)
			{
				 CMD_DATA[i]=CMD_DATA[1000+i];
			
			}
			 gb_Le = len;
			return SW9000;
		
		}
		else  if((CMD_P1&0x0F)==2)	  
		{


             //extern unsigned char GetKey(unsigned char * key,unsigned char flag,unsigned char index);
              //extern unsigned char SaveKey(unsigned char * key,unsigned char index);
              //unsigned char rootkey[24];
              //unsigned char usedkey[24];
			  
			  
			   



			   //if((CMD_DATA[0]==0x39)&&(CMD_DATA[1]==0x00))

				  if(((CMD_DATA[0]==0x39)&&(IsRootKeyHas(0x01)==0))||((CMD_DATA[0]==0x36)&&(IsRootKeyHas(0x02)==0))||((CMD_DATA[0]==0x30)&&(IsRootKeyHas(0x04)==0))||((CMD_DATA[0]==0x3A)&&(IsRootKeyHas(0x08)==0)))
				  {

					    if((CMD_DATA[6]==0)&&(CMD_DATA[7]==0x10))
						{
				   	       SaveKey(CMD_DATA,0,0);
						   gb_Le=0;
						   return SW9000;
						}
						else
						{
						   gb_Le=0;
						   return SW6A81;
						}
				   }
				   else
				   {
				   	   gb_Le=0;
				   	   return SW6A82;
				   }



			  /* else
			   {
			   
			       i=24;
				   j=16;
				   t=24;

				   GetKey(rootkey,0,0);

			   	   DES_Triple_Decrypt(CMD_DATA,&rootkey[8],usedkey,i,&t);

				  // DES_Triple_Decrypt(CMD_DATA+i,CMD_DATA+640,CMD_DATA+512*i,j,&t);				  

				  //if(CheckKey(CMD_DATA+512*i+8,CMD_DATA+640+8,16))

				  if((usedkey[7]==0x10)&&(usedkey[0]==0x36))
				  {
				 
				 	   SaveKey(usedkey,0,0);	
				 
				 	   return SW9000;
				  }
				  else
				  {
				  
				       return SW6E00;
				  
				  }
					
  
			   
			   } */
		
		
		}
		else  if((CMD_P1&0x0F)==3)	  
		{
		
		   t  = CMD_DATA[0];
		    
		   GetKey(CMD_DATA,t,0);

		   gb_Le = 24;

		   return SW9000;
		
		}
		else if((CMD_P1&0x0F)==4)
		{
		
		    InitRootKey();
			return SW9000;
		}
		else if((CMD_P1&0x0F)==5)
		{
		
			 	   i=24;
				   j=16;
				   t=24;

				   GetKey(rootkey,0,0);

			   	   //DES_Triple_Decrypt(CMD_DATA,&rootkey[8],usedkey,i,&t);
				   API_Decrypt(CMD_DATA,&rootkey[8],usedkey,i,&t);


				  // DES_Triple_Decrypt(CMD_DATA+i,CMD_DATA+640,CMD_DATA+512*i,j,&t);				  

				  //if(CheckKey(CMD_DATA+512*i+8,CMD_DATA+640+8,16))

				  //if((usedkey[7]==0x10)&&(usedkey[0]==0x36))
				 // {
				 
				 	   SaveKey(usedkey,0,CMD_P2);	
				 
				 	   return SW9000;
				  //}
				  //else
				  //{
				  
				  //     return SW6E00;
				  
				  //}
		
		
		}
		else if((CMD_P1&0x0F)==6)
		{
		
			 	   //i=24;
				   //j=16;
				   //t=24;

				   //GetKey(rootkey,0,0);

			   	  // DES_Triple_Decrypt(CMD_DATA,&rootkey[8],usedkey,i,&t);

				  // DES_Triple_Decrypt(CMD_DATA+i,CMD_DATA+640,CMD_DATA+512*i,j,&t);				  

				  //if(CheckKey(CMD_DATA+512*i+8,CMD_DATA+640+8,16))

				 // if((usedkey[7]==0x10)&&(usedkey[0]==0x36))
				 // {
				 
				 	   SaveKey(CMD_DATA,0,1);	
				 
				 	   return SW9000;
				 // }
				 // else
				  //{
				  
				   //    return SW6E00;
				  
				 // }
		
		
		}
	 	else if((CMD_P1&0x0F)==7)
		{
			  	  // i=24;
				   //j=16;
				   //t=24;

                   for(j=0;j<16;j++)
				   {
				   
				   	  CMD_DATA[j]=0;
				   
				   }

				   GetKey(rootkey,0,0);

			   	   //DES_Triple_Decrypt(CMD_DATA,&rootkey[8],usedkey,i,&t);
				   API_Encrypt(CMD_DATA,&rootkey[8],usedkey,16,&t);
	
				   CMD_DATA[0]=usedkey[0];
				   CMD_DATA[1]=usedkey[1];
				   CMD_DATA[2]=usedkey[2];
				   CMD_DATA[3]=usedkey[3];
					gb_Le = 4;

				   return SW9000;
		
		
		
		}
	 
	 }
	 else if(CMD_P1==0x70)
	 {
	 
	        CMD_DATA[0]=0;
            CMD_DATA[1]=0;
		    CMD_DATA[2]=0;
		    CMD_DATA[3]=0;
		    CMD_DATA[4]=0;
		    CMD_DATA[5]=0xFF;
		    CMD_DATA[6]=0x0;
		    CMD_DATA[7]=0xFF;
		 


		  if(THM_TestReady())
		  {
		  	 CMD_DATA[2]=1;
		  }
	
		  if(Test_ICCReady())
		  {
		     CMD_DATA[1]=1;
		  }

		    gb_Le = 8; 
		 
	      return SW9000;
	 
	 }
	 else if(CMD_P1==0x80)
	 {
	   if(CMD_P2==0)
	   {
	      SPI_Delay_Time=CMD_DATA[0];
		  SPI_Flash_Init_ISU256();
		   	IS8_Rst(); 
		   return   SW9000;
	   
	   }
	   else	 if(CMD_P2==1)
	   {
			i=CMD_DATA[0];
			temp_rbuf[0]=CMD_DATA[1];
			//temp_rbuf[1]=CMD_DATA[2];
			temp_rbuf[1]=CMD_DATA[2];
			temp_rbuf[2]=CMD_DATA[3];
			temp_rbuf[3]=CMD_DATA[4];
		IWDG_Configuration_FingerPrint();
	      	while(i--)
			{
	   		
				
	   			CMD_DATA[0]=0xFB;
				CMD_DATA[1]=0x33;
				CMD_DATA[2]=0x44;
				CMD_DATA[3]=0x55;
				CMD_DATA[4]=0x66;
				CMD_DATA[5]=0xBF;

				CMD_DATA[6]=0xFB;
				CMD_DATA[7]=0x00;
				CMD_DATA[8]=0x44;
				CMD_DATA[9]=0x00;
				CMD_DATA[10]=0x66;
				CMD_DATA[11]=0xD9;


				buffer[0]=0;
		        buffer[1]=0;
		        buffer[2]=0;
		        buffer[3]=0;
		        buffer[4]=0;
		        buffer[5]=0;
				buffer[6]=0;
		        buffer[7]=0;
		        buffer[8]=0;
		        buffer[9]=0;
		        buffer[10]=0;
		        buffer[11]=0;
		        buffer[12]=0;

				g_SPI_srdelay=temp_rbuf[0];
				IWDG_ReloadCounter();
					IS8_Rst(); 
	   				IS8_TransferData_Recieving(CMD_DATA,6,buffer,4,0xff,1000);
			//IS8_TransferData(CMD_DATA,6,buffer,4,0);
						IWDG_ReloadCounter();
				SPI_Delay();

				g_SPI_srdelay=temp_rbuf[0];



			   if((buffer[0]==temp_rbuf[1])&&(buffer[1]==temp_rbuf[2]) &&(buffer[2]==temp_rbuf[3]))
			   {
			          temp_rbuf[1]=0;
					  temp_rbuf[2]=0;
					  temp_rbuf[3]=0;
				     for(r=0;r<3;r++)
				     {
							IWDG_ReloadCounter();
		   			   IS8_TransferData(CMD_DATA+6,6,buffer+4,3,0);
					  
					  	if((buffer[4]==0x90)&&(buffer[5]==0x00))
						{
						   g_SPI_srdelay=0;
						  IWDG_Configuration();
							return SW9000;
							
						}
	
					 }

					 g_SPI_srdelay=0;
					  IWDG_Configuration();
					 return SW6A85;
				}

			 }
			   g_SPI_srdelay=0;
			   IWDG_Configuration();
			    return SW6A86;
	   
	   }
	   else	 if((CMD_P2==0x2)||(CMD_P2==0x12))
	   {
	   	  len = CMD_DATA[1];
		  g_SPI_srdelay=CMD_DATA[2];
		  //r=0;
			 //if(CMD_DATA[0]!=1)
			 //{
			  CMD_DATA[4+CMD_DATA[0]] = Is8uXor(CMD_DATA+4,CMD_DATA[0]);
			 //}
			 //else
			 //{
			 //	  CMD_DATA[0]=0;
			 //}

		   if(CMD_P2==0x12)
		   {
		   		//t=	CMD_DATA[3];
					//if(CMD_DATA[5]==0x40)
					//{
					
					//	 CMD_DATA[4+CMD_DATA[0]]=0;
					//}
		   
		   }
		   else
		   {
				if((CMD_DATA[5]==0x44)) //||(CMD_DATA[5]==0x40))
				{
					t= CMD_DATA[3]*0x1000;
										  
				}
				else
				{

				//	if(CMD_DATA[5]==0x40)
				//	{

				//		 r=1;
				//	}

					t= CMD_DATA[3]*0x10;	
					
				  
				}
		   }

				buffer[0]=0;
		        buffer[1]=0;
		        buffer[2]=0;
		        buffer[3]=0;
		        buffer[4]=0;
		        buffer[5]=0;
				buffer[6]=0;
		        buffer[7]=0;
		        buffer[8]=0;
		        buffer[9]=0;
		        buffer[10]=0;
		        buffer[11]=0;
		        buffer[12]=0;

		  	IWDG_ReloadCounter();

		 if(CMD_P2==0x12)
		 {
		 	 IS8_TransferData_Recieving(CMD_DATA+4,CMD_DATA[0]+1,buffer,len,CMD_DATA[3],200000);
		 }
		 else
		 {
		     IS8_TransferData(CMD_DATA+4,CMD_DATA[0]+1,buffer,len,t);

			 //if(r==1)
			 //{
			 //  						SPI_Delay();
			//			SPI_Delay();
			 //  r=0;
			 //}
		 }
		    
          for(i=0;i<len;i++)
		  {
		  	CMD_DATA[i]	=	  buffer[i];
		  
		  }

          gb_Le  = len;
		  g_SPI_srdelay= 0;
          return   SW9000;
	   
	   }
	   else	 if(CMD_P2==3)
	   {
	   
	   	  g_SPI_srdelay =  CMD_DATA[0];

          if(IsIs8uStatuesModeReady())
		  {
		  	 return SW6A86;
		  
		  }
	
	      CMD_DATA[0]=0xc1;
		  len= CMD_DATA[1]+2;
		  	IWDG_ReloadCounter();
		  IS8_TransferData(CMD_DATA,len,buffer,0,0);

	   	  if(IsIs8uStatuesModeReady())
		  {
		  	 return SW6A86;
		  
		  }

		  buffer[0]=0xA1;

		  IS8_TransferData_RecievNBytes(buffer,1,CMD_DATA,&len);


	   	  gb_Le = len;

		  return SW9000;
	   
	   }
	   else	 if(CMD_P2==0x4)
	   {
		
		  len = CMD_DATA[1];
		  	IWDG_ReloadCounter();
		  IS8_TransferData(CMD_DATA+3,CMD_DATA[0],buffer,len,CMD_DATA[2]);
		    
          for(i=0;i<len;i++)
		  {
		  	CMD_DATA[i]	=	  buffer[i];
		  
		  }


          gb_Le  = len;

          return   SW9000;
		
	   }
	   	 else	 if(CMD_P2==0x5)
	 {
	 
	 	 SPI_Delay_Time=CMD_DATA[0];
		 return   SW9000;
	 
	 }
	   else
	   {
	   	   return SW6E00;
	   
	   }
	 
	 }
	 else
	 {
	   return SW6E00;
	 
	 }





	 return   SW9000;
}






void Reset_Device(void)
{
  USB_Cable_Config(DISABLE);
  NVIC_SystemReset();
}

void AppBootWord(void)
{

    FLASH_Unlock();	//FLASH解锁
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清标志位
    FLASH_ErasePage(0x303F800); 


}

extern unsigned short int ADCConvertedValue;
unsigned char firmware_v[8]={VER1,VER2,'.',VER3,'.',VER4,'.',VER5};
BYTE cmd_F001(void)
{
	 unsigned int adc_value=0,sum_value=0;
	 unsigned char i,j;
	 if(CMD_P1==0x00)	//指示灯1控制
	 {
/*	 	   if(CMD_P2==0x00)
		   {
		   	   led1_on();
		   }
		   else
		   {
		   	   led1_off();		   
		   }  */

	 	       //led4_off();
			   //led3_off();
			   //led1_off();
               ShowDefault();
			   times1=0;
			   ISO14443_carrier_off();
			   less_flag =0;
	 }
	 else if(CMD_P1==0x01)  //指示灯2控制
	 {
	 	//	if(CMD_P2==0x00)
	//	   {
		   //	   led2_on();
		  // }
		  // else
		 //  {
		   	   //led6_off();
		   	   //led2_off();
			   //led3_off();
			   //led1_off();		   
		 //  }
		   ShowDefault();
		 //LCD_BackLight_Off();

	 	       return   SW9000;
	 
	 }
	 else if(CMD_P1==0x02)  //指示灯3控制
	 {
	 	  if(CMD_P2==0x00)
		   {
		   	   //led3_on();
			   LCD_BackLight_On();
		   }
		   else
		   {
		      LCD_BackLight_Off();
		   	   //led3_off();		   
		   }
	 
	 
	 }
	 else if(CMD_P1==0x03)  //指示灯4控制
	 {
	 	if(CMD_P2==0x00)
		   {
		   	  led4_on();
		   }
		   else
		   {		   
		   	   led4_off();
		   }
	 
	 
	 }
	 else if(CMD_P1==0x04)  //指示灯5控制
	 {
	 	if(CMD_P2==0x00)
		   {
		   	  led5_on();
		   }
		   else
		   {
		   	   led5_off();		   
		   }
	 
	 
	 }
	 else if(CMD_P1==0x05)  //指示灯6控制
	 {
	 	if(CMD_P2==0x00)
		   {
		   
		   }
		   else
		   {
		   
		   
		   }
	 
	 
	 }
	 else if(CMD_P1==0x06)  //指示灯7控制
	 {
	 	if(CMD_P2==0x00)
		   {
		   
		   }
		   else
		   {
		   
		   
		   }
	 
	 
	 }
	 else if(CMD_P1==0x07)  //指示灯8控制
	 {
	 	if(CMD_P2==0x00)
		   {
		   
		   }
		   else
		   {
		   
		   
		   }
	 
	 
	 }
	 else if(CMD_P1==0x10)  //蜂鸣器控制
	 {
	 	  if(CMD_P2==0x00)
		   {
		   	  beep_on();
		   }
		   else
		   {
		   	  beep_off();		   
		   }
	 
	 
	 }
	 else if(CMD_P1==0x20)  //取设备信息
	 {
	 	  if(CMD_P2==0x00)
		   {
		   	   for(i=0;i<8;i++)
				CMD_DATA[i] =firmware_v[i];
				gb_Le =8;
			  	return SW9000;
		   }
		   else
		   {
		   
		   
		   }
	 
	 
	 }
	  else if(CMD_P1==0x21)  //取dianliang信息
	 {

	      sum_value =  0;

		  for(j=0;j<10;j++)
		  {
		 	 start_adc();
			// adc_value = ADCConvertedValue;
		 	 for(i=0;i<190;i++)
			 {
			   ;
			 }
			 for(i=0;i<190;i++)
			 {
			   ;
			 }
			 for(i=0;i<190;i++)
			 {
			   ;
			 }
			 for(i=0;i<190;i++)
			 {
			   ;
			 }
			 for(i=0;i<190;i++)
			 {
			   ;
			 }
			 adc_value = GetAD();
			 end_adc();

		 //	 for(i=0;i<190;i++)
		//	 {
		//	   ;
		//	 }
				
			 sum_value  += adc_value;
	
		  }

	     adc_value  = 	sum_value/10;

		 adc_value = (adc_value*825)/512;

		 //if(CMD_P2==0x01)
		 //{
		 
		 	  CMD_DATA[0] = (unsigned char) ( adc_value>>24);
              CMD_DATA[1] = (unsigned char) ( adc_value>>16);
			  CMD_DATA[2] = (unsigned char) ( adc_value>>8);
			  CMD_DATA[3] = (unsigned char) ( adc_value);

			  
			  gb_Le =4;
			  return SW9000;
			  		 

		 //}
	/*	 if(adc_value>=0xa2e)
		 {
			  CMD_DATA[0] = 0x64 ;
			  gb_Le =1;
			  return SW9000;
		 }else if(adc_value<=0x8ba)
		 {
		 
			  CMD_DATA[0] = 0x32 ;
			  gb_Le =1;
			  return SW9000;
		 }
		 else
		 {
			 
			  CMD_DATA[0] = 0x50; 
			  gb_Le =1;
			  return SW9000;
		 }*/
	 
	 }
	 else if(CMD_P1==0x80)  //写设备信息
	 {
	 	   if(CMD_P2==0x00)
		   {
               API_WriteData(0x1120,CMD_DATA, 16);	
			   return SW9000;	   
		   }
		   else
		   {
		   
		   
		   }
	 
	 
	 }
	 else if(CMD_P1==0x81)  //恢复bootloader
	 {
	 	  if(CMD_P2==0x00)
		   {
		   	   if((CMD_DATA[0]==0xf1)&&(CMD_DATA[1]==0xe2)&&(CMD_DATA[2]==0xd3)&&(CMD_DATA[3]==0xc4)&&(CMD_DATA[4]==0xb5))
		   	   		SetBootMode(1);
//					AppBootWord();
					Reset_Device();
		   }
		   else
		   {
		   	   SetBootMode(0);
		   	   return SW6300;
		   }

		   gb_Le=0;
		   return SW9000;
	 
	 
	 }
	else if(CMD_P1==0x82) 
	{

	    
		
		
		LCD_display_UTF8Line(CMD_DATA,21,0);


	 }
	 else
	 {
	 
	 	   return SW6E00;
	 
	 
	 }



	 return   SW9000;
}



