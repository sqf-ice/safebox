/*===============================================================================//
//== 文 件 名：usb_endp.c
//== author	 ：huyx@cashwaytech.com
//== 说    明：STM32F103VCT6平台
//== 修改日志：
//== 清华同方|恒银金融有限公司
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
;** 相关全局变量
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
//	usbMsg.EP1_TX_INT_FLAG = 0;		//==发送完成产生中断标志
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
		/* 判断剩余数据长度是否超过最大包长度 */
		if(g_ep2_bulk_in_state.data_remainsize >= 0x40)
		{
			/* 剩余数据长度 超过EP2端点最大包长度 */
			USB_Write(ENDP2,g_ep2_bulk_in_state.pEndpDatbuf,0x40);

			/* 更新端点状态 */
			g_ep2_bulk_in_state.data_completesize += 0x40;
			g_ep2_bulk_in_state.pEndpDatbuf += 0x40;
			g_ep2_bulk_in_state.data_remainsize -= 0x40;
		}
		else
		{
			/* 剩余数据长度 超过EP2端点最大包长度 */
			USB_Write(ENDP2,g_ep2_bulk_in_state.pEndpDatbuf,g_ep2_bulk_in_state.data_remainsize);

			/* 更新端点状态 */
			g_ep2_bulk_in_state.data_completesize += g_ep2_bulk_in_state.data_remainsize;
			g_ep2_bulk_in_state.pEndpDatbuf += g_ep2_bulk_in_state.data_remainsize;
			g_ep2_bulk_in_state.data_remainsize = 0;
            g_ep2_bulk_in_state.data_thanse_complate = 0xff;
		}
		SetEPTxStatus(ENDP2,EP_TX_VALID);					// 允许端点发送
	}
	else
	{
		/* 剩余数据为0 */
		SetEPTxStatus(ENDP2,EP_TX_NAK);						// 不允许端点发送
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
//	usbMsg.EP3_TX_INT_FLAG = 0;		//==发送完成产生中断标志
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
		/* 判断剩余数据长度是否超过最大包长度 */
		if(g_ep4_bulk_in_state.data_remainsize >= 0x40)
		{
			/* 剩余数据长度 超过EP2端点最大包长度 */
			USB_Write(ENDP4,g_ep4_bulk_in_state.pEndpDatbuf,0x40);

			/* 更新端点状态 */
			g_ep4_bulk_in_state.data_completesize += 0x40;
			g_ep4_bulk_in_state.pEndpDatbuf += 0x40;
			g_ep4_bulk_in_state.data_remainsize -= 0x40;
		}
		else
		{
			/* 剩余数据长度 超过EP2端点最大包长度 */
			USB_Write(ENDP4,g_ep4_bulk_in_state.pEndpDatbuf,g_ep4_bulk_in_state.data_remainsize);

			/* 更新端点状态 */
			g_ep4_bulk_in_state.data_completesize += g_ep4_bulk_in_state.data_remainsize;
			g_ep4_bulk_in_state.pEndpDatbuf += g_ep4_bulk_in_state.data_remainsize;
			g_ep4_bulk_in_state.data_remainsize = 0;
            g_ep4_bulk_in_state.data_thanse_complate = 0xff;
		}
		SetEPTxStatus(ENDP4,EP_TX_VALID);					// 允许端点发送
	}
	else
	{
		/* 剩余数据为0 */
		SetEPTxStatus(ENDP4,EP_TX_NAK);						// 不允许端点发送
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
//	SetEPRxStatus(ENDP3,EP_RX_NAK);                             // 设置端点NAK
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
 ****函数名称:	USB_SendData
 ****入口参数:	uint8_t bEpNum,
 ****入口参数:	uint8_t *pTxDat,
 ****入口参数:	uint16_t wTxLen
 ****出口参数: 	uint8_t
 ****说    明: 	USB发送数据
 **************************************************************************/
uint8_t USB_SendData(uint8_t bEpNum,uint8_t *pDatBuf,uint16_t wTxLen)
{



#ifdef 0
	/* 首先判断 USB设备是否是 已配置状态 */
		if(bDeviceState != CONFIGURED)
		{
			return USB_REQ_ERROR;
		}

		bEpNum &= 0x7f;												// 过滤端点方向


		switch (bEpNum)
		{
		case ENDP2:
		{
			/* 设置EP2 端点状态以及数据指针  */
			SetEPTxStatus(ENDP2,EP_TX_NAK);
			g_ep2_bulk_in_state.data_completesize = 0;
			g_ep2_bulk_in_state.pEndpDatbuf = pDatBuf;
			g_ep2_bulk_in_state.data_remainsize = wTxLen;

			/* 判断剩余数据长度是否超过最大包长度 */
			if(g_ep2_bulk_in_state.data_remainsize >= 0x40)
			{
				/* 剩余数据长度 超过EP2端点最大包长度 */
				USB_Write(ENDP2,g_ep2_bulk_in_state.pEndpDatbuf,0x40);

				/* 更新端点状态 */
				g_ep2_bulk_in_state.data_completesize = 0x40;
				g_ep2_bulk_in_state.pEndpDatbuf += 0x40;
				g_ep2_bulk_in_state.data_remainsize -= 0x40;
				g_ep2_bulk_in_state.data_thanse_complate = 0;
			}
			else
			{
				/* 剩余数据长度 超过EP2端点最大包长度 */
				USB_Write(ENDP2,g_ep2_bulk_in_state.pEndpDatbuf,g_ep2_bulk_in_state.data_remainsize);

				/* 更新端点状态 */
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
            /* 设置EP3 端点状态以及数据指针  */
            g_ep3_int_in_state.data_completesize = 0;
            g_ep3_int_in_state.pEndpDatbuf = pDatBuf;
            g_ep3_int_in_state.data_remainsize = wTxLen;
      
                /* 剩余数据长度 小于EP3端点最大包长度 */
            USB_Write(ENDP3,g_ep3_int_in_state.pEndpDatbuf,g_ep3_int_in_state.data_remainsize);

                /* 更新端点状态 */
            g_ep3_int_in_state.data_completesize = wTxLen;
            g_ep3_int_in_state.pEndpDatbuf += wTxLen;
            g_ep3_int_in_state.data_remainsize = 0;
            g_ep3_int_in_state.data_thanse_complate = 0xff;

            SetEPTxStatus(ENDP3,EP_TX_VALID);

            break;
        }
		case ENDP4:
		{
			/* 设置EP2 端点状态以及数据指针  */
			SetEPTxStatus(ENDP4,EP_TX_NAK);
			g_ep4_bulk_in_state.data_completesize = 0;
			g_ep4_bulk_in_state.pEndpDatbuf = pDatBuf;
			g_ep4_bulk_in_state.data_remainsize = wTxLen;

			/* 判断剩余数据长度是否超过最大包长度 */
			if(g_ep4_bulk_in_state.data_remainsize >= 0x40)
			{
				/* 剩余数据长度 超过EP2端点最大包长度 */
				USB_Write(ENDP4,g_ep4_bulk_in_state.pEndpDatbuf,0x40);

				/* 更新端点状态 */
				g_ep4_bulk_in_state.data_completesize = 0x40;
				g_ep4_bulk_in_state.pEndpDatbuf += 0x40;
				g_ep4_bulk_in_state.data_remainsize -= 0x40;
				g_ep4_bulk_in_state.data_thanse_complate = 0;
			}
			else
			{
				/* 剩余数据长度 超过EP2端点最大包长度 */
				USB_Write(ENDP4,g_ep4_bulk_in_state.pEndpDatbuf,g_ep4_bulk_in_state.data_remainsize);

				/* 更新端点状态 */
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
            /* 设置EP3 端点状态以及数据指针  */
            g_ep5_int_in_state.data_completesize = 0;
            g_ep5_int_in_state.pEndpDatbuf = pDatBuf;
            g_ep5_int_in_state.data_remainsize = wTxLen;

            
                /* 剩余数据长度 小于EP3端点最大包长度 */
            USB_Write(ENDP5,g_ep5_int_in_state.pEndpDatbuf,g_ep5_int_in_state.data_remainsize);

                /* 更新端点状态 */
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


