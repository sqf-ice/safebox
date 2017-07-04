				   #include <stm32f10x_lib.h>               
#include "TS36EBG.h"
#include "headfile.h"
//#include "../usart/usart.h"
extern void init_usartx(USART_TypeDef* USARTx, unsigned int BaudRate);
extern unsigned int RxData(USART_TypeDef* USARTx, unsigned char* rxBuf,unsigned int rxLen,unsigned int TimeOut);
extern void TxData(USART_TypeDef* USARTx, unsigned char *txBuf, unsigned int txLen);
unsigned char   TS36ebg_UartSendData(unsigned char * buf,unsigned short len );
unsigned char   TS36ebg_UartRecieveData(unsigned char * buf,unsigned short * len );
unsigned char TS36ebg_Transbuf1[395];
extern unsigned char UsbBulkInMsgBuf[2300];
#define   TS36ebg_Transbuf  UsbBulkInMsgBuf 


unsigned char FingerPrintDevice=0;


extern unsigned int waittimeout;



/*
void TS36ebg_UartInit(void)
{
   init_usartx(UART4,9600);

}





unsigned short TS36ebg_ToTransforSequence(unsigned char *  src, unsigned char * des, unsigned short len)
{
   unsigned  short i;

   for(i=0;i<len;i++)
   {
	  des[2*i] = ((src[i]>>4)&0x0F)|0x30;
	  des[2*i+1] = ((src[i])&0x0F)|0x30;
   }

   return (2*len);

}

unsigned short TS36ebg_FromTransforSequence(unsigned char *  src, unsigned char * des, unsigned short len)
{
  unsigned  short i;

   len = len>>1;
   for(i=0;i<len;i++)
   {
	  des[i] = ((src[2*i]<<4)&0xF0)|((src[2*i+1])&0x0F);
   }

   return len;

}



unsigned char TS36ebg_checksum(unsigned char * buf, unsigned short len)
{
		unsigned char i,sum;

		sum = 0;

		for(i=0;i<len;i++)
		{
			 sum = sum + buf[i];						    
		}

		return sum;

}


unsigned char  TS36ebg_IsRightCheckSum(unsigned short  sum, unsigned char * buf, unsigned short len)
{
		  unsigned char tsum;
		  tsum = TS36ebg_checksum(buf,len);

		  if(tsum == sum)
		  {
		    return 0;
		  }
		  else
		  {
		     return 1;
		  
		  }


}

unsigned char   TS36ebg_UartRecieveData(unsigned char * buf,unsigned short * len )
{
	unsigned char res;
	unsigned char Hh,Lh;
	unsigned short tLen;

   //	res = RxData(UART4,TS36ebg_Transbuf,len*2,0x800000);

//	if(res)
//	{
//		return 0xFF;
//	}

//	TS36ebg_FromTransforSequence(TS36ebg_Transbuf,buf,len*2);

//	res = TS36ebg_IsRightCheckSum(buf[len-1],buf,len-1);

//	if(res)
	//{
  //        return 0xFE;	
//	}
																								  h
  //  return 0;
										 

 	res = RxData(UART4,TS36ebg_Transbuf,10,0x800000);

 	if(res)
	{
		return 0xFF;
	}

	 Hh = ((TS36ebg_Transbuf[6]<<4)&0xF0)|(TS36ebg_Transbuf[7]&0x0F);
	 Lh = ((TS36ebg_Transbuf[8]<<4)&0xF0)|(TS36ebg_Transbuf[9]&0x0F);
	 tLen = (Hh<<8)|Lh;

	 res = RxData(UART4,TS36ebg_Transbuf+10,tLen*2,0x800000);

	 if(res)
	 {
		return 0xFF;
	 }

	 TS36ebg_FromTransforSequence(TS36ebg_Transbuf,buf,(tLen+5)*2);

	 res = TS36ebg_IsRightCheckSum(buf[tLen+4],buf,tLen+4);

  	if(res)
	{
          return 0xFE;	
	}

    *len = tLen + 5;

	return 0;

}

unsigned char   TS36ebg_UartSendData(unsigned char * buf,unsigned short len )
{
    unsigned short tlen;

	buf[len-1] = TS36ebg_checksum(buf,len-1);

	tlen = TS36ebg_ToTransforSequence(buf, TS36ebg_Transbuf, len);
	
	TxData(UART4,TS36ebg_Transbuf,tlen);

    return 0;

}





 unsigned char TS36EBG_CONNECT(unsigned  char *buf)
 {
 
 		unsigned char  cmd[7];
		unsigned char  result[8];
		unsigned char res;
		unsigned char i,num;
 
		unsigned short rlen;

 		cmd[0]=0x1b;
		cmd[1]=0x72;
		cmd[2]=0x73;

		cmd[3]=0x00;
		cmd[4]=0x02;
 
 		cmd[5]=0x61;

	//	cmd[6] = TS36ebg_checksum(cmd,6);


	    num=0;

 __RETRYLABLE:

		TS36ebg_UartSendData(cmd,7);

		res = TS36ebg_UartRecieveData(result,&rlen);


        for(i=0;i<8;i++)
		{
		    buf[i]=	result[i];
		
		}

		if(res)
		{
		   if(num<3)
		   {
		      num++;		 
			  goto __RETRYLABLE;
		   }
		   else
		   {
		     return res;
		   }
		}

		return 	  result[6];
 
 }


 unsigned  char TS36EBG_DeviceAppNumber(void)
 {
 

 
   return 0;
 }


unsigned long func_S(char n,unsigned long num)
{
    return (num<<n) | (num>>(32-n));
}

unsigned long func_F(unsigned int t,unsigned long B,unsigned long C,unsigned long D)
{
    if(t<20)
    {
		return ((B & C) | ((~B) & D));
	}else if(t<40)
	{
		return (B ^ C ^ D);
	}else if(t<60)
	{
		return ((B & C) | (B & D) | (C & D));
	}else if(t<80)
	{
		return (B ^ C ^ D);
	}else
	return 0;
}


unsigned long TSFID_SHA_1(unsigned char *mess,unsigned int messlen) 
{
	unsigned long messagelength;	// length of message
	unsigned char i,ctemp;
	unsigned int  index;
	unsigned char hasAdd1=0;
	unsigned char hasAddLen=0;
	unsigned long W[16];
	unsigned long H[5];
	unsigned long A,B,C,D,E,s;
	unsigned long MASK=0x0000000F;
	unsigned long K[4]={0x5A827999,0x6ED9EBA1,0x8F1BBCDC,0xCA62C1D6};
	unsigned long TEMP;
	unsigned int  t;

	messagelength=messlen;
	H[0]=0x67452301;
	H[1]=0xEFCDAB89;
	H[2]=0x98BADCFE;
	H[3]=0x10325476;
	H[4]=0xC3D2E1F0;
	index=0;
	while(!hasAddLen)
	{
		for(t=0;t<16;t++)
		{
			W[t]=0;
			for(i=0;i<4;i++)
			{
				if(!hasAdd1)
				{
					if(index>=messlen)
					{
						ctemp=0x80;
						hasAdd1=1;
					}else
						ctemp=mess[index++];   
				}else
					ctemp=0x00;
				W[t]=W[t]*256+ctemp;
			}
		}
		if(hasAdd1)
			if(W[14]==0x00 && W[15]==0x00)
			{
				W[15]=messagelength;
				for(t=0;t<3;t++)
				{
					if(W[15]&0x80000000)
					W[14]=W[14]*2+1;
					W[15]=W[15]*2;
				}
				hasAddLen=1;
			}
		//==============================//
		A=H[0];B=H[1];C=H[2];D=H[3];E=H[4];
		for(t=0;t<80;t++)
		{
			s=t&MASK;
			if(t>=16)
			{
				W[s]=W[(s+13)&MASK] ^ W[(s+8)&MASK] ^ W[(s+2)&MASK] ^ W[s];
				W[s]=func_S(1,W[s]);
			}
			TEMP=func_S(5,A)+func_F(t,B,C,D)+E+W[s]+K[t/20];
			E=D;D=C;C=func_S(30,B);B=A;A=TEMP;
		}
		H[0]+=A;
		H[1]+=B;
		H[2]+=C;
		H[3]+=D;
		H[4]+=E;
	}
	return H[1];
}


 int CheckMinuate(unsigned char *minuate)
{
	short 	i;
	unsigned char	num;
	unsigned char	temp;
	short	length;

	length=minuate[0];
	length=length*256+minuate[1];
	num=minuate[2];
	if(length!=(num*4+1))
		return 0;
	for(i=0;i<num;i++)
	{
		temp=minuate[3+i*4];
		temp=temp/16;
		if(temp!=1 && temp!=2)
			return 0;
	}
	return 1;
}

int Sub_DisconnectMinu(unsigned char *zwmb,unsigned char TS_dzwmb1[200],unsigned char TS_dzwmb2[100])
{


}
*/

