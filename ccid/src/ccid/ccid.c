/*===============================================================================//
//== �� �� ����ccid.c
//== author	 ��huyx@cashwaytech.com
//== ˵    ����STM32F103VC8ƽ̨ ccid ����
//== �����������޹�˾
//== ����˵����
//===============================================================================*/
#include "ccid.h"
#include "IFD.h"

#include "usb_endp.h"
//#include "function.h"	//== ����ƽ̨��������������
#include "usb_lib.h"


unsigned char UsbBulkMsgBuf[271];	 //��USBͨ�ŵĻ���buf
unsigned char UsbBulkInMsgBuf[2300];	 //��USBͨ�ŵĻ���buf
unsigned char Iso7816CurParam=0;    //Ӧ�����յ�ATR��ʱ�����ø�ֵ	  
extern unsigned char RC_flag;
unsigned char power_flag=0;
unsigned char bTransactionType;
extern void  beep_con(void);
ISO7816_param_T01_t IfdParameters;
void CCID_AnalyseBulkMessage(void);
void CCID_AnalyseBulkMessage1(void);
extern void close_time5_isr(void);
extern void open_time5_isr(void);
unsigned char PC_to_RDR_ResetParameters(void);
unsigned char PC_to_RDR_GetParameters(void);
unsigned char ISO7816_SetParameters(unsigned char *pParamBuf,unsigned char T01);
unsigned char IFD_GetParameters(unsigned char *pParamBuf);
unsigned char ISO7816_ApplyParameters(void);
unsigned char  PC_to_RDR_IccPowerOn_SAM(void);
unsigned char PC_to_RDR_IccPowerOff_SAM(void);
unsigned char PC_to_RDR_XfrBlock_SAM(void);
void memcpy(unsigned char *str,unsigned char *ptr,unsigned char len);
extern unsigned char ISO14443_PowerOn(unsigned char *Atr,unsigned char *Atr_len);
extern void process_usart_tmsg(USART_TypeDef* USARTx,unsigned int len);
unsigned char PPS_Exchange(unsigned char *p_req_buf, unsigned int w_req_len,unsigned char *p_resp_buf,unsigned int *p_w_resp_len);
//unsigned int T1ApduCommand(unsigned char* sbuf, unsigned int slen, unsigned char* rbuf, unsigned int* rlen);
unsigned int T0ApduCommand(unsigned char* sbuf, unsigned int slen, unsigned char* rbuf, unsigned int* rlen);
unsigned char ISO7816_XfrBlock(unsigned char *pTxBlockBuf,unsigned int wTxBlcokLen,unsigned char *pRxBlockBuf,unsigned int *wRxBlcokLen,
                       unsigned int wLevelParameter);
#define Fi_RFU 		372						//==����δ��ʹ��
#define Di_RFU 		1						//==����δ��ʹ�� 
volatile unsigned int f_Table[16]={372,	372,	558,	744,	1116,	1488,	1860,	Fi_RFU,\
					 		 	   Fi_RFU,512,	768,	1024,	1536,	2048,	Fi_RFU,	Fi_RFU}; //==ʱ����ת������
volatile unsigned int d_Table[16]={Di_RFU,1,		2,		4,		8,		16,		32,		Di_RFU,\
					 		       12,	20,		Di_RFU,	Di_RFU,	Di_RFU,	Di_RFU,	Di_RFU,	Di_RFU}; //==������У������

unsigned char test_atr[]={0x3b,0x98,0x95,0x40,0xf0,0x2e,0x6e,0x65,0x74,0x56,0x07,0xff,0xee};							   

extern unsigned char num_SN;

extern unsigned char beep_flag;
unsigned char ICC_in=0,solt_flag=0;
unsigned char PC_to_RDR_Escape(void)
{
	unsigned char  bErrorCode;

	if((UsbBulkMsgBuf[0x0a] ==0xff)&&(UsbBulkMsgBuf[0x0b] ==0x69))
	{
		 if((UsbBulkMsgBuf[0x17] ==0x03)&&(UsbBulkMsgBuf[0x19] ==0x03))
		 {
			 if(UsbBulkMsgBuf[0x18] ==0x00)	    
			 {
				 if(UsbBulkMsgBuf[0x20] ==0x00)	  //DIS
				 {
					  beep_flag =0;
					  bErrorCode = 0x81;
				 }
				 else if(UsbBulkMsgBuf[0x20] ==0x01)   //EN 
				 {
					 beep_flag =1;
					 bErrorCode = 0x81;
				 }
			 }
			 else if(UsbBulkMsgBuf[0x18] ==0x01)   //����ʱ��
			 {
				
			 }
		 }
	}
	else if((UsbBulkMsgBuf[0x0a] ==0x68)&&(UsbBulkMsgBuf[0x0b] ==0x92))
	{
		solt_flag =1;
		if(UsbBulkMsgBuf[0x0c] ==0x04)
		{
			if(0==GPIO_ReadInputDataBit(SLOT_ICC1_IN_GPIO,SLOT_ICC_IN_PIN))
			{
				 ICC_in =0x01;
			}
			else if(0==GPIO_ReadInputDataBit(SLOT_ICC2_IN_GPIO,SLOT_ICC_IN_PIN))
			{
				  ICC_in =0x02;
			}
			else
				  ICC_in =0x00;

		}
	}
	else
	{
		 bErrorCode = 0xfe;
	}
	return bErrorCode; 
}



