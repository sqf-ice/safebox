/*===============================================================================//
//== 文 件 名：ccid.c
//== author	 ：huyx@cashwaytech.com
//== 说    明：STM32F103VC8平台 ccid 函数
//== 恒银金融有限公司
//== 附加说明：
//===============================================================================*/
#include "ccid.h"
#include "IFD.h"

#include "usb_endp.h"
//#include "function.h"	//== 包含平台库和所有外调函数
#include "usb_lib.h"


unsigned char UsbBulkMsgBuf[271];	 //与USB通信的缓存buf
unsigned char UsbBulkInMsgBuf[2300];	 //与USB通信的缓存buf
unsigned char Iso7816CurParam=0;    //应该在收到ATR的时候设置该值	  
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
#define Fi_RFU 		372						//==留作未来使用
#define Di_RFU 		1						//==留作未来使用 
volatile unsigned int f_Table[16]={372,	372,	558,	744,	1116,	1488,	1860,	Fi_RFU,\
					 		 	   Fi_RFU,512,	768,	1024,	1536,	2048,	Fi_RFU,	Fi_RFU}; //==时针率转换因子
volatile unsigned int d_Table[16]={Di_RFU,1,		2,		4,		8,		16,		32,		Di_RFU,\
					 		       12,	20,		Di_RFU,	Di_RFU,	Di_RFU,	Di_RFU,	Di_RFU,	Di_RFU}; //==波特率校正参数

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
			 else if(UsbBulkMsgBuf[0x18] ==0x01)   //控制时间
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
//;****函数名称：CCID_AnalyseBulkMessage
//****入口参数：无
//****出口参数：无
//****全局变量：
//****调用模块：
//****说    明: CCID BULK数据分析
//**************************************************************************/
void CCID_AnalyseBulkMessage(void)
{
	unsigned char bErrorCode;

//	unsigned int wTempBulkLen;    //通过USB发送的数据的长度
	bBulkOutCompeleteFlag=1;
	/*  判断BULK OUT数据是否接受完成    */
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
				/*  帧错误  */
				CCID_CmdNotSupport();
				break;
		}

		///////发送函数/////////////////////////////////////////////////////////////////
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

//	unsigned int wTempBulkLen;    //通过USB发送的数据的长度
	bBulkOutCompeleteFlag=1;
	/*  判断BULK OUT数据是否接受完成    */
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
				/*  帧错误  */
				CCID_CmdNotSupport();
				break;
		}


	}
}


///**************************************************************************
//;****函数名称：PC_to_RDR_IccPowerOn
//;****入口参数：无
//;****出口参数：操作状态
//;****全局变量：
//;****调用模块：
//;****说    明: 上电
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
//;****函数名称：PC_to_RDR_IccPowerOff
//;****入口参数：无
//;****出口参数：操作状态
//;****全局变量：
//;****调用模块：
//;****说    明: 下电
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
;****函数名称：PC_to_RDR_GetParameters
;****入口参数：无
;****出口参数：操作状态
;****全局变量：
;****调用模块：
;****说    明: 获得参数
;**************************************************************************/
unsigned char PC_to_RDR_GetParameters(void)
{
     /*  校验参数    */
     if(UsbBulkMsgBuf[5] != 0)
     {
         return ERROR_BAD_BSLOT;                             //返回卡槽编号错误
     }

     if(     (UsbBulkMsgBuf[1] != 0)   ||  (UsbBulkMsgBuf[2] != 0)   ||
             (UsbBulkMsgBuf[3] != 0)   ||  (UsbBulkMsgBuf[4] != 0)   )
     {
         return ERROR_BAD_DWLENGTH;                          //返回长度域错误
     }

     if((UsbBulkMsgBuf[7] != 0)   ||  (UsbBulkMsgBuf[8] != 0)   ||	(UsbBulkMsgBuf[9] != 0))
     {
         return ERROR_BAD_ABRFU_3;                           //返回保留字节错误
     }

     return SLOT_NO_ERROR;
}

