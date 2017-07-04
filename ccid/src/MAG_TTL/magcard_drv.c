#include "magcard_drv.h"


/*
MagcardTTLT1 magcardTTLT1;
MagcardTTLT2 magcardTTLT2;
MagcardTTLT3 magcardTTLT3;

//unsigned char testData[200] = {0x00};

void MagcardCLS_GPIOInit(void);
unsigned char ReadMagcardCLSDataBit(void);

void SetRCP2Interrupt(unsigned char status);
void MagcardRDP2_GPIOInit(void);
unsigned char ReadMagcardRDP2DataBit(void);
void MagcardRCP2_GPIOInit(void);
void MagcardRCP2_IRQHandler(void);

void SetRCP3Interrupt(unsigned char status);
void MagcardRDP3_GPIOInit(void);
unsigned char ReadMagcardRDP3DataBit(void);
void MagcardRCP3_GPIOInit(void);
void MagcardRCP3_IRQHandler(void);

unsigned int DRV_MagDriveCheck5Bits(unsigned char a5Bits);


unsigned int DRV_MagDriveCheck5Bits(unsigned char a5Bits)   
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


void MagcardCLS_GPIOInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
 
  RCC_APB2PeriphClockCmd(CLS_MAGCARD_CLK, ENABLE);


  GPIO_InitStructure.GPIO_Pin = CLS_MAGCARD_PIN;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;    //GPIO_Mode_IPD;// 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CLS_MAGCARD_PORT, &GPIO_InitStructure);
	
}

void MagcardCLS3_GPIOInit(void)
{										   
  GPIO_InitTypeDef  GPIO_InitStructure;
  

  RCC_APB2PeriphClockCmd(CLS_MAGCARD_CLK, ENABLE);


  GPIO_InitStructure.GPIO_Pin = CLS_MAGCARD_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //GPIO_Mode_IN_FLOATING; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(CLS_MAGCARD_PORT, &GPIO_InitStructure);
	
}


void MagcardRDP1_GPIOInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  

  RCC_APB2PeriphClockCmd(RDP1_MAGCARD_CLK, ENABLE);


  GPIO_InitStructure.GPIO_Pin = RDP1_MAGCARD_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(RDP1_MAGCARD_PORT, &GPIO_InitStructure);
	
}
unsigned char ReadMagcardRDP1DataBit(void)
{
	return GPIO_ReadInputDataBit(RDP1_MAGCARD_PORT, RDP1_MAGCARD_PIN);
}

void MagcardRCP1_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    RCC_APB2PeriphClockCmd(RCP1_MAGCARD_CLK | RCC_APB2Periph_AFIO, ENABLE);
  
 
    GPIO_InitStructure.GPIO_Pin = RCP1_MAGCARD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(RCP1_MAGCARD_PORT, &GPIO_InitStructure);


  
    GPIO_EXTILineConfig(RCP1_MAGCARD_PORT_SOURCE, RCP1_MAGCARD_PIN_SOURCE);  


    EXTI_InitStructure.EXTI_Line = RCP1_MAGCARD_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = RCP1_MAGCARD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  
}

void SetRCP1Interrupt(unsigned char status)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	 
    EXTI_InitStructure.EXTI_Line = RCP1_MAGCARD_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  	if(status)
    	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	else
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);	
}

void MagcardRCP1_IRQHandler(void)
{ 
	unsigned char chTemp;   
    unsigned char ftemp;   
   	if(EXTI_GetITStatus(RCP1_MAGCARD_EXTI_LINE) != RESET)
	{
		EXTI_ClearITPendingBit(RCP1_MAGCARD_EXTI_LINE);

	    magcardTTLT1.fTrack1HaveData = TRUE;   
   	    ftemp = !ReadMagcardRDP1DataBit();   
	   

		if( magcardTTLT1.fTrack1Start )  
	    {   
	        if( magcardTTLT3.fTrack3Direction ) 
	        {   
	            magcardTTLT1.chTrack1Buf1 = magcardTTLT1.chTrack1Buf1 >> 1;   
	            if( ftemp )   
	            {   
	                magcardTTLT1.chTrack1Buf1 = magcardTTLT1.chTrack1Buf1 | 0x10;   
	            }   			   
	        }   
	        else  
	        {   
	            if( ftemp )   
	            {   
	                magcardTTLT1.chTrack1Buf1 = magcardTTLT1.chTrack1Buf1 | 0x01;   
	            }   
	            magcardTTLT1.chTrack1Buf1 = magcardTTLT1.chTrack1Buf1 << 1;   
	        }   
	        magcardTTLT1.chTrack1Counter++;   
	        if( magcardTTLT1.chTrack1Counter == 5 )   
	        {   
	            magcardTTLT1.chTrack1Counter = 0;   
	            if( magcardTTLT1.chTrackIndex < READ_TRACK1BUFSIZE - 1 )   
	            {   
	                magcardTTLT1.chTrack1Buffer[magcardTTLT1.chTrackIndex++] = magcardTTLT1.chTrack1Buf1;   
	                magcardTTLT1.chTrack1Buf1 = 0x00;   
	            }   
	        } 

//			if( magcardTTLT3.chTrack3Index == 100)
//			{
//				magcardTTLT3.chTrack3Index = 155;
//			} 
			  
	    } 
		
	    else   //先收低位在收高位
	    {   
	        chTemp = magcardTTLT1.chTrack1Buf1 & 0x01 ;   
	        magcardTTLT1.chTrack1Buf1 = magcardTTLT1.chTrack1Buf1 >> 1;   
	        if( ftemp ) magcardTTLT1.chTrack1Buf1 = magcardTTLT1.chTrack1Buf1 | 0x10;   
	        magcardTTLT1.chTrack1Buf2 = magcardTTLT1.chTrack1Buf2 >> 1;   
	        if( chTemp ) magcardTTLT1.chTrack1Buf2 = magcardTTLT1.chTrack1Buf2 | 0x10;   
	   	
	       
	        if( ( magcardTTLT1.chTrack1Buf1 & 0x1F ) == READ_TRACK23_ENDCODE )   
	        {   
	     
	            if( ( magcardTTLT1.chTrack1Buf2 & 0x1F ) == READ_TRACK23_STARTCODE )   
	            {   
	                magcardTTLT1.chTrack1Buffer[magcardTTLT1.chTrackIndex++] = magcardTTLT1.chTrack1Buf2;   
	                magcardTTLT1.chTrack1Buffer[magcardTTLT1.chTrackIndex++] = magcardTTLT1.chTrack1Buf1;   
	                magcardTTLT1.fTrack1Start = TRUE;   
	                magcardTTLT1.fTrack1Direction = TRUE;   
	                magcardTTLT1.fTrack1StartEnd = TRUE;   
	                magcardTTLT1.chTrack1Buf1 = 0x00;   
	            }   
	   
	             //if(  magcardTTLT3.chTrack3Buf2==0x19 )
				else if(  DRV_MagDriveCheck5Bits(magcardTTLT1.chTrack1Buf2 & 0x1F) == 0 )  //奇检验判断正确则证明数据是完整的
	            {   	               								   
	                    magcardTTLT1.chTrack1Buffer[magcardTTLT1.chTrackIndex++] = magcardTTLT1.chTrack1Buf2;   
	                    //magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = 0x1F;   
						magcardTTLT1.chTrack1Buffer[magcardTTLT1.chTrackIndex++] = magcardTTLT1.chTrack1Buf1;
	                    magcardTTLT1.fTrack1Start = TRUE;   
	                    magcardTTLT1.fTrack1Direction = FALSE;    
	                    magcardTTLT1.fTrack1StartEnd = FALSE;   
	                    magcardTTLT1.chTrack1Buf1 = 0x00;   
	            } 
			
	        }   
	   	
	        else if( ( magcardTTLT1.chTrack1Buf2 & 0x1F ) == READ_TRACK23_STARTCODE )   
	        {   
	                magcardTTLT1.chTrack1Buffer[magcardTTLT1.chTrackIndex++] = magcardTTLT1.chTrack1Buf2;   
	                magcardTTLT1.chTrack1Buffer[magcardTTLT1.chTrackIndex++] = magcardTTLT1.chTrack1Buf1;   
	                magcardTTLT1.fTrack1Start = TRUE;   
	                magcardTTLT1.fTrack1Direction = TRUE;    
	                magcardTTLT1.fTrack1StartEnd = FALSE;   
	                magcardTTLT1.chTrack1Buf1 = 0x00;   
	        }   
	    } 
 	} 

}



unsigned char ReadMagcardCLSDataBit(void)
{
	return GPIO_ReadInputDataBit(CLS_MAGCARD_PORT, CLS_MAGCARD_PIN);
}


unsigned char ReadMagcardCLS3DataBit(void)
{
	return GPIO_ReadInputDataBit(CLS3_MAGCARD_PORT, CLS3_MAGCARD_PIN);
}

void MagcardRDP2_GPIOInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  
  RCC_APB2PeriphClockCmd(RDP2_MAGCARD_CLK, ENABLE);

 
  GPIO_InitStructure.GPIO_Pin = RDP2_MAGCARD_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPD; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(RDP2_MAGCARD_PORT, &GPIO_InitStructure);
	
}

unsigned char ReadMagcardRDP2DataBit(void)
{
	return GPIO_ReadInputDataBit(RDP2_MAGCARD_PORT, RDP2_MAGCARD_PIN);
}


void MagcardRCP2_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    
    RCC_APB2PeriphClockCmd(RCP2_MAGCARD_CLK | RCC_APB2Periph_AFIO, ENABLE);
  
   
    GPIO_InitStructure.GPIO_Pin = RCP2_MAGCARD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  //GPIO_Mode_IPD;//
    GPIO_Init(RCP2_MAGCARD_PORT, &GPIO_InitStructure);


   
    GPIO_EXTILineConfig(RCP2_MAGCARD_PORT_SOURCE, RCP2_MAGCARD_PIN_SOURCE);  

 
    EXTI_InitStructure.EXTI_Line = RCP2_MAGCARD_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

 
    NVIC_InitStructure.NVIC_IRQChannel = RCP2_MAGCARD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  
}

void SetRCP2Interrupt(unsigned char status)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	 
    EXTI_InitStructure.EXTI_Line = RCP2_MAGCARD_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  	if(status)
    	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	else
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);	
}
extern void close_time5_isr(void);;
void MagcardRCP2_IRQHandler(void)
{ 
	unsigned char chTemp;   
    unsigned char ftemp;   
	close_time5_isr();
   	if(EXTI_GetITStatus(RCP2_MAGCARD_EXTI_LINE) != RESET)
	{
		EXTI_ClearITPendingBit(RCP2_MAGCARD_EXTI_LINE);

	    magcardTTLT2.fTrack2HaveData = TRUE;   
	    ftemp = !ReadMagcardRDP2DataBit();   
	   
	    
		if( magcardTTLT2.fTrack2Start )  
	    {   
	       if( magcardTTLT2.fTrack2Direction ) 
	        {   
	            magcardTTLT2.chTrack2Buf1 = magcardTTLT2.chTrack2Buf1 >> 1;   
	            if( ftemp )   
	            {   
	                magcardTTLT2.chTrack2Buf1 = magcardTTLT2.chTrack2Buf1 | 0x10;   
	            }   			   
	        }   
	        else   
	        {   			    
	            if( ftemp )   
	            {   
	                magcardTTLT2.chTrack2Buf1 = magcardTTLT2.chTrack2Buf1 | 0x01;   
	            }   
	            magcardTTLT2.chTrack2Buf1 = magcardTTLT2.chTrack2Buf1 << 1;   
	        }   
	        magcardTTLT2.chTrack2Counter++;   
	        if( magcardTTLT2.chTrack2Counter == 5 )   
	        {   
	            magcardTTLT2.chTrack2Counter = 0;   
	            if( magcardTTLT2.chTrack2Index < READ_TRACK2BUFSIZE - 1 )   
	            {   
	                magcardTTLT2.chTrack2Buffer[magcardTTLT2.chTrack2Index++] = magcardTTLT2.chTrack2Buf1;   
	                magcardTTLT2.chTrack2Buf1 = 0x00;   
	            }
			if(magcardTTLT2.chTrack2Index==38)  
				ftemp=magcardTTLT2.chTrack2Index; 
	        }   
	    } 
	 
	    else   
	    {   
	        chTemp = magcardTTLT2.chTrack2Buf1 & 0x01 ;   
	        magcardTTLT2.chTrack2Buf1 = magcardTTLT2.chTrack2Buf1 >> 1;   
	        if( ftemp ) magcardTTLT2.chTrack2Buf1 = magcardTTLT2.chTrack2Buf1 | 0x10;   
	        magcardTTLT2.chTrack2Buf2 = magcardTTLT2.chTrack2Buf2 >> 1;   
	        if( chTemp ) magcardTTLT2.chTrack2Buf2 = magcardTTLT2.chTrack2Buf2 | 0x10;   
	   		
	      
	        if( ( magcardTTLT2.chTrack2Buf1 & 0x1F ) == READ_TRACK23_ENDCODE )   
	        {   
	            
				if( ( magcardTTLT2.chTrack2Buf2 & 0x1F ) == READ_TRACK23_STARTCODE )   
	            {   
	                magcardTTLT2.chTrack2Buffer[magcardTTLT2.chTrack2Index++] = magcardTTLT2.chTrack2Buf2;   
	                magcardTTLT2.chTrack2Buffer[magcardTTLT2.chTrack2Index++] = magcardTTLT2.chTrack2Buf1;   
	                magcardTTLT2.fTrack2Start = TRUE;   
	                magcardTTLT2.fTrack2Direction = TRUE;   
	                magcardTTLT2.fTrack2StartEnd = TRUE;   
	                magcardTTLT2.chTrack2Buf1 = 0x00;   
	            }   
	   
	           
	            else if(DRV_MagDriveCheck5Bits(magcardTTLT2.chTrack2Buf2 & 0x1F) == 0 )  
	            {   	               
	                    magcardTTLT2.chTrack2Buffer[magcardTTLT2.chTrack2Index++] = magcardTTLT2.chTrack2Buf2;   
	                    magcardTTLT2.chTrack2Buffer[magcardTTLT2.chTrack2Index++] = magcardTTLT2.chTrack2Buf1;   
	                    magcardTTLT2.fTrack2Start = TRUE;   
	                    magcardTTLT2.fTrack2Direction = FALSE;    
	                    magcardTTLT2.fTrack2StartEnd = FALSE;   
	                    magcardTTLT2.chTrack2Buf1 = 0x00;   
	            }   
	        }   
	   	
	        else if( ( magcardTTLT2.chTrack2Buf2 & 0x1F ) == READ_TRACK23_STARTCODE )   
	        {   
	                magcardTTLT2.chTrack2Buffer[magcardTTLT2.chTrack2Index++] = magcardTTLT2.chTrack2Buf2;   
	                magcardTTLT2.chTrack2Buffer[magcardTTLT2.chTrack2Index++] = magcardTTLT2.chTrack2Buf1;   
	                magcardTTLT2.fTrack2Start = TRUE;   
	                magcardTTLT2.fTrack2Direction = TRUE;    
	                magcardTTLT2.fTrack2StartEnd = FALSE;   
	                magcardTTLT2.chTrack2Buf1 = 0x00;   
	        }   
	    } 
 	}
//    EXTI_ClearITPendingBit(RCP2_MAGCARD_EXTI_LINE);
}



void MagcardRDP3_GPIOInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  
  RCC_APB2PeriphClockCmd(RDP3_MAGCARD_CLK, ENABLE);

 
  GPIO_InitStructure.GPIO_Pin = RDP3_MAGCARD_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(RDP3_MAGCARD_PORT, &GPIO_InitStructure);
	
}
unsigned char ReadMagcardRDP3DataBit(void)
{
	return GPIO_ReadInputDataBit(RDP3_MAGCARD_PORT, RDP3_MAGCARD_PIN);
}

void MagcardRCP3_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

   
    RCC_APB2PeriphClockCmd(RCP3_MAGCARD_CLK | RCC_APB2Periph_AFIO, ENABLE);
  
   
    GPIO_InitStructure.GPIO_Pin = RCP3_MAGCARD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(RCP2_MAGCARD_PORT, &GPIO_InitStructure);


   
    GPIO_EXTILineConfig(RCP3_MAGCARD_PORT_SOURCE, RCP3_MAGCARD_PIN_SOURCE);  

   
    EXTI_InitStructure.EXTI_Line = RCP3_MAGCARD_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

  
    NVIC_InitStructure.NVIC_IRQChannel = RCP3_MAGCARD_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  
}

void SetRCP3Interrupt(unsigned char status)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	   
    EXTI_InitStructure.EXTI_Line = RCP3_MAGCARD_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  	if(status)
    	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	else
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);	
}

void MagcardRCP3_IRQHandler(void)
{ 
	unsigned char chTemp;   
    unsigned char ftemp;  
	close_time5_isr(); 
   	if(EXTI_GetITStatus(RCP3_MAGCARD_EXTI_LINE) != RESET)
	{
		EXTI_ClearITPendingBit(RCP3_MAGCARD_EXTI_LINE);

	    magcardTTLT3.fTrack3HaveData = TRUE;   
   	    ftemp = !ReadMagcardRDP3DataBit();   
	   

		if( magcardTTLT3.fTrack3Start )
	    {   
	        if( magcardTTLT3.fTrack3Direction ) 
	        {   
	            magcardTTLT3.chTrack3Buf1 = magcardTTLT3.chTrack3Buf1 >> 1;   
	            if( ftemp )   
	            {   
	                magcardTTLT3.chTrack3Buf1 = magcardTTLT3.chTrack3Buf1 | 0x10;   
	            }   			   
	        }   
	        else 
	        {   
	            if( ftemp )   
	            {   
	                magcardTTLT3.chTrack3Buf1 = magcardTTLT3.chTrack3Buf1 | 0x01;   
	            }   
	            magcardTTLT3.chTrack3Buf1 = magcardTTLT3.chTrack3Buf1 << 1;   
	        }   
	        magcardTTLT3.chTrack3Counter++;   
	        if( magcardTTLT3.chTrack3Counter == 5 )   
	        {   
	            magcardTTLT3.chTrack3Counter = 0;   
	            if( magcardTTLT3.chTrack3Index < READ_TRACK3BUFSIZE - 1 )   
	            {   
	                magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = magcardTTLT3.chTrack3Buf1;   
	                magcardTTLT3.chTrack3Buf1 = 0x00;   
	            }   
	        } 

//			if( magcardTTLT3.chTrack3Index == 100)
//			{
//				magcardTTLT3.chTrack3Index = 155;
//			} 
			  
	    } 
		
	    else   //先收低位在收高位
	    {   
	        chTemp = magcardTTLT3.chTrack3Buf1 & 0x01 ;   
	        magcardTTLT3.chTrack3Buf1 = magcardTTLT3.chTrack3Buf1 >> 1;   
	        if( ftemp ) magcardTTLT3.chTrack3Buf1 = magcardTTLT3.chTrack3Buf1 | 0x10;   
	        magcardTTLT3.chTrack3Buf2 = magcardTTLT3.chTrack3Buf2 >> 1;   
	        if( chTemp ) magcardTTLT3.chTrack3Buf2 = magcardTTLT3.chTrack3Buf2 | 0x10;   
	   	
	         
	        if( ( magcardTTLT3.chTrack3Buf1 & 0x1F ) == READ_TRACK23_ENDCODE )   
	        {   
	            
	            if( ( magcardTTLT3.chTrack3Buf2 & 0x1F ) == READ_TRACK23_STARTCODE )   
	            {   
	                magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = magcardTTLT3.chTrack3Buf2;   
	                magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = magcardTTLT3.chTrack3Buf1;   
	                magcardTTLT3.fTrack3Start = TRUE;   
	                magcardTTLT3.fTrack3Direction = TRUE;   
	                magcardTTLT3.fTrack3StartEnd = TRUE;   
	                magcardTTLT3.chTrack3Buf1 = 0x00;   
	            }   
	   
	           //if(  magcardTTLT3.chTrack3Buf2==0x19 )
				else if(  DRV_MagDriveCheck5Bits(magcardTTLT3.chTrack3Buf2 & 0x1F) == 0 )  //奇检验判断正确则证明数据是完整的
	            {   	               								   
	                    magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = magcardTTLT3.chTrack3Buf2;   
	                    //magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = 0x1F;   
						magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = magcardTTLT3.chTrack3Buf1;
	                    magcardTTLT3.fTrack3Start = TRUE;   
	                    magcardTTLT3.fTrack3Direction = FALSE;    
	                    magcardTTLT3.fTrack3StartEnd = FALSE;   
	                    magcardTTLT3.chTrack3Buf1 = 0x00;   
	            }   

	        }   
	   	
	        else if( ( magcardTTLT3.chTrack3Buf2 & 0x1F ) == READ_TRACK23_STARTCODE )   
	        {   
	                magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = magcardTTLT3.chTrack3Buf2;   
	                magcardTTLT3.chTrack3Buffer[magcardTTLT3.chTrack3Index++] = magcardTTLT3.chTrack3Buf1;   
	                magcardTTLT3.fTrack3Start = TRUE;   
	                magcardTTLT3.fTrack3Direction = TRUE;    
	                magcardTTLT3.fTrack3StartEnd = FALSE;   
	                magcardTTLT3.chTrack3Buf1 = 0x00;   
	        }   
	    }  
 	} 

}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(RCP2_MAGCARD_EXTI_LINE) != RESET)
	{
		MagcardRCP2_IRQHandler();
	}	
}


void EXTI2_IRQHandler(void)
{

	if(EXTI_GetITStatus(RCP3_MAGCARD_EXTI_LINE) != RESET)
	{
		MagcardRCP3_IRQHandler();
	} 
}


*/