void RDR_to_PC_Escape(unsigned char err)
{
	UsbBulkInMsgBuf[0] = 0x83;
	UsbBulkInMsgBuf[6] = UsbBulkMsgBuf[6];
	//UsbBulkInMsgBuf[7] = IFD_Slot_GetStatus();
	UsbBulkInMsgBuf[8] = 0x00;
	if(UsbBulkInMsgBuf[6]==0)
		UsbBulkInMsgBuf[6]=	num_SN;
	if(solt_flag==1)
	{
		if(err != SLOT_NO_ERROR)
		{
			UsbBulkInMsgBuf[1] = 0x04;
			UsbBulkInMsgBuf[2] = 0x00;
			UsbBulkInMsgBuf[10] = 0x00;
			UsbBulkInMsgBuf[11] = 0x00;
			UsbBulkInMsgBuf[12] = 0x63;
			UsbBulkInMsgBuf[13] = 0x00;
		}
		else
		{
			UsbBulkInMsgBuf[1] = 0x04;
			UsbBulkInMsgBuf[2] = 0x00;
			UsbBulkInMsgBuf[10] = 0x00;
			UsbBulkInMsgBuf[11] = ICC_in;
			UsbBulkInMsgBuf[10] = 0x90;
			UsbBulkInMsgBuf[11] = 0x00;
		}
	}
	else 
	{
		if(err != SLOT_NO_ERROR)
		{
			UsbBulkInMsgBuf[1] = 0x02;
			UsbBulkInMsgBuf[2] = 0x00;
			UsbBulkInMsgBuf[10] = 0x63;
			UsbBulkInMsgBuf[11] = 0x00;
		}
		else
		{
			UsbBulkInMsgBuf[1] = 0x02;
			UsbBulkInMsgBuf[2] = 0x00;
			UsbBulkInMsgBuf[10] = 0x90;
			UsbBulkInMsgBuf[11] = 0x00;
		}
	}
	UsbBulkInMsgBuf[9] = 0x00;

	return ;
}


//**************************************************************************
//;****�������ƣ�CCID_AnalyseBulkMessage
//****��ڲ�������
//****���ڲ�������
//****ȫ�ֱ�����
//****����ģ�飺
//****˵    ��: CCID BULK���ݷ���
//**************************************************************************/
void CCID_AnalyseBulkMessage(void)
{
	unsigned char bErrorCode;

//	unsigned int wTempBulkLen;    //ͨ��USB���͵����ݵĳ���
	bBulkOutCompeleteFlag=1;
	/*  �ж�BULK OUT�����Ƿ�������    */
	if(bBulkOutCompeleteFlag==1)
	{
		close_time5_isr();
		switch(UsbBulkMsgBuf[0])
		{
			case PC_TO_RDR_ICC_POWERON:
				bErrorCode = PC_to_RDR_IccPowerOn();
				RDR_to_PC_DataBlock(bErrorCode);
				break;
			case PC_TO_RDR_ICC_POWEROFF:
				bErrorCode = PC_to_RDR_IccPowerOff();
				RDR_to_PC_SlotStatus(bErrorCode);
				break;
			case PC_TO_RDR_GET_SLOTSTATUS:
				RDR_to_PC_SlotStatus(0);
				break;
			case PC_TO_RDR_XFR_BLOCK:
				bErrorCode = PC_to_RDR_XfrBlock();
				RDR_to_PC_DataBlock(bErrorCode);
				break;
			case PC_TO_RDR_SET_PARAMETERS:
				bErrorCode = PC_to_RDR_SetParameters();
				RDR_to_PC_Parameters(bErrorCode);
				break;
			case PC_TO_RDR_GETPARAMETERS:
	            bErrorCode = PC_to_RDR_GetParameters();
	            RDR_to_PC_Parameters(bErrorCode);
	            break;
	        case PC_TO_RDR_RESETPARAMETERS:
	            bErrorCode = PC_to_RDR_ResetParameters();
	            RDR_to_PC_Parameters(bErrorCode);
	            break;
			case PC_TO_RDR_ESCAPE:
				bErrorCode = PC_to_RDR_Escape();
			    RDR_to_PC_Escape(bErrorCode);
				break;
			default:
				/*  ֡����  */
				CCID_CmdNotSupport();
				break;
		}

		///////���ͺ���/////////////////////////////////////////////////////////////////
/*		wTempBulkLen  = 10 +(UsbBulkInMsgBuf[2]<<8)+UsbBulkInMsgBuf[1];
		if(send_flag==0)
			USB_SendData(0x02,UsbBulkInMsgBuf,wTempBulkLen);
		else
		{
		    send_flag=0;
			USB_SendData(0x04,UsbBulkInMsgBuf,wTempBulkLen);
		}
//		open_time5_isr();
		Clr_bBulkOutCompeleteFlag;
	}

	if(bBulkInCompeleteFlag)
	{
		Clr_bBulkInCompeleteFlag;
	}	*/
	}
}

