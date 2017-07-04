/*===============================================================================//
//== �� �� ����usb_endp.c
//== author	 ��huyx@cashwaytech.com
//== ˵    ����STM32F103VCT6ƽ̨
//== �޸���־��
//== �廪ͬ��|�����������޹�˾
//===============================================================================*/

#include "usb_lib.h"
#include "usb_istr.h" 
#include "usb_endp.h"
#include "global.h"
#include "usb_pwr.h"
#include "ccid.h"



extern void Mass_Storage_In (void);
extern void Mass_Storage_Out (void);


extern struct _UsbMod usbMsg;	//==usb_hid.c
extern void usb_rev_data(void);	//==usb_hid.c
extern void usb_rev_data1(void);
uint8_t USB_SendData(uint8_t bEpNum,uint8_t *pDatBuf,uint16_t wTxLen);                
/*************************************************************************
;** ���ȫ�ֱ���
 *************************************************************************/
//ENDP_STATUS_T	g_ep1_bulk_out_state;
ENDP_STATUS_T	g_ep2_bulk_in_state;
ENDP_STATUS_T	g_ep3_int_in_state;
//ENDP_STATUS_T	g_ep4_bulk_out_state;
ENDP_STATUS_T	g_ep4_bulk_in_state;
ENDP_STATUS_T	g_ep5_int_in_state;
/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{	
//	usbMsg.EP1_TX_INT_FLAG = 0;		//==������ɲ����жϱ�־
//  	usbMsg.tCount =0; 
//	NOP_Process();

 Mass_Storage_In();

}

/*******************************************************************************
* Function Name  : EP2_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_IN_Callback(void)
{
//	NOP_Process();

#ifdef 0
	 if(g_ep2_bulk_in_state.data_thanse_complate == 0x00)
    {
		/* �ж�ʣ�����ݳ����Ƿ񳬹��������� */
		if(g_ep2_bulk_in_state.data_remainsize >= 0x40)
		{
			/* ʣ�����ݳ��� ����EP2�˵��������� */
			USB_Write(ENDP2,g_ep2_bulk_in_state.pEndpDatbuf,0x40);

			/* ���¶˵�״̬ */
			g_ep2_bulk_in_state.data_completesize += 0x40;
			g_ep2_bulk_in_state.pEndpDatbuf += 0x40;
			g_ep2_bulk_in_state.data_remainsize -= 0x40;
		}
		else
		{
			/* ʣ�����ݳ��� ����EP2�˵��������� */
			USB_Write(ENDP2,g_ep2_bulk_in_state.pEndpDatbuf,g_ep2_bulk_in_state.data_remainsize);

			/* ���¶˵�״̬ */
			g_ep2_bulk_in_state.data_completesize += g_ep2_bulk_in_state.data_remainsize;
			g_ep2_bulk_in_state.pEndpDatbuf += g_ep2_bulk_in_state.data_remainsize;
			g_ep2_bulk_in_state.data_remainsize = 0;
            g_ep2_bulk_in_state.data_thanse_complate = 0xff;
		}
		SetEPTxStatus(ENDP2,EP_TX_VALID);					// ����˵㷢��
	}
	else
	{
		/* ʣ������Ϊ0 */
		SetEPTxStatus(ENDP2,EP_TX_NAK);						// ������˵㷢��
	}
#endif  

}
/*******************************************************************************
* Function Name  : EP3_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_IN_Callback(void)
{
//	usbMsg.EP3_TX_INT_FLAG = 0;		//==������ɲ����жϱ�־
	NOP_Process();


}
/*******************************************************************************
* Function Name  : EP4_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP4_IN_Callback(void)
{

#ifdef 0

	 if(g_ep4_bulk_in_state.data_thanse_complate == 0x00)
    {
		/* �ж�ʣ�����ݳ����Ƿ񳬹��������� */
		if(g_ep4_bulk_in_state.data_remainsize >= 0x40)
		{
			/* ʣ�����ݳ��� ����EP2�˵��������� */
			USB_Write(ENDP4,g_ep4_bulk_in_state.pEndpDatbuf,0x40);

			/* ���¶˵�״̬ */
			g_ep4_bulk_in_state.data_completesize += 0x40;
			g_ep4_bulk_in_state.pEndpDatbuf += 0x40;
			g_ep4_bulk_in_state.data_remainsize -= 0x40;
		}
		else
		{
			/* ʣ�����ݳ��� ����EP2�˵��������� */
			USB_Write(ENDP4,g_ep4_bulk_in_state.pEndpDatbuf,g_ep4_bulk_in_state.data_remainsize);

			/* ���¶˵�״̬ */
			g_ep4_bulk_in_state.data_completesize += g_ep4_bulk_in_state.data_remainsize;
			g_ep4_bulk_in_state.pEndpDatbuf += g_ep4_bulk_in_state.data_remainsize;
			g_ep4_bulk_in_state.data_remainsize = 0;
            g_ep4_bulk_in_state.data_thanse_complate = 0xff;
		}
		SetEPTxStatus(ENDP4,EP_TX_VALID);					// ����˵㷢��
	}
	else
	{
		/* ʣ������Ϊ0 */
		SetEPTxStatus(ENDP4,EP_TX_NAK);						// ������˵㷢��
	}
