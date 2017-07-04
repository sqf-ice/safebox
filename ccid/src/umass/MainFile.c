#include "headfile.h"
#include "globle.h"
#include "kernel.h"
#include "usb_support.h"




const unsigned short gSW_Table[]=
{
	0x9000	,	//00
	0x6E00	,	//01 
	0x6D00	,	//02
	0x6300   ,	//03
	0x6700	,	//04
	0x6900	,	//05
	0x6901	,	//06
	0x6981	,	//07
	0x6982	,	//08
	0x6983	,	//09
	0x6984	,	//0a
	0x6985	,	//0b
	0x6986	,	//0c
	0x6988	,	//0d
	0x6A80	,	//0e
	0x6A81 	,	//0f
	0x6A82	,	//10
	0x6A83	,	//11
	0x6A84	,	//12
	0x6A85	,	//13
	0x6A86	,	//14
	0x6A88	,	//15
	0x6A89	,	//16
	0x6A8B	,	//17
	0x6B00	,	//18
	0x6F00	,	//19
	0x9302	,	//1a
	0x9303	,	//1b
	0x9401	,	//1c
	0x9403	,	//1d
	0x9406	,	//1e
	0x6283	,	//1f
	0x0020	,	//20
	0x0021	,	//21
	0x0022	,	//22
	0x63C0  ,	//23
	0x6100  ,	//24
	0x6C00  ,	//25
	0x0000	,//26
	0x6581
};



void API_Instuction_Hook(void)
{

	unsigned char SW12;
	BYTE  CLA;

    CLA = CMD_CLA & 0xFB;
	gb_Le =0;
	if (CLA == 0x00)
	{
	
		  if (CMD_INS == 0x00) {SW12 = cmd_0000();  }else		//复位  
		  if (CMD_INS == 0x84) {SW12 = cmd_0084();  }else		//取随机数
          {
            SW12 = SW6D00;
          }
	
	}
	else if(CLA == 0xF0)
	{
	
		     if (CMD_INS == 0x00) {SW12 = cmd_F000();  }else		//复位
			 if (CMD_INS == 0x01) {SW12 = cmd_F001();  }else		//复位
			 if (CMD_INS == 0x02) {SW12 = cmd_F002();  }else		//复位
             {
                SW12 = SW6D00;
             }	
	
	
	}
		




    g_bSW = SW12;




}




