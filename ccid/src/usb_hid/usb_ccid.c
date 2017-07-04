/*===============================================================================//
//== 文 件 名：usb_ccid.c
//== author	 ：huyuxin
//== 说    明：STM32F103VC8平台
//== 清华同方|同方锐安科技有限公司
//===============================================================================*/
//#include <string.h>
#include "usb_lib.h" 
#include "usb_pwr.h"
#include "ccid.h"
#include "IFD.h"
#include "usb_endp.h"
#include "stm32f10x_iwdg.h"
#include "function.h"						//== 包含平台库和所有外调函数
struct _UsbMod usbMsg;						//==global.h  
unsigned char R_Count=0,R_flag=0,R_len;
unsigned char bDevice_flag=0,send_flag=0;
unsigned char  slot1;
extern void USB_Interrupts_Config(void);	//==hw_config.c
extern void Set_USBClock(void);				//==hw_config.c	
unsigned char UsbIntMsgBuf[2]; 
volatile CCID_SLOT_STATUS CCIDSlotStatus;							//CCID 卡槽状态
volatile CCID_NSLOT_STATUS CCIDNSlotStatus;
//volatile unsigned int hid_set_report_flag = 0;	//==hid set report
//volatile unsigned int hid_report_process  = 0;	//==hid report process
void RDR_to_PC_NotifySlotChange(void);
unsigned char temp_chenge=0;
void CCID_StatePolling(void);
void CCID_Init(void);
void IFD_14443_dec_Process(void);


//===============================================================================//
//== 函 数 名：init_usb
//== 功    能：初始化usb,hid模式
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
void init_usb(void)
{
	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();
	memset((unsigned char*)&usbMsg, 0, sizeof(struct _UsbMod));		//==usb参数清零
	//CCID_Init();
}
//////////////////////////////////////////////////////////////////////////
//===============================================================================//
//== 函 数 名：CCID_Init
//== 功    能：初始化CCID_Init模式
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
void CCID_Init(void)
{

	Reset_CCIDStatus;
	Reset_NCCIDStatus;
	CCIDSlotStatus._byte = 0x00;							//清空CCID卡槽状态
	CCIDNSlotStatus._Nbyte = 0x00;							//清空CCID卡槽状态

	CCIDSlotStatus.BitCtl.Slot0ChangedState = 1;			//设置SLOT0 状态改变
	CCIDNSlotStatus.NBitCtl.NSlot0ChangedState = 1;			//设置SLOT0 状态改变

}
//===============================================================================//
//== 函 数 名：CCID_StatePolling
//== 功    能：轮训模式
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
extern void open_time5_isr(void);
void CCID_StatePolling(void)
{	
	unsigned char temp=0;
	while(bDeviceState != CONFIGURED);
//	open_time5_isr();

	/* USB设备枚举成功，将不退出CCID模块，处于联机模式 */
	if(bDeviceState == CONFIGURED)
	{
        bDevice_flag=1;
		IWDG_ReloadCounter();
		temp= IFD_Slot_GetStatus();
		if(temp == ICC_NO_PRESENT)
		{
		//卡拔出
			if(CCIDSlotStatus.BitCtl.Slot0CurrentState == 1)
			{
				CCIDSlotStatus.BitCtl.Slot0ChangedState = 1;	//设置SLOT0状态改变
				CCIDSlotStatus.BitCtl.Slot0CurrentState = 0;	//保存SLOT0当前状态
			}
		}
		else
		{
		//卡插入
			if(CCIDSlotStatus.BitCtl.Slot0CurrentState == 0)
			{
				CCIDSlotStatus.BitCtl.Slot0ChangedState = 1;	//设置SLOT0状态改变
				CCIDSlotStatus.BitCtl.Slot0CurrentState = 1;	//保存SLOT0当前状态
			}
			if(power_flag==1)
			{
				CCIDSlotStatus._byte=0x02;
				power_flag=0;	
			}
		}

		if( (CCIDSlotStatus._byte & 0x02) !=0 )
		{
		/*	卡槽状态改变	*/
			RDR_to_PC_NotifySlotChange();
			USB_SendData(0x03,UsbIntMsgBuf,2);
			CCIDSlotStatus.BitCtl.Slot0ChangedState = 0;	//设置SLOT0状态改变
		}

	  	if(usbMsg.EP2_RX_INT_FLAG==1)
	  	{
	  		Set_bBulkOutCompeleteFlag;
			slot1=0;
			CCID_AnalyseBulkMessage();
			usbMsg.EP2_RX_INT_FLAG=0;		
	  	}
		if(usbMsg.EP5_RX_INT_FLAG==1)
	  	{
	  		send_flag=1;
			slot1=1;
			Set_bBulkOutCompeleteFlag;
			CCID_AnalyseBulkMessage();
			usbMsg.EP5_RX_INT_FLAG=0;		
	  	}
	}

} 

