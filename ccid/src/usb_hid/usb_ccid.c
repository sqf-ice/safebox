/*===============================================================================//
//== �� �� ����usb_ccid.c
//== author	 ��huyuxin
//== ˵    ����STM32F103VC8ƽ̨
//== �廪ͬ��|ͬ���񰲿Ƽ����޹�˾
//===============================================================================*/
//#include <string.h>
#include "usb_lib.h" 
#include "usb_pwr.h"
#include "ccid.h"
#include "IFD.h"
#include "usb_endp.h"
#include "stm32f10x_iwdg.h"
#include "function.h"						//== ����ƽ̨��������������
struct _UsbMod usbMsg;						//==global.h  
unsigned char R_Count=0,R_flag=0,R_len;
unsigned char bDevice_flag=0,send_flag=0;
unsigned char  slot1;
extern void USB_Interrupts_Config(void);	//==hw_config.c
extern void Set_USBClock(void);				//==hw_config.c	
unsigned char UsbIntMsgBuf[2]; 
volatile CCID_SLOT_STATUS CCIDSlotStatus;							//CCID ����״̬
volatile CCID_NSLOT_STATUS CCIDNSlotStatus;
//volatile unsigned int hid_set_report_flag = 0;	//==hid set report
//volatile unsigned int hid_report_process  = 0;	//==hid report process
void RDR_to_PC_NotifySlotChange(void);
unsigned char temp_chenge=0;
void CCID_StatePolling(void);
void CCID_Init(void);
void IFD_14443_dec_Process(void);


