
#include "function.h"	//== 包含平台库和所有外调函数
#include "V_SAM.h"


#define REQ_TIME 			10					//==寻证时间ms  有卡是10ms、没卡时25ms左右能返回
#define SEL_TIME 			5					//==选证时间ms	 有卡是5ms左右能返回
#define READ_INFO_TIME 		1500					//==读信息时间ms
#define READ_ADD_INFO_TIME 	400					//==读追加信息时间ms

extern volatile struct _golbal_var	gVar;		//==public.c
SAM_UsartMod umod;								//==定义SAM_V的TTL串口
volatile unsigned int time_cnt = 1000;			//==SAM_TTL 处理数据超时
static unsigned int i =0;
unsigned int gVar_v_delay_ms,gVar_ms;
extern unsigned int sam_rfid_se2(unsigned char *buf, unsigned int *len_v);
extern unsigned char search_typeb(unsigned char *rbuf,unsigned int *rlen);

#define	V_PIN_CKL					GPIO_Pin_14
#define	V_PIN_CKL_GPIO				GPIOD
#define	V_PIN_CKL_GPIO_CLK			RCC_APB2Periph_GPIOD

#define	V_PIN_SDA					GPIO_Pin_13
#define	V_PIN_SDA_GPIO				GPIOD
#define	V_PIN_SDA_GPIO_CLK			RCC_APB2Periph_GPIOD
//== MCU与SAM_V身份证认证模块控制接口
#define	V_TX_FRAME_PIN				GPIO_Pin_11			
#define	V_TX_FRAME_GPIO				GPIOD
#define	V_TX_FRAME_GPIO_CLK			RCC_APB2Periph_GPIOD

#define	V_RX_FRAME_PIN				GPIO_Pin_12			
#define	V_RX_FRAME_GPIO				GPIOD
#define	V_RX_FRAME_GPIO_CLK			RCC_APB2Periph_GPIOD



void for_us(unsigned int us)
{	
	unsigned int i = 11;
	for(; us>0; us--)
	{
		for(;i>0;i--);
	}
			
}

void clk_gpio_config_out(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(V_PIN_CKL_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= V_PIN_CKL;					//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 			//== 指定输出模式
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_OD; 			//== GPIO_Mode_Out_OD   GPIO_Mode_Out_PP
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度  
	GPIO_Init(V_PIN_CKL_GPIO, &GPIO_InitStructure);

	GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_SET);			
}


void clk_gpio_config_in(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(V_PIN_CKL_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= V_PIN_CKL;					//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;				//== GPIO_Mode_Out_OD   GPIO_Mode_Out_PP
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_OD; 			//== GPIO_Mode_Out_OD   GPIO_Mode_Out_PP
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度
	GPIO_Init(V_PIN_CKL_GPIO, &GPIO_InitStructure);
}


void clk_gpio_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(V_PIN_CKL_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= V_PIN_CKL;					//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_OD; 			//== GPIO_Mode_Out_OD   GPIO_Mode_Out_PP
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度  GPIO_Speed_2MHz GPIO_Speed_10MHz GPIO_Speed_50MHz
	GPIO_Init(V_PIN_CKL_GPIO, &GPIO_InitStructure);

	GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_SET);			
}


void sda_gpio_config_out(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(V_PIN_SDA_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= V_PIN_SDA;					//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 			//== GPIO_Mode_Out_OD   GPIO_Mode_Out_PP
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度
	GPIO_Init(V_PIN_SDA_GPIO, &GPIO_InitStructure);

	GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);				//== 开始为高电平
}


void sda_gpio_config_in(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(V_PIN_SDA_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= V_PIN_SDA;					//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;				//== GPIO_Mode_Out_OD   GPIO_Mode_Out_PP
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度
	GPIO_Init(V_PIN_SDA_GPIO, &GPIO_InitStructure);	 

}

void sda_gpio_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(V_PIN_SDA_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= V_PIN_SDA;					//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_OD; 			//== GPIO_Mode_Out_OD   GPIO_Mode_Out_PP
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度  GPIO_Speed_2MHz GPIO_Speed_10MHz GPIO_Speed_50MHz
	GPIO_Init(V_PIN_SDA_GPIO, &GPIO_InitStructure);		 

	GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);				
}

