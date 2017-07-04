
#ifndef _HEADFILE_H
#define _HEADFILE_H

//#define BYTE unsigned char
//#define WORD unsigned short

#define BYTE unsigned char
#define WORD unsigned short
#define ADWORD unsigned int
#define DWORD unsigned int

//typedef unsigned char   BOOLEAN;
typedef unsigned char   UINT8;
typedef signed   char   SINT8;
typedef unsigned short  UINT16;
typedef signed   short  SINT16;
typedef unsigned int    UINT32;
typedef signed 	 int    SINT32;
//typedef unsigned char   BYTE;


#define    FUN_CD_ROM      0x80
#define    FUN_TKEY        0x20
#define    FUN_LCD         0x10
#define    FUN_VOICE       0x08
#define    FUN_DrvSwitch   0x04
#define    FUN_TRANS2    0x02
#define    FUN_SMPIN       0x01



extern unsigned char buffer[2050];

#define MAXDISLPAYLEN           640 //768
#define MAXBUFFERLEN            256//512
#define HEADTAGSLEN             128
//#define g_HeadTags (buffer+400)
//extern unsigned char g_HeadTags[HEADTAGSLEN];//400~1750不足（640+640+128）

//#define g_HeadTags (buffer + 128)
#define g_Buff    (buffer + 450)//(buffer+400)// (g_HeadTags + HEADTAGSLEN)
#define g_bAddData  g_Buff
#define g_Display (g_bAddData + MAXDISLPAYLEN)







#define TAPDU_BUF_LEN (3500+(1200*10)) //512+4


typedef struct {
    //unsigned char temp;
    //unsigned char sessionID; //Dock通信模式下前2字节为sessionID
    unsigned char ccid_head[10]; //CCID头部10个字节
    unsigned char tmp2usb[3]; //MSC/HID协议下用作一字节T和两字节L
    unsigned char head[5];   //CLA, INS, P1, P2, Lc0
    unsigned short  len; //Lc1, Lc2
    unsigned char   buf[TAPDU_BUF_LEN];
} TAPDU_Data_Struct;



typedef struct {
	unsigned char temp;   
       unsigned char cla;
       unsigned char ins;
       unsigned char p1;
       unsigned char p2;
       unsigned char Lc0;
	unsigned short len;
} STRU_CMDHEAD;




#define MYREG16(addr)  (((unsigned short) (*((unsigned char volatile *)(addr))) <<8) | (*((unsigned char volatile *)(addr+1))))







#define CMD_CLA                ga_cmdhead.cla
#define CMD_INS                ga_cmdhead.ins
#define CMD_P1                 ga_cmdhead.p1
#define CMD_P2                 ga_cmdhead.p2
#define CMD_P3                 ga_cmdhead.len
#define CMD_DATA 	      gs_cmd.buf



//----------------------------------------------------------
#define 		SW9000		0x00	//成功执行
#define 		SW6E00		0x01	//CLA不支持或错误
#define 		SW6D00		0x02	//INS不支持或错误
#define 		SW6300		0x03	
#define 		SW6700		0x04	//Ln长度错误
#define 		SW6900		0x05	//CLA与线路保护属性不匹配
#define 		SW6901		0x06	//状态机不满足
#define 		SW6981		0x07	//文件类型不匹配
#define 		SW6982		0x08	//安全状态不满足
#define 		SW6983		0x09	//密钥锁定
#define 		SW6984		0x0A	//引用数据无效
#define 		SW6985		0x0B	//使用条件不满足
#define 		SW6986		0x0C	//没有EF被选择
#define 		SW6988		0x0D	//线路保护错误
#define 		SW6A80		0x0E	//数据域有错误
#define 		SW6A81		0x0F	//无MF或者卡片锁定s
#define 		SW6A82		0x10	//文件未找到
#define 		SW6A83		0x11	//记录未找到
#define 		SW6A84		0x12	//空间不足
#define 		SW6A85		0x13
#define 		SW6A86		0x14	//参数错误
#define 		SW6A88		0x15	
#define 		SW6A89		0x16	//文件已经存在(FID重复)
#define 		SW6A8B		0x17	//KEY文件没有建立
#define 		SW6B00		0x18	//偏移超出范围
#define 		SW6F00		0x19	//无响应数据
#define 		SW9302		0x1A	//MAC无效
#define 		SW9303		0x1B	//应用锁定	
#define 		SW9401		0x1C
#define	  		SW9403		0x1D	//不支持的密钥索引
#define	  		SW9406		0x1E
#define	  		SW6283		0x1F
#define 		SW0020		0x20
#define 		SW0021		0x21
#define 		SW0022		0x22
#define 		SW63CX		0x23	//验证失败，还剩下X次尝试机会
#define 		SW61XX		0x24	//取响应，XX为返回数据长度
#define	  		SW6CXX		0x25	//LC应该为XX
#define 		SW_ATR		0x26
#define                 SW6581          0x27

//=======================================================================


#endif