////////////////////////////////////////////////////////////////////////////////////////////////

//	 AS606_FPC1020 的透传代码；
			
extern  void init_usartx_cc(USART_TypeDef* USARTx, unsigned int BaudRate);
void AS606_FPC1020_UartInit(void)
{
   //init_usartx(USART1,57600);
   init_usartx_cc(USART1,57600);
}


unsigned  char  AS606_FPC1020_CheckSum(void)
{


   return 0;
}

extern void TxSendByte(USART_TypeDef *USARTx, unsigned char txData);
unsigned  char  AS606_FPC1020_UartSendData(unsigned char * buf,unsigned int len )
{
   unsigned  int i;

   TxSendByte(USART1,0xFF);

   for(i=0;i<len;i++)
   {
	 TxSendByte(USART1,buf[i]);
   }

	 return 0;
}


unsigned char IsRightPackage(unsigned char *  buf)
{

  	 if((buf[0]==0xef)&&(buf[1]==0x01))
	 {
	    return 1;
	 }
	 else
	 {
	    return 0;
	 }


}


unsigned  char  AS606_FPC1020_UartRecieveData_OnePackage(unsigned char * buf,unsigned int * len )
{
	 unsigned short wlen;
	 unsigned char res;

	 res = RxData(USART1,buf,9,0x800000);

	 if(res)
	 {
		return 0x1;
	 }

	 wlen  = (buf[7]<<8) | buf[8];

	 res = RxData(USART1,buf+9,wlen,0x800000);

  	 if(res)
	 {
		return 0x2;
	 }


	 res = AS606_FPC1020_CheckSum();

	 if(res)
	 {
		return 0x3;
	 }
	
	 *len =	(wlen + 9);

	 return 0;

}