#endif


}
/*******************************************************************************
* Function Name  : EP5_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP5_IN_Callback(void)
{
	NOP_Process();
}

/*******************************************************************************
* Function Name  : EP6_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP6_IN_Callback(void)
{
	NOP_Process();
}
/*******************************************************************************
* Function Name  : EP7_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP7_IN_Callback(void)
{
	NOP_Process();
}

/*******************************************************************************
* Function Name  : EP1_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
	NOP_Process();
	
} 

/*******************************************************************************
* Function Name  : EP2_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_OUT_Callback(void)
{ 	
//	SetEPRxStatus(ENDP3,EP_RX_NAK);                             // ���ö˵�NAK
// 	usbMsg.EP2_RX_INT_FLAG=0;
//	usb_rev_data();
	
}

/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
   
//   search_typea();
   Mass_Storage_Out();
}

/*******************************************************************************
* Function Name  : EP4_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP4_OUT_Callback(void)
{
	usb_rev_data1();
}

/*******************************************************************************
* Function Name  : EP5_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP5_OUT_Callback(void)
{
	NOP_Process();
}

/*******************************************************************************
* Function Name  : EP6_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP6_OUT_Callback(void)
{
	NOP_Process();
}

/*******************************************************************************
* Function Name  : EP7_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP7_OUT_Callback(void)
{
	NOP_Process();
}

/**************************************************************************
 ****��������:	USB_SendData
 ****��ڲ���:	uint8_t bEpNum,
 ****��ڲ���:	uint8_t *pTxDat,
 ****��ڲ���:	uint16_t wTxLen
 ****���ڲ���: 	uint8_t
 ****˵    ��: 	USB��������
 **************************************************************************/
