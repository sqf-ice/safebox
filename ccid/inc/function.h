#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <stdio.h>
#include <string.h>	

#include "stm32f10x.h" 		//== STM32F103VC 库头文件
#include "stm32f10x_conf.h"	

#include "platform_config.h"//== STM32F103VCT6平台IO资源定义 
#include "global.h"			//== 自定义全局变量				



#define SWAP16(x)  (unsigned short)((((unsigned short)(x))<<8)|(((unsigned short)(x))>>8))	//== 高低字节交换

extern unsigned char bDevice_flag;
extern void TimeConfig(void);
extern  void CCID_StatePolling(void);
//==hw_config.c   
extern void Set_System(void);
		
extern void delay(unsigned int delay_nsm); 	//== 延时毫秒
extern unsigned char xor(unsigned char* pbuf, unsigned int len);
extern unsigned char GetChkSum(unsigned char *pBuf,unsigned int len);
extern void delay_us(unsigned char us);

//== source.c
extern void clock_counter(unsigned int num);//== 数时钟个数
extern void clock_counter_no_wait(unsigned int num);

extern void beep_tcf(unsigned int beep_time, unsigned int beep_conuter, unsigned int beep_frequency);
extern void beep(unsigned int beep_time, unsigned int beep_conuter);
extern void beep_success(void);
extern void beep_fail(void);
extern void beep_on(void); 			//== 开蜂鸣
extern void beep_off(void);			//== 关蜂鸣

extern void led_open(unsigned int led);	  	//==led1 led2
extern void led_close(unsigned int led);
extern void led_flash(unsigned int led, unsigned int light_time, unsigned int light_conuter);
extern void led3_breath(void);
extern void led3_stop_breath(void);

extern void init_board_led(void);	//== 初始化	
extern void board_led_open(void); 	//== 灯亮
extern void board_led_close(void); 	//== 灯灭

extern void usb_ctl_on(void);		//==usb 控制管脚
extern void usb_ctl_off(void);

//== usart.c
extern void init_usartx(USART_TypeDef* USARTx, unsigned int BaudRate);
extern void TxByte(USART_TypeDef *USARTx, unsigned char *txData);
extern void TxData(USART_TypeDef* USARTx, unsigned char *txBuf, unsigned int txLen);
extern unsigned int RxByte(USART_TypeDef *USARTx, unsigned char *rxData,unsigned int TimeOut);
extern unsigned int RxData(USART_TypeDef* USARTx, unsigned char* rxBuf,unsigned int rxLen,unsigned int TimeOut);

extern unsigned int process_usart_rmsg(MsgMod* rMsg);




//==sam_v.c
extern void init_sam_v(void);
extern unsigned int search_id(unsigned char *dat,unsigned int *len);
extern unsigned int read_id_info(unsigned char *dat,unsigned int *len);
extern unsigned int sam_v_test(void);
extern unsigned int get_sam_v_statue(void);

//=======================================状态码定义==20100323===============================================
//==1. 通用
#define STAT_OK				0x0000	//==命令执行正确应答
#define ERR_UART			0x0001 	//==串口设置参数不支持
#define ERR_0003			0x0003	//==开关射频场失败

//==2. 协议状态
#define ERR_FRAME			0x00F1	//==数据包格式错误（如：包头包尾）
#define ERR_LEN				0x00F2	//==数据报文长度错误
#define ERR_LRC				0x00F3	//==数据校验错误
#define ERR_CMD				0x00F4	//==命令错误
#define ERR_00F5			0x00F5	//==参数错误
#define ERR_PROG			0x00FC	//==Flash操作错误
#define ERR_PCTIMEOUT		0x00FD	//==PC通信超时错误
#define ERR_TIMEOUT			0x00FE	//==通信超时错误 （读写器与模块）
#define ERR_UND				0x00FF	//==未定义错误

#define ERR_F001			0xF001	//==操作不成功
#define ERR_F002			0xF002	//==操作数据长度错误

#define ERR_1001			0x1001	//==不支持接触用户卡
#define ERR_1002			0x1002	//==接触用户卡未插到位
#define ERR_1003			0x1003	//==接触用户卡已上电
#define ERR_1004			0x1004	//==接触用户卡未上电
#define ERR_1005			0x1005	//==接触用户卡上电失败
#define ERR_1006			0x1006	//==操作接触用户卡数据无响应
#define ERR_1007			0x1007	//==操作接触用户卡数据出现错误

#define ERR_2001			0x2001	//==不支持PSAM卡
#define ERR_2002			0x2002	//==
#define ERR_2003			0x2003	//==PSAM卡已上电
#define ERR_2004			0x2004	//==PSAM卡未上电
#define ERR_2005			0x2005	//==PSAM卡上电失败
#define ERR_2006			0x2006	//==操作接触PSAM卡数据无响应
#define ERR_2007			0x2007	//==操作接触PSAM卡数据出现错

#define ERR_3001			0x3001	//==不支持非接触用户卡
#define ERR_3004			0x3004	//==非接触用户卡未激活
#define ERR_3005			0x3005	//==非接触用户卡激活失败
#define ERR_3006			0x3006	//==操作非接触用户卡数据无响应
#define ERR_3007			0x3007	//==操作非接触用户卡数据出现错
#define ERR_3008			0x3008	//==非接触用户卡halt失败
#define ERR_3009			0x3009	//==有多张卡在感应区

#define ERR_6001			0x6001	//==不支持逻辑操作
#define ERR_6020			0x6020	//==卡片类型不对（卡状态6A82）
#define ERR_6021			0x6021	//==余额不足（卡状态9401）
#define ERR_6022			0x6022	//==卡片功能不支持（卡状态6A81）
#define ERR_6030			0x6030	//==卡片未启用
#define ERR_6031			0x6031	//==卡片不在有效期
#define ERR_6032			0x6032	//==交易明细无此记录
#define ERR_6033			0x6033	//==交易记录未处理完成
#define ERR_6040			0x6040	//==需要做防拔处理
#define ERR_6041			0x6041	//==防拔处理中出错，非原来卡
#define ERR_6042			0x6042	//==交易中断，没有资金损失

//==M1状态码
#define ERR_C001			0xC001	//==操作不成功
#define ERR_C002			0xC002	//==数据长度错误
#define ERR_C003			0xC003	//==过程数据错误
#define ERR_C004			0xC004	//==操作步骤错误
#define ERR_C005			0xC005	//==设备状态错误
#define ERR_C0FF			0xC0FF	//==其他错误 

#endif 