void CCID_AnalyseBulkMessage1(void)
{
	unsigned char bErrorCode;

//	unsigned int wTempBulkLen;    //ͨ��USB���͵����ݵĳ���
	bBulkOutCompeleteFlag=1;
	/*  �ж�BULK OUT�����Ƿ�������    */
	if(bBulkOutCompeleteFlag==1)
	{
		close_time5_isr();
		switch(UsbBulkMsgBuf[0])
		{
			case PC_TO_RDR_ICC_POWERON:
				bErrorCode = PC_to_RDR_IccPowerOn_SAM();
				RDR_to_PC_DataBlock(bErrorCode);
				break;
			case PC_TO_RDR_ICC_POWEROFF:
				bErrorCode = PC_to_RDR_IccPowerOff_SAM();
				RDR_to_PC_SlotStatus(bErrorCode);
				break;
			case PC_TO_RDR_XFR_BLOCK:
				bErrorCode = PC_to_RDR_XfrBlock_SAM();
				RDR_to_PC_DataBlock(bErrorCode);
				break;

			default:
				/*  ֡����  */
				CCID_CmdNotSupport();
				break;
		}


	}
}


///**************************************************************************
//;****�������ƣ�PC_to_RDR_IccPowerOn
//;****��ڲ�������
//;****���ڲ���������״̬
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: �ϵ�
//;**************************************************************************/
extern unsigned char ifd_icc_active_sate;
unsigned char  PC_to_RDR_IccPowerOn(void)
{
	unsigned char i, bErrorCode,temp,bMis=0;

	unsigned char wRBulk_len=0;
	unsigned long timeout1=0;

	if(slot1==0)
	{
		temp=0;
		for(i=0;i<10;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[10] =temp;
		process_usart_tmsg(USART2,11);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>9800000)	
			{
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			beep_con();
			return bErrorCode;
		}	
	}
	else
	   {
	    //	close_time5_isr();
			bErrorCode=ISO14443_PowerOn(UsbBulkInMsgBuf+0x0a ,&wRBulk_len);
		//	open_time5_isr();
		
			if((bErrorCode==0)||(bErrorCode==SLOT_NO_ERROR))
			{
				beep_con();
				ifd_icc_active_sate =ICC_PRESENT_ACTIVE;
				UsbBulkInMsgBuf[2] = 0;
				UsbBulkInMsgBuf[1] = wRBulk_len&0xff;
			}
			else
			{
				power_flag=1;
				return 	bErrorCode;
			}
		}
   return SLOT_NO_ERROR;
}

unsigned char  PC_to_RDR_IccPowerOn_SAM(void)
{
	unsigned char i, bErrorCode,temp,bMis=0;

	unsigned char wRBulk_len=0;
	unsigned long timeout1=0;

	if(slot1==2)
	{
		temp=0;
		for(i=0;i<10;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[10] =temp;
		process_usart_tmsg(USART2,11);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>9800000)	
			{
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			beep_con();
			return bErrorCode;
		}	
	}
	else
	   {
		   	temp=0;
		for(i=0;i<10;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[10] =temp;
		process_usart_tmsg(USART2,11);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>800000)	
			{
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			beep_con();
			return bErrorCode;
		}	
		}
   return SLOT_NO_ERROR;
}
///**************************************************************************
//;****�������ƣ�PC_to_RDR_IccPowerOff
//;****��ڲ�������
//;****���ڲ���������״̬
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: �µ�
//;**************************************************************************/
unsigned char PC_to_RDR_IccPowerOff(void)
{
    unsigned char bErrorCode=0,temp,i,bMis=0; 
	unsigned long timeout1=0;
	if(slot1==0)
	{
		temp=0;
		for(i=0;i<10;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[10] =temp;
		process_usart_tmsg(USART2,11);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>800000)
			{	
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			return bErrorCode;
		}	
	}
	else
		return SLOT_NO_ERROR;
}


unsigned char PC_to_RDR_IccPowerOff_SAM(void)
{
    unsigned char bErrorCode=0,temp,i,bMis=0; 
	unsigned long timeout1=0;
	if(slot1==2)
	{
		temp=0;
		for(i=0;i<10;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[10] =temp;
		process_usart_tmsg(USART2,11);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>800000)
			{	
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			return bErrorCode;
		}	
	}
	else
	{
	 		temp=0;
		for(i=0;i<10;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[10] =temp;
		process_usart_tmsg(USART2,11);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>800000)
			{	
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			return bErrorCode;
		}
	}
		return SLOT_NO_ERROR;
}
/**************************************************************************
;****�������ƣ�PC_to_RDR_GetParameters
;****��ڲ�������
;****���ڲ���������״̬
;****ȫ�ֱ�����
;****����ģ�飺
;****˵    ��: ��ò���
;**************************************************************************/
unsigned char PC_to_RDR_GetParameters(void)
{
     /*  У�����    */
     if(UsbBulkMsgBuf[5] != 0)
     {
         return ERROR_BAD_BSLOT;                             //���ؿ��۱�Ŵ���
     }

     if(     (UsbBulkMsgBuf[1] != 0)   ||  (UsbBulkMsgBuf[2] != 0)   ||
             (UsbBulkMsgBuf[3] != 0)   ||  (UsbBulkMsgBuf[4] != 0)   )
     {
         return ERROR_BAD_DWLENGTH;                          //���س��������
     }

     if((UsbBulkMsgBuf[7] != 0)   ||  (UsbBulkMsgBuf[8] != 0)   ||	(UsbBulkMsgBuf[9] != 0))
     {
         return ERROR_BAD_ABRFU_3;                           //���ر����ֽڴ���
     }

     return SLOT_NO_ERROR;
}

