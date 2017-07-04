#include "magcard_drv.h"
#include "magcardTTL.h"

#include <stdio.h>
#include <string.h>
/////////////////////// 上海众恒 2015.6.29   胡玉新  ////////////////////////////////////////////////////////////////////////////////

unsigned char CardData[280];
unsigned char Dir,error,fTrackHaveData=0;
unsigned int Get_Data;
unsigned char ReadTrack2Len=0,ReadTrack3Len =0;
unsigned char ReadTrack2buf[40];
unsigned char ReadTrack3buf[110];
#define   delay_time  300 
extern void beep_on(void);
extern void beep_off(void);
extern void  beep_con(void);
extern unsigned short gb_Le; 
unsigned char MAG_XfrBlock(unsigned char *T2T3rbuf,unsigned int len);
unsigned char GeneraldataTest_new(unsigned char *T2T3rbuf,unsigned int len);

void delay_ms_g(int i){
  for(;i>0;i--);
}

void MagcardCLS3_GPIOInit_Out(void)
{										   
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(CLS3_MAGCARD_CLK, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = CLS3_MAGCARD_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CLS3_MAGCARD_PORT, &GPIO_InitStructure);
	
}
void MagcardCLS3_GPIOInit_In(void)
{										   
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(CLS3_MAGCARD_CLK, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = CLS3_MAGCARD_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CLS3_MAGCARD_PORT, &GPIO_InitStructure);
	
}

void Data_Out_H(void)
{
   GPIO_WriteBit(CLS3_MAGCARD_PORT, CLS3_MAGCARD_PIN, Bit_SET);
}

void Data_Out_L(void)
{
	GPIO_WriteBit(CLS3_MAGCARD_PORT, CLS3_MAGCARD_PIN, Bit_RESET);
}

void MagcardCLD2_GPIOInit_Out(void)
{										   
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(CLS_MAGCARD_CLK, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = CLS_MAGCARD_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CLS_MAGCARD_PORT, &GPIO_InitStructure);
	
}

void CLK_Out_H(void)
{
   GPIO_WriteBit(CLS_MAGCARD_PORT, CLS_MAGCARD_PIN, Bit_SET);
}

void CLK_Out_L(void)
{
	GPIO_WriteBit(CLS_MAGCARD_PORT, CLS_MAGCARD_PIN, Bit_RESET);
}

void track2check(){
          unsigned char  tmp,tmp1,k;
		  unsigned int i,x;
		  tmp=0;
//		  if(!Dir)
		  if((CardData[140]==0x14)&&(CardData[141]!=0x00))
		  	 Dir =1;
		  else
		     Dir =0;
		  

          //LRC Check
		  
		  if(Dir){
		  for(i=140;i<280;i++)
		  if((CardData[i]==0x1F)&&(CardData[i+1]==0x1F))break;
		  x =i;
		  tmp =0;
          for(i=140;i<x-1;i++)
		  tmp =tmp^(~CardData[i]);
		  if((tmp&0x0F)!=((~CardData[x-1])&0x0F))error =1;
		  }


          if(!Dir){
		  for(i=140;i<280;i++)
		  if((CardData[i]==0x1F)&&(CardData[i+1]==0x1F))break;

		  x =i;

		  for(k=0;k<5;k++)
		  if((CardData[i-1]&(0x10>>k))==0x00)break;

          

		  for(;i>140;i--){
		  tmp =(CardData[i]<<k)&0x1F;
		  tmp1 =(CardData[i-1]&0x1F)>>(5-k);
		  tmp =tmp1|tmp;
		  CardData[i] =tmp;		  
		  }

		  if(k!=0){
		  tmp =(CardData[140]<<k)&0x1F;
		  tmp1=0;
		  while(k--){
		  tmp1=tmp1<<1;
		  tmp1=tmp1|0x01;
		  }
		  tmp =tmp1|tmp;
		  CardData[0] =tmp;
		  }
 //LRC Check
         if(x!=0){
		  tmp =0;
          for(i=141;i<x;i++)
		  tmp =tmp^(~CardData[i]);
		  if((tmp&0x1e)!=((~CardData[0])&0x1e))error =1;

          }
		  }
}

void track3check(){
          unsigned char tmp,tmp1,k;
		  unsigned int i,x;
		  tmp=0;
//		  if(!Dir)
		  if((CardData[0]==0x14)&&(CardData[1]!=0x00))
		  	 Dir =1;
		  else
		     Dir =0;
		  

		  //LRC Check
		  
		  if(Dir){
		  for(i=0;i<140;i++)
		  if((CardData[i]==0x1F)&&(CardData[i+1]==0x1F))break;
		  x =i;
		  tmp =0;
          for(i=0;i<x-1;i++)
		  tmp =tmp^(~CardData[i]);
		  if((tmp&0x0F)!=((~CardData[x-1])&0x0F))error =1;
		  }

          if(!Dir){
		  for(i=0;i<140;i++)
		  if((CardData[i]==0x1F)&&(CardData[i+1]==0x1F))break;
		  x =i;
		  
		  for(k=0;k<5;k++)
		  if((CardData[i-1]&(0x10>>k))==0x00)break;

		  for(;i>0;i--){
		  tmp =(CardData[i]<<k)&0x1F;
		  tmp1 =(CardData[i-1]&0x1F)>>(5-k);
		  tmp =tmp1|tmp;								   
		  CardData[i] =tmp;		  
		  }

		  if(k!=0){
		  tmp =(CardData[0]<<k)&0x1F;
		  tmp1=0;
		  while(k--){
		  tmp1=tmp1<<1;
		  tmp1=tmp1|0x01;
		  }
		  tmp =tmp1|tmp;
		  CardData[0] =tmp;

		  }
 //LRC Check
          if(x!=0){
		  tmp =0;
          for(i=1;i<x;i++)
		  tmp =tmp^(~CardData[i]);
		  if((tmp&0x1e)!=((~CardData[0])&0x1e))error =1;

          }
		  }
}