///**************************************************************************
//;****函数名称：PC_to_RDR_ResetParameters
//;****入口参数：无
//;****出口参数：操作状态
//;****全局变量：
//;****调用模块：
//;****说    明: 参数复位
//;**************************************************************************/
unsigned char PC_to_RDR_ResetParameters(void)
{
  	unsigned char bErrorCode;


    /*  校验参数    */
    if(UsbBulkMsgBuf[5] != 0)
    {
        return ERROR_BAD_BSLOT;                             //返回卡槽编号错误
    }

    if( (UsbBulkMsgBuf[1] != 0)   ||  (UsbBulkMsgBuf[2] != 0)   ||
        (UsbBulkMsgBuf[3] != 0)   ||  (UsbBulkMsgBuf[4] != 0)   )
    {
        return ERROR_BAD_DWLENGTH;                   //返回长度域错误
    }

    if((UsbBulkMsgBuf[7] != 0) || (UsbBulkMsgBuf[8] != 0) ||(UsbBulkMsgBuf[9] != 0))
    {
        return ERROR_BAD_ABRFU_3;                           //返回保留字节错误
    }
  
    UsbBulkMsgBuf[0x0a] = 0x11;			     //设置为默认值
    UsbBulkMsgBuf[0x0b] = 0;
    UsbBulkMsgBuf[0x0c] = 0;
    UsbBulkMsgBuf[0x0d] = 0x0a;
    UsbBulkMsgBuf[0x0e] = 0x03;
    UsbBulkMsgBuf[0x0f] = 0x20;
    UsbBulkMsgBuf[0x10] = 0;

    /*  设置IFD参数 */
    bErrorCode = ISO7816_SetParameters(UsbBulkMsgBuf + 0x0a,0);

    if(bErrorCode != SLOT_NO_ERROR)
    {
        return bErrorCode;
    }

    return bErrorCode;
}
///**************************************************************************
//;****函数名称：PC_to_RDR_SetParameters
//;****入口参数：无
//;****出口参数：操作状态
//;****全局变量：
//;****调用模块：
//;****说    明: 参数设置
//;**************************************************************************/
unsigned char PC_to_RDR_SetParameters(void)
{
	unsigned char bErrorCode,len,i;

	/*  校验dwLength域  */
	if (    (UsbBulkMsgBuf[2] != 0)	||
			(UsbBulkMsgBuf[3] != 0)	||
			(UsbBulkMsgBuf[4] != 0)	||
			((UsbBulkMsgBuf[1] != 5) &&  (UsbBulkMsgBuf[1] != 7) ) )

	{
		return ERROR_BAD_DWLENGTH;
	}

	/*  判断保留字域是否正确    */
	if (    (UsbBulkMsgBuf[8] != 0)	||(UsbBulkMsgBuf[9] != 0)    )
	{
		return ERROR_BAD_ABRFU_2;
	}

	/*  判断bProtocolNum    */
	if (    ((UsbBulkMsgBuf[1] == 5)     &&  (UsbBulkMsgBuf[7] != 0x00))   ||
			((UsbBulkMsgBuf[1] == 7)     &&  (UsbBulkMsgBuf[7] != 0x01))    )

	{
		return ERROR_BAD_BPROTOCOLNUM;
	}

	/*	IFD 设置参数	*/
	if(slot1==0)
		bErrorCode = ISO7816_SetParameters(UsbBulkMsgBuf +0x0a,UsbBulkMsgBuf[7]);
	else
	{
		len= UsbBulkMsgBuf[1];
		for(i=0;i<len;i++)
			UsbBulkInMsgBuf[10+i]= UsbBulkMsgBuf[10+i];
		bErrorCode = SLOT_NO_ERROR;
	}
	/*  判断参数设置操作是否成功    */
	if(bErrorCode != SLOT_NO_ERROR)
	{
		return bErrorCode;
	}
   return bErrorCode;
}
//**************************************************************************
//****函数名称：PC_to_RDR_XfrBlock
//****入口参数：无
//****出口参数：操作状态
//****全局变量：
//****调用模块：
//****说    明: 数据交互
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
	/*  校验dwLength数据域  */
	unsigned int wBlockSize1,i,ret;
//	unsigned char i;
	if ((UsbBulkMsgBuf[3] != 0)||(UsbBulkMsgBuf[4] != 0))
	{
		return ERROR_BAD_DWLENGTH;	         
	}

	/*  向ICC传送数据   */
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
	/*  校验dwLength数据域  */
	unsigned int wBlockSize1,i,ret;