///**************************************************************************
//;****�������ƣ�PC_to_RDR_ResetParameters
//;****��ڲ�������
//;****���ڲ���������״̬
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: ������λ
//;**************************************************************************/
unsigned char PC_to_RDR_ResetParameters(void)
{
  	unsigned char bErrorCode;


    /*  У�����    */
    if(UsbBulkMsgBuf[5] != 0)
    {
        return ERROR_BAD_BSLOT;                             //���ؿ��۱�Ŵ���
    }

    if( (UsbBulkMsgBuf[1] != 0)   ||  (UsbBulkMsgBuf[2] != 0)   ||
        (UsbBulkMsgBuf[3] != 0)   ||  (UsbBulkMsgBuf[4] != 0)   )
    {
        return ERROR_BAD_DWLENGTH;                   //���س��������
    }

    if((UsbBulkMsgBuf[7] != 0) || (UsbBulkMsgBuf[8] != 0) ||(UsbBulkMsgBuf[9] != 0))
    {
        return ERROR_BAD_ABRFU_3;                           //���ر����ֽڴ���
    }
  
    UsbBulkMsgBuf[0x0a] = 0x11;			     //����ΪĬ��ֵ
    UsbBulkMsgBuf[0x0b] = 0;
    UsbBulkMsgBuf[0x0c] = 0;
    UsbBulkMsgBuf[0x0d] = 0x0a;
    UsbBulkMsgBuf[0x0e] = 0x03;
    UsbBulkMsgBuf[0x0f] = 0x20;
    UsbBulkMsgBuf[0x10] = 0;

    /*  ����IFD���� */
    bErrorCode = ISO7816_SetParameters(UsbBulkMsgBuf + 0x0a,0);

    if(bErrorCode != SLOT_NO_ERROR)
    {
        return bErrorCode;
    }

    return bErrorCode;
}
///**************************************************************************
//;****�������ƣ�PC_to_RDR_SetParameters
//;****��ڲ�������
//;****���ڲ���������״̬
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: ��������
//;**************************************************************************/
unsigned char PC_to_RDR_SetParameters(void)
{
	unsigned char bErrorCode,len,i;

	/*  У��dwLength��  */
	if (    (UsbBulkMsgBuf[2] != 0)	||
			(UsbBulkMsgBuf[3] != 0)	||
			(UsbBulkMsgBuf[4] != 0)	||
			((UsbBulkMsgBuf[1] != 5) &&  (UsbBulkMsgBuf[1] != 7) ) )

	{
		return ERROR_BAD_DWLENGTH;
	}

	/*  �жϱ��������Ƿ���ȷ    */
	if (    (UsbBulkMsgBuf[8] != 0)	||(UsbBulkMsgBuf[9] != 0)    )
	{
		return ERROR_BAD_ABRFU_2;
	}

	/*  �ж�bProtocolNum    */
	if (    ((UsbBulkMsgBuf[1] == 5)     &&  (UsbBulkMsgBuf[7] != 0x00))   ||
			((UsbBulkMsgBuf[1] == 7)     &&  (UsbBulkMsgBuf[7] != 0x01))    )

	{
		return ERROR_BAD_BPROTOCOLNUM;
	}

	/*	IFD ���ò���	*/
	if(slot1==0)
		bErrorCode = ISO7816_SetParameters(UsbBulkMsgBuf +0x0a,UsbBulkMsgBuf[7]);
	else
	{
		len= UsbBulkMsgBuf[1];
		for(i=0;i<len;i++)
			UsbBulkInMsgBuf[10+i]= UsbBulkMsgBuf[10+i];
		bErrorCode = SLOT_NO_ERROR;
	}
	/*  �жϲ������ò����Ƿ�ɹ�    */
	if(bErrorCode != SLOT_NO_ERROR)
	{
		return bErrorCode;
	}
   return bErrorCode;
}
//**************************************************************************
//****�������ƣ�PC_to_RDR_XfrBlock
//****��ڲ�������
//****���ڲ���������״̬
//****ȫ�ֱ�����
//****����ģ�飺
//****˵    ��: ���ݽ���
//**************************************************************************/
extern unsigned int  ISO14443Layer4TransceiveApdu(unsigned char *ApduBuf , unsigned char * ResponseBuf , unsigned char len);
extern unsigned char  card_type;
extern unsigned char  CID_flag1;



unsigned char Test_ICCReady(void)
{
	//	unsigned long timeout1=0;
	//	UsbBulkMsgBuf[0]=0x65;
	//	UsbBulkMsgBuf[1]=0;  
	//	UsbBulkMsgBuf[2]=0;  
	//	UsbBulkMsgBuf[3]=0;  
	//	UsbBulkMsgBuf[4]=0;  
	//	UsbBulkMsgBuf[5]=0;  
	//	UsbBulkMsgBuf[6]=0;  
	//	UsbBulkMsgBuf[7]=0;
	//	UsbBulkMsgBuf[8]=0; 
	//	UsbBulkMsgBuf[9]=0; 
	//	UsbBulkMsgBuf[10]=65; 
		  
	//	init_usartx(USART2,38400); 

	 //   RC_flag=0;
	//   	process_usart_tmsg(USART2,11);
	//  	while(!RC_flag)
	//	{
	//		timeout1++;
	//		if(timeout1>1600000)	
	//		{
	//		  return 1;
	//		}
		
	//	}

		return 0;
}