void mag_fail(void)
{
 	unsigned long j=0;
	beep_on();
	for(j=0;j<55000;j++);
	for(j=0;j<45000;j++);
	beep_off();
	for(j=0;j<155000;j++);
	for(j=0;j<145000;j++);
	beep_on();
	for(j=0;j<55000;j++);
	for(j=0;j<45000;j++);
	beep_off();
}

void reverse(void){
        int i;
		unsigned char tmp;
/*		for(i=0;i<100;i++){
        tmp=(CardData[i]>>6)&0x01;
        tmp|=(CardData[i]>>4)&0x02;
		tmp|=(CardData[i]>>2)&0x04;
		tmp|=(CardData[i])&0x08;
		tmp|=(CardData[i]<<2)&0x10;
		tmp|=(CardData[i]<<4)&0x20;
		tmp|=(CardData[i]<<6)&0x40;
		CardData[i]=tmp|0x80;
		}  */
		for(i=0;i<140;i++){
        tmp=(CardData[i]>>4)&0x01;
        tmp|=(CardData[i]>>2)&0x02;
		tmp|=(CardData[i])&0x04;
		tmp|=(CardData[i]<<2)&0x08;
		tmp|=(CardData[i]<<4)&0x10;
		CardData[i]=tmp;
		
		}        
		for(;i<280;i++){
        tmp=(CardData[i]>>4)&0x01;
        tmp|=(CardData[i]>>2)&0x02;
		tmp|=(CardData[i])&0x04;
		tmp|=(CardData[i]<<2)&0x08;
		tmp|=(CardData[i]<<4)&0x10;
		CardData[i]=tmp;
		}
             

}

void mag_init (void)
{
	 unsigned int i;
//	unsigned long timeout=0,j=0;
//    unsigned char tmp,tmp1,k=0;
	Get_Data=0;
	error=0;
	ReadTrack2Len =0;
	ReadTrack3Len =0;
	for(i=0;i<280;i++)CardData[i]=0;
    MagcardCLS3_GPIOInit_Out();
	MagcardCLD2_GPIOInit_Out();
    CLK_Out_H();
    Data_Out_H();
    delay_ms_g(delay_time);
		   
  //reset
    Data_Out_L();     
    delay_ms_g(delay_time);;
    CLK_Out_L();  
    delay_ms_g(delay_time);
    CLK_Out_H();  
    delay_ms_g(delay_time);
    CLK_Out_L();  
    delay_ms_g(delay_time);

    //config data to input mode
    MagcardCLS3_GPIOInit_In();
	Data_Out_H();  //add by csq
    delay_ms_g(delay_time);
    CLK_Out_H(); 
    delay_ms_g(delay_time);
    CLK_Out_L(); 
    delay_ms_g(delay_time);
    CLK_Out_H();
    delay_ms_g(delay_time);
    CLK_Out_L();
    delay_ms_g(delay_time);     
   //reset over  
}

void MagcardRCP1_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable MAGCARD GPIO clock */
    RCC_APB2PeriphClockCmd(RCP1_MAGCARD_CLK | RCC_APB2Periph_AFIO, ENABLE);
  
    /* Configure MAGCARD pin as input floating */
    GPIO_InitStructure.GPIO_Pin = RCP1_MAGCARD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(RCP1_MAGCARD_PORT, &GPIO_InitStructure);


    /* Connect MAGCARD EXTI Line to MAGCARD GPIO Pin */
    GPIO_EXTILineConfig(RCP1_MAGCARD_PORT_SOURCE, RCP1_MAGCARD_PIN_SOURCE);  

    /* Configure MAGCARD EXTI line */
    EXTI_InitStructure.EXTI_Line = RCP1_MAGCARD_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set MAGCARD EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = RCP1_MAGCARD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  
}
void MagcardRCP1_GPIOInitDesable(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable MAGCARD GPIO clock */
    RCC_APB2PeriphClockCmd(RCP1_MAGCARD_CLK | RCC_APB2Periph_AFIO, ENABLE);
  
    /* Configure MAGCARD pin as input floating */
    GPIO_InitStructure.GPIO_Pin = RCP1_MAGCARD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(RCP1_MAGCARD_PORT, &GPIO_InitStructure);


    /* Connect MAGCARD EXTI Line to MAGCARD GPIO Pin */
    GPIO_EXTILineConfig(RCP1_MAGCARD_PORT_SOURCE, RCP1_MAGCARD_PIN_SOURCE);  

    /* Configure MAGCARD EXTI line */
    EXTI_InitStructure.EXTI_Line = RCP1_MAGCARD_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set MAGCARD EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = RCP1_MAGCARD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;

    NVIC_Init(&NVIC_InitStructure); 
  
}