uint8_t USB_SendData(uint8_t bEpNum,uint8_t *pDatBuf,uint16_t wTxLen)
{



#ifdef 0
	/* �����ж� USB�豸�Ƿ��� ������״̬ */
		if(bDeviceState != CONFIGURED)
		{
			return USB_REQ_ERROR;
		}

		bEpNum &= 0x7f;												// ���˶˵㷽��


		switch (bEpNum)
		{
		case ENDP2:
		{
			/* ����EP2 �˵�״̬�Լ�����ָ��  */
			SetEPTxStatus(ENDP2,EP_TX_NAK);
			g_ep2_bulk_in_state.data_completesize = 0;
			g_ep2_bulk_in_state.pEndpDatbuf = pDatBuf;
			g_ep2_bulk_in_state.data_remainsize = wTxLen;

			/* �ж�ʣ�����ݳ����Ƿ񳬹��������� */
			if(g_ep2_bulk_in_state.data_remainsize >= 0x40)
			{
				/* ʣ�����ݳ��� ����EP2�˵��������� */
				USB_Write(ENDP2,g_ep2_bulk_in_state.pEndpDatbuf,0x40);

				/* ���¶˵�״̬ */
				g_ep2_bulk_in_state.data_completesize = 0x40;
				g_ep2_bulk_in_state.pEndpDatbuf += 0x40;
				g_ep2_bulk_in_state.data_remainsize -= 0x40;
				g_ep2_bulk_in_state.data_thanse_complate = 0;
			}
			else
			{
				/* ʣ�����ݳ��� ����EP2�˵��������� */
				USB_Write(ENDP2,g_ep2_bulk_in_state.pEndpDatbuf,g_ep2_bulk_in_state.data_remainsize);

				/* ���¶˵�״̬ */
				g_ep2_bulk_in_state.data_completesize = wTxLen;
				g_ep2_bulk_in_state.pEndpDatbuf += wTxLen;
				g_ep2_bulk_in_state.data_remainsize = 0;
				g_ep2_bulk_in_state.data_thanse_complate = 0xff;
			}
			SetEPTxStatus(ENDP2,EP_TX_VALID);
			break;
		}

		/*******************************************************/
        case ENDP3:
        {
            /* ����EP3 �˵�״̬�Լ�����ָ��  */
            g_ep3_int_in_state.data_completesize = 0;
            g_ep3_int_in_state.pEndpDatbuf = pDatBuf;
            g_ep3_int_in_state.data_remainsize = wTxLen;
      
                /* ʣ�����ݳ��� С��EP3�˵��������� */
            USB_Write(ENDP3,g_ep3_int_in_state.pEndpDatbuf,g_ep3_int_in_state.data_remainsize);

                /* ���¶˵�״̬ */
            g_ep3_int_in_state.data_completesize = wTxLen;
            g_ep3_int_in_state.pEndpDatbuf += wTxLen;
            g_ep3_int_in_state.data_remainsize = 0;
            g_ep3_int_in_state.data_thanse_complate = 0xff;

            SetEPTxStatus(ENDP3,EP_TX_VALID);

            break;
        }
		case ENDP4:
		{
			/* ����EP2 �˵�״̬�Լ�����ָ��  */
			SetEPTxStatus(ENDP4,EP_TX_NAK);
			g_ep4_bulk_in_state.data_completesize = 0;
			g_ep4_bulk_in_state.pEndpDatbuf = pDatBuf;
			g_ep4_bulk_in_state.data_remainsize = wTxLen;

			/* �ж�ʣ�����ݳ����Ƿ񳬹��������� */
			if(g_ep4_bulk_in_state.data_remainsize >= 0x40)
			{
				/* ʣ�����ݳ��� ����EP2�˵��������� */
				USB_Write(ENDP4,g_ep4_bulk_in_state.pEndpDatbuf,0x40);

				/* ���¶˵�״̬ */
				g_ep4_bulk_in_state.data_completesize = 0x40;
				g_ep4_bulk_in_state.pEndpDatbuf += 0x40;
				g_ep4_bulk_in_state.data_remainsize -= 0x40;
				g_ep4_bulk_in_state.data_thanse_complate = 0;
			}
			else
			{
				/* ʣ�����ݳ��� ����EP2�˵��������� */
				USB_Write(ENDP4,g_ep4_bulk_in_state.pEndpDatbuf,g_ep4_bulk_in_state.data_remainsize);

				/* ���¶˵�״̬ */
				g_ep4_bulk_in_state.data_completesize = wTxLen;
				g_ep4_bulk_in_state.pEndpDatbuf += wTxLen;
				g_ep4_bulk_in_state.data_remainsize = 0;
				g_ep4_bulk_in_state.data_thanse_complate = 0xff;
			}
			SetEPTxStatus(ENDP4,EP_TX_VALID);
			break;
		}
		case ENDP5:
        {
            /* ����EP3 �˵�״̬�Լ�����ָ��  */
            g_ep5_int_in_state.data_completesize = 0;
            g_ep5_int_in_state.pEndpDatbuf = pDatBuf;
            g_ep5_int_in_state.data_remainsize = wTxLen;

            
                /* ʣ�����ݳ��� С��EP3�˵��������� */
            USB_Write(ENDP5,g_ep5_int_in_state.pEndpDatbuf,g_ep5_int_in_state.data_remainsize);

                /* ���¶˵�״̬ */
            g_ep5_int_in_state.data_completesize = wTxLen;
            g_ep5_int_in_state.pEndpDatbuf += wTxLen;
            g_ep5_int_in_state.data_remainsize = 0;
            g_ep5_int_in_state.data_thanse_complate = 0xff;

            SetEPTxStatus(ENDP5,EP_TX_VALID);

            break;
        }
        default:
			break;
		}
		Set_bBulkInCompeleteFlag;
		return USB_REQ_SUCCESS;

#endif
		return 0;
} /* End of USB_SendData	*/