unsigned char PC_to_RDR_XfrBlock(void)
{
	unsigned char bErrorCode=0,temp,bMis=0;
	unsigned long timeout1=0;													
	unsigned short RxBlockSize,wBlockSize;
	/*  У��dwLength������  */
	unsigned int wBlockSize1,i,ret;
//	unsigned char i;
	if ((UsbBulkMsgBuf[3] != 0)||(UsbBulkMsgBuf[4] != 0))
	{
		return ERROR_BAD_DWLENGTH;	         
	}

	/*  ��ICC��������   */
	wBlockSize1 = (UsbBulkMsgBuf[2]<<8)+UsbBulkMsgBuf[1]+10;
	wBlockSize=	UsbBulkMsgBuf[1];
//	wLevelParam = (UsbBulkMsgBuf[9]<<8)+UsbBulkMsgBuf[8];
	if(slot1==0)
	{

//	if((UsbBulkMsgBuf[11]==0xb2)&&(UsbBulkMsgBuf[12]==0x03)&&(UsbBulkMsgBuf[13]==0x14)&&((UsbBulkMsgBuf[14]==0xfe)))
//	{
//		 timeout1=0;
//	
//	}
		if(UsbBulkMsgBuf[11]==0xc1)
		{
			for(i=0;i<15;i++)
				UsbBulkInMsgBuf[i]=	UsbBulkMsgBuf[i];
			UsbBulkInMsgBuf[11]=0xe1;
			temp=0;
			temp = temp ^ UsbBulkInMsgBuf[11];
			temp = temp ^ UsbBulkInMsgBuf[12];
			temp = temp ^ UsbBulkInMsgBuf[13];
			UsbBulkInMsgBuf[14]=temp;
			return SLOT_NO_ERROR;
		}
		temp=0;
		for(i=0;i<wBlockSize1;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[wBlockSize1] =temp;
		process_usart_tmsg(USART2,wBlockSize1+1);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>96000000)	
			{
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			beep_con();
			return bErrorCode;
		}	
	//	UsbBulkInMsgBuf[10] =0x6a;
	//	UsbBulkInMsgBuf[11] =0x82;
	//	RxBlockSize = 2;
	}
	else
	{
		if((card_type==0xff)||(card_type==0xfe))
		{
			if((UsbBulkMsgBuf[10]==0xff)&&(UsbBulkMsgBuf[11]==0xff))
			{
//				 init_sam_v();
				 ret = read_id_info(UsbBulkInMsgBuf,&wBlockSize1);
				 if(ret == 0)
				 {
					UsbBulkInMsgBuf[2] = (wBlockSize1>>8)&0xff;
					UsbBulkInMsgBuf[1] = wBlockSize1&0xff;
					return SLOT_NO_ERROR;
				 }
				 else
				 {
					UsbBulkInMsgBuf[2] = 0;
					UsbBulkInMsgBuf[1] = 0x02;
					UsbBulkInMsgBuf[10] = 0x69;
					UsbBulkInMsgBuf[11] = 0x83;
					return SLOT_NO_ERROR;
				 }
				 
			}
			 else
			 {
				UsbBulkInMsgBuf[10]=0x6a;
				UsbBulkInMsgBuf[11]=0x82;
				RxBlockSize =2;
				bErrorCode =SLOT_NO_ERROR;
			}
		}
		else
		{




			RxBlockSize = ISO14443_sendAPDU(0,UsbBulkMsgBuf + 0x0a,wBlockSize,UsbBulkInMsgBuf+0x0a); 
			beep_con();
			if(RxBlockSize<0x8000)
				bErrorCode =SLOT_NO_ERROR;
			else
				bErrorCode =1;
		}
	   	
//		open_time5_isr();
	}

	if(bErrorCode!=SLOT_NO_ERROR)
	{
	   UsbBulkInMsgBuf[1] =2;
	   UsbBulkInMsgBuf[10] =0x63;
	   UsbBulkInMsgBuf[11] =0;
	   return SLOT_NO_ERROR;
	}
	UsbBulkInMsgBuf[2] = (RxBlockSize>>8)&0xff;
	UsbBulkInMsgBuf[1] = RxBlockSize&0xff;
	return bErrorCode;
}

unsigned char PC_to_RDR_XfrBlock_SAM(void)
{
	unsigned char bErrorCode=0,temp,bMis=0;
	unsigned long timeout1=0;													
	unsigned char RxBlockSize;
	/*  У��dwLength������  */
	unsigned int wBlockSize1,i,ret;
//	unsigned char i;
	if ((UsbBulkMsgBuf[3] != 0)||(UsbBulkMsgBuf[4] != 0))
	{
		return ERROR_BAD_DWLENGTH;	         
	}

	/*  ��ICC��������   */
	wBlockSize1 = (UsbBulkMsgBuf[2]<<8)+UsbBulkMsgBuf[1]+10;
//	wLevelParam = (UsbBulkMsgBuf[9]<<8)+UsbBulkMsgBuf[8];
	if(slot1==0)
	{
		temp=0;
		for(i=0;i<wBlockSize1;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[wBlockSize1] =temp;
		process_usart_tmsg(USART2,wBlockSize1+1);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>9800000)	
			{
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			beep_con();
			return bErrorCode;
		}	
	}
	else
	{
		
		temp=0;
		for(i=0;i<wBlockSize1;i++)
		{
		  	temp = temp ^ UsbBulkMsgBuf[i];
		}
		UsbBulkMsgBuf[wBlockSize1] =temp;
		process_usart_tmsg(USART2,wBlockSize1+1);
		while(!RC_flag)
		{
			timeout1++;
			if(timeout1>9800000)	
			{
				bMis =1;
				break;
			}
			bErrorCode=SLOT_NO_ERROR;
		}
		RC_flag =0;
		if(bMis != 0)
		{
			return 1;
		}else
		{
			beep_con();
			return bErrorCode;
		}	
	}

	if(bErrorCode!=SLOT_NO_ERROR)
	{
	   return bErrorCode;
	}
//	UsbBulkInMsgBuf[2] = (RxBlockSize>>8)&0xff;
	UsbBulkInMsgBuf[1] = RxBlockSize&0xff;
	return bErrorCode;
}


