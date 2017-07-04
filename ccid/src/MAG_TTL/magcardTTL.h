#ifndef __MAGCARDTTL_H
#define __MAGCARDTTL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes */
#include "stm32f10x.h"

/* Track Buffer Size */

/***************************************
* Return State Code
***************************************/
/* CORRECT CODE For USER*/
#define	MAGCARDOK	        0x0000
#define ERR_TimeOut			0x6300	//==读2/3道数据超时错误



typedef struct _MagcardGlobalIOTTL
{
	volatile unsigned int magcardRevcTimeFlag;		
	volatile unsigned int magcardRevcTimeCount;	

}MagcardGlobalIOTTL;							  

extern 	MagcardGlobalIOTTL magcardGlobalIOTTL;

/*Extern Function Declarations*/
extern void ReadyToReadMagcard(void);
extern void MagcardTTLGPIOInit(void);
extern void ResetMagcard_TTL(void);
extern int ReadMagcardT2Data_TTL(unsigned char *rbuf);
extern int ReadMagcardT3Data_TTL(unsigned char *rbuf);
extern int ReadMagcardT2T3Data_TTL(unsigned char *T2T3rbuf);
#ifdef __cplusplus
}
#endif

#endif /* __MAGCARDTTL_H */

