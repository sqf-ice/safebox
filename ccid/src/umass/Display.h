#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "headfile.h"

extern unsigned char  slot1;
extern unsigned char  card_type;
extern unsigned char UsbBulkMsgBuf[271];	 //与USB通信的缓存buf
extern unsigned char UsbBulkInMsgBuf[2300];	 //与USB通信的缓存buf
//待机交易显示
#define W_PIC0	0x01
#define W_PIC1  0x01+DIS_MAX_TOTAL_LINE
#define W_PIC2  0x01+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE
#define W_PIC3  0x01+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE
#define W_PIC4  0x01+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE
#define W_PIC5  0x01+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE+DIS_MAX_TOTAL_LINE

/*
#define W_TAG1  0x01
#define W_TAG2  0x02
#define W_TAG3  0x03
#define W_TAG4  0x04

#define W_TAG5  0x05
#define W_TAG6  0x06
#define W_TAG7  0x07
#define W_TAG8  0x08

#define W_TAG9  0x09
#define W_TAG10  0x0a
#define W_TAG11  0x0b
#define W_TAG12  0x0c

#define W_TAG13  0x0d
#define W_TAG14  0x0e
#define W_TAG15  0x0f
#define W_TAG16  0x10
*/
#define LEN_TMPLCODE        0x06
#define TAG_TMPLCODE        0x01BF
#define TAG_TRASNAME        0x02BF
/*
//CS-GP14
//A0-GP22
//RES-GP17
#define LCD_IO_Init()      {REG32(GPAUR)&=0xFFFFCFFF, REG32(GPALR)&=0xCFFFFFFF, REG32(GPDIR)|=0x424000;}
#define LCD_Select()       {REG32(GPDR)&=0xFFFFBFFF;}
#define LCD_Deselect()   {REG32(GPDR)|=0x4000;}
#define LCD_A0_CMD()    {REG32(GPDR)&=0xFFBFFFFF;}
#define LCD_A0_DATA()  {REG32(GPDR)|=0x400000;}
#define LCD_RES_LOW()  {REG32(GPDR)&=0xFFFDFFFF;}
#define LCD_RES_HIGH() {REG32(GPDR)|=0x20000;}
*/
#define DEBUG_LCD

extern unsigned char X_Witch;
extern unsigned char Y_Witch;
extern unsigned char DIS_PAGE_LINE;

typedef struct 
{
    BYTE tag;
    BYTE flag;        //显示标志
    BYTE len_add; //附加项长度
    BYTE pos;         //附加项偏移地址
} Tags_Struct;

#define MAX_NUM_SCR             (Dis_X_MAX * Dis_Y_MAX /8)
#define MAX_NUM_PAGE           (LCD_X_MAX * 2) //单页最大显示

#define MAX_NUM_AddTOTAL   0x0280
#define MAX_NUM_TAGS           32 
#define MAX_NUM_PAGES         16  //屏幕最大分页数
#define MAX_NUM_Add             45  //单项最大ASCII字符数

//#define MZ_LCD
/*
#ifdef MZ_LCD
#define LCD_X_MAX			128//			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			64//			//屏幕的Y轴的物理宽度
#else
#define LCD_X_MAX			192//			//屏幕的X轴的物理宽度
#define LCD_Y_MAX			64//			//屏幕的Y轴的物理宽度
#endif
*/
#define LCD_XY_Switch		0				//显示时X轴和Y由交换
#define LCD_X_Rev			0				//显示时X轴反转
#define LCD_Y_Rev			0				//显示时Y轴反转

#ifdef MZ_LCD
#define LCD_SCR_Rev               0                          //显示时屏幕颠倒
#else
#define LCD_SCR_Rev               0                        //显示时屏幕颠倒
#endif

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		(LCD_X_MAX)
	#define Dis_Y_MAX		(LCD_Y_MAX)	
#endif

#if LCD_XY_Switch == 1
	#define Dis_X_MAX		(LCD_Y_MAX)
	#define Dis_Y_MAX		(LCD_X_MAX)	
#endif

//#define DIS_NUM_LINE    (Dis_X_MAX / 8) //单行显示字节数
//#define DIS_NUM_SCR     ((Dis_X_MAX / 8) * (Dis_Y_MAX / 8)) //每屏显示字节数
//#define DIS_PAGE_LINE   (Dis_Y_MAX / 16) //每页行数
#ifdef MZ_LCD
#define DIS_MAX_LINE    32  //最大显示行数
#define DIS_MAX_YNUM   6    //最大Y轴值
#define DIS_MAX_TOTAL_LINE 4
#else
#define DIS_MAX_LINE    72  //最大显示行数
#define DIS_MAX_YNUM   	8    //最大Y轴值
#define DIS_MAX_TOTAL_LINE 4
#endif


#define FangGuan_128x64_LCD
#ifdef FangGuan_128x64_LCD

#define HANZI_FONT_HIGH   2			//汉字高度单位bit
#if LCD_SCR_Rev
#define NextDots_Line_Y_Address(y)     (unsigned char)  (y-Y_Witch)  
#else
//换下一行显示时，行地址更新 (在横向扫描模式LCD中)
//y : 当前行地址
#define NextDots_Line_Y_Address(y)     (unsigned char)  (y+Y_Witch)

//同一行，显示下一个点阵时，列地址更新
//x:当前列地址  X_Width: 下一个点阵字体宽度
#define NextDots_Column_X_Address(x,X_Width) (unsigned char) (x+X_Width)

// WeiXinNuo 地址转换
#define X_Address_convert(x)  (unsigned char) (x)
#define Y_Address_convert(y)  (unsigned char) (y)


#endif

#endif  // end of 'FangGuan_128x64_LCD'
// add end

#define DISFLAG_DATA   0x80 //当前项为附加数据
#define DISFLAG_SUM     0x40 //数据项为金额
#define DISFLAG_GBK     DISFLAG_SUM //前导项为GBK码

#define LCD_INITIAL_COLOR	0x00			//定义LCD屏初始化时的背景色

extern unsigned char g_linetotal; //总行数
extern unsigned char g_linecur;   //当前行

void LCD_Init(void);
void Clear_LCD(void);
BYTE LCD_Refresh(BYTE start);
void LCD_Scr_W(BYTE Stype);
void LCD_Fill(unsigned char Data);
void Process_DisData(void);
unsigned char Make_DisData(unsigned char *pBuf, unsigned char bLen);
void LCD_display_UTF8(unsigned char *buf,unsigned char len);
WORD UnicodeToUTF8(unsigned char *dest, WORD * unicode, unsigned char len);
void delay_3ms(unsigned char count);
unsigned char api_GetKey_OTP(void);
unsigned char api_GetKey_noBlock(void);
void Clear_LCD(void);
void LCD_display_UTF8Line(unsigned char *buf,unsigned char len, unsigned char linet);
//extern unsigned int  ISO14443Layer4TransceiveApdu(unsigned char *ApduBuf , unsigned char * ResponseBuf , unsigned char len);
#endif