///**************************************************************************
//;****�������ƣ�CCID_CmdNotSupport
//;****��ڲ�������
//;****���ڲ�������
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: ��֧�ֵ�CCID����Ϣ�Ĵ���
//;**************************************************************************/
void CCID_CmdNotSupport(void)
{
	unsigned char i;
	for(i=0;i<10;i++)
		UsbBulkInMsgBuf[i] = UsbBulkMsgBuf[i];
	UsbBulkInMsgBuf[0] = RDR_TO_PC_SLOTSTATUS;
  	UsbBulkInMsgBuf[1] = 0x00;
	UsbBulkInMsgBuf[2] = 0x00;
	UsbBulkInMsgBuf[3] = 0x00;
	UsbBulkInMsgBuf[4] = 0x00;

	UsbBulkInMsgBuf[8] = 0x00;
	UsbBulkInMsgBuf[9] = 0x00;
}

///**************************************************************************
//;****�������ƣ�RDR_to_PC_DataBlock
//;****��ڲ������������
//;****���ڲ�������
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: CCID ���ݽ�����Ӧ��
//;**************************************************************************/

void RDR_to_PC_DataBlock(unsigned char err)
{
	
   	UsbBulkInMsgBuf[0] = RDR_TO_PC_DATABLOCK;
	UsbBulkInMsgBuf[6] = UsbBulkMsgBuf[6];
	//UsbBulkInMsgBuf[7] = IFD_Slot_GetStatus();
	UsbBulkInMsgBuf[8] = 0x00;
	if(UsbBulkInMsgBuf[6]==0)
		UsbBulkInMsgBuf[6]=	num_SN;
	if(err != SLOT_NO_ERROR)
	{
		UsbBulkInMsgBuf[7] += 0x40;
		UsbBulkInMsgBuf[1] = 0x00;
		UsbBulkInMsgBuf[2] = 0x00;
		UsbBulkInMsgBuf[3] = 0x00;
		UsbBulkInMsgBuf[4] = 0x00;
		UsbBulkInMsgBuf[8] = err;
	}
	UsbBulkInMsgBuf[9] = 0x00;

	return ;
}
///**************************************************************************
//;****�������ƣ�RDR_to_PC_SlotStatus
//;****��ڲ������������
//;****���ڲ�������
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: ��ȡ���۵�Ӧ��
//;**************************************************************************/
void RDR_to_PC_SlotStatus(unsigned char err)
{
	UsbBulkInMsgBuf[0] = RDR_TO_PC_SLOTSTATUS;
	UsbBulkInMsgBuf[1] = 0x00;
	UsbBulkInMsgBuf[2] = 0x00;
	UsbBulkInMsgBuf[3] = 0x00;
	UsbBulkInMsgBuf[4] = 0x00;
	UsbBulkInMsgBuf[6] = UsbBulkMsgBuf[6];
	if(slot1==0)
		UsbBulkInMsgBuf[7] = IFD_Slot_GetStatus();
	else
	    UsbBulkInMsgBuf[7] = 0x01;
	UsbBulkInMsgBuf[8] = 0x00;
	if(err != SLOT_NO_ERROR)
	{
		UsbBulkInMsgBuf[7] += 0x40;
		UsbBulkInMsgBuf[1] = 0x00;
		UsbBulkInMsgBuf[2] = 0x00;
		UsbBulkInMsgBuf[3] = 0x00;
		UsbBulkInMsgBuf[4] = 0x00;
		UsbBulkInMsgBuf[8] = err;
	}
	UsbBulkInMsgBuf[9] = 0x00;

	return ;

}

///**************************************************************************
//;****�������ƣ�RDR_to_PC_Parameters
//;****��ڲ�����err �������
//;****���ڲ�������
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: ���ò�����Ӧ��
//;**************************************************************************/
void RDR_to_PC_Parameters(unsigned char err)
{
  	unsigned char i;
	for(i=0;i<10;i++)
	{
	   UsbBulkInMsgBuf[i] = UsbBulkMsgBuf[i];
	}
	UsbBulkInMsgBuf[0] = RDR_TO_PC_PARAMETERS;
	if(slot1==0)
   		UsbBulkInMsgBuf[7] = IFD_Slot_GetStatus();
	else
	   	return;
   	UsbBulkInMsgBuf[2] = 0x00;
   	UsbBulkInMsgBuf[3] = 0x00;
	UsbBulkInMsgBuf[4] = 0x00;
	UsbBulkInMsgBuf[8] = 0x00;
	if(err != SLOT_NO_ERROR)
	{
		UsbBulkInMsgBuf[7] += 0x40;
		UsbBulkInMsgBuf[1] =  0x00;
		UsbBulkInMsgBuf[8] =  err;
		return;
	}

	UsbBulkInMsgBuf[9]= IFD_GetParameters(&UsbBulkInMsgBuf[0x0a]);
	if(UsbBulkInMsgBuf[9])
	{
		UsbBulkInMsgBuf[1] = 0x07;
	}
	else
	{	
		UsbBulkInMsgBuf[1] = 0x05;	
	}
	if(slot1==1)
	{
		UsbBulkInMsgBuf[0x0a]=0x11;	
		UsbBulkInMsgBuf[0x0b]=0x00;
		UsbBulkInMsgBuf[0x0c]=0x10;
		UsbBulkInMsgBuf[0x0d]=0x0a;
		UsbBulkInMsgBuf[0x0e]=0x00;
	}
	return ;
}