void EXTI9_5_IRQHandler(void)
{
	unsigned int i;
//	unsigned long timeout=0;
    unsigned char tmp,tmp1;
	if(EXTI_GetITStatus(RCP1_MAGCARD_EXTI_LINE) != RESET)
	{	
		MagcardRCP1_GPIOInitDesable();
		
		  CLK_Out_H();
    error=0;
    delay_ms_g(delay_time);
    
	         
 //check data high -- CP finish
    while((GPIO_ReadInputDataBit(CLS3_MAGCARD_PORT,CLS3_MAGCARD_PIN))==0){
    delay_ms_g(delay_time);
    }
    

 //wait BR
	CLK_Out_L();
	    
//wait data low -- BR start
          i=0;
    while((GPIO_ReadInputDataBit(CLS3_MAGCARD_PORT,CLS3_MAGCARD_PIN))==1) {
	     i++;
	    if(i>30000)
		{
    		break;
		}
//	delay_led(300);
    delay_ms_g(delay_time);
    }
    
    CLK_Out_H();

 // check data high -- BR finish
    while((GPIO_ReadInputDataBit(CLS3_MAGCARD_PORT,CLS3_MAGCARD_PIN))==0){
    delay_ms_g(delay_time);
    }
  
    //read 16bit preamble
    delay_ms_g(delay_time);
    for (i=0;i<16;i++) {
        CLK_Out_L();				
        delay_ms_g(delay_time);
        CLK_Out_H();
        delay_ms_g(delay_time);
     }


        
//read strip 1       
    tmp=0;       

 //read strip 2             
    for (i=0;i<704;i++) {
        CLK_Out_L();				
        delay_ms_g(delay_time);
        tmp1 = (GPIO_ReadInputDataBit(CLS3_MAGCARD_PORT,CLS3_MAGCARD_PIN));
        tmp1 = (tmp1 << 4) & 0x10;
        tmp = (tmp >> 1) & 0x0f;
        tmp = tmp | tmp1;       
        if (((i+1) % 5) == 0)  {
 //       tmp=tmp|0x60;
		CardData[Get_Data++]=tmp;
        tmp = 0;
        }
	    CLK_Out_H();				
        delay_ms_g(delay_time);
        } 

      	track3check();
	      
 //read strip 3
    for (i=0;i<704;i++) {
        CLK_Out_L();				
       delay_ms_g(delay_time);
        tmp1 = (GPIO_ReadInputDataBit(CLS3_MAGCARD_PORT,CLS3_MAGCARD_PIN));
        tmp1 = (tmp1 << 4) & 0x10;
        tmp = (tmp >> 1) & 0x0f;
        tmp = tmp | tmp1;      
        if (((i+1) % 5)==0)  {
 //         tmp=tmp|0x60;
		  CardData[Get_Data++]=tmp;
          tmp = 0;    
            }
	    CLK_Out_H();				
        delay_ms_g(delay_time);
        }
		
		track2check();
			if(!Dir)
		{ 
			reverse();
		} 
	
		for(i=0;i<140;i++)
		{
		   ReadTrack3Len++;
		   if(CardData[i]==0x1f)  break;
		}
		for(i=140;i<280;i++)
		{
		   ReadTrack2Len++;
		   if(CardData[i]==0x1f)  break;
		}
		if(ReadTrack3Len==1)
		{
			ReadTrack3Len =0x00;
		}
		else
		{
			ReadTrack3Len-=2;
		}
		if(ReadTrack2Len==1)
		{
			ReadTrack2Len =0x00;
		}
		else
		{
			ReadTrack2Len-=2;
		}
		for(i=0;i<280;i++)
		  CardData[i]=(~CardData[i])&0x0f;
	  EXTI_ClearITPendingBit(RCP1_MAGCARD_EXTI_LINE);
//	  if(CardData[0]==0x0b)
	  	fTrackHaveData = 1;
	}	
}

unsigned char GeneraldataTest_new(unsigned char *T2T3rbuf,unsigned int len)
{ 
//	unsigned int i;
	unsigned long j=0;		 // timeout=0,
//    unsigned char tmp,tmp1,k=0;
//wait card swipe					   , 
 /*    
 	while((GPIO_ReadInputDataBit(CLS3_MAGCARD_PORT,CLS3_MAGCARD_PIN))==1) 
	 {
	 	timeout++;
		if(timeout>20000000)
		{
			mag_fail();
			return 1;
		}
     	//delay_ms_g(delay_time);
	 } 
*/
  
	
		if(!Dir)
		{
			for(j=0;j<ReadTrack3Len;j++)
			{
			   ReadTrack3buf[j]= CardData[ReadTrack3Len-j]+0x30;
			  // CardData[i]==CardData[ReadTrack3Len-i];  
			}
			for(j=0;j<ReadTrack2Len;j++)
			{
			    ReadTrack2buf[j]= CardData[140+ReadTrack2Len-j]+0x30;  
			}
		}
		else
		{
			for(j=0;j<ReadTrack3Len;j++)
			{
			   ReadTrack3buf[j]= CardData[j]+0x30;
			  // CardData[i]==CardData[ReadTrack3Len-i];  
			}
			for(j=0;j<ReadTrack2Len;j++)
			{
			    ReadTrack2buf[j]= CardData[140+j]+0x30;  
			}
		}
		if((ReadTrack2Len!=0)||(ReadTrack3Len!=0))
		{
			if(ReadTrack2Len!=0)
			{
				memcpy(T2T3rbuf,ReadTrack2buf,ReadTrack2Len);
				gb_Le = ReadTrack2Len;
			}
			if(ReadTrack3Len!=0)
			{
				if(ReadTrack2Len!=0)
				{
					memcpy(T2T3rbuf+ReadTrack2Len,ReadTrack3buf,ReadTrack3Len);
					gb_Le = ReadTrack2Len+ReadTrack3Len;
				}
				else
				{
					memcpy(T2T3rbuf+ReadTrack2Len,ReadTrack3buf,ReadTrack3Len);
					gb_Le =ReadTrack3Len;
				}
			}
	     	beep_on();
			for(j=0;j<35000;j++);
			for(j=0;j<35000;j++);
			for(j=0;j<35000;j++);
			for(j=0;j<35000;j++);
			beep_off();	
			return 0;
		}
		else
		{
			mag_fail();
			return 2;
		}
/*		
		if(CardData[0]==0x0f)
		{
			for(i=0;i<140;i++)
		  		CardData[i]=(~CardData[140+i])&0x0f;
		} 
*/		  
}



