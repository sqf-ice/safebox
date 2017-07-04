#include "globle.h"
#include "bluetooth.h"
#include "../usart/usart.h"	 
//#include "usart.h"
//extern BYTE  gb_apdu_flag;

//extern TAPDU_Data_Struct  gs_cmd;
//extern STRU_CMDHEAD  ga_cmdhead;

extern unsigned int RxData(USART_TypeDef* USARTx, unsigned char* rxBuf,unsigned int rxLen,unsigned int TimeOut);
	
extern void TxData(USART_TypeDef* USARTx, unsigned char *txBuf, unsigned int txLen);

extern unsigned char UsbBulkMsgBuf[271];	 //与USB通信的缓存buf
extern unsigned char UsbBulkInMsgBuf[2300];	 //与USB通信的缓存buf

extern const unsigned short gSW_Table[];
extern void APL_USBMemcpy(unsigned char *  ucDestination, unsigned char * ucSource, unsigned short ucLen);
extern void APL_USBMemset(unsigned char * ucDestination,unsigned char ucVal,unsigned short ucLen) ;
extern void API_Instuction_Hook(void);
unsigned char CommandReceive(void)
{
   
	WORD len = 0;

	UINT8 CHECK[1];

	Receive1:
	CHECK[0]=RxData(UART4,gs_cmd.tmp2usb,3,0x8000);
	if(CHECK[0] != 0)
		{
	//	return 3;
		IWDG_ReloadCounter();
		 goto Receive1;
		  //return 3;
		}
	

    len = MYREG16(&gs_cmd.tmp2usb[1]);
	//UART_ReceiveData(gs_cmd.head,len);

//	Receive2 :
	CHECK[0] = RxData(UART4,gs_cmd.head,len,0x8000);//UART_ReceiveData(gs_cmd.head,len);
	if(CHECK[0] != 0)
		{
		 return 3;
	//	 goto Receive2;
		}
	CHECK[0] = ~(gs_cmd.tmp2usb[1] ^ gs_cmd.tmp2usb[2]);	 //异或
	
	
	if(CHECK[0] != gs_cmd.tmp2usb[0])
	 {
		   
	 return 1;
	 }

	 gs_cmd.ccid_head[1] = (unsigned char)(gs_cmd.len>>8);
	 gs_cmd.ccid_head[0] = (unsigned char)gs_cmd.len;
	 gs_cmd.len = MYREG16(&gs_cmd.ccid_head[0]);
    if (len >= 7)
    {
      
		if(len > 7)
			{
				if ((len - 7) != gs_cmd.len)
			        {
//			          gb_apdu_flag = 2;
			           
			        }
			}

    }
    else
    {
			  
		return 2;
    }

	/*if (gs_cmd.tmp2usb[0] != 'C') 
		  {
			  
			  return 1;
		  }*/
	if((len<7)||(len>512+7+8))
	 {
	 
	  return 2;
	 }							

	APL_USBMemcpy((unsigned char *)(&ga_cmdhead.cla), (unsigned char *)(&gs_cmd.head[0]), 7); //APDU指令头
//	APL_USBMemcpy((unsigned char *)(&gs_cmd.tmp2usb), (unsigned char *)(&gs_cmd.buf[0]), gs_cmd.len);
    APL_USBMemset(gs_cmd.tmp2usb, 0, 10);

//debug	Enable_Interrupts;
	//USIPDR&=0xFC;
    API_Instuction_Hook();
   // ipt_LCD_displaybuf((unsigned char *)"int2",4,1);
	//USIPDR|=0x03;
	return 0;
}

extern  unsigned char API_WriteData(DWORD adwAddr, BYTE * pucBuf, UINT16 ucLen);
extern void API_ReadData(DWORD adwAddr,BYTE *pucBuf,WORD ucLen);

BYTE cmd_F002(void)
{
	 unsigned char temp_buf[26]={0},tt;
	 unsigned long ii=0;
	 unsigned int i=0;
	 if(CMD_P1==0x01)
	 {
		BluesetM();
		return	 SW9000;
	 }
	 else if(CMD_P1==0x00)   
	 {

	   if((CMD_DATA[4]==0x00)&&(CMD_DATA[5]==0x0b))
	   {
	   
	   		 API_WriteData(0x1200,&CMD_DATA[6],CMD_DATA[6]+1);
	   
	   }


	        
	 	for(ii=0;ii<0x1fffff;ii++);
		TxData(UART4,&(CMD_DATA[0]),CMD_P3);
Receive1:
		tt= RxData(UART4,temp_buf,7,0xf000);
	//	if(tt!= 0)
		{
	//	return 3;
	//	 goto Receive1;
		}
	
//		UART_ReceiveData(CMD_DATA,7);
			
		if(temp_buf[6]==0) return SW9000;
		else return SW6700;
	 }
	 else if(CMD_P1==0x02)
	 {
		 BluesetM_end();
		 return	 SW9000;
	 }
	 else if(CMD_P1==0x03)
	 {
		BluesetM();
		for(ii=0;ii<0xfffff;ii++);
		TxData(UART4,&(CMD_DATA[0]),CMD_P3);

		tt= RxData(UART4,temp_buf,26,0xf000);
	//	for(ii=0;ii<0xffff;ii++);
		if(temp_buf[6]==0) 
		{
			gb_Le = temp_buf[9];
		    for(i=0;i<gb_Le;i++)
		   {
		
		    	CMD_DATA[i]=temp_buf[10+i];				
			
					  
		   }
			return SW9000;
		}
	   	else if(CMD_P1==0x04)
		{
		
			  API_ReadData(0x1200,&CMD_DATA[0],1);
			  tt= CMD_DATA[0];
			  API_ReadData(0x1200,&CMD_DATA[0],tt+1);
			  gb_Le = 	tt+1 ;
			  return  SW9000;
		
		}
		else
		{
			return SW6700;
		}
	 }
}

