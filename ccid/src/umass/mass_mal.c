/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : mass_mal.c
* Author             : MCD Application Team
* Version            : V3.1.1
* Date               : 04/07/2010
* Description        : Medium Access Layer interface
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
//#include "SPI_GD25Q64.h"
#include "mass_mal.h"

#include "stdio.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BlockSize            512 /* Block Size in Bytes */
//#define BlockSize            128 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)

#define NumberOfBlocks1       2  /* For Multi Blocks operation (Read/Write) */
#define MultiBufferWordsSize ((BlockSize * NumberOfBlocks1) >> 2)


unsigned   int Mass_Memory_Size[2];
unsigned   int Mass_Block_Size[2];
unsigned   int Mass_Block_Count[2];
extern unsigned char fac_id;


volatile unsigned   int Status = 0;

unsigned   int Buffer_Block_Tx[BufferWordsSize], Buffer_Block_Rx[BufferWordsSize];
unsigned   int Buffer_MultiBlock_Tx[MultiBufferWordsSize], Buffer_MultiBlock_Rx[MultiBufferWordsSize];
unsigned char dis_mem=0;
//volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;




/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*unsigned short  int MAL_Init(unsigned char lun)
{
  unsigned short  int status = MAL_OK;

  switch (lun)
  {
    case 0:
#ifdef USE_STM3210E_EVAL


#else
      MSD_Init();
#endif
      break;
#ifdef USE_STM3210E_EVAL
    case 1:
      //status = NAND_Init();
      break;
#endif
    default:
      return MAL_FAIL;
  }
  return status;
}
  */
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//unsigned short  int MAL_Write(unsigned char lun, unsigned int Memory_Offset, unsigned int *Writebuff, unsigned short  int Transfer_Length)
/*unsigned short  int MAL_Write(unsigned char lun, unsigned int Memory_Offset, unsigned char *Writebuff, unsigned short  int Transfer_Length)
{

  switch (lun)
  {
    case 0:
#ifdef USE_STM3210E_EVAL
//      SST25_W_BLOCK(Memory_Offset, Writebuff, Transfer_Length);
     
#else
 //     MSD_WriteBlock((unsigned char*)Writebuff, Memory_Offset, Transfer_Length);
#endif
      break;
#ifdef USE_STM3210E_EVAL

#endif
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}  */

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
//unsigned short  int MAL_Read(unsigned char lun, unsigned int Memory_Offset, unsigned int *Readbuff, unsigned short  int Transfer_Length)
/*unsigned short  int MAL_Read(unsigned char lun, unsigned int Memory_Offset, unsigned char *Readbuff, unsigned short  int Transfer_Length)
{

  switch (lun)
  {
    case 0:
#ifdef USE_STM3210E_EVAL
  //    SST25_R_BLOCK(Memory_Offset, Readbuff, Transfer_Length);
     
#else
   //   MSD_ReadBlock((unsigned char*)Readbuff, Memory_Offset, Transfer_Length);
#endif
      break;
#ifdef USE_STM3210E_EVAL

#endif
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}  */

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short  int MAL_GetStatus (unsigned char lun)
{

  unsigned char fac_id;
  if (lun == 0)
  {
	 // fac_id = SPI_ExFlash_GetCapcity();
	 fac_id=0;
	  if(fac_id==0x40){
	  	Mass_Block_Size[0]=4096;
		Mass_Block_Count[0]=512;	
	  }
     
	  if(dis_mem==0){						  //显示TF卡的容量	由于是周期性更新状态，所以，加了条件，只显示一次容量值 
		Mass_Memory_Size[0] = Mass_Block_Count[0] * Mass_Block_Size[0];
      	dis_mem=1;
	  }
	  return MAL_OK;

  }

  
  return MAL_FAIL;
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