void init_sam_v_gpio(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	clk_gpio_config();
	sda_gpio_config();

//==TX_FRAME配置成输入脚
	RCC_APB2PeriphClockCmd(V_TX_FRAME_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= V_TX_FRAME_PIN;				//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;				//== GPIO_Mode_IPU GPIO_Mode_Out_OD   GPIO_Mode_Out_PP  GPIO_Mode_IPD
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度
	GPIO_Init(V_TX_FRAME_GPIO, &GPIO_InitStructure);

//==RX_FRAME配置成输出脚
	RCC_APB2PeriphClockCmd(V_RX_FRAME_GPIO_CLK , ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= V_RX_FRAME_PIN;				//== 指定引脚
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP; 			//== 指定输出模式
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;				//== 指定速度
	GPIO_Init(V_RX_FRAME_GPIO, &GPIO_InitStructure);

	GPIO_WriteBit(V_RX_FRAME_GPIO, V_RX_FRAME_PIN, Bit_RESET);		
}


unsigned int read_rec(unsigned char *dat, unsigned int *len)
{ 
	unsigned char tmp = 0, i = 0;
	unsigned int rlen = 0;

	*len  = 0;

	gVar_v_delay_ms = 0; 

	while((GPIO_ReadInputDataBit(V_TX_FRAME_GPIO, V_TX_FRAME_PIN)==0)&&(gVar_v_delay_ms < 90));	
	if(gVar_v_delay_ms >= 90) return 1;


	gVar_v_delay_ms = 0; 
	
	GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);
	while((GPIO_ReadInputDataBit(V_PIN_CKL_GPIO, V_PIN_SDA)==1)&&(gVar_v_delay_ms < 4));	
	if(gVar_v_delay_ms >= 4) return 2;
		
	while((GPIO_ReadInputDataBit(V_PIN_CKL_GPIO, V_PIN_CKL)==1)&&(gVar_v_delay_ms < 4));		
	if(gVar_v_delay_ms >= 4) return 2;

	gVar_v_delay_ms = 0;
	while((GPIO_ReadInputDataBit(V_TX_FRAME_GPIO, V_TX_FRAME_PIN)==1)&&(gVar_v_delay_ms < 90))
	{
		tmp = 0x00;
		for(i = 0;i<8;i++)
		{	
			GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);
			while((GPIO_ReadInputDataBit(V_PIN_CKL_GPIO, V_PIN_CKL)==0)&&(gVar_v_delay_ms < 90));	
			if(gVar_v_delay_ms >= 90) return 3;													
			tmp <<= 1;		
			tmp |=((GPIO_ReadInputDataBit(V_PIN_SDA_GPIO, V_PIN_SDA))&0x01);						
			while((GPIO_ReadInputDataBit(V_PIN_CKL_GPIO, V_PIN_CKL)==1)&&(gVar_v_delay_ms < 90)) 
			{				
				if(GPIO_ReadInputDataBit(V_TX_FRAME_GPIO, V_TX_FRAME_PIN)==0)
					goto stop;
			}
			if(gVar_v_delay_ms >= 90) return 4;																
		}
		
		GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_RESET);								
		
		while((GPIO_ReadInputDataBit(V_PIN_CKL_GPIO, V_PIN_CKL)==0)&&(gVar_v_delay_ms < 90))	
		{
			GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_RESET);							
		}
		if(gVar_v_delay_ms >= 90) return 5;
		*dat =tmp;
		rlen++;	
		while((GPIO_ReadInputDataBit(V_PIN_CKL_GPIO, V_PIN_CKL)==1)&&(gVar_v_delay_ms < 90)) 
		{						
			if(GPIO_ReadInputDataBit(V_TX_FRAME_GPIO, V_TX_FRAME_PIN)==0)
				goto stop;
		}
		if(gVar_v_delay_ms >= 90) return 6;
		dat++;
//		rlen++;
	}
	if(gVar_v_delay_ms >= 90) return 7;

stop:
	GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_SET);				
	GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);			

	*len  = rlen;	

	return 0;

}



