

#define BYTE unsigned char
#define WORD unsigned short
#define ADWORD unsigned int
#define DWORD unsigned int
typedef unsigned char  			BOOLEAN;
typedef unsigned char           UINT8;
typedef signed char             INT8;
typedef unsigned short       UINT16;
typedef signed short         INT16;
typedef unsigned int            UINT32;
typedef signed int              INT32;
typedef float          		    FP32;
typedef double         		    FP64;


#define FLASH_PAGE_SIZE    ((uint16_t)0x800) 
#define FLASH_PAGE_NUM 0xfffff800
#define FLASH_PAGE_ADDR 0x7ff

#define FLASH_ADDR_USER 0x08030000