unsigned char MAG_XfrBlock(unsigned char *T2T3rbuf,unsigned int len)
{
	/*	unsigned int j=0;
	    GeneraldataTest_new();
		if(!Dir)
		{
			for(j=0;j<ReadTrack3Len;j++)
			{
			   ReadTrack3buf[j]= CardData[ReadTrack3Len-j]+0x30;
			  // CardData[i]==CardData[ReadTrack3Len-i];  
			}
			for(j=0;j<ReadTrack2Len;j++)
			{
			    ReadTrack2buf[j]= CardData[140+ReadTrack2Len-j]+0x30;  
			}
		}
		else
		{
			for(j=0;j<ReadTrack3Len;j++)
			{
			   ReadTrack3buf[j]= CardData[j]+0x30;
			  // CardData[i]==CardData[ReadTrack3Len-i];  
			}
			for(j=0;j<ReadTrack2Len;j++)
			{
			    ReadTrack2buf[j]= CardData[140+j]+0x30;  
			}
		}
		if(ReadTrack2Len!=0)
		{
			memcpy(T2T3rbuf,ReadTrack2buf,ReadTrack2Len);
			gb_Le = ReadTrack2Len;
		}
		if(ReadTrack3Len!=0)
		{
			if(ReadTrack2Len!=0)
			{
				memcpy(T2T3rbuf+ReadTrack2Len,ReadTrack3buf,ReadTrack3Len);
				gb_Le = ReadTrack2Len+ReadTrack3Len;
			}
			else
			{
				memcpy(T2T3rbuf+ReadTrack2Len,ReadTrack3buf,ReadTrack3Len);
				gb_Le =ReadTrack3Len;
			}
		}
		beep_on();
		for(j=0;j<25000;j++);
		for(j=0;j<25000;j++);
		beep_off();	  */

	return 0;
} 
unsigned char mag_flag(void)
{
	if(fTrackHaveData==1)
	{
		return 1;
	}
	else 
	{
	 	return 0;
	}

}

void CLR_mag_flag(void)
{
	fTrackHaveData =0;
}

/////////////////////// 上海众恒 2015.6.29   胡玉新  ////////////////////////////////////////////////////////////////////////////////