unsigned int write_rec(unsigned char *dat, unsigned int wlen)
{ 
	unsigned char tmp = 0, i = 0;

	if(wlen < 1)
	 	return 1;

	GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_SET);			
	GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);			
	
	GPIO_WriteBit(V_RX_FRAME_GPIO, V_RX_FRAME_PIN, Bit_SET);		

	for_us(2);

	GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_RESET);			

	for_us(2);

	GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_RESET);		

	while(wlen--)
	{
		tmp =*dat;	
		for (i=0;i<8;i++)
		{  
			if(tmp&0x80)
				GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);
			else
				GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_RESET);
			tmp <<= 1;		
			GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_SET);	

			for_us(2);
			GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_RESET);	

			for_us(2);	
		}
		GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);			
		
		gVar_v_delay_ms = 0; 
//		sda_gpio_config_in();	
		while((GPIO_ReadInputDataBit(V_PIN_SDA_GPIO, V_PIN_SDA)==1)&&(gVar_v_delay_ms < 4)); 	 
		if(gVar_v_delay_ms >= 4) 
		{	 			
			GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_SET);			
			GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);				
			GPIO_WriteBit(V_RX_FRAME_GPIO, V_RX_FRAME_PIN, Bit_RESET);	
			umod.rCount = 0;   	
			return 2;		
		}
		GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_SET);		

		for_us(2);
		GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_RESET);	

		for_us(2);
		dat++;
	}
	GPIO_WriteBit(V_PIN_CKL_GPIO, V_PIN_CKL, Bit_SET);				
	GPIO_WriteBit(V_PIN_SDA_GPIO, V_PIN_SDA, Bit_SET);				
	GPIO_WriteBit(V_RX_FRAME_GPIO, V_RX_FRAME_PIN, Bit_RESET);	 
			
	umod.rCount = 0;   	

	return 0;
		
}


unsigned char buf_typeAB[20];
unsigned int VSAM_len;
unsigned int sam_rfid(unsigned char* dat, unsigned int *len)
{
	unsigned int ret = 0,i;	

	ret = read_rec(dat, len);	
	if (ret != 0)
		return 1;

	ret = rfid_tx_rx(dat,len);
/*	if(ret == 0)
	{
	   *len =0x0c;
	    for(i=0;i<0x0c;i++)
			dat[i]=buf_typeAB[i];
	} */
	if (ret != 1)
	{
		dat[0] = 0x5a;
		dat[1] = 0xa5;		
		*len = 2;
	}else
	{
		dat[*len] = 0x00;	
		*len +=1; 
	}
	
	ret = write_rec(dat, *len);

	if (ret != 0)
		return 3;

	return ret;
}


unsigned int sam_rfid2(unsigned char* dat, unsigned int *len)
{
	unsigned int ret = 0,i;	

	ret = read_rec(dat, len);	
	if (ret != 0)
		return 1;

	ret = rfid_tx_rx(dat, len);
/*	if(ret == 0)
	{
	   *len =VSAM_len;
	    for(i=0;i<(VSAM_len+1);i++)
			dat[i]=buf_typeAB[i];
	}  */
	if (ret != 1)
	{
		dat[0] = 0x5a;
		dat[1] = 0xa5;	
		*len = 2;
	}else
	{
		dat[*len] = 0x00;	
		*len +=1; 
	}
	
	ret = write_rec(dat, *len);

	if (ret != 0)
		return 3;

	return ret;
}

unsigned char GetChkSum(unsigned char *pBuf,unsigned int len)
{
	unsigned int i;
	unsigned char xorVal = 0;

	for(i=0;i<len;i++)
		xorVal ^= pBuf[i];
	return xorVal;
}


