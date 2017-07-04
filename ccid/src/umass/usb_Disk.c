#include"usb_Disk.h"

#define xdata     

extern BYTE APL_ExFlash_GetCapcity(void);
extern void APL_ExFlash_EraseBlock(unsigned long BlockAddr);
extern void Spi_Chip_Erase(void);
extern BYTE APL_ExFlash_WriteData(unsigned long PageAddr, BYTE xdata *pbString, UINT16 Len);
extern void APL_ExFlash_ReadData (unsigned long ReadAddr,unsigned char xdata* DataAddr,unsigned int n); 



extern void  BULK_SENDSET(unsigned char * buf, unsigned int len);
extern void BULK_RCVSET(unsigned char * buf,unsigned short len);
extern void SetCWBotDataLenAndSCSI_BlkLen(unsigned int tempLen);
extern void SetCWBotsendPtr(void);
extern unsigned char * CWBotsendPtr;
extern unsigned char * CWBotRecievePtr;
extern		   void  SPI_Delay(void) ;
extern void SetCWBotrecievePtr(void);
unsigned char InBuff[512];


unsigned char udisk_read(void) //unsigned char udisk_read(unsigned long *udisk_lba_param)
{
	unsigned short i,t;
  if (udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT])
	{
   		//if(g_FunType & FUN_CD_ROM)
		  {	
			//Spi_Read_Fast(udisk_lba_param[INDEX_UDISK_LBA_START], BULK_BUFFER, SIZE_OF_SPIFLASH_PAGE);
				SPI_Flash_Init();	 
		     APL_ExFlash_ReadData(udisk_lba_param[INDEX_UDISK_LBA_START], InBuff, 256);	
			//		 SPI_Delay();
			   APL_ExFlash_ReadData(udisk_lba_param[INDEX_UDISK_LBA_START]+256, InBuff+256, 256);	
			//		 SPI_Delay();
		/*			
		      
					 APL_ExFlash_EraseBlock(0);
			   for(i=0;i<256;i++)
			   {
			        InBuff[i]=i;
			   }

			    APL_ExFlash_WriteData(0, InBuff, 256);

				 // SPI_Flash_Init();
				 			   for(i=0;i<256;i++)
			   {
			        InBuff[i]=0xFF;
			   }
				  
			   APL_ExFlash_ReadData(0, InBuff, 256);	 */
		  }

     //  	BULK_SENDSET(BULK_BUFFER, SIZE_OF_SPIFLASH_PAGE);

		udisk_lba_param[INDEX_UDISK_LBA_START] += SIZE_OF_SPIFLASH_PAGE*2;
		udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] --;
    }

	
    if (udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] == 0)
    {
	    //scsiRdWtDone = 1;
		  //g_USB_BOT_CBWBuf.dwCBWXferLength = 0;
	 }
 return K_SUCCES;
}


unsigned char testnum = 0;
unsigned char udisk_write(void)//unsigned char udisk_write(unsigned long *udisk_lba_param)
{

	    if (udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] == 0)
		return K_ERROR;

    if (udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] > 1)
    {
    //    BULK_RCVSET(BULK_BUFFER, SIZE_OF_SPIFLASH_PAGE);

			
			SetCWBotrecievePtr();
			
		//if (WriteFlashDone)
		//{
		//	WriteFlashDone = 0;
		//	return K_SUCCES;
		//}
    }

    if (udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT])
    {
	//	    if (!WriteFlashDone)
	//	    {
		     	//Spi_Page_Program(udisk_lba_param[INDEX_UDISK_LBA_START], BULK_BUFFER, SIZE_OF_SPIFLASH_PAGE);

			//	SPI_Flash_Init();
		      //  APL_ExFlash_WriteData(udisk_lba_param[INDEX_UDISK_LBA_START], InBuff, 256);
			//	 SPI_Delay();
			//	 APL_ExFlash_WriteData(udisk_lba_param[INDEX_UDISK_LBA_START]+256, InBuff+256, 256);
				//  SPI_Delay();
				//  SPI_Flash_Init();
				 // SetCWBotrecievePtr();
			  
				 
				// if(testnum>1)
			//	 {
				/// 	 testnum==1;
                //    return 0;				 
				// }
				// else
				// {
				// 	 testnum++;	 
				 
				// }
			   	  // SPI_Delay();
			   APL_ExFlash_WriteData(udisk_lba_param[INDEX_UDISK_LBA_START], InBuff, 256);
			   //SPI_Delay();
				   //SPI_Delay();

			   APL_ExFlash_WriteData(udisk_lba_param[INDEX_UDISK_LBA_START]+256, InBuff+256, 256);
			   			  // SPI_Delay();
			
				  //SPI_Delay();
	//	     }
//		APL_memset(BULK_BUFFER,0x33,255);
//		APL_memset(BULK_BUFFER,0x33,128);
//		Spi_Read_Fast(udisk_lba_param[INDEX_UDISK_LBA_START], BULK_BUFFER, SIZE_OF_SPIFLASH_PAGE);

        udisk_lba_param[INDEX_UDISK_LBA_START] += SIZE_OF_SPIFLASH_PAGE*2;
        udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] --;
    }
	
    if (udisk_lba_param[INDEX_UDISK_LBA_XFER_COUNT] == 0)
    {
      //  WriteFlashDone = 1;
       // scsiRdWtDone = 1;
    }
    return K_SUCCES;
}
unsigned char ExFlash_erase_block(unsigned long wAddr,unsigned int count)
{
    unsigned int i;

   
    for(i=0;i<count;i++)
    {    
        //Spi_Block_Erase(wAddr);
	      APL_ExFlash_EraseBlock(wAddr);
        wAddr=(wAddr+BLOCK_SIZE);
    }

    return 0;
}
unsigned char udisk_erasechip(void)
{
    Spi_Chip_Erase();
   //APL_ExFlash_EraseChip();

    return 0;
}
unsigned char udisk_getcapacity(void)
{
    unsigned char chip_id;

    chip_id = APL_ExFlash_GetCapcity();//JEDEC_ID_Read(chip_id);
    if (chip_id > 0x10)
    {
        return (0x01 << (chip_id - 0x11));

    }
    else
    {
        return 0;
    }
}

