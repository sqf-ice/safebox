 #include "mem.h"
 #include "stm32f10x_flash.h"





void APL_memcpy(unsigned char* ucDestination, unsigned char* ucSource, WORD ucLen)
{
     WORD i;
	
	if( ucLen == 0 ) return ;
	if( ucSource > ucDestination )
	{
		i = 0;
		while(1)
		{
			ucDestination[i] = ucSource[i];
			i++;
			if( i == ucLen ) return;
		}	
	}
	else    //tail-->head
	{
		i = ucLen;
		while(1)
		{
			ucDestination[i-1] = ucSource[i-1];
			i--;
			if( i == 0 ) return;
		}
	}

}

void APL_memset(unsigned char* ucDestination, unsigned char ucVal, WORD ucLen)
{
    WORD i;

	for(i=0;i<ucLen;i++)
	{
		ucDestination[i] = ucVal;
	}
}

 void API_ReadData(DWORD adwAddr,BYTE *pucBuf,WORD ucLen)
{
    WORD i;

    if(adwAddr>0xFFFF)  return;
    if(ucLen == 0) 
    {
        return ;
    }

    adwAddr += FLASH_ADDR_USER;
    for (i = 0; i < ucLen; i++)
    {
        *pucBuf++ = *(volatile UINT8 *)(adwAddr++) ;
    }
	
}

extern unsigned char buf_typeAB[20];
extern unsigned char g_configflag;
extern  BYTE API_WriteData(DWORD adwAddr, BYTE * pucBuf, UINT16 ucLen);


void GetConfigPara(void)
{
	 API_ReadData(0x1120,buf_typeAB,16);

	 if(buf_typeAB[1]==0x59)
	 {
	 	  g_configflag|=0x1;
	 }
	 else
	 {
	 	  g_configflag&=0xFE;
	 }


}

void ResetContactlessDebug()
{
	if(g_configflag&0x1)
	{
   	   buf_typeAB[1]=0xff;
	   API_WriteData(0x1121,&buf_typeAB[1],1);
	}

}


void eflash_read(DWORD adwAddr,BYTE *pucBuf,WORD ucLen)
{
    WORD i;

	for (i = 0; i < ucLen; i++)
    {
        *pucBuf++ = *(volatile UINT8 *)(adwAddr++) ;
    }
}
volatile FLASH_Status FLASHStatus;
void eflash_sector_program(UINT32 sector_addr, UINT8 *buf, UINT16 len)
{
    
	UINT16 i;
    UINT32 *ptr;
	FLASHStatus = FLASH_COMPLETE;

    /* Unlock the Flash Program Erase controller */
    FLASH_Unlock();	//FLASH解锁
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清标志位
    FLASHStatus = FLASH_ErasePage(sector_addr); 
	len >>= 2;
    ptr = (unsigned int *)buf;
    for(i =0; i < len; i++)
    {
       // EFLASH_program(sector_addr, *ptr++);
	   //FLASH_ProgramOptionByteData(sector_addr, *ptr++);
	    FLASH_ProgramWord(sector_addr, *ptr++);//*ptr++);	0x1122
        sector_addr += 4;
    }



}










 BYTE API_WriteData(DWORD adwAddr, BYTE * pucBuf, UINT16 ucLen)
{
    DWORD addr, PageAddr;
    WORD copylen;
    WORD MyAddr;     
   // BYTE * tempbuf = g_ComBuf;
    BYTE tempbuf[FLASH_PAGE_SIZE]; 

    if(ucLen == 0) 
    {
        return 0;
    }

    addr = adwAddr + FLASH_ADDR_USER;
    PageAddr = addr & FLASH_PAGE_NUM;  //sector size:  2k bytes	  0x800
    MyAddr = (WORD)(addr & FLASH_PAGE_ADDR);
    copylen = FLASH_PAGE_SIZE - MyAddr;
    if(copylen > ucLen)
    {
        copylen = ucLen;
    }
	do
    {
	
		eflash_read(PageAddr,tempbuf,FLASH_PAGE_SIZE); //FLASH_PAGE_SIZE =0x800
        APL_memcpy(&tempbuf[MyAddr], pucBuf, copylen);
        eflash_sector_program(PageAddr,tempbuf,FLASH_PAGE_SIZE);

        MyAddr = 0;
        ucLen -= copylen;
        pucBuf += copylen;
        if(ucLen > FLASH_PAGE_SIZE)
        {
            copylen = FLASH_PAGE_SIZE;
        }
        else
        {
            copylen = ucLen;
        }
        
        PageAddr += FLASH_PAGE_SIZE;
    }
    while(ucLen > 0);

    return 0;	
} 



  
void SetBootMode(unsigned char mode)
 {
 	unsigned  char  temp[8];

   if(mode==1)
   {
	 temp[0]	= 0x55;
	 temp[1]	= 0x5A;
     temp[2]	= 0xAA;
	 temp[3]	= 0x03;
   }
   else
   {
   	 temp[0]	= 0xFF;
	 temp[1]	= 0xFF;
	 temp[2]	= 0xFF;
	 temp[3]	= 0xFF;
   
   }

	API_WriteData(0xF800,temp,4);

 }




