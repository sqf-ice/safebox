#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <stdio.h>
#include <string.h>	

#include "stm32f10x.h" 		//== STM32F103VC ��ͷ�ļ�
#include "stm32f10x_conf.h"	

#include "platform_config.h"//== STM32F103VCT6ƽ̨IO��Դ���� 
#include "global.h"			//== �Զ���ȫ�ֱ���				



#define SWAP16(x)  (unsigned short)((((unsigned short)(x))<<8)|(((unsigned short)(x))>>8))	//== �ߵ��ֽڽ���

extern unsigned char bDevice_flag;
extern void TimeConfig(void);
extern  void CCID_StatePolling(void);
//==hw_config.c   
extern void Set_System(void);
		
extern void delay(unsigned int delay_nsm); 	//== ��ʱ����
extern unsigned char xor(unsigned char* pbuf, unsigned int len);
extern unsigned char GetChkSum(unsigned char *pBuf,unsigned int len);
extern void delay_us(unsigned char us);

//== source.c
extern void clock_counter(unsigned int num);//== ��ʱ�Ӹ���
extern void clock_counter_no_wait(unsigned int num);

extern void beep_tcf(unsigned int beep_time, unsigned int beep_conuter, unsigned int beep_frequency);
extern void beep(unsigned int beep_time, unsigned int beep_conuter);
extern void beep_success(void);
extern void beep_fail(void);
extern void beep_on(void); 			//== ������
extern void beep_off(void);			//== �ط���

extern void led_open(unsigned int led);	  	//==led1 led2
extern void led_close(unsigned int led);
extern void led_flash(unsigned int led, unsigned int light_time, unsigned int light_conuter);
extern void led3_breath(void);
extern void led3_stop_breath(void);

extern void init_board_led(void);	//== ��ʼ��	
extern void board_led_open(void); 	//== ����
extern void board_led_close(void); 	//== ����

extern void usb_ctl_on(void);		//==usb ���ƹܽ�
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

//=======================================״̬�붨��==20100323===============================================
//==1. ͨ��
#define STAT_OK				0x0000	//==����ִ����ȷӦ��
#define ERR_UART			0x0001 	//==�������ò�����֧��
#define ERR_0003			0x0003	//==������Ƶ��ʧ��

//==2. Э��״̬
#define ERR_FRAME			0x00F1	//==���ݰ���ʽ�����磺��ͷ��β��
#define ERR_LEN				0x00F2	//==���ݱ��ĳ��ȴ���
#define ERR_LRC				0x00F3	//==����У�����
#define ERR_CMD				0x00F4	//==�������
#define ERR_00F5			0x00F5	//==��������
#define ERR_PROG			0x00FC	//==Flash��������
#define ERR_PCTIMEOUT		0x00FD	//==PCͨ�ų�ʱ����
#define ERR_TIMEOUT			0x00FE	//==ͨ�ų�ʱ���� ����д����ģ�飩
#define ERR_UND				0x00FF	//==δ�������

#define ERR_F001			0xF001	//==�������ɹ�
#define ERR_F002			0xF002	//==�������ݳ��ȴ���

#define ERR_1001			0x1001	//==��֧�ֽӴ��û���
#define ERR_1002			0x1002	//==�Ӵ��û���δ�嵽λ
#define ERR_1003			0x1003	//==�Ӵ��û������ϵ�
#define ERR_1004			0x1004	//==�Ӵ��û���δ�ϵ�
#define ERR_1005			0x1005	//==�Ӵ��û����ϵ�ʧ��
#define ERR_1006			0x1006	//==�����Ӵ��û�����������Ӧ
#define ERR_1007			0x1007	//==�����Ӵ��û������ݳ��ִ���

#define ERR_2001			0x2001	//==��֧��PSAM��
#define ERR_2002			0x2002	//==
#define ERR_2003			0x2003	//==PSAM�����ϵ�
#define ERR_2004			0x2004	//==PSAM��δ�ϵ�
#define ERR_2005			0x2005	//==PSAM���ϵ�ʧ��
#define ERR_2006			0x2006	//==�����Ӵ�PSAM����������Ӧ
#define ERR_2007			0x2007	//==�����Ӵ�PSAM�����ݳ��ִ�

#define ERR_3001			0x3001	//==��֧�ַǽӴ��û���
#define ERR_3004			0x3004	//==�ǽӴ��û���δ����
#define ERR_3005			0x3005	//==�ǽӴ��û�������ʧ��
#define ERR_3006			0x3006	//==�����ǽӴ��û�����������Ӧ
#define ERR_3007			0x3007	//==�����ǽӴ��û������ݳ��ִ�
#define ERR_3008			0x3008	//==�ǽӴ��û���haltʧ��
#define ERR_3009			0x3009	//==�ж��ſ��ڸ�Ӧ��

#define ERR_6001			0x6001	//==��֧���߼�����
#define ERR_6020			0x6020	//==��Ƭ���Ͳ��ԣ���״̬6A82��
#define ERR_6021			0x6021	//==���㣨��״̬9401��
#define ERR_6022			0x6022	//==��Ƭ���ܲ�֧�֣���״̬6A81��
#define ERR_6030			0x6030	//==��Ƭδ����
#define ERR_6031			0x6031	//==��Ƭ������Ч��
#define ERR_6032			0x6032	//==������ϸ�޴˼�¼
#define ERR_6033			0x6033	//==���׼�¼δ�������
#define ERR_6040			0x6040	//==��Ҫ�����δ���
#define ERR_6041			0x6041	//==���δ����г�����ԭ����
#define ERR_6042			0x6042	//==�����жϣ�û���ʽ���ʧ

//==M1״̬��
#define ERR_C001			0xC001	//==�������ɹ�
#define ERR_C002			0xC002	//==���ݳ��ȴ���
#define ERR_C003			0xC003	//==�������ݴ���
#define ERR_C004			0xC004	//==�����������
#define ERR_C005			0xC005	//==�豸״̬����
#define ERR_C0FF			0xC0FF	//==�������� 

#endif 



