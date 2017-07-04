/*===============================================================================//
//== �� �� ����global.h
//== author	 ��guohaibo@thtf.com.cn
//== ˵    �������ݽṹ
//== �޸���־��201012
//== �廪ͬ��|ͬ���񰲿Ƽ����޹�˾
//===============================================================================*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//================�������ݿռ�ĳ���==============================
#define LEN64				64
#define LEN128				128
#define LEN256				256
#define LEN512				512 
#define LEN1024				1024
#define LEN1300				1300
//#define DATALEN				LEN512			//==���ݳ���
#define DATALEN				LEN1300				//==���ݳ��� 
#define MSGLEN				271			//==����Э�鱨�ĵ���󳤶�	

struct _golbal_var
{
	volatile unsigned int delay_ms;				//== ��ʱ������
	volatile unsigned int run_time;				//== ��¼����ʱ���е�ʱ�䣬���������
	volatile unsigned int psam_clock_flag;		//== 0��û�����õ�ʱ������1�����˻򳬹�Ҫ���ʱ�Ӹ���
	volatile unsigned int search_card_flag;		//== 0��û��Ѱ�� 1������Ѱ�� ���Ӵ�ʽ�ͷǽӴ�ʽ��Ƭ��
	volatile unsigned int search_card_time;		//== Ѱ��ʱ��
	volatile unsigned int data_path_flag; 		//== 0:����ͨ��������usbͨ�� 
	volatile unsigned int v_delay_ms;			//== SAM_V����
	volatile unsigned int ms;					//== SAM_V��ʱ
	volatile unsigned int thm3060_ms;			//== thm3060���ռ�ʱ
};

struct _sys_param
{
	volatile unsigned int run_flag;				//== 0:����ģʽ����Ҫ�����Զ�Ѱ���������ߣ�; 1:��������
	volatile unsigned int check;
	volatile unsigned int ncheck;

};

//==��������ͨ�ű���Э��ṹ
typedef struct _MsgMod						//== ͨ������֡�ṹ
{
	unsigned char 	STX;					//== ���ݰ�ͷ(STX) 0x02
	unsigned short 	DataLen;				//== ���ݵ�Ԫ���ȣ�Data_len��
	unsigned char 	Data[DATALEN];			//== �贫������ݵ�Ԫ(Data)
	unsigned char 	LRC;					//== �������ֵ��LRC��
	unsigned char 	ETX;					//== ���ݰ�β��ETX�� 0x03
}MsgMod;

//== ���崮�����ݽṹ
typedef struct _UsartMod		
{
	volatile unsigned int 	rCount;			//== ���ո���  
	volatile unsigned int 	tCount;			//== ���͸���
	volatile unsigned int 	rFlag;			//== ���ڽ������ݵȴ���ʶ
	volatile unsigned int 	rTimeOut;		//== ��ʱ�ȴ�ʱ��
	volatile unsigned int 	timeCounter;	//== �������� 	
	unsigned char 			rBuf[MSGLEN];	//== ��������buf	
	unsigned char 			tBuf[MSGLEN];	//== ��������buf

}UsartMod;

//== ����USB���ݽṹ
//typedef struct _UsbMod		
//{
//	volatile unsigned int 	rCount;			//== ���ո���
//	volatile unsigned int 	tCount;			//== ���͸���
//	volatile unsigned int 	rFlag;			//== usb�������ݵȴ���ʶ
//	volatile unsigned int 	rTimeOut;		//== ��ʱ�ȴ�ʱ��
//	volatile unsigned int 	timeCounter;	//== ��������
//   	volatile unsigned int 	EP1_TX_INT_FLAG;//== ep1���ͱ�ʶ	
//	unsigned char 			rBuf[MSGLEN];	//== ��������buf 	
//	unsigned char 			tBuf[MSGLEN];	//== ��������buf
//
//}UsbMod;

//== ����USB���ݽṹ
typedef struct _UsbMod		
{
	volatile unsigned int 	rCount;			//== ���ո���
	volatile unsigned int 	tCount;			//== ���͸���
	volatile unsigned int 	rFlag;			//== usb�������ݵȴ���ʶ
	volatile unsigned int 	rTimeOut;		//== ��ʱ�ȴ�ʱ��
	volatile unsigned int 	timeCounter;	//== ��������
   	volatile unsigned int 	EP2_RX_INT_FLAG;//== ep1���ͱ�ʶ
	volatile unsigned int   EP5_RX_INT_FLAG;
	volatile unsigned int 	EP2_TX_INT_FLAG;//== ep1���ͱ�ʶ	
	unsigned char 			rBuf[MSGLEN];	//== ��������buf 	
	unsigned char 			tBuf[MSGLEN];	//== ��������buf

}UsbMod;

#endif 