void MakeDataback(void)
{

	 UINT16 SW12;
	 UINT16 len;
	 BYTE sw;
	// RC4_KEY xdata rc4_key;
	// USIPDR= 0x01;
//debug	  Disable_Interrupts;//
	 sw = g_bSW;
	 
	 SW12 = gSW_Table[sw];
	 
	 if (sw == SW6CXX)
	 {
		 SW12  |= gb_6c ; 
	 }
	 if (sw == SW63CX)
	 {
		 SW12  |= gb_63 ;
	 }

	 if (sw == SW_ATR)
	 {
		 gs_cmd.head[4] = 'R';
		 gs_cmd.head[1] = (unsigned char)(gb_Le>>8);
		 gs_cmd.head[0] = (unsigned char)gb_Le;
		 
		 gs_cmd.len = MYREG16(&gs_cmd.head[0]);//gb_Le;
		 len = gb_Le + 3;  



		 TxData(UART4,&(gs_cmd.head[4]),len);
		  
		  
	 }
	 else  
	 {
		 if (gb_Le == 0)
		 {
			 gs_cmd.head[0] = 'R';
			 gs_cmd.head[1] = 0;
			 gs_cmd.head[2] = 2;
			 gs_cmd.head[3] = (BYTE)(SW12 >> 8);
			 gs_cmd.head[4] = (BYTE)SW12;
			 len = gb_Le + 5;
			
			 TxData(UART4,&(gs_cmd.head[0]),len);
			 
			 
		 }
		 else
		 { 
			//ipt_LCD_displaybuf((unsigned char *)"int2",4,3);
			 len = gb_Le + 2 ;
			 gs_cmd.head[4] = 'R';
			 gs_cmd.head[1] = (unsigned char)(len>>8);
		     gs_cmd.head[0] = (unsigned char)len;
		//	 gs_cmd.len = len;
		     gs_cmd.len = MYREG16(&gs_cmd.head[0]);//len;
			 CMD_DATA[gb_Le] = (SW12 >> 8);//0x90;//
			 CMD_DATA[gb_Le + 1] = (BYTE)SW12;//0x00;//
			 len = gb_Le + 5;
			 
			 TxData(UART4,&(gs_cmd.head[4]),len);
			 
	 
	 
			/* //增加RC4加密，DINGMY, 2012.06.01
			 if (((gb_clc & 0xF0) == 0x10)&&(!gb_rc4_flag)) //生命周期0x1X	gb_rc4_flag=1当取复位信息时不需要加密
			 {
				 APL_ALG_RC4_SetKey(&rc4_key, ga_rc4key, 8);
				 APL_ALG_RC4(&rc4_key, CMD_DATA, gb_Le, CMD_DATA);
			 }
			 gb_rc4_flag=0;*/
		 }
		 
	 }
	  gb_Le = 0; 
//debug	  Enable_Interrupts;
	 	// UART_SendData_au(&(gs_cmd.head[4]),len);

}
extern void TxByte(USART_TypeDef *USARTx, unsigned char *txData);


void MakeDataback_Error(unsigned char flag)
{

//	 UINT16 SW12;
	 
//debug	Disable_Interrupts;//


	 if(flag == 1)
	 	{

	//	SW12 = 0x6a80;			
		gs_cmd.head[0] = 'R';
		gs_cmd.head[1] = 0;
		gs_cmd.head[2] = 2;
		gs_cmd.head[3] = 0x6a;
		gs_cmd.head[4] = 0x80;
	   // delay(1000);

	 	}
	 else if(flag == 2)
	 	{
	 //	SW12 = 0x6700;				
		gs_cmd.head[0] = 'R';
		gs_cmd.head[1] = 0;
		gs_cmd.head[2] = 2;
		gs_cmd.head[3] = 0x67;
		gs_cmd.head[4] = 0x00;
	    
	 	}
	 else if(flag == 3)
	 	{
//	 	SW12 = 0x6901;				
		gs_cmd.head[0] = 'R';
		gs_cmd.head[1] = 0;
		gs_cmd.head[2] = 2;
		gs_cmd.head[3] = 0x69;
		gs_cmd.head[4] = 0x01;
	 	}
		else if(flag == 4)
	 	{
//	 	SW12 = 0x6901;				
		gs_cmd.head[0] = 'R';
		gs_cmd.head[1] = 0;
		gs_cmd.head[2] = 2;
		gs_cmd.head[3] = 0x69;
		gs_cmd.head[4] = 0x84;
	 	}

	TxData(UART4,&(gs_cmd.head[0]),5);
//debug	 Enable_Interrupts;
	 	

}