/*
#define READ_OK	        	0x00

#define READ_LRCERROR		0xF1 
#define READ_PARITYERROR	0xF2 
#define	READ_NOENDCODE		0xF3 
#define READ_NOSTARTCODE	0xF4 
#define READ_NODATA		    0xF5 



MagcardGlobalIOTTL magcardGlobalIOTTL;



void MagcardTTLGPIOInit(void);
void ResetMagcard_TTL(void);
int ReadMagcardT2Data_TTL(unsigned char *rbuf);
int ReadMagcardT3Data_TTL(unsigned char *rbuf);
int ReadMagcardT2T3Data_TTL(unsigned char *T2T3rbuf);

//extern void Track1Check(); 
unsigned char Track2Check(void); 
unsigned char Track3Check(void);
extern void beep_on(void);
extern void beep_off(void);
extern void  beep_con(void);
unsigned int MagDriveCheck5Bits(unsigned char a5Bits);
unsigned int MagDriveCheckTrackData(unsigned char* pBuff,unsigned char nLen);
unsigned int MagDriveCheckLRC(unsigned char *pBuff,unsigned char nLen);
unsigned char MAG_XfrBlock(unsigned char *T2T3rbuf,unsigned int len);
unsigned char mag_flag(void);
void CLR_mag_flag(void);
int JudgeRecvT2Data_IOTTL(unsigned char *rbuf,unsigned int* rlen);
int JudgeRecvT3Data_IOTTL(unsigned char *rbuf,unsigned int* rlen);
void MagcardDelay(unsigned int count);
//static void split(unsigned char **des, unsigned char  *src, const unsigned char *del);


unsigned char ConvertReverseData(unsigned char *srcData,unsigned char slen)
{
	unsigned char offset=0;
	unsigned char temp=0,temp2=0,i=0,j=0,htmp=0,ltmp=0,bitCount=0;
	unsigned char desData[160]={0x00};
	memset(desData,0,160);
	for(i=slen;i>0;i--)
		{
			if(srcData[i] != 0x00)
				break;
			if(i<=1)
				return 1;
		}
		temp=srcData[i]&0x1F;
		if(temp==0x1A)
		{
			bitCount = 0;	
		}
		else
		{
			for(bitCount =0;bitCount<5;bitCount++)
			{
				temp=srcData[i]&0x1F;
				temp2 = (  (srcData[i-1]&0x1F) >> (5 - bitCount)  );
				temp = (temp<<bitCount) | temp2;
				if(temp==0x1A)
				{
					//bitCount = 5-bitCount;
					break;	
				}
			}
			
		}
		if(bitCount!=0)
		{
			for(j=i;j > 0;j--)
			{
				htmp = ((srcData[j]&0x1F) << bitCount)&0x1F; 
				ltmp = ((srcData[j-1]&0x1F) >> (5 - bitCount))&0x1F; 
				desData[j] = htmp|ltmp;
			
			}
			
			for(offset=0;offset<slen;offset++)
			{
				if(desData[offset]&0x1F == 0x1F)
					break;
			}
			if(offset>=slen)
				return 1;
			memcpy(srcData,desData+offset,i+offset);
		}

		return 0;
}


int JudgeRecvT2Data_IOTTL(unsigned char *rbuf,unsigned int* rlen)
{
	return 0;
}

int ReadMagcardT2Data_TTL(unsigned char *rbuf)
{
  	 unsigned int retCheck = 1;
	 magcardGlobalIOTTL.magcardRevcTimeFlag = 1;
	 magcardGlobalIOTTL.magcardRevcTimeCount = 0;
	 ReadyToReadMagcard(); 
	 do
	 {	   
		   if(!ReadMagcardCLSDataBit() )   
	       {   
	            while( !ReadMagcardCLSDataBit() )   
	            {
				 	if(magcardGlobalIOTTL.magcardRevcTimeCount > 10000)
					break;
					if( !magcardTTLT2.fTrack2Direction ) //反向
						MagcardDelay(80);	 
					else							     //正向
						MagcardDelay(80);
				}  
				SetRCP2Interrupt(0);  
				    
	            if( magcardTTLT2.fTrack2HaveData ) 
				{
					retCheck = Track2Check();
					if(retCheck != 1)
					break;
				}  
	            else 
				{
					magcardTTLT2.nReadTrack2Len = 0; 
				}  	
				 magcardGlobalIOTTL.magcardRevcTimeCount++;									        
	       }  
	 }	
	while (magcardGlobalIOTTL.magcardRevcTimeCount < 10000);
	SetRCP2Interrupt(1); 
	magcardGlobalIOTTL.magcardRevcTimeFlag = 0;	

	if ( magcardGlobalIOTTL.magcardRevcTimeCount >= 10000 )
	{
		magcardGlobalIOTTL.magcardRevcTimeCount = 0; 
		return ERR_TimeOut;					
	}
	else 
	{
	   	magcardGlobalIOTTL.magcardRevcTimeCount = 0; 
		if( retCheck!=0 )
			return 0x0001;
		else
		{
			memcpy(rbuf,magcardTTLT2.chTrack2Buffer,magcardTTLT2.nReadTrack2Len);
			return MAGCARDOK;
		}
	} 
}

int JudgeRecvT3Data_IOTTL(unsigned char *rbuf,unsigned int* rlen)
{
	return 0;
}

int ReadMagcardT3Data_TTL(unsigned char *rbuf)
{
	 unsigned int retCheck = 1;
	 magcardGlobalIOTTL.magcardRevcTimeFlag = 1;
	 magcardGlobalIOTTL.magcardRevcTimeCount = 0;
	 ReadyToReadMagcard(); 
	 do
	 {	
		    
		   if(!ReadMagcardCLSDataBit() )   
	       {   
	            while( !ReadMagcardCLSDataBit() )   
	            {
				 	if(magcardGlobalIOTTL.magcardRevcTimeCount > 10000)
					break;
					MagcardDelay(80);
			
				}  
				SetRCP3Interrupt(0);   
	   
	            if( magcardTTLT3.fTrack3HaveData ) 
				{
					retCheck = Track3Check();
					if(retCheck != 1)
					break;
				}  
	            else 
				{
					magcardTTLT3.nReadTrack3Len = 0; 
				}  										      
	   		  magcardGlobalIOTTL.magcardRevcTimeCount++;
	       }  
	 }	
	while (magcardGlobalIOTTL.magcardRevcTimeCount < 10000);
	SetRCP3Interrupt(1); 
	magcardGlobalIOTTL.magcardRevcTimeFlag = 0;	

	if ( magcardGlobalIOTTL.magcardRevcTimeCount >= 10000 )
	{
		magcardGlobalIOTTL.magcardRevcTimeCount = 0; 
		return ERR_TimeOut;						
	}
	else 
	{
	   	magcardGlobalIOTTL.magcardRevcTimeCount = 0; 
		if( retCheck!=0 )
			return 0x0001;
		else
		{
			memcpy(rbuf,magcardTTLT3.chTrack3Buffer,magcardTTLT3.nReadTrack3Len);
			return MAGCARDOK;
		}
	} 
}



int JudgeRecvT2T3Data_IOTTL(unsigned char *rbuf,unsigned int* rlen)
{
     return 0;
}



void ResetMagcard_TTL(void)
{
	ReadyToReadMagcard();
}



void ReadyToReadMagcard(void)   
{   
	memset((unsigned char*)&magcardTTLT2, 0, sizeof(struct _MagcardTTLT2));
	memset((unsigned char*)&magcardTTLT3, 0, sizeof(struct _MagcardTTLT3));
} 


unsigned char Track2Check(void)
{
	unsigned int i=0,j=0,k=0,nTemp=0;  
//	unsigned char ret = 0;
											     
	for(i=magcardTTLT2.chTrack2Index;i>1;i--)   
    {   
        if( magcardTTLT2.chTrack2Buffer[i] != 0x00 )   
            break;   
    }   
   
    if( i < 2 )   
    {   
        magcardTTLT2.nReadTrack2Len = i;   
        return READ_NODATA;   
    }   
   
    magcardTTLT2.nReadTrack2Len = i+1;   

    if( magcardTTLT2.fTrack2StartEnd && ( magcardTTLT2.nReadTrack2Len > 3 ) )   
    {   
        // LRC    
        for(i=0;i<5;i++)   
        {   
            nTemp = nTemp << 1;   
            if( ( magcardTTLT2.chTrack2Buffer[0] >> i ) & 0x01 )   
                nTemp++;   
        }      
        magcardTTLT2.chTrack2Buffer[0] = nTemp;   
   
        for(i=2;i<magcardTTLT2.nReadTrack2Len;i++)   
        {   
            nTemp = 0;   
            for(j=0;j<5;j++)   
            {   
                nTemp = nTemp << 1;   
                if( ( magcardTTLT2.chTrack2Buffer[i] >> j ) & 0x01 )   
                    nTemp++;   
            }      
            magcardTTLT2.chTrack2Buffer[i] = nTemp;   
        }   
   
        j = magcardTTLT2.nReadTrack2Len>>1;   
        k = magcardTTLT2.nReadTrack2Len-1;   
        for(i=0;i<j;i++)   
        {   
            nTemp = magcardTTLT2.chTrack2Buffer[i];   
            magcardTTLT2.chTrack2Buffer[i] = magcardTTLT2.chTrack2Buffer[k];   
            magcardTTLT2.chTrack2Buffer[k] = nTemp;   
            k--;   
        }   
    }   
   

    if( !magcardTTLT2.fTrack2Direction )   
    {   
   
        for(i=0;i<5;i++)   
        {   
            nTemp = nTemp << 1;   
            if( ( magcardTTLT2.chTrack2Buffer[0] >> i ) & 0x01 )   
                nTemp++;   
        }   
        magcardTTLT2.chTrack2Buffer[0] = nTemp;  

        for(i=2;i<magcardTTLT2.nReadTrack2Len;i++)   
        {   
            nTemp = 0;   
            for(j=0;j<5;j++)   
            {   
                nTemp = nTemp << 1;   
                if( ( magcardTTLT2.chTrack2Buffer[i] >> j ) & 0x01 )   
                    nTemp++;   
            }      
            magcardTTLT2.chTrack2Buffer[i] = nTemp;   
        }   
	   
        j = magcardTTLT2.nReadTrack2Len>>1;   
        k = magcardTTLT2.nReadTrack2Len-1;   
        for(i=0;i<j;i++)   
        {   
            nTemp = magcardTTLT2.chTrack2Buffer[i];   
            magcardTTLT2.chTrack2Buffer[i] = magcardTTLT2.chTrack2Buffer[k];   
            magcardTTLT2.chTrack2Buffer[k] = nTemp;   
            k--;   
        }   
    }   
   

    if( magcardTTLT2.chTrack2Buffer[0] != READ_TRACK23_STARTCODE )   
    {   
        return READ_NOSTARTCODE;   
    }   
   
 
    if( magcardTTLT2.chTrack2Buffer[magcardTTLT2.nReadTrack2Len-2] != READ_TRACK23_ENDCODE )   
    {   
        for(i=magcardTTLT2.nReadTrack2Len-3;i>0;i--)   
        {   
            if( magcardTTLT2.chTrack2Buffer[i] == READ_TRACK23_ENDCODE )   
            {   
                magcardTTLT2.nReadTrack2Len = i+2;   
                break;   
            }   
        }   
   
        if( magcardTTLT2.nReadTrack2Len < 3 )   
        {   
            return READ_NOENDCODE;   
        }   
    }   
   
 
//	if(MagDriveCheckTrackData(magcardTTLT2.chTrack2Buffer,magcardTTLT2.nReadTrack2Len))
//	return READ_PARITYERROR; 
							   
   
  
//	if( (magcardTTLT2.fTrack2Direction==0) && (magcardTTLT2.chTrack2Buffer[magcardTTLT2.nReadTrack2Len-1]!=0x1F)  ) 
	{
  //  	if(MagDriveCheckLRC(magcardTTLT2.chTrack2Buffer,magcardTTLT2.nReadTrack2Len))
//			return READ_LRCERROR;
	}	
   

    for(i=0;i<magcardTTLT2.nReadTrack2Len-1;i++)   
    {   
        magcardTTLT2.chTrack2Buffer[i] = ( magcardTTLT2.chTrack2Buffer[i] & 0x0F ) + 0x30;   
    }   

    magcardTTLT2.nReadTrack2Len--; 
	return READ_OK;
	
}


unsigned char Track3Check(void)
{
	unsigned int i=0,j=0,k=0,nTemp=0;  
	unsigned char ret=0;
	if( !magcardTTLT3.fTrack3Direction ) 
	{
		ret = ConvertReverseData( magcardTTLT3.chTrack3Buffer,magcardTTLT3.chTrack3Index);
		if(ret)
			return READ_NODATA; 
	}


//	for(i=magcardTTLT3.chTrack3Index;i>1;i--)  
	for(i=0;i<magcardTTLT3.chTrack3Index;i++)  
    {   
        if( magcardTTLT3.chTrack3Buffer[i] == 0x00 )   
            break;   
    }   
   
    if( i < 3 )   
    {   
        magcardTTLT3.nReadTrack3Len = i;   
        return READ_NODATA;   
    }   
   	magcardTTLT3.nReadTrack3Len = i; 
    //magcardTTLT3.nReadTrack3Len = i+1;  
   
   
    if( magcardTTLT3.fTrack3StartEnd && ( magcardTTLT3.nReadTrack3Len > 3 ) )   
    {   
        // LRC    
        for(i=0;i<5;i++)   
        {   
            nTemp = nTemp << 1;   
            if( ( magcardTTLT3.chTrack3Buffer[0] >> i ) & 0x01 )   
                nTemp++;   
        }      
        magcardTTLT3.chTrack3Buffer[0] = nTemp;   
   
        for(i=2;i<magcardTTLT3.nReadTrack3Len;i++)   
        {   
            nTemp = 0;   
            for(j=0;j<5;j++)   
            {   
                nTemp = nTemp << 1;   
                if( ( magcardTTLT3.chTrack3Buffer[i] >> j ) & 0x01 )   
                    nTemp++;   
            }      
            magcardTTLT3.chTrack3Buffer[i] = nTemp;   
        }   
   
        j = magcardTTLT3.nReadTrack3Len>>1;   
        k = magcardTTLT3.nReadTrack3Len-1;   
        for(i=0;i<j;i++)   
        {   
            nTemp = magcardTTLT3.chTrack3Buffer[i];   
            magcardTTLT3.chTrack3Buffer[i] = magcardTTLT3.chTrack3Buffer[k];   
            magcardTTLT3.chTrack3Buffer[k] = nTemp;   
            k--;   
        }   
    }   
   

    if( !magcardTTLT3.fTrack3Direction )   
    {   
   
        for(i=0;i<5;i++)   
        {   
            nTemp = nTemp << 1;   
            if( ( magcardTTLT3.chTrack3Buffer[0] >> i ) & 0x01 )   
                nTemp++;   
        }   
        magcardTTLT3.chTrack3Buffer[0] = nTemp;  

        for(i=2;i<magcardTTLT3.nReadTrack3Len;i++)   
        {   
            nTemp = 0;   
            for(j=0;j<5;j++)   
            {   
                nTemp = nTemp << 1;   
                if( ( magcardTTLT3.chTrack3Buffer[i] >> j ) & 0x01 )   
                    nTemp++;   
            }      
            magcardTTLT3.chTrack3Buffer[i] = nTemp;   
        }   
	   
        j = magcardTTLT3.nReadTrack3Len>>1;   
        k = magcardTTLT3.nReadTrack3Len-1;   
        for(i=0;i<j;i++)   
        {   
            nTemp = magcardTTLT3.chTrack3Buffer[i];   
            magcardTTLT3.chTrack3Buffer[i] = magcardTTLT3.chTrack3Buffer[k];   
            magcardTTLT3.chTrack3Buffer[k] = nTemp;   
            k--;   
        }   
		  
    }   
   
  
    if( magcardTTLT3.chTrack3Buffer[0] != READ_TRACK23_STARTCODE )   
    {   
        return READ_NOSTARTCODE;   
    }   
   
  
    if( magcardTTLT3.chTrack3Buffer[magcardTTLT3.nReadTrack3Len-3] != READ_TRACK23_ENDCODE )   
    {   
        for(i=magcardTTLT3.nReadTrack3Len-3;i>0;i--)   
        {   
            if( magcardTTLT3.chTrack3Buffer[i] == READ_TRACK23_ENDCODE )   
            {   
                magcardTTLT3.nReadTrack3Len = i+3;   
                break;   
            }   
        }   
   
        if( magcardTTLT3.nReadTrack3Len < 3 )   
        {   
            return READ_NOENDCODE;   
        }   
    }   
   

	if(MagDriveCheckTrackData(magcardTTLT3.chTrack3Buffer,magcardTTLT3.nReadTrack3Len))
	return READ_PARITYERROR; 
							   
   
 
   if(MagDriveCheckLRC(magcardTTLT3.chTrack3Buffer,magcardTTLT3.nReadTrack3Len))
	return READ_LRCERROR;
   
 
    for(i=0;i<magcardTTLT3.nReadTrack3Len-1;i++)   
    {   
        magcardTTLT3.chTrack3Buffer[i] = ( magcardTTLT3.chTrack3Buffer[i] & 0x0F ) + 0x30;   
    }   
 
    magcardTTLT3.nReadTrack3Len--; 
	return READ_OK;
	
}



unsigned int MagDriveCheck5Bits(unsigned char a5Bits)   
{   
    unsigned char i = 0;   
    unsigned char nOddNum = 0;   
           
    for (;i<5;i++)    
    {   
        if (a5Bits & (0x01<<i)) nOddNum++;    
    }      
   
    if(nOddNum%2)
		return 0; 
	else
		return 1; 
	   
} 

unsigned int MagDriveCheckTrackData(unsigned char* pBuff,unsigned char nLen)   
{   
    unsigned char i;   
       
    for(i = 0 ; i < nLen ; i++)   
    {   
        if (1 ==  MagDriveCheck5Bits(pBuff[i]))  
		return 1;       
    }   
       
    return 0;   
}


unsigned int MagDriveCheckLRC(unsigned char *pBuff,unsigned char nLen)   
{   
    unsigned char aTmp = 0;   
    unsigned char i;   
       
    for(i = 0; i < nLen; i++)   
    {   
        aTmp ^= pBuff[i];       
    }   
    return (aTmp&0x0F);   
}


static void MagcardDelay(unsigned int count)
{
	 for(; count != 0; count--);
}


void MagcardTTLGPIOInit(void)
{
  	MagcardCLS_GPIOInit();
  	MagcardRCP2_GPIOInit();
 	MagcardRDP2_GPIOInit(); 
	MagcardCLS3_GPIOInit();
  	MagcardRCP3_GPIOInit();
 	MagcardRDP3_GPIOInit();
} 


void ReadyToRead(void)
{
	memset((unsigned char*)&magcardTTLT2, 0, sizeof(struct _MagcardTTLT2));
	memset((unsigned char*)&magcardTTLT3, 0, sizeof(struct _MagcardTTLT3));
}

extern void open_time5_isr(void);
extern unsigned short gb_Le; 
unsigned char MAG_XfrBlock(unsigned char *T2T3rbuf,unsigned int len)
{
	 unsigned int retCheck2 = 1,i,retCheck3 = 1;
	 magcardGlobalIOTTL.magcardRevcTimeFlag = 1;
	 magcardGlobalIOTTL.magcardRevcTimeCount = 0;
//	 unsigned long timess=0;
//	 for(timess=0;timess<80000;timess++);
//	 ReadyToReadMagcard(); 
//	memset((unsigned char*)&T2T3rbuf, 0, sizeof(T2T3rbuf));
	gb_Le=0;
 do
	 {	
		   
		   if(!ReadMagcardCLSDataBit() )   
	       {   
	            while( !ReadMagcardCLSDataBit() )   
	            {
				 
					if(magcardGlobalIOTTL.magcardRevcTimeCount > 10000)
					break;
					MagcardDelay(180); 
				}     
				SetRCP2Interrupt(0);
				SetRCP3Interrupt(0);    

				if( magcardTTLT2.fTrack2HaveData ) 
				{
					retCheck2 = Track2Check();
					if((retCheck2 != 1)&&( !magcardTTLT3.fTrack3HaveData )) 
					break;
				}  
	            else 
				{
					magcardTTLT2.nReadTrack2Len = 0; 
				}  
 
	            if( magcardTTLT3.fTrack3HaveData ) 
				{
					retCheck3 = Track3Check();
					if(retCheck3 != 1)
					break;
				}  
	            else 
				{
					magcardTTLT3.nReadTrack3Len = 0; 
				}  										      
	       } 
//		   magcardGlobalIOTTL.magcardRevcTimeCount++; 
		   IWDG_ReloadCounter();
	 }
	while (magcardGlobalIOTTL.magcardRevcTimeCount < 10000);
	SetRCP2Interrupt(1);
	SetRCP3Interrupt(1); 
	magcardGlobalIOTTL.magcardRevcTimeFlag = 0;	

	if ( magcardGlobalIOTTL.magcardRevcTimeCount >= 10000 )
	{
		magcardGlobalIOTTL.magcardRevcTimeCount = 0; 
		ReadyToReadMagcard();
		return ERR_TimeOut;						
	}
	else 
	{
	   	magcardGlobalIOTTL.magcardRevcTimeCount = 0; 
		if(( retCheck2!=0)&&(retCheck3!=0))
		{
			ReadyToReadMagcard();
			return 0x0001;
		}
		else
		{
			if(retCheck2==0)
			{
				memcpy(T2T3rbuf,magcardTTLT2.chTrack2Buffer,magcardTTLT2.nReadTrack2Len);
				gb_Le = magcardTTLT2.nReadTrack2Len;
			}
			if(retCheck3==0)
			{
				if(retCheck2==0)
				{
				//	T2T3rbuf[magcardTTLT2.nReadTrack2Len] = 0x41;
					memcpy(T2T3rbuf+magcardTTLT2.nReadTrack2Len,magcardTTLT3.chTrack3Buffer,magcardTTLT3.nReadTrack3Len);
					gb_Le = magcardTTLT2.nReadTrack2Len+magcardTTLT3.nReadTrack3Len;
				}
				else
				{
					memcpy(T2T3rbuf+magcardTTLT2.nReadTrack2Len,magcardTTLT3.chTrack3Buffer,magcardTTLT3.nReadTrack3Len);
					gb_Le = magcardTTLT3.nReadTrack3Len;
				}
			}
			beep_on();
			for(i=0;i<25000;i++);
			for(i=0;i<25000;i++);
			beep_off();
			ReadyToReadMagcard();	 
			return MAGCARDOK;
		}
	} 
}


unsigned char mag_flag(void)
{
	if(magcardTTLT2.fTrack2HaveData||magcardTTLT3.fTrack3HaveData)
	{
		return 1;
	}
	else 
	{
	 	return 0;
	}
}

void CLR_mag_flag(void)
{

	magcardTTLT2.fTrack2HaveData=0;
	magcardTTLT3.fTrack3HaveData=0;

}
 */





