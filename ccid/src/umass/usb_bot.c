/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_bot.c
* Author             : MCD Application Team
* Version            : V3.1.1
* Date               : 04/07/2010
* Description        : BOT State Machine management
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_scsi.h"
#include "hw_config.h"
#include "usb_regs.h"
#include "usb_mem.h"
#include "usb_conf.h"
#include "usb_bot.h"
#include "usb_disk.h"
//#include "memory.h"
#include "usb_lib.h"
//#include "SPI_GD25Q64.h"
#include "headfile.h"
#include "globle.h"
#include "mem.h"
#include "usb_support.h"
#include "umassfunc.h"






u32 Max_Lun	= 0;
unsigned char CWBotState=0;
unsigned int CWBotDataLen=0;
unsigned char * CWBotsendPtr;
unsigned char * CWBotRecievePtr;
extern void SetCWBotrecievePtr(void);
void  ProcessUDiskWrite(void);


//---------------------------------------------------------------------------------------------------
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Bot_State;
uint8_t Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  /* data buffer*/
uint16_t Data_Len;
Bulk_Only_CBW CBW;
Bulk_Only_CSW CSW;
unsigned int SCSI_LBA , SCSI_BlkLen;
extern uint32_t Max_Lun;
extern void API_Instuction_Hook(void);
unsigned char g_cswStatus;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/





void SendUSBBotDataService(void)
{


			 if(SCSI_BlkLen)
			 {
			 	 USB_SIL_Write(EP1_IN, (uint8_t *)CWBotsendPtr , 64);
	  	         SetEPTxCount(ENDP1,64);
                 SetEPTxStatus(ENDP1, EP_TX_VALID);
				 CWBotsendPtr+=64;
			 	 Bot_State = BOT_DATA_IN;
				 SCSI_BlkLen--;
				 //CWBotState=0x03;
			 }
			 else
			 {


				 if(CWBotDataLen!=0)
				 {
			 	     USB_SIL_Write(EP1_IN, (uint8_t *)CWBotsendPtr , CWBotDataLen);	 
	  	             SetEPTxCount(ENDP1,(uint16_t) CWBotDataLen);  
                     SetEPTxStatus(ENDP1, EP_TX_VALID);
			   													
			 	     Bot_State = BOT_DATA_IN_LAST;					}
				 else
				 {
				   
				     Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
				 }

			 
			 }


}


/*******************************************************************************
* Function Name  : Mass_Storage_In
* Description    : Mass Storage IN transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Mass_Storage_In (void)
{
    
  switch (Bot_State)
  {											
    case BOT_CSW_Send:
    case BOT_ERROR:
      Bot_State = BOT_IDLE;
    #ifndef STM32F10X_CL
      SetEPRxStatus(ENDP3, EP_RX_VALID);/* enable the Endpoint to recive the next cmd*/
    #else
      if (GetEPRxStatus(EP3_OUT) == EP_RX_STALL)
      {
        SetEPRxStatus(EP3_OUT, EP_RX_VALID);/* enable the Endpoint to recive the next cmd*/
      }
    #endif /* STM32F10X_CL */
      break;
    case BOT_DATA_IN:
      switch (CBW.CB[0])
	  {
	     case 0xFE:
	     case 0xFA:
		   if(CWBotState==0x02)
		   {
		        
	          SendUSBBotDataService();
		   }	 
		     break;
		 		   
	     case SCSI_READ10:
	          SCSI_Read10_Cmd(CBW.bLUN , SCSI_LBA , SCSI_BlkLen);
	          break;
		
      }
      break;
    case BOT_DATA_IN_LAST:
	    CWBotState=0;
      Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
    #ifndef STM32F10X_CL
      SetEPRxStatus(ENDP3, EP_RX_VALID);
    #else
      if (GetEPRxStatus(EP3_OUT) == EP_RX_STALL)
      {
        SetEPRxStatus(EP3_OUT, EP_RX_VALID);/* enable the Endpoint to recive the next cmd*/
      }      
    #endif /* STM32F10X_CL */
      break;

    default:
      break;
  }
}