unsigned int sam_v_tmsg(V_MsgMod *tMsg)
{ 
//    memset((unsigned char*)&umod, 0, sizeof(SAM_UsartMod));			//==清零
	memset((unsigned char*)&umod, 0, 16);			//==清零

	umod.tBuf[0] = 0xAA; umod.tBuf[1] = 0xAA; umod.tBuf[2] = 0xAA; umod.tBuf[3] = 0x96;	umod.tBuf[4] = 0x69;
	umod.tCount  = tMsg->Len;
	umod.tBuf[5] = ((umod.tCount)>>8)&0xff; 
	umod.tBuf[6] = (umod.tCount)&0xff;
	umod.tBuf[7] = tMsg->CMD; 
	umod.tBuf[8] = tMsg->Para;
	memcpy(umod.tBuf+9, tMsg->Data,umod.tCount - 3);
	umod.tBuf[umod.tCount+6] =GetChkSum(umod.tBuf+5,umod.tCount+1);
	umod.tCount += 7;  
	


	USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);	
	TxData(UART5, umod.tBuf, umod.tCount);	

	umod.rCount = 0;   								
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);	
	return 0;	
} 


unsigned int sam_v_rmsg(V_MsgMod *rMsg)
{
	gVar_v_delay_ms = 0;  
	while(1)
	{
		if(gVar_v_delay_ms > time_cnt)	  	
			break;
		if(umod.rCount > 7)
		{
			if ( (umod.rBuf[0] == 0xAA)&&(umod.rBuf[1] == 0xAA)&&(umod.rBuf[2] == 0xAA))
			{
			 	memcpy(rMsg->P, umod.rBuf, 5);
				rMsg->Len = (unsigned short)(umod.rBuf[5]<<8|umod.rBuf[6]);
			} else
				break;

			if ( (rMsg->Len) > (umod.rCount-7))
			{
				if((rMsg->Len) > time_cnt)
					time_cnt = rMsg->Len; 
			}

			if( (rMsg->Len) == (umod.rCount-7))
			{
				rMsg->SW1 = umod.rBuf[7];
				rMsg->SW2 = umod.rBuf[8];
				rMsg->SW3 = umod.rBuf[9];
				memcpy(rMsg->Data,umod.rBuf+10,(unsigned int)(rMsg->Len - 3));
				break;	
			}
		}	
	}

	umod.rCount = 0;   					

	if(gVar_v_delay_ms >= time_cnt) 
		return 1;  							  		

	return 0;
	
}


unsigned int sam_v_reset(V_MsgMod *msg)
{ 
	unsigned int ret = 0;

	msg->Len  = 2+1;	
	msg->CMD  = 0x10;
	msg->Para = 0xff;
	sam_v_tmsg(msg);

	time_cnt = REQ_TIME;	

	ret = sam_v_rmsg(msg);
		

	return ret;		
}

unsigned int sam_v_check_statue(V_MsgMod *msg)
{
	msg->Len  = 2+1;	//==CMD+Para+Data+CHK
	msg->CMD  = 0x11;
	msg->Para = 0xff;
	sam_v_tmsg(msg);

	time_cnt = REQ_TIME;

	return sam_v_rmsg(msg);
}


unsigned int sam_v_read_manage_info(V_MsgMod *msg)
{
	msg->Len  = 2+1;	
	msg->CMD  = 0x12;
	msg->Para = 0xff;
	sam_v_tmsg(msg);

	time_cnt = 100;	

	return sam_v_rmsg(msg);
}



unsigned int sam_v_set_uart_rate(V_MsgMod *msg, unsigned char rate)
{
	switch(rate)
	{
	 	case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			break;
		default: return 2;
	}

	msg->Len  = 2+1;	
	msg->CMD  = 0x60;
	msg->Para = rate;
	sam_v_tmsg(msg);

	time_cnt = REQ_TIME;	

	return sam_v_rmsg(msg);
}


unsigned int sam_v_set_frame_size(V_MsgMod *msg, unsigned char mBytes)
{		
	if(mBytes < 0x18) return 2;

	msg->Len  = 2+1+1;	
	msg->CMD  = 0x61;
	msg->Para = 0xff;
	msg->Data[0] = mBytes;
	sam_v_tmsg(msg);

	time_cnt = REQ_TIME;	

	return sam_v_rmsg(msg);
}