///**************************************************************************
//;****�������ƣ�memcpy
//;****��ڲ�����pParamBuf �������� T01 Э������
//;****���ڲ�������
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: IFD���ò���
//;**************************************************************************/
void memcpy(unsigned char *str,unsigned char *ptr,unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
	 	*str=*ptr;
		str++;
		ptr++;
	}
}

///**************************************************************************
//;****�������ƣ�ISO7816_SetParameters
//;****��ڲ�����pParamBuf �������� T01 Э������
//;****���ڲ�������
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: IFD���ò���
//;**************************************************************************/
unsigned char ISO7816_SetParameters(unsigned char *pParamBuf,unsigned char T01)
{
	unsigned char bErrorCode;

	ISO7816_param_T01_t newIccParam;
	//���Ʋ���
	newIccParam.FiDi = pParamBuf[0];
	newIccParam.T01ConvChecksum = pParamBuf[1];
	newIccParam.GuardTimeInteger = pParamBuf[2];
	newIccParam.WaitingTimeInteger = pParamBuf[3];
	newIccParam.ClockStop = pParamBuf[4];

	if(T01 == 1)
	{
		newIccParam.Ifsc = pParamBuf[5];
		newIccParam.Nad = pParamBuf[6];
		bTransactionType=1;		        //t1
	}
	else
	{
		newIccParam.Ifsc = 0;
		newIccParam.Nad = 0;
		bTransactionType=0;		  
	}

	/*	У��FiDi���� ,����Ϊ0�򱨴�	*/
	if(	(f_Table[newIccParam.FiDi >> 4] == 0)	||
			(d_Table[newIccParam.FiDi & 0x0f] == 0)	)
	{
		/*	Fi Di��������	*/
		return ERROR_BAD_FIDI;
	}

	if(T01 == 0)	//����
	{
		bTransactionType=0;		 //t=0
		if(	(newIccParam.T01ConvChecksum != 0x00)	&&
				(newIccParam.T01ConvChecksum != 0x02)	)
		{
			return ERROR_BAD_TCCKS;
		}
	}
	else
	{
		if(	(newIccParam.T01ConvChecksum != 0x10)	&&
				(newIccParam.T01ConvChecksum != 0x11)	&&
				(newIccParam.T01ConvChecksum != 0x12)	&&
				(newIccParam.T01ConvChecksum != 0x13)	)
		{
			return ERROR_BAD_TCCKS;
		}
	}

	if(	(newIccParam.ClockStop != 0x00)		&&
			(newIccParam.ClockStop != 0x03)	)
	{
		return ERROR_BAD_CLOCKSTOP;
	}

//	if(newIccParam.Nad != 0)
//	{
//		return ERROR_BAD_NAD;
//	}

	/*	����У����ȷ�����Ʋ�����IccParameters��	*/
	(void)memcpy((unsigned char *)&IfdParameters,(unsigned char *)&newIccParam,sizeof(ISO7816_param_T01_t));

	bErrorCode = 0;						//���²�������

	if(bErrorCode != 0)
	{
		return bErrorCode;
	}
	else
	{

		return SLOT_NO_ERROR;
	}

}



///**************************************************************************
//;****�������ƣ�IFD_GetParameters
//;****��ڲ�����pParamBuf ��������
//;****���ڲ���������Э������ T0 T1
//;****ȫ�ֱ�����
//;****����ģ�飺
//;****˵    ��: IFD��ȡ����
//;**************************************************************************/
unsigned char IFD_GetParameters(unsigned char *pParamBuf)
{

	memcpy(pParamBuf,(unsigned char*)&IfdParameters,7);

	if(IfdParameters.T01ConvChecksum & 0x10)
	{
		return 0x01;
	}

	return 0x00;
}


