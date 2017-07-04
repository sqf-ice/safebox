/*===============================================================================//
//== 文 件 名：global.h
//== author	 ：guohaibo@thtf.com.cn
//== 说    明：数据结构
//== 修改日志：201012
//== 清华同方|同方锐安科技有限公司
//===============================================================================*/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//================定义数据空间的长度==============================
#define LEN64				64
#define LEN128				128
#define LEN256				256
#define LEN512				512 
#define LEN1024				1024
#define LEN1300				1300
//#define DATALEN				LEN512			//==数据长度
#define DATALEN				LEN1300				//==数据长度 
#define MSGLEN				271			//==银联协议报文的最大长度	

struct _golbal_var
{
	volatile unsigned int delay_ms;				//== 延时基数器
	volatile unsigned int run_time;				//== 记录空闲时运行的时间，管理呼吸灯
	volatile unsigned int psam_clock_flag;		//== 0：没到设置的时钟数，1：到了或超过要求的时钟个数
	volatile unsigned int search_card_flag;		//== 0：没有寻卡 1：正在寻卡 （接触式和非接触式卡片）
	volatile unsigned int search_card_time;		//== 寻卡时间
	volatile unsigned int data_path_flag; 		//== 0:串口通道，其它usb通道 
	volatile unsigned int v_delay_ms;			//== SAM_V计数
	volatile unsigned int ms;					//== SAM_V计时
	volatile unsigned int thm3060_ms;			//== thm3060接收计时
};

struct _sys_param
{
	volatile unsigned int run_flag;				//== 0:测试模式（主要用于自动寻卡调试天线）; 1:正常运行
	volatile unsigned int check;
	volatile unsigned int ncheck;

};

//==定义银联通信报文协议结构
typedef struct _MsgMod						//== 通信数据帧结构
{
	unsigned char 	STX;					//== 数据包头(STX) 0x02
	unsigned short 	DataLen;				//== 数据单元长度（Data_len）
	unsigned char 	Data[DATALEN];			//== 需传输的数据单元(Data)
	unsigned char 	LRC;					//== 冗余检验值（LRC）
	unsigned char 	ETX;					//== 数据包尾（ETX） 0x03
}MsgMod;

//== 定义串口数据结构
typedef struct _UsartMod		
{
	volatile unsigned int 	rCount;			//== 接收个数  
	volatile unsigned int 	tCount;			//== 发送个数
	volatile unsigned int 	rFlag;			//== 串口接收数据等待标识
	volatile unsigned int 	rTimeOut;		//== 超时等待时间
	volatile unsigned int 	timeCounter;	//== 计数次数 	
	unsigned char 			rBuf[MSGLEN];	//== 接收数据buf	
	unsigned char 			tBuf[MSGLEN];	//== 发送数据buf

}UsartMod;

//== 定义USB数据结构
//typedef struct _UsbMod		
//{
//	volatile unsigned int 	rCount;			//== 接收个数
//	volatile unsigned int 	tCount;			//== 发送个数
//	volatile unsigned int 	rFlag;			//== usb接收数据等待标识
//	volatile unsigned int 	rTimeOut;		//== 超时等待时间
//	volatile unsigned int 	timeCounter;	//== 计数次数
//   	volatile unsigned int 	EP1_TX_INT_FLAG;//== ep1发送标识	
//	unsigned char 			rBuf[MSGLEN];	//== 接收数据buf 	
//	unsigned char 			tBuf[MSGLEN];	//== 发送数据buf
//
//}UsbMod;

//== 定义USB数据结构
typedef struct _UsbMod		
{
	volatile unsigned int 	rCount;			//== 接收个数
	volatile unsigned int 	tCount;			//== 发送个数
	volatile unsigned int 	rFlag;			//== usb接收数据等待标识
	volatile unsigned int 	rTimeOut;		//== 超时等待时间
	volatile unsigned int 	timeCounter;	//== 计数次数
   	volatile unsigned int 	EP2_RX_INT_FLAG;//== ep1发送标识
	volatile unsigned int   EP5_RX_INT_FLAG;
	volatile unsigned int 	EP2_TX_INT_FLAG;//== ep1发送标识	
	unsigned char 			rBuf[MSGLEN];	//== 接收数据buf 	
	unsigned char 			tBuf[MSGLEN];	//== 发送数据buf

}UsbMod;

#endif 