unsigned int sam_v_req(V_MsgMod *msg)
{
	unsigned char dat[32];
	unsigned int len = 0, ret =0;

	msg->Len  = 2+1;	
	msg->CMD  = 0x20;
	msg->Para = 0x01;
	sam_v_tmsg(msg); 
	time_cnt = REQ_TIME;
	ret = sam_rfid(dat, &len);
//	if (ret != 0)
//		return 1;

		

	ret = sam_v_rmsg(msg);
	if (ret != 0)
		return 2;

	return ret;
}

unsigned int sam_rfid1(unsigned char* dat, unsigned int *len)
{
	unsigned int ret = 0;	

	ret = read_rec(dat, len);	
	if (ret != 0)
		return 1;

	ret = rfid_tx_rx(dat, len);
/*	if (ret == 0)
	{
		*len =VSAM_len;
	    for(i=0;i<VSAM_len;i++)
			dat[i]=buf_typeAB[i];
	}  */
	if (ret != 1)
	{
		dat[0] = 0x5a;
		dat[1] = 0xa5;	
		*len = 2;
	}else
	{
		dat[*len] = 0x00;		
		*len +=1; 
	}

	ret = write_rec(dat, *len);

	if (ret != 0)
		return 3;

	return ret;
}
unsigned int sam_v_select(V_MsgMod *msg)
{
	unsigned char dat[32];
	unsigned int len = 0, ret =0;

	msg->Len  = 2+1;	//==CMD+Para+Data+CHK
	msg->CMD  = 0x20;
	msg->Para = 0x02;
	sam_v_tmsg(msg); 

	ret = sam_rfid1(dat, &len);
	

	

	time_cnt = SEL_TIME;	

	ret = sam_v_rmsg(msg);
	if (ret != 0)
		return 2;

	return ret;
} 


unsigned int sam_v_info(V_MsgMod *msg)
{
	unsigned char dat[256];
	unsigned int len = 0, ret =0;

	msg->Len  = 2+1;
	msg->CMD  = 0x30;
//	msg->Para = 0x10; 
	msg->Para = 0x01;
	sam_v_tmsg(msg);
	
	while(1)
	{
		memset(dat,0,8);
		ret = sam_rfid2(dat, &len);
		if (ret != 0)
			break;
	}

	time_cnt = READ_INFO_TIME; 

	ret = sam_v_rmsg(msg);
	if (ret != 0)
		return 1;

	return ret;
}


unsigned int sam_v_info1(V_MsgMod *msg)
{
	unsigned char dat[256];
	unsigned int len = 0, ret =0;

	msg->Len  = 2+1;
	msg->CMD  = 0x30;
	msg->Para = 0x03;   
	sam_v_tmsg(msg);

	while(1)
	{
		memset(dat,0,8);
		ret = sam_rfid(dat, &len);
		if (ret != 0)
			break;
	}

	time_cnt = READ_INFO_TIME; 	

	ret = sam_v_rmsg(msg);
	if (ret != 0)
		return 1;

	return ret;
}

unsigned int sam_v_add_info(V_MsgMod *msg)
{
	unsigned char dat[256];
	unsigned int len = 0, ret =0;

	msg->Len  = 2+1;	
	msg->CMD  = 0x30;
	msg->Para = 0x03;
	sam_v_tmsg(msg);

	while(1)
	{
		memset(dat,0,8);
		ret = sam_rfid(dat, &len);
		if (ret != 0)
			break;
	}

	time_cnt = READ_ADD_INFO_TIME;

	ret = sam_v_rmsg(msg);
	if (ret != 0)
		return 2;

	return ret;
}


unsigned int sam_v_readcard_manageinfo(V_MsgMod *msg)
{
	msg->Len  = 2+1;	//==CMD+Para+Data+CHK
	msg->CMD  = 0x30;
	msg->Para = 0x05;
	sam_v_tmsg(msg);

	time_cnt = REQ_TIME;

	return sam_v_rmsg(msg);
}


unsigned int sam_v_test(void)
{

	return 0;		
}


unsigned int search_id(unsigned char *dat,unsigned int *len)
{
	V_MsgMod msg;
	unsigned int ret=0;

	memset((unsigned char*)&msg, 0, sizeof(V_MsgMod));

	msg.Data = dat;
	*len = 0;

	ret = sam_v_req(&msg);
	if ((ret == 0)&&(msg.SW3 == 0x9f))
	{ 
		*len = msg.Len -3; 	
	}else
		ret =1;

	return ret;		
}


