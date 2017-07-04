//#include "stm32f10x_nvic.h	"
/*
typedef unsigned char   BOOLEAN;
typedef unsigned char   UINT8;
typedef signed   char   SINT8;
typedef unsigned short  UINT16;
typedef signed   short  SINT16;
typedef unsigned int    UINT32;
typedef signed 	 int    SINT32;
typedef unsigned char   BYTE;



#define BYTE unsigned char
#define WORD unsigned short
#define ADWORD unsigned int
#define DWORD unsigned int

#define 	UCHAR	 unsigned char
#define     USHORT   unsigned short
#define     UINT	 unsigned int
#define     ULONG    unsigned long	 */
//extern void	 NVIC_RESETPRIMASK(void) ;
//extern void	 NVIC_SETPRIMASK(void);
//#define Enable_Interrupts     NVIC_RESETPRIMASK()    	  //Enable_Interrupts	   cli()
//#define Disable_Interrupts  NVIC_SETPRIMASK()   //__set_primask(0)  
//#define MYREG16(addr)  (((WORD) (*((unsigned char volatile *)(addr))) <<8) | (*((unsigned char volatile *)(addr+1))))
//#define CLI()      __set_PRIMASK(1)  
//#define SEI()      __set_PRIMASK(0)  

#define Disable_Interrupts      __set_PRIMASK(1)  
#define Enable_Interrupts      __set_PRIMASK(0)  




