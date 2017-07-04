
#ifndef __SAM_V_H__
#define __SAM_V_H__

#define DataLen 	2500			//==最大长度0x0BB8

//==定义SAM_V数据传输格式
typedef struct _V_MsgMod			
{
	unsigned char 	P[5+3];			//== 帧数据的帧头 0xAA 0xAA 0xAA 0x96 0x69
	unsigned short 	Len;			//== 数据的有效数据长度
	unsigned char 	CMD;			//== 命令码
	unsigned char 	Para;			//== 命令参数 
	unsigned char 	SW1;			//== 
	unsigned char 	SW2;			//== SW1 SW2表示证/卡返回的状态参数
	unsigned char 	SW3;			//== 表示SAM_V操作状态
//	unsigned char 	CHK;			//== xor检验和 CHK_SUM
//	unsigned char 	Data[DataLen];	//== 需传输的数据单元(Data+CHK_SUM)
	unsigned char 	*Data;			//== 需传输的数据单元(Data+CHK_SUM)
}__attribute__((__packed__)) V_MsgMod; 


typedef struct _SAM_UsartMod		
{														  
	volatile unsigned int 	rCount;			//== 接收个数
	volatile unsigned int 	tCount;			//== 发送个数  	
	unsigned char 			tBuf[256];		//== 发送数据buf
	unsigned char 			rBuf[DataLen];	//== 接收数据buf

}__attribute__((__packed__)) SAM_UsartMod;

#endif 