unsigned int 	AS606_FPC1020_length;
unsigned int 	AS606_FPC1020_index;

unsigned  short AS606_FPC1020_UartRecieveData_ContinuePackages(unsigned char * buf,unsigned int * len)
{

		unsigned int length,sumlen;
		unsigned char res;
		unsigned  short num;
		sumlen=0;
		num	 =0;

		  while(1)
		  {
			  
			 res = AS606_FPC1020_UartRecieveData_OnePackage(buf+sumlen,&length);
			 if(res)
			 {
	            break;	 
			 }
			 num++;

			 if(IsRightPackage(buf+sumlen))
			 {
				 if(buf[6+sumlen]==0x08)
			     {
					
				    *len = sumlen+length;
				 	return num;	 
			     }

			 }
			 else
		 	 {
			     return 0;
			 }
		
			 sumlen+=length;
	
		  }
	
	      return 0;

}

unsigned  char	  AS606_FPC1020_GetRecievedPackage(unsigned char * buf, unsigned short numpackage,unsigned short* len)
{

		 unsigned int index;
		 unsigned short length,num;

		 index = 0;
		 num=0;

		 while(1)
		 {
		
		  
		 		  if(IsRightPackage(buf+index))
				  {
				  
				  
				  	  length = (buf[7+index]<<8)|buf[8+index];
				  
				  	  length +=9;
				  
					  if(num==numpackage)
				       {
				  	     *len =  length ;
						 AS606_FPC1020_index = index;
				         return 	 0;
				  
				       }

					  index += length;
					  num++;

				  }
				  else
				  {
				  	   return 1;
				     
				  }
		 
		 
		 
		 }





}


////////////////////////////////////////////////////////////////////////////////////////////////

