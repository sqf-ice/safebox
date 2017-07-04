#include "headfile.h"
#include "usb_support.h"
#include "globle.h"

extern const unsigned short gSW_Table[];
extern unsigned int SCSI_BlkLen;


extern unsigned char * CWBotRecievePtr;

//unsigned char usbtemp[0x200];


extern	 unsigned char InBuff[512];

void SetCWBotsendPtr(void)
{
	 
      CWBotsendPtr = InBuff;  //(unsigned  char *)(&gs_cmd.head[4]);
	    
	
}

void SetCWBotrecievePtr(void)
{
     CWBotRecievePtr = InBuff;  //(unsigned  char *)(&gs_cmd.head[4]);

}

void SetCWBotDataLenAndSCSI_BlkLen(unsigned int tempLen)
{

	CWBotsendPtr = InBuff; //(unsigned  char *)(&gs_cmd.head[4]);
	CWBotDataLen = tempLen%64;
	
	SCSI_BlkLen = tempLen/64;

}



void APL_USBMemcpy(unsigned char *  ucDestination, unsigned char * ucSource, unsigned short ucLen)
{
	unsigned short i;
	
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


void APL_USBMemset(unsigned char * ucDestination,unsigned char ucVal,unsigned short ucLen)
{
	unsigned short i;

	for(i=0;i<ucLen;i++)
	{
		ucDestination[i] = ucVal;
	}
}


unsigned short WordHtoL(unsigned short w)
{
	unsigned short wtemp;
	unsigned char  	temp;

	temp = (unsigned char )(w>>8);
	wtemp = ((w&0xFF)<<8)|temp;

	return wtemp;

    

}




void  API_MakeData(void)
{

    unsigned short SW12;
    unsigned short len;
    unsigned char sw;


    sw = g_bSW;
    SW12 = gSW_Table[sw];

    if (sw == SW6CXX)
    {
        SW12  |= gb_6c ; 
    }
    if (sw == SW63CX)
    {
        SW12  |= gb_63 ;
    }
    
	
	CWBotsendPtr =  (unsigned  char *)(&gs_cmd.head[4]);

    if (sw == SW_ATR)
    {
		SCSI_BlkLen = gb_Le+3;
		CWBotsendPtr[0] = 'R';
        CWBotsendPtr[1] = (BYTE)(gb_Le >> 8);
        CWBotsendPtr[2] = (BYTE)(gb_Le);
    }
	else
	{
		SCSI_BlkLen = gb_Le+5;
		len =  gb_Le+2;
		CWBotsendPtr[0] = 'R';
        CWBotsendPtr[1] = (BYTE)(len >> 8);
        CWBotsendPtr[2] = (BYTE)(len);

		CWBotsendPtr[gb_Le+3] = (BYTE)(SW12>>8);
		CWBotsendPtr[gb_Le+4] = (BYTE)SW12;
	
	}

	CWBotDataLen = SCSI_BlkLen%64;
	
	SCSI_BlkLen = SCSI_BlkLen/64;





}