//===============================================================================//
//== 函 数 名：RDR_to_PC_NotifySlotChange1
//== 功    能：卡到位检测
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
void RDR_to_PC_NotifySlotChange1(void)
{
    UsbIntMsgBuf[0] = 0x50;

	/*  获取各卡槽状态  */
//	UsbIntMsgBuf[OFFSET_INT_BMSLOTICCSTATE] = 0;
	if (2==IFD_nContact_Slot_GetStatus())
	{
		UsbIntMsgBuf[1] = 0x02;
	}
	else
	{
		UsbIntMsgBuf[1] = 0x03;
	}
	return ;
}

//===============================================================================//
//== 函 数 名：IFD_14443_dec_isr
//== 功    能：非接触卡到位检测
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
void IFD_14443_dec_Process(void)
{

    /* 检测非接触卡槽状态 */
	
     if(IFD_nContact_Slot_GetStatus() == ICC_NO_PRESENT)
     {
        if(CCIDNSlotStatus.NBitCtl.NSlot0CurrentState == 1)
		{
			CCIDNSlotStatus.NBitCtl.NSlot0ChangedState = 1;	//设置SLOT0状态改变
			CCIDNSlotStatus.NBitCtl.NSlot0CurrentState = 0;	//保存SLOT0当前状态
		}
	}	
	else
	{
		   
		//卡插入
		if(CCIDNSlotStatus.NBitCtl.NSlot0CurrentState == 0)
		{
			CCIDNSlotStatus.NBitCtl.NSlot0ChangedState = 1;	//设置SLOT0状态改变
			CCIDNSlotStatus.NBitCtl.NSlot0CurrentState = 1;	//保存SLOT0当前状态
		}
	       
	  }
    if( (CCIDNSlotStatus._Nbyte & 0x02) !=0 )
		{
		/*	卡槽状态改变	*/
			RDR_to_PC_NotifySlotChange1();
			USB_SendData(0x05,UsbIntMsgBuf,2);
			CCIDNSlotStatus.NBitCtl.NSlot0ChangedState = 0;	//设置SLOT0状态改变
		}    
}	



//===============================================================================//
//== 函 数 名：RDR_to_PC_NotifySlotChange
//== 功    能：卡到位检测
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
void RDR_to_PC_NotifySlotChange(void)
{
    UsbIntMsgBuf[0] = 0x50;

	/*  获取各卡槽状态  */
//	UsbIntMsgBuf[OFFSET_INT_BMSLOTICCSTATE] = 0;
	if (2==IFD_Slot_GetStatus())
	{
		UsbIntMsgBuf[1] = 0x02;
	}
	else
	{
		UsbIntMsgBuf[1] = 0x03;
	}
	return ;
}


//===============================================================================//
//== 函 数 名：usb_rev_data
//== 功    能：usb 接收数据	ep2
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
 void usb_rev_data(void)
{
	unsigned int counter=0;

	if(bDeviceState != CONFIGURED) return; 	//==USB没连接直接返回

	if(R_flag==0)
	{
		counter = GetEPRxCount(ENDP2);
		PMAToUserBufferCopy(UsbBulkMsgBuf, GetEPRxAddr(ENDP2), counter);
		R_Count=10+(UsbBulkMsgBuf[2]<<8)+UsbBulkMsgBuf[1];
		SetEPRxValid(ENDP2);
		if(R_Count >64)
		{
		   R_flag=1;
		   R_Count-=counter;
		   R_len=counter;
		}
		else
		{
			R_flag=2;   //接收完成
			R_Count=0;
			R_len=0;
		//	SetEPRxStatus(ENDP2,EP_RX_NAK); 
		}	
	}
	else if(R_flag==1)
	{
		counter = GetEPRxCount(ENDP2);
		PMAToUserBufferCopy(UsbBulkMsgBuf + R_len, GetEPRxAddr(ENDP2), counter);
		SetEPRxValid(ENDP2);
//		R_Count-=counter;
		if(R_Count >64)
		{
		   R_flag=1;
		   R_Count-=counter;
		   R_len+=counter;
		}
		else
		{
			R_flag=2;   //接收完成	
			R_Count=0;
			R_len=0;
		//	SetEPRxStatus(ENDP2,EP_RX_NAK); 
		}	
	}
	else
	{
	 	R_flag=2;
		R_Count=0;
		R_len=0;
	//	SetEPRxStatus(ENDP2,EP_RX_NAK); 
	}
	
	//ccidmsg.EP2_RX_INT_FLAG=1;
	if(2 == R_flag)	
	{
		R_flag = 0;
		usbMsg.EP2_RX_INT_FLAG=1;
	}	
}