///**************************************************************************
// ****��������:  ISO7816_XfrBlock
// ****��ڲ���:  unsigned char  *pTxBlockBuf,unsigned int wTxBlcokLen,
// ****��ڲ���:  unsigned char  *pRxBlockBuf,unsigned int wRxBlcokLen,
// ****��ڲ���:  unsigned int wLevelParameter
// ****���ڲ���:  unsigned char 
// ****˵          ����
// **************************************************************************/
unsigned char ISO7816_XfrBlock(unsigned char *pTxBlockBuf,unsigned int wTxBlcokLen,unsigned char *pRxBlockBuf,unsigned int *wRxBlcokLen,
                         unsigned int wLevelParameter)
{
    char bErrorCode=0;

    if(wLevelParameter != 0)
    {
        return ERROR_BAD_WLEVELPARAMETER;
    }

    if(bTransactionType == 0X00)
    {
         /*  ʹ��T0 Э�鷢������ */
         /*  ����Block���ݰ��������һ���ֽ���0xff��ʾ����PPS  */
		if(pTxBlockBuf[0] == 0xff)
        {
        	bErrorCode = PPS_Exchange(pTxBlockBuf,wTxBlcokLen,pRxBlockBuf,wRxBlcokLen);
        }
        else
        {
          //  bErrorCode = SCT0Apdu(0,pTxBlockBuf,wTxBlcokLen,pRxBlockBuf,wRxBlcokLen);
        }

	}
    else
    {
        if (pTxBlockBuf[0]==0xff)
        {
          	bErrorCode = PPS_Exchange(pTxBlockBuf,wTxBlcokLen,pRxBlockBuf,wRxBlcokLen);
        }
        else																										  
        {
           //bErrorCode = SCT1AccessApdu(0,pTxBlockBuf,wTxBlcokLen,pRxBlockBuf,wRxBlcokLen);
        }
	}
    if(bErrorCode != 0)
    {
        return (unsigned char)bErrorCode;
    }
    return SLOT_NO_ERROR;

}

///**************************************************************************
// ****��������:  PPS_Match
// ****��ڲ���:  unsigned char *pRequest,                              PPS�������ݻ���
// ****��ڲ���:  unsigned char len_request,                            PPS�������ݳ���
// ****��ڲ���:  unsigned char *pConfirm,                              PPSӦ�����ݻ���
// ****��ڲ���:  unsigned char len_confirm,                            PPSӦ�����ݳ���
// ****���ڲ���:  unsigned char
// ****˵    ��:    PPS У��
// **************************************************************************/
unsigned char PPS_Match(unsigned char *p_req_buf, unsigned int w_req_len, unsigned char *p_resp_buf, unsigned int w_resp_len)
{
    unsigned int i;
    if(w_req_len != w_resp_len)
    {
        return 1;
    }
	for(i=0;i<w_req_len;i++)
	{
	 	if(*p_req_buf!=*p_resp_buf)
			return 1;
		else
		{
			p_resp_buf++;
			p_req_buf++;
		}
	}
   
    return 0;
}	

///**************************************************************************
// ****��������:  PPS_SendData
// ****��ڲ���:  unsigned char *pSendPPSBuf,unsigned char PPSLen
// ****���ڲ���:  unsigned char
// ****˵          ����   ����PPS����
// **************************************************************************/
unsigned char PPS_SendData(unsigned char *pSendPPSBuf,unsigned int PPSLen)
{
   return 0;
}

///**************************************************************************
// ****��������:  PPS_ReceiveData
// ****��ڲ���:  unsigned char *pPPSData,unsigned char *wInlen
// ****���ڲ���:  unsigned char
// ****˵          ����   ����PPS�ش�����
// **************************************************************************/
unsigned char PPS_ReceiveData(unsigned char *pPPSData,unsigned int *wInlen)
{
	return 0;
}
///**************************************************************************
// ****��������:  PPS_Exchange
// ****��ڲ���:  unsigned char *p_req_buf, unsigned int w_req_len,unsigned char *p_resp_buf,unsigned int *p_w_resp_len
// ****��ڲ���:
// ****���ڲ���:  uint8_t
// ****˵    ��: 	PPS���ݽ���
// **************************************************************************/
unsigned char PPS_Exchange(unsigned char *p_req_buf, unsigned int w_req_len,unsigned char *p_resp_buf,unsigned int *p_w_resp_len)
{
    unsigned char bErrorCode;


    bErrorCode = PPS_SendData(p_req_buf,w_req_len);
    if(bErrorCode != 0)
    {
        return bErrorCode;
    }

    bErrorCode = PPS_ReceiveData(p_resp_buf,p_w_resp_len);

    if(bErrorCode != 0)
    {
        return bErrorCode;
    }

    /*********************************************************************************************************
     * �ȶ� PPS�������Ӧ����
    *********************************************************************************************************/
    if(PPS_Match(p_req_buf,w_req_len,p_resp_buf,*p_w_resp_len) != 0)
    {
        return 1;
    }

    return 0;
}	
//===============================================================================//
//== �� �� �� : T0ApduCommand
//== ��    �� : /*T0������*/
//== ˵    �� : 
//== ʱ    �� : 
//== ��ڲ��� : sbuf=���ͻ�������slen=�������ݳ���
//== ���ڲ��� : rbuf=�������ݻ�������rlen=�������ݳ���
//== �� �� ֵ : ִ��״̬
//===============================================================================//
/*unsigned int T0ApduCommand(unsigned char* sbuf, unsigned int slen, unsigned char* rbuf, unsigned int* rlen)
{
	 return 0;
}*/

///**************************************************************************
// ****��������:  T1ApduCommand
// ****��ڲ���:  unsigned char* sbuf, unsigned int slen,
// ****��ڲ���:  unsigned char* rbuf, unsigned int* rlen
// ****���ڲ���:  uint16_t
// ****˵    ���� �ó���֮�������ISO7816-3�涨��APDU
// **************************************************************************/
/*unsigned int T1ApduCommand(unsigned char* sbuf, unsigned int slen, unsigned char* rbuf, unsigned int* rlen)
{
	return 0;
}*/

