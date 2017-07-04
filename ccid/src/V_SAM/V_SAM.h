
#ifndef __SAM_V_H__
#define __SAM_V_H__

#define DataLen 	2500			//==��󳤶�0x0BB8

//==����SAM_V���ݴ����ʽ
typedef struct _V_MsgMod			
{
	unsigned char 	P[5+3];			//== ֡���ݵ�֡ͷ 0xAA 0xAA 0xAA 0x96 0x69
	unsigned short 	Len;			//== ���ݵ���Ч���ݳ���
	unsigned char 	CMD;			//== ������
	unsigned char 	Para;			//== ������� 
	unsigned char 	SW1;			//== 
	unsigned char 	SW2;			//== SW1 SW2��ʾ֤/�����ص�״̬����
	unsigned char 	SW3;			//== ��ʾSAM_V����״̬
//	unsigned char 	CHK;			//== xor����� CHK_SUM
//	unsigned char 	Data[DataLen];	//== �贫������ݵ�Ԫ(Data+CHK_SUM)
	unsigned char 	*Data;			//== �贫������ݵ�Ԫ(Data+CHK_SUM)
}__attribute__((__packed__)) V_MsgMod; 


typedef struct _SAM_UsartMod		
{														  
	volatile unsigned int 	rCount;			//== ���ո���
	volatile unsigned int 	tCount;			//== ���͸���  	
	unsigned char 			tBuf[256];		//== ��������buf
	unsigned char 			rBuf[DataLen];	//== ��������buf

}__attribute__((__packed__)) SAM_UsartMod;

#endif 