//===============================================================================//
//== 函 数 名：usb_rev_data
//== 功    能：usb 接收数据	ep4
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
unsigned char num_SN=0;
void usb_rev_data1(void)
{
	unsigned int counter=0;

	if(bDeviceState != CONFIGURED) return; 	//==USB没连接直接返回
	if(R_flag==0)
	{
		counter = GetEPRxCount(ENDP4);
		PMAToUserBufferCopy(UsbBulkMsgBuf, GetEPRxAddr(ENDP4), counter);
		R_Count=10+(UsbBulkMsgBuf[2]<<8)+UsbBulkMsgBuf[1];
		SetEPRxValid(ENDP4);
		if(R_Count >64)
		{
		   R_flag=1;
		   R_Count-=counter;
		   R_len=counter;
		}
		else
		{
			R_flag=2;   //接收完成
			R_Count=0;
			R_len=0;
		//	SetEPRxStatus(ENDP2,EP_RX_NAK); 
		}	
	}
	else if(R_flag==1)
	{
		counter = GetEPRxCount(ENDP4);
		PMAToUserBufferCopy(UsbBulkMsgBuf + R_len, GetEPRxAddr(ENDP4), counter);
		SetEPRxValid(ENDP4);
//		R_Count-=counter;
		if(R_Count >64)
		{
		   R_flag=1;
		   R_Count-=counter;
		   R_len+=counter;
		}
		else
		{
			R_flag=2;   //接收完成	
			R_Count=0;
			R_len=0;
		//	SetEPRxStatus(ENDP2,EP_RX_NAK); 
		}	
	}
	else
	{
	 	R_flag=2;
		R_Count=0;
		R_len=0;
	//	SetEPRxStatus(ENDP2,EP_RX_NAK); 
	}
	
	if(2 == R_flag)	
	{
		R_flag = 0;
		num_SN =UsbBulkMsgBuf[6];
		usbMsg.EP5_RX_INT_FLAG=1;
	}			
}
//===============================================================================//
//== 函 数 名：usb_snd_data
//== 功    能：usb 发送数据	ep1
//== 说    明：
//== 时    间：
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//===============================================================================//
void usb_snd_data()
{
	unsigned int pack_num;					//== 包数
	unsigned int i;
		
	if(bDeviceState != CONFIGURED) return; 	//==USB没连接直接返回

	pack_num = usbMsg.tCount % Device_Property.MaxPacketSize;

	if ( pack_num )							//== 不是整64字节的整数倍
	{
		pack_num = (usbMsg.tCount/Device_Property.MaxPacketSize) + 1;
		//== 不足一包的多余字节 补零
		memset( usbMsg.tBuf + usbMsg.tCount, 0, (Device_Property.MaxPacketSize-(usbMsg.tCount % Device_Property.MaxPacketSize)) );		
	}
	else
	{
		pack_num = usbMsg.tCount/Device_Property.MaxPacketSize;
	}

	for (i=0; i<pack_num; i++)
	{ 
//		UserToPMABufferCopy(usbMsg.tBuf+(i*Device_Property.MaxPacketSize), ENDP1_TXADDR, Device_Property.MaxPacketSize);
//		SetEPTxCount(ENDP1, Device_Property.MaxPacketSize);
//		SetEPTxValid(ENDP1);
//		usbMsg.EP1_TX_INT_FLAG = 1;
//		while( (usbMsg.EP1_TX_INT_FLAG==1) );
	}
}









