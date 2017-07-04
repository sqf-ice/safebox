/* header file */
#ifndef _DECLARE_H_
#define _DECLARE_H_

//#include "Z8D256U.h"

#ifdef SYS_GLOBALS
#define SYS_EXT
#else
#define SYS_EXT extern
#endif

#define		FLASH_CS				P1_7	//GP15	

#define     W25X16_BLOCK                0x001FFFFF
#define     W25X32_BLOCK                0x003FFFFF
#define     W25X64_BLOCK                0x007FFFFF
#define     BLOCK_SIZE      			0x10000//(64*1024)     //64k

#define     LED_PIN                     0x01
//#define     LED_DRIVE                   0x01
//#define     LED_FREQUENCE               0x05
//#define     TIMER0_FREQUENCE            0x3CB0   //15536
//#define     TIMER1_FREQUENCE            0x3CB0   //15536

#define     spi_SS_PIN                  4//16spi flash
//#define     spi_SS_DRIVE                16
//#define     SPI_MODE_RECV               0x01
//#define     SPI_MODE_SEND               0x00

#define     EXECUTE_OK                  0x00
#define     ERR_ADDRESS                 0x01
#define     CMP_EQUAL                   0x00
#define     CMP_GREATER                 0x01
#define     CMP_LESS                    0x02

#define		TIME0_AUTO_16_MASK  		0xFC
#define 	TIME0_AUTO_16_MODE0 		0x00
#define     TIME1_AUTO_16_MASK          0xCF
#define     TIME1_AUTO_16_MODE0         0x10

#define     ReleaseDevice               0x01
#define     ReadDeviceID                0x02
#define     ReleaseDevice_ReadDeviceID  0x03

//SYS_EXT unsigned char idata program[256];
//SYS_EXT unsigned char idata receive[256];
//SYS_EXT unsigned char idata Flashcmd[5];

void APL_ExFlash_EraseBlock(unsigned long BlockAddr);
//void APL_ExFlash_EraseChip(void);
//unsigned char APL_ExFlash_GetCapcity(void);
//unsigned char APL_ExFlash_WriteData(unsigned long PageAddr, unsigned char *pbString, unsigned int Len);
//void APL_ExFlash_ReadData (unsigned long ReadAddr,unsigned char* DataAddr,unsigned int n);

void spi_PowerDown(void);
void spi_ReadStatusRegister(void);
void spi_WriteDisable(void);
void spi_WriteEnable(void);
//void spi_WriteStatusRegister(unsigned char StatusByte);
void workingstatus_TmrIsr(void);
extern void SPI_sendNByte(unsigned char * buf, unsigned int len);
extern void SPI_recieveNByte(unsigned char * buf, unsigned int len);

#endif
