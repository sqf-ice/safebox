#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "headfile.h"

extern unsigned char  slot1;
extern unsigned char  card_type;
extern unsigned char UsbBulkMsgBuf[271];	 //��USBͨ�ŵĻ���buf
extern unsigned char UsbBulkInMsgBuf[2300];	 //��USBͨ�ŵĻ���buf
//����������ʾ
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
    BYTE flag;        //��ʾ��־
    BYTE len_add; //�������
    BYTE pos;         //������ƫ�Ƶ�ַ
} Tags_Struct;

#define MAX_NUM_SCR             (Dis_X_MAX * Dis_Y_MAX /8)
#define MAX_NUM_PAGE           (LCD_X_MAX * 2) //��ҳ�����ʾ

#define MAX_NUM_AddTOTAL   0x0280
#define MAX_NUM_TAGS           32 
#define MAX_NUM_PAGES         16  //��Ļ����ҳ��
#define MAX_NUM_Add             45  //�������ASCII�ַ���

//#define MZ_LCD
/*
#ifdef MZ_LCD
#define LCD_X_MAX			128//			//��Ļ��X���������
#define LCD_Y_MAX			64//			//��Ļ��Y���������
#else
#define LCD_X_MAX			192//			//��Ļ��X���������
#define LCD_Y_MAX			64//			//��Ļ��Y���������
#endif
*/
#define LCD_XY_Switch		0				//��ʾʱX���Y�ɽ���
#define LCD_X_Rev			0				//��ʾʱX�ᷴת
#define LCD_Y_Rev			0				//��ʾʱY�ᷴת

#ifdef MZ_LCD
#define LCD_SCR_Rev               0                          //��ʾʱ��Ļ�ߵ�
#else
#define LCD_SCR_Rev               0                        //��ʾʱ��Ļ�ߵ�
#endif

#if LCD_XY_Switch == 0
	#define Dis_X_MAX		(LCD_X_MAX)
	#define Dis_Y_MAX		(LCD_Y_MAX)	
#endif

#if LCD_XY_Switch == 1
	#define Dis_X_MAX		(LCD_Y_MAX)
	#define Dis_Y_MAX		(LCD_X_MAX)	
#endif

//#define DIS_NUM_LINE    (Dis_X_MAX / 8) //������ʾ�ֽ���
//#define DIS_NUM_SCR     ((Dis_X_MAX / 8) * (Dis_Y_MAX / 8)) //ÿ����ʾ�ֽ���
//#define DIS_PAGE_LINE   (Dis_Y_MAX / 16) //ÿҳ����
#ifdef MZ_LCD
#define DIS_MAX_LINE    32  //�����ʾ����
#define DIS_MAX_YNUM   6    //���Y��ֵ
#define DIS_MAX_TOTAL_LINE 4
#else
#define DIS_MAX_LINE    72  //�����ʾ����
#define DIS_MAX_YNUM   	8    //���Y��ֵ
#define DIS_MAX_TOTAL_LINE 4
#endif


#define FangGuan_128x64_LCD
#ifdef FangGuan_128x64_LCD

#define HANZI_FONT_HIGH   2			//���ָ߶ȵ�λbit
#if LCD_SCR_Rev
#define NextDots_Line_Y_Address(y)     (unsigned char)  (y-Y_Witch)  
#else
//����һ����ʾʱ���е�ַ���� (�ں���ɨ��ģʽLCD��)
//y : ��ǰ�е�ַ
#define NextDots_Line_Y_Address(y)     (unsigned char)  (y+Y_Witch)

//ͬһ�У���ʾ��һ������ʱ���е�ַ����
//x:��ǰ�е�ַ  X_Width: ��һ������������
#define NextDots_Column_X_Address(x,X_Width) (unsigned char) (x+X_Width)

// WeiXinNuo ��ַת��
#define X_Address_convert(x)  (unsigned char) (x)
#define Y_Address_convert(y)  (unsigned char) (y)


#endif

#endif  // end of 'FangGuan_128x64_LCD'
// add end

#define DISFLAG_DATA   0x80 //��ǰ��Ϊ��������
#define DISFLAG_SUM     0x40 //������Ϊ���
#define DISFLAG_GBK     DISFLAG_SUM //ǰ����ΪGBK��

#define LCD_INITIAL_COLOR	0x00			//����LCD����ʼ��ʱ�ı���ɫ

extern unsigned char g_linetotal; //������
extern unsigned char g_linecur;   //��ǰ��

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