/*******************************************************************************
* Function Name  : Mass_Storage_Out
* Description    : Mass Storage OUT transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

extern unsigned char Rx_Succ;
unsigned char rx_buf[32];
//extern void NRF24L01_TXBUF(uint8_t* data_buffer, uint8_t Nb_bytes);



extern  unsigned char Flashcmd[5];










extern unsigned char UsbBulkMsgBuf[271];	 //与USB通信的缓存buf




void Mass_Storage_Out (void)
{
  uint8_t CMD;
  unsigned short wlen,i;
  
//		     search_typea();
  //search_typea();
  CMD = CBW.CB[0];
    // USART_OUT(USART1,"Mass_Storage_Out"); 
  Data_Len = USB_SIL_Read(EP3_OUT, Bulk_Data_Buff);
	
  switch (Bot_State)
  {
    case BOT_IDLE:
	//USART_OUT(USART1,"Mass_Storage_Out2"); 
      CBW_Decode();
      break;
    case BOT_DATA_OUT:
		 
	  if(CWBotState==0x01)
	  {

		 		
		 
		 APL_USBMemcpy((&gs_cmd.tmp2usb[0]+CWBotDataLen),Bulk_Data_Buff,Data_Len);
		 CWBotDataLen += Data_Len;
		 
		 if(CWBotDataLen<SCSI_BlkLen)
		 {	
		 
		 				Bot_State = BOT_DATA_OUT;
				SetEPRxStatus(ENDP3, EP_RX_VALID); 
		     return;

		 } 
		  
         gs_cmd.len  = WordHtoL(gs_cmd.len);




        if (gs_cmd.tmp2usb[0] != 'C') 
        {
             g_cos_sw = SW6700;
             goto __LableReturn;
        }

        
		wlen = MYREG16(&gs_cmd.tmp2usb[1]);
        if ((wlen < 7) || (wlen != SCSI_BlkLen - 3))
        {
            g_cos_sw = SW6700;
             goto __LableReturn;
        }


		if (wlen > 7)
        {
          // gb_apdu_flag = 1;        
 //          if ((wlen - 7) != gs_cmd.len)
           {
  //              g_cos_sw = SW6700;
  //             goto __LableReturn;
           }
//		   	APL_USBMemcpy(UsbBulkMsgBuf, (unsigned char *)(&gs_cmd.head[0]), wlen);
       }
       else
       {
                 
       }
	
//	  	APL_USBMemcpy(UsbBulkMsgBuf, (unsigned char *)(&(gs_cmd.tmp2usb+3)), wlen);

		APL_USBMemcpy((unsigned char  *)(&ga_cmdhead.cla), (unsigned char *)(&gs_cmd.head[0]), 7);
		APL_USBMemset((unsigned char *)gs_cmd.tmp2usb, 0, 10);

	



		API_Instuction_Hook();

__LableReturn:

			CWBotState=0x00;
			//SetEPTxCount(ENDP1, BULK_MAX_PACKET_SIZE);
            //SetEPTxStatus(ENDP1, EP_TX_VALID);
		//	Bot_State = BOT_DATA_IN_LAST;
			Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
			break;

	  }	 
	 else if(CWBotState==0x03)
	  {
	  	  APL_USBMemcpy((CWBotRecievePtr+CWBotDataLen),Bulk_Data_Buff,Data_Len);
		  CWBotDataLen += Data_Len;
	  	 if(CWBotDataLen<SCSI_BlkLen)
		 {	
		 
		 		Bot_State = BOT_DATA_OUT;
				SetEPRxStatus(ENDP3, EP_RX_VALID); 
		     return;

		 } 
      
	     ProcessUDiskWrite();	  

		 CWBotState=0x00;
	     Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
		 break;

	  }


      if (CMD == SCSI_WRITE10)
      {
        SCSI_Write10_Cmd(CBW.bLUN , SCSI_LBA , SCSI_BlkLen);
        break;
      }
      Bot_Abort(DIR_OUT);
      Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
      Set_CSW (CSW_PHASE_ERROR, SEND_CSW_DISABLE);
      break;
    default:
      Bot_Abort(BOTH_DIR);
      Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
      Set_CSW (CSW_PHASE_ERROR, SEND_CSW_DISABLE);
      break;
  }
}








/*******************************************************************************
* Function Name  : CBW_Decode
* Description    : Decode the received CBW and call the related SCSI command
*                 routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/



unsigned char sendData[16];
extern unsigned long  udisk_lba_param[2];
#define INDEX_UDISK_LBA_START                       0
#define INDEX_UDISK_LBA_XFER_COUNT                  1


extern unsigned char udisk_read(void);
extern unsigned char udisk_write(void);
extern unsigned char  udisk_erasechip(void);

extern SetCWBotDataLenAndSCSI_BlkLen(unsigned int tempLen);
extern unsigned char g_FunType;



void  ProcessUDiskWrite(void)
{

		unsigned short calcint;
	
		udisk_lba_param[INDEX_UDISK_LBA_START] <<= 1;//udisk_lba_param[INDEX_UDISK_LBA_START] <<= 9;
		udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] <<= 1; //512/256

				//calcint = 0x2000;

	    calcint = (UINT16)(udisk_lba_param[INDEX_UDISK_LBA_START] + udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT]);
				//calcint>>8;
				if (calcint > 0x4000)//if ((calcint) > SIZE_OF_CN_LIB)
				{
					//calcint = 0;//usb_data_split();
					return;
				}
	
				udisk_lba_param[INDEX_UDISK_LBA_START] <<= 8;
							  // SPI_Flash_Init();	
                g_cswStatus = udisk_write(); 

				//  udisk_read();
}





void CBW_Decode(void)
{
  uint32_t Counter;
//  	uint32_t	tlenth;
	//unsigned char  temp;
//	 unsigned char ts;

	
  for (Counter = 0; Counter < Data_Len; Counter++)
  {
    *((uint8_t *)&CBW + Counter) = Bulk_Data_Buff[Counter];
  }
  CSW.dTag = CBW.dTag;
  CSW.dDataResidue = CBW.dDataLength;
  if (Data_Len != BOT_CBW_PACKET_LENGTH)
  {
    Bot_Abort(BOTH_DIR);
    /* reset the CBW.dSignature to desible the clear feature until receiving a Mass storage reset*/
    CBW.dSignature = 0;
    Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, PARAMETER_LIST_LENGTH_ERROR);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    return;
  }

  if ((CBW.CB[0] == SCSI_READ10 ) || (CBW.CB[0] == SCSI_WRITE10 ))
  {
    /* Calculate Logical Block Address */
    SCSI_LBA = (CBW.CB[2] << 24) | (CBW.CB[3] << 16) | (CBW.CB[4] <<  8) | CBW.CB[5];
    /* Calculate the Number of Blocks to transfer */
    SCSI_BlkLen = (CBW.CB[7] <<  8) | CBW.CB[8];
  }

  if (CBW.dSignature == BOT_CBW_SIGNATURE)
  {
    /* Valid CBW */

	if (CBW.CB[0] >= 0xFA)
	{
		if(CBW.CB[0]==0xFA)
		{

		   //  Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
			 if(CBW.CB[1]==0x3A)
			 {

				SCSI_BlkLen =   CBW.dDataLength;

				CWBotDataLen=0;
				CWBotState=0x01;
				Bot_State = BOT_DATA_OUT;
				SetEPRxStatus(ENDP3, EP_RX_VALID);
				return;
			 }
			 if(CBW.CB[1]==0x08)
			 {

				CWBotState=0x02;


				if (g_cos_sw)
                {
                    g_bSW = SW6700;
                    gb_Le = 0;
                    g_cos_sw = 0;
                }

				
			  API_MakeData();

			  SendUSBBotDataService();

			//	SetEPTxCount(ENDP1, BULK_MAX_PACKET_SIZE);
            //  SetEPTxStatus(ENDP1, EP_TX_VALID);
					
				return;
			 }		
		
		}
		else if(CBW.CB[0]==0xFF) 
		{
		 /*	temp =  SPI_ExFlash_GetCapcity();
			sendData[0]=temp;
		  	
			USB_SIL_Write(EP1_IN, (uint8_t *)sendData , 1);
	  	    SetEPTxCount(ENDP1,(uint16_t) 1);
            SetEPTxStatus(ENDP1, EP_TX_VALID);

			Bot_State = BOT_DATA_IN_LAST;
			return;
		*/
			
			 if (CBW.CB[9]  == 0x01)
			 {
			 
                 sendData[0] = 0x00; //udisk_getcapacity();
			 
				 if (g_FunType & FUN_LCD)
				 {
					sendData[0] = 0x0A;
				 }
				
				 CWBotsendPtr = 	sendData;
				 CWBotDataLen = 1;	
				 SCSI_BlkLen= 0;
                 
				 SendUSBBotDataService();
							
			 
			 }
			 else if(CBW.CB[9]  == 0x10) 
			 {

				 if (g_FunType & FUN_LCD)
				 {
					    //ExFlashCapacity = udisk_getcapacity() << 1;//the total block count of ex flash
					    //Spi_Chip_Erase();
	         	        //g_cswStatus = 0; //ExFlash_erase_block(SIZE_OF_CN_LIB_COMPRESS, BLOCK_CNT_OF_CD_ROM);
					    // ExFlash_erase_block(SIZE_OF_CN_LIB,(ExFlashCapacity - BLOCK_CNT_OF_CN_LIB));

				 } else 
				 {
					  g_cswStatus = udisk_erasechip();
	             }				 
			 
			 
			 }

		} 
        else if(CBW.CB[0]==0xFC) 
		{
		
            udisk_lba_param[INDEX_UDISK_LBA_START]  = (CBW.CB[2]<<24) + (CBW.CB[3]<<16) +(CBW.CB[4]<<8) +CBW.CB[5];
			udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] = CBW.CB[8];
		
           if(CBW.CB[9] == 0x10)
			{

			     SPI_Flash_Init();
			     g_cswStatus = ExFlash_erase_block(0, BLOCK_CNT_OF_CN_LIB);
				 //udisk_erasechip();
             	 Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
			}
			else 
			{
			
				
				SCSI_BlkLen =   CBW.dDataLength;

				CWBotDataLen=0;
				CWBotState=0x03;
				Bot_State = BOT_DATA_OUT;
				SetCWBotrecievePtr();
				SetEPRxStatus(ENDP3, EP_RX_VALID);
				
			
				

			
			}

			return;
		
		}
		else
		{
		
		 	udisk_lba_param[INDEX_UDISK_LBA_START]  = (CBW.CB[2]<<24) + (CBW.CB[3]<<16) +(CBW.CB[4]<<8) +CBW.CB[5];
			udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] = CBW.CB[8];
			//scsiRdWtDone = 0;		
			//udisk_lba_param[INDEX_UDISK_LBA_START] <<= 9;
			//udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] <<= 1; //512/256  
			if (g_FunType & FUN_LCD)
			            udisk_lba_param[INDEX_UDISK_LBA_START] += SIZE_OF_CN_LIB_COMPRESS;

			if (CBW.CB[0] == 0xFD)
			{ 									//0xFD  Write CD-ROM
			//	g_cswStatus = udisk_write();
				//if (g_USB_BOT_StateMachine != BOT_SPLIT_DATA_IN)
					//g_USB_BOT_StateMachine = BOT_SDATA_OUT;
			}else
			{		
			
			
			      SPI_Flash_Init();										 							//Read CD-ROM
				 g_cswStatus = udisk_read();

				 SetCWBotDataLenAndSCSI_BlkLen(512);

				 CWBotState=0x02;
				 SendUSBBotDataService();
				//if (g_USB_BOT_StateMachine != BOT_SPLIT_DATA_OUT)
					//g_USB_BOT_StateMachine = BOT_SDATA_IN;
			}	
			
			return;
		
		} 


	}




	//#endif

    if ((CBW.bLUN > Max_Lun) || (CBW.bCBLength > 16))
    {
      Bot_Abort(BOTH_DIR);
      Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
      Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    }
	else if(CBW.bCBLength < 1)
	{
	
		 Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
	
	
	}
    else
    {
      switch (CBW.CB[0])
      {
        case SCSI_REQUEST_SENSE:
          SCSI_RequestSense_Cmd (CBW.bLUN);
          break;
        case SCSI_INQUIRY:
          SCSI_Inquiry_Cmd(CBW.bLUN);
          break;
        case SCSI_START_STOP_UNIT:
          SCSI_Start_Stop_Unit_Cmd(CBW.bLUN);
          break;
        case SCSI_ALLOW_MEDIUM_REMOVAL:
          SCSI_Start_Stop_Unit_Cmd(CBW.bLUN);
          break;
        case SCSI_MODE_SENSE6:
          SCSI_ModeSense6_Cmd (CBW.bLUN);
          break;
        case SCSI_MODE_SENSE10:
          SCSI_ModeSense10_Cmd (CBW.bLUN);
          break;
        case SCSI_READ_FORMAT_CAPACITIES:
          SCSI_ReadFormatCapacity_Cmd(CBW.bLUN);
          break;
        case SCSI_READ_CAPACITY10:
          SCSI_ReadCapacity10_Cmd(CBW.bLUN);
          break;
        case SCSI_TEST_UNIT_READY:
          SCSI_TestUnitReady_Cmd(CBW.bLUN);
          break;
        case SCSI_READ10:
          SCSI_Read10_Cmd(CBW.bLUN, SCSI_LBA , SCSI_BlkLen);
          break;
        case SCSI_WRITE10:
          SCSI_Write10_Cmd(CBW.bLUN, SCSI_LBA , SCSI_BlkLen);
          break;
        case SCSI_VERIFY10:
          SCSI_Verify10_Cmd(CBW.bLUN);
          break;
        case SCSI_FORMAT_UNIT:
          SCSI_Format_Cmd(CBW.bLUN);
          break;
          /*Unsupported command*/

        case SCSI_MODE_SELECT10:
          SCSI_Mode_Select10_Cmd(CBW.bLUN);
          break;
        case SCSI_MODE_SELECT6:
          SCSI_Mode_Select6_Cmd(CBW.bLUN);
          break;

        case SCSI_SEND_DIAGNOSTIC:
          SCSI_Send_Diagnostic_Cmd(CBW.bLUN);
          break;
        case SCSI_READ6:
          SCSI_Read6_Cmd(CBW.bLUN);
          break;
        case SCSI_READ12:
          SCSI_Read12_Cmd(CBW.bLUN);
          break;
        case SCSI_READ16:
          SCSI_Read16_Cmd(CBW.bLUN);
          break;
        case SCSI_READ_CAPACITY16:
          SCSI_READ_CAPACITY16_Cmd(CBW.bLUN);
          break;
        case SCSI_WRITE6:
          SCSI_Write6_Cmd(CBW.bLUN);
          break;
        case SCSI_WRITE12:
          SCSI_Write12_Cmd(CBW.bLUN);
          break;
        case SCSI_WRITE16:
          SCSI_Write16_Cmd(CBW.bLUN);
          break;
        case SCSI_VERIFY12:
          SCSI_Verify12_Cmd(CBW.bLUN);
          break;
        case SCSI_VERIFY16:
          SCSI_Verify16_Cmd(CBW.bLUN);
          break;

        default:
        {
          Bot_Abort(BOTH_DIR);
          Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_COMMAND);
          Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
        }
      }
    }
  }
  else
  {
    /* Invalid CBW */
    Bot_Abort(BOTH_DIR);
    Set_Scsi_Sense_Data(CBW.bLUN, ILLEGAL_REQUEST, INVALID_COMMAND);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
  }
}

