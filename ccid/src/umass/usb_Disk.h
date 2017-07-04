 #ifndef __USBDISK_H
#define __USBDISK_H
 #include"headfile.h"
 //#include"usb_msc.h"
#define K_SUCCES		0
#define K_ERROR			1
 extern unsigned char udisk_read(void);
 extern unsigned char udisk_write(void);
 extern unsigned long  udisk_lba_param[2];
 extern unsigned char g_ramBuf[];//global USB Var
#define SIZE_OF_SPIFLASH_PAGE  		0x100
#define SIZE_OF_CN_LIB         		0x200000UL//(2*1024*1024)	//用于液晶显示的汉字字库大小
#define SIZE_OF_CN_LIB_COMPRESS  	0x0a8000UL//(2*1024*1024*5/8) //从字库1.25M开始至1.25+640K处作为CD-ROM空间
#define BLOCK_CNT_OF_CN_LIB    		0x20	//(SIZE_OF_CN_LIB/BLOCK_SIZE)
#define BLOCK_CNT_OF_CN_LIB_COMPRESS    0x14//(SIZE_OF_CN_LIB_COMPRESS/BLOCK_SIZE)
#define BLOCK_CNT_OF_CD_ROM    		(0x0A)		//640K
#define     BLOCK_SIZE      			0x10000//(64*1024)     //64k

#define INDEX_UDISK_LBA_START                       0
#define INDEX_UDISK_LBA_XFER_COUNT                  1
#define SIZE_OF_USBSPILT 0x200
#define SIZE_OF_SPIFLASH_PAGE  		0x100
#endif