//===============================================================================//
//== �� �� ����init_usb
//== ��    �ܣ���ʼ��usb,hidģʽ
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
void init_usb(void)
{
	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();
	memset((unsigned char*)&usbMsg, 0, sizeof(struct _UsbMod));		//==usb��������
	//CCID_Init();
}
//////////////////////////////////////////////////////////////////////////
//===============================================================================//
//== �� �� ����CCID_Init
//== ��    �ܣ���ʼ��CCID_Initģʽ
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
void CCID_Init(void)
{

	Reset_CCIDStatus;
	Reset_NCCIDStatus;
	CCIDSlotStatus._byte = 0x00;							//���CCID����״̬
	CCIDNSlotStatus._Nbyte = 0x00;							//���CCID����״̬

	CCIDSlotStatus.BitCtl.Slot0ChangedState = 1;			//����SLOT0 ״̬�ı�
	CCIDNSlotStatus.NBitCtl.NSlot0ChangedState = 1;			//����SLOT0 ״̬�ı�

}
//===============================================================================//
//== �� �� ����CCID_StatePolling
//== ��    �ܣ���ѵģʽ
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
extern void open_time5_isr(void);
void CCID_StatePolling(void)
{	
	unsigned char temp=0;
	while(bDeviceState != CONFIGURED);
//	open_time5_isr();

	/* USB�豸ö�ٳɹ��������˳�CCIDģ�飬��������ģʽ */
	if(bDeviceState == CONFIGURED)
	{
        bDevice_flag=1;
		IWDG_ReloadCounter();
		temp= IFD_Slot_GetStatus();
		if(temp == ICC_NO_PRESENT)
		{
		//���γ�
			if(CCIDSlotStatus.BitCtl.Slot0CurrentState == 1)
			{
				CCIDSlotStatus.BitCtl.Slot0ChangedState = 1;	//����SLOT0״̬�ı�
				CCIDSlotStatus.BitCtl.Slot0CurrentState = 0;	//����SLOT0��ǰ״̬
			}
		}
		else
		{
		//������
			if(CCIDSlotStatus.BitCtl.Slot0CurrentState == 0)
			{
				CCIDSlotStatus.BitCtl.Slot0ChangedState = 1;	//����SLOT0״̬�ı�
				CCIDSlotStatus.BitCtl.Slot0CurrentState = 1;	//����SLOT0��ǰ״̬
			}
			if(power_flag==1)
			{
				CCIDSlotStatus._byte=0x02;
				power_flag=0;	
			}
		}

		if( (CCIDSlotStatus._byte & 0x02) !=0 )
		{
		/*	����״̬�ı�	*/
			RDR_to_PC_NotifySlotChange();
			USB_SendData(0x03,UsbIntMsgBuf,2);
			CCIDSlotStatus.BitCtl.Slot0ChangedState = 0;	//����SLOT0״̬�ı�
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
//== �� �� ����RDR_to_PC_NotifySlotChange1
//== ��    �ܣ�����λ���
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
void RDR_to_PC_NotifySlotChange1(void)
{
    UsbIntMsgBuf[0] = 0x50;

	/*  ��ȡ������״̬  */
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
//== �� �� ����IFD_14443_dec_isr
//== ��    �ܣ��ǽӴ�����λ���
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
void IFD_14443_dec_Process(void)
{

    /* ���ǽӴ�����״̬ */
	
     if(IFD_nContact_Slot_GetStatus() == ICC_NO_PRESENT)
     {
        if(CCIDNSlotStatus.NBitCtl.NSlot0CurrentState == 1)
		{
			CCIDNSlotStatus.NBitCtl.NSlot0ChangedState = 1;	//����SLOT0״̬�ı�
			CCIDNSlotStatus.NBitCtl.NSlot0CurrentState = 0;	//����SLOT0��ǰ״̬
		}
	}	
	else
	{
		   
		//������
		if(CCIDNSlotStatus.NBitCtl.NSlot0CurrentState == 0)
		{
			CCIDNSlotStatus.NBitCtl.NSlot0ChangedState = 1;	//����SLOT0״̬�ı�
			CCIDNSlotStatus.NBitCtl.NSlot0CurrentState = 1;	//����SLOT0��ǰ״̬
		}
	       
	  }
    if( (CCIDNSlotStatus._Nbyte & 0x02) !=0 )
		{
		/*	����״̬�ı�	*/
			RDR_to_PC_NotifySlotChange1();
			USB_SendData(0x05,UsbIntMsgBuf,2);
			CCIDNSlotStatus.NBitCtl.NSlot0ChangedState = 0;	//����SLOT0״̬�ı�
		}    
}	



//===============================================================================//
//== �� �� ����RDR_to_PC_NotifySlotChange
//== ��    �ܣ�����λ���
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
void RDR_to_PC_NotifySlotChange(void)
{
    UsbIntMsgBuf[0] = 0x50;

	/*  ��ȡ������״̬  */
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
//== �� �� ����usb_rev_data
//== ��    �ܣ�usb ��������	ep2
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
 void usb_rev_data(void)
{
	unsigned int counter=0;

	if(bDeviceState != CONFIGURED) return; 	//==USBû����ֱ�ӷ���

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
			R_flag=2;   //�������
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
			R_flag=2;   //�������	
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
//== �� �� ����usb_rev_data
//== ��    �ܣ�usb ��������	ep4
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
unsigned char num_SN=0;
void usb_rev_data1(void)
{
	unsigned int counter=0;

	if(bDeviceState != CONFIGURED) return; 	//==USBû����ֱ�ӷ���
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
			R_flag=2;   //�������
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
			R_flag=2;   //�������	
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
//== �� �� ����usb_snd_data
//== ��    �ܣ�usb ��������	ep1
//== ˵    ����
//== ʱ    �䣺
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
//===============================================================================//
void usb_snd_data()
{
	unsigned int pack_num;					//== ����
	unsigned int i;
		
	if(bDeviceState != CONFIGURED) return; 	//==USBû����ֱ�ӷ���

	pack_num = usbMsg.tCount % Device_Property.MaxPacketSize;

	if ( pack_num )							//== ������64�ֽڵ�������
	{
		pack_num = (usbMsg.tCount/Device_Property.MaxPacketSize) + 1;
		//== ����һ���Ķ����ֽ� ����
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