/*******************************************************************************
* Function Name  : Transfer_Data_Request
* Description    : Send the request response to the PC HOST.
* Input          : uint8_t* Data_Address : point to the data to transfer.
*                  uint16_t Data_Length : the nember of Bytes to transfer.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Transfer_Data_Request(uint8_t* Data_Pointer, uint16_t Data_Len)
{
  USB_SIL_Write(EP1_IN, Data_Pointer, Data_Len);

  SetEPTxStatus(ENDP1, EP_TX_VALID);
  Bot_State = BOT_DATA_IN_LAST;
  CSW.dDataResidue -= Data_Len;
  CSW.bStatus = CSW_CMD_PASSED;
}

/*******************************************************************************
* Function Name  : Set_CSW
* Description    : Set the SCW with the needed fields.
* Input          : uint8_t CSW_Status this filed can be CSW_CMD_PASSED,CSW_CMD_FAILED,
*                  or CSW_PHASE_ERROR.
* Output         : None.
* Return         : No
ne.
*******************************************************************************/
void Set_CSW (uint8_t CSW_Status, uint8_t Send_Permission)
{
  CSW.dSignature = BOT_CSW_SIGNATURE;
  CSW.bStatus = CSW_Status;

  USB_SIL_Write(EP1_IN, ((uint8_t *)& CSW), CSW_DATA_LENGTH);

  Bot_State = BOT_ERROR;
  if (Send_Permission)
  {
    Bot_State = BOT_CSW_Send;
    SetEPTxStatus(ENDP1, EP_TX_VALID);
  }

}

/*******************************************************************************
* Function Name  : Bot_Abort
* Description    : Stall the needed Endpoint according to the selected direction.
* Input          : Endpoint direction IN, OUT or both directions
* Output         : None.
* Return         : None.
*******************************************************************************/
void Bot_Abort(uint8_t Direction)
{
  switch (Direction)
  {
    case DIR_IN :
      SetEPTxStatus(ENDP1, EP_TX_STALL);
      break;
    case DIR_OUT :
      SetEPRxStatus(ENDP3, EP_RX_STALL);
      break;
    case BOTH_DIR :
      SetEPTxStatus(ENDP1, EP_TX_STALL);
      SetEPRxStatus(ENDP3, EP_RX_STALL);
      break;
    default:
      break;
  }
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