//	WEL-401AFE-V30 的透传代码；


unsigned short WEL_ToTransforSequence(unsigned char *  src, unsigned char * des, unsigned short len)
{
	 unsigned  short i;

   for(i=0;i<len;i++)
   {
	  des[2*i] = ((src[i]>>4)&0x0F)|0x30;
	  des[2*i+1] = ((src[i])&0x0F)|0x30;
   }

   return (2*len);

}

unsigned short WEL_FromTransforSequence(unsigned char *  src, unsigned char * des, unsigned short len)
{
   unsigned  short i;

   len = len>>1;
   for(i=0;i<len;i++)
   {
	  des[i] = ((src[2*i]<<4)&0xF0)|((src[2*i+1])&0x0F);
   }

   return len;

}




void WEL_Init(void)
{

  init_usartx_c(USART1,9600);
  TxSendByte(USART1,0xFF);

  return;
}


void WEL_Init_Image(void)
{
   init_usartx_c(USART1,115200);
 // init_usartx(USART1,115200);
  TxSendByte(USART1,0xFF);

  return;
}



unsigned char	WEL_SendData(unsigned char * buf,  unsigned int len)
{

   unsigned  int i;
   unsigned short r;
   //unsigned short tlen;
   //TxSendByte(USART1,0xFF)

   for(i=0;i<len;i++)
   {
	 TxSendByte(USART1,buf[i]);
   }

	 return 0;
}

extern unsigned int RxByte_FingerPrint(USART_TypeDef *USARTx, unsigned char *rxData,unsigned int TimeOut);

unsigned char	WEL_RecieveData(unsigned char * buf,  unsigned int len,unsigned char flag)
{

	 unsigned char r;
	 unsigned int i;
	 IWDG_Configuration_FingerPrint();
	// r = RxData(USART1,buf,len,0x1200000);
	// WEL_ToTransforSequence
	 //IWDG_ReloadCounter();

	 for(i=0;i<len;i++)
	 {
	 	 //IWDG_ReloadCounter();
		 if(flag==0)
		 {
		 	  r = RxByte_FingerPrint(USART1,&buf[i],0xFFFFFFFF);
		 }
		 else
		 {
		 
		 	  r = RxByte_FingerPrint(USART1,&buf[i],3000);
		 }
		  if(r!=0)  break;
	 }
     


	 IWDG_Configuration();

	 return r;

}




unsigned char	WEL_TransferData(unsigned char * sbuf, unsigned char * rbuf, unsigned int slen, unsigned int * rlen)
{

			unsigned char r;
			unsigned short sl;

			WEL_SendData(sbuf,slen);
		    r = WEL_RecieveData(rbuf,3,0);

			if(r==1) 
			   return r;

			sl = (rbuf[1]<<8)|rbuf[2];

			r = WEL_RecieveData(rbuf+3,sl+2,0);

			if(r) 
			   return r;

			*rlen = sl+5;

			return 0;

}

 
unsigned  char WEL_CheckFingerExist(void)
{
	  unsigned  int delay;

	while(waittimeout)
	{
	   delay=50000;
	   while(delay--)
	   {
		if(CheckCancleByteReceive())  return 2;
	   }

        TxSendByte(USART1,0x02);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x34);
		TxSendByte(USART1,0x34);
		TxSendByte(USART1,0x33);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x34);
		TxSendByte(USART1,0x37);
		TxSendByte(USART1,0x03);
	

		WEL_RecieveData(TS36ebg_Transbuf,16,1);

		if(TS36ebg_Transbuf[9]==0x30)
		{
			  	
			   delay=50000;
	           while(delay--)
	           {
	         	if(CheckCancleByteReceive())  return 2;
	           }

				return 0;
		
		}
		else 
		{
		
				//WEL_RecieveData(TS36ebg_Transbuf,7);
				//return 1;
		
		}



	}

	return 1;
}


unsigned  char SM_2B_SendFree(void)
{
	    unsigned  int delay;



		TxSendByte(USART1,0x02);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x34);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x35);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x30);
		TxSendByte(USART1,0x31);
		TxSendByte(USART1,0x03);

	    return 1;
}

 



 