unsigned int read_id_info(unsigned char *dat,unsigned int *len)
{  	 
	V_MsgMod msg;
	unsigned int ret=0,i;

	memset((unsigned char*)&msg, 0, sizeof(V_MsgMod)); 
	msg.Data = dat;
	*len = 0;
	init_sam_v_gpio();
	gVar_ms = 0;					
	while(gVar_ms < 200)			
	{
		ret = sam_v_req(&msg);
		if ((ret == 0)&&(msg.SW3 == 0x9f))
		{ 	
			ret = sam_v_select(&msg);
			if ((ret == 0)&&(msg.SW3 == 0x90))	
			{				
				ret = sam_v_info(&msg);
				if ( (ret == 0) && (msg.SW3 == 0x90) )
				{
					*len = msg.Len -3;
					beep_con();	
					break;
				}else		
					ret = 3;		
			} else
				ret = 2;			
		}else
			ret = 1;				
	}

	return ret;	  
} 


unsigned int read_id_info1(unsigned char *dat,unsigned int *len)
{  	 
	V_MsgMod msg;
	unsigned int ret=0;

	memset((unsigned char*)&msg, 0, sizeof(V_MsgMod)); 
	msg.Data = dat;
	*len = 0;
	init_sam_v_gpio();
	gVar_ms = 0;				
	while(gVar_ms < 200)			
	{
		ret = sam_v_req(&msg);
		if ((ret == 0)&&(msg.SW3 == 0x9f))
		{ 	
			ret = sam_v_select(&msg);
			if ((ret == 0)&&(msg.SW3 == 0x90))	
			{							
				ret = sam_v_info1(&msg);
				if ( (ret == 0) && (msg.SW3 == 0x90) )
				{
					*len = msg.Len -3;
					break;
				}else		
					ret = 3;		
			} else
				ret = 2;		
		}else
			ret = 1;				
	}

	return ret;	  
} 


unsigned int read_sam_v_id(unsigned char *dat,unsigned int *len)
{
	V_MsgMod msg;
	unsigned int ret=0;

	memset((unsigned char*)&msg, 0, sizeof(V_MsgMod));

	msg.Data = dat;
	*len = 0;

	ret = sam_v_read_manage_info(&msg);
	if ((ret == 0)&&(msg.SW3 == 0x90))
	{ 
		*len = msg.Len -3; 	
	}else
		ret =1;

	return ret;		
}  


unsigned int get_sam_v_statue(void)
{
	V_MsgMod msg;
	unsigned int ret=0;

	memset((unsigned char*)&msg, 0, sizeof(V_MsgMod));

	ret = sam_v_check_statue(&msg);

	if (ret == 0)
	{
		if (msg.SW3 == 0x90)
			ret = 0;
		else
			ret = 1;
	}else
		ret = 2;		

	return ret;		
} 


void init_sam_v(void)
{
	V_MsgMod msg;
	unsigned char dat[256];

	memset((unsigned char*)&msg, 0, sizeof(V_MsgMod));	
	msg.Data = dat;			 
	init_usartx(UART5,115200);
//	TM4_config();									
	memset((unsigned char*)&umod, 0, sizeof(SAM_UsartMod));				
	init_sam_v_gpio();								
	sam_v_reset(&msg);							

	sam_v_set_frame_size(&msg,0xff);			
}


void sam_uartIRQProcess(USART_TypeDef* USARTx)
{
	//== 接收中断
	if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET) 
	{
		if(umod.rCount == 0){			
			umod.rBuf[umod.rCount++] = USART_ReceiveData(USARTx); 
		}else
			umod.rBuf[umod.rCount++] = USART_ReceiveData(USARTx);			
	}
			
	//==发送中断
	if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
	{
		for (i= 0; i< umod.tCount; i++)
		{
			USART_SendData(USARTx, umod.tBuf[i]);
			while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){ }
			USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
		}		
		if(i >= umod.tCount)
		{
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);  //== 禁止发送中断
			umod.tCount = 0;
			umod.rCount = 0;
		}		
	}	
}