//	unsigned char i;
	if ((UsbBulkMsgBuf[3] != 0)||(UsbBulkMsgBuf[4] != 0))
	{
		return ERROR_BAD_DWLENGTH;	         
	}

	/*  向ICC传送数据   */
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
//;****函数名称：CCID_CmdNotSupport
//;****入口参数：无
//;****出口参数：无
//;****全局变量：
//;****调用模块：
//;****说    明: 不支持的CCID的消息的处理
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
//;****函数名称：RDR_to_PC_DataBlock
//;****入口参数：错误代码
//;****出口参数：无
//;****全局变量：
//;****调用模块：
//;****说    明: CCID 数据交互的应答
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
//;****函数名称：RDR_to_PC_SlotStatus
//;****入口参数：错误代码
//;****出口参数：无
//;****全局变量：
//;****调用模块：
//;****说    明: 获取卡槽的应答
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
//;****函数名称：RDR_to_PC_Parameters
//;****入口参数：err 错误代码
//;****出口参数：无
//;****全局变量：
//;****调用模块：
//;****说    明: 设置参数的应答
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
//;****函数名称：memcpy
//;****入口参数：pParamBuf 参数缓存 T01 协议类型
//;****出口参数：无
//;****全局变量：
//;****调用模块：
//;****说    明: IFD设置参数
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
//;****函数名称：ISO7816_SetParameters
//;****入口参数：pParamBuf 参数缓存 T01 协议类型
//;****出口参数：无
//;****全局变量：
//;****调用模块：
//;****说    明: IFD设置参数
//;**************************************************************************/
unsigned char ISO7816_SetParameters(unsigned char *pParamBuf,unsigned char T01)
{
	unsigned char bErrorCode;

	ISO7816_param_T01_t newIccParam;
	//复制参数
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

	/*	校验FiDi参数 ,参数为0则报错	*/
	if(	(f_Table[newIccParam.FiDi >> 4] == 0)	||
			(d_Table[newIccParam.FiDi & 0x0f] == 0)	)
	{
		/*	Fi Di参数错误	*/
		return ERROR_BAD_FIDI;
	}

	if(T01 == 0)	//正反
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

	/*	参数校验正确，复制参数到IccParameters中	*/
	(void)memcpy((unsigned char *)&IfdParameters,(unsigned char *)&newIccParam,sizeof(ISO7816_param_T01_t));

	bErrorCode = 0;						//更新参数设置

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
//;****函数名称：IFD_GetParameters
//;****入口参数：pParamBuf 参数缓存
//;****出口参数：返回协议类型 T0 T1
//;****全局变量：
//;****调用模块：
//;****说    明: IFD获取参数
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
// ****函数名称:  ISO7816_XfrBlock
// ****入口参数:  unsigned char  *pTxBlockBuf,unsigned int wTxBlcokLen,
// ****入口参数:  unsigned char  *pRxBlockBuf,unsigned int wRxBlcokLen,
// ****入口参数:  unsigned int wLevelParameter
// ****出口参数:  unsigned char 
// ****说          明：
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
         /*  使用T0 协议发送数据 */
         /*  分析Block数据包，如果第一个字节是0xff表示交换PPS  */
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
// ****函数名称:  PPS_Match
// ****入口参数:  unsigned char *pRequest,                              PPS请求数据缓存
// ****入口参数:  unsigned char len_request,                            PPS请求数据长度
// ****入口参数:  unsigned char *pConfirm,                              PPS应答数据缓存
// ****入口参数:  unsigned char len_confirm,                            PPS应答数据长度
// ****出口参数:  unsigned char
// ****说    明:    PPS 校验
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
// ****函数名称:  PPS_SendData
// ****入口参数:  unsigned char *pSendPPSBuf,unsigned char PPSLen
// ****出口参数:  unsigned char
// ****说          明：   发送PPS数据
// **************************************************************************/
unsigned char PPS_SendData(unsigned char *pSendPPSBuf,unsigned int PPSLen)
{
   return 0;
}

///**************************************************************************
// ****函数名称:  PPS_ReceiveData
// ****入口参数:  unsigned char *pPPSData,unsigned char *wInlen
// ****出口参数:  unsigned char
// ****说          明：   接收PPS回传数据
// **************************************************************************/
unsigned char PPS_ReceiveData(unsigned char *pPPSData,unsigned int *wInlen)
{
	return 0;
}
///**************************************************************************
// ****函数名称:  PPS_Exchange
// ****入口参数:  unsigned char *p_req_buf, unsigned int w_req_len,unsigned char *p_resp_buf,unsigned int *p_w_resp_len
// ****入口参数:
// ****出口参数:  uint8_t
// ****说    明: 	PPS数据交换
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
     * 比对 PPS请求和响应数据
    *********************************************************************************************************/
    if(PPS_Match(p_req_buf,w_req_len,p_resp_buf,*p_w_resp_len) != 0)
    {
        return 1;
    }

    return 0;
}	
//===============================================================================//
//== 函 数 名 : T0ApduCommand
//== 功    能 : /*T0处理函数*/
//== 说    明 : 
//== 时    间 : 
//== 入口参数 : sbuf=发送缓冲区；slen=发送数据长度
//== 出口参数 : rbuf=接收数据缓冲区；rlen=接收数据长度
//== 返 回 值 : 执行状态
//===============================================================================//
/*unsigned int T0ApduCommand(unsigned char* sbuf, unsigned int slen, unsigned char* rbuf, unsigned int* rlen)
{
	 return 0;
}*/

///**************************************************************************
// ****函数名称:  T1ApduCommand
// ****入口参数:  unsigned char* sbuf, unsigned int slen,
// ****入口参数:  unsigned char* rbuf, unsigned int* rlen
// ****出口参数:  uint16_t
// ****说    明： 该程序之处理符合ISO7816-3规定的APDU
// **************************************************************************/
/*unsigned int T1ApduCommand(unsigned char* sbuf, unsigned int slen, unsigned char* rbuf, unsigned int* rlen)
{
	return 0;
}*/

