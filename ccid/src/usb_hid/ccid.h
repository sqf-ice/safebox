#ifndef _CCID_H_
#define _CCID_H_
//extern  uint32_t bDeviceState; /* USB device status */
extern unsigned char UsbBulkMsgBuf[271];
extern unsigned char send_flag;
extern unsigned char  slot;
extern unsigned char power_flag;
/*********************************************************************************************************
 * 定义 CCID PC_TO_RDR 命令字
*********************************************************************************************************/
#define PC_TO_RDR_ICC_POWERON							0x62
#define PC_TO_RDR_ICC_POWEROFF							0x63
#define PC_TO_RDR_GET_SLOTSTATUS						0x65
#define PC_TO_RDR_XFR_BLOCK								0x6F
#define PC_TO_RDR_GETPARAMETERS							0x6C
#define PC_TO_RDR_RESETPARAMETERS						0x6D
#define PC_TO_RDR_SET_PARAMETERS						0x61
#define PC_TO_RDR_ESCAPE								0x6B
#define PC_TO_RDR_ICCCLOCK								0x6E
#define PC_TO_RDR_T0APDU								0x6A
#define PC_TO_RDR_SECURE								0x69
#define PC_TO_RDR_MECHANICAL							0x71
#define PC_TO_RDR_ABORT									0x72
#define PC_TO_RDR_SETDATARATEANDCLOCKFREQUENCY			0x73


#define	RDR_TO_PC_DATABLOCK								0x80
#define	RDR_TO_PC_SLOTSTATUS							0x81
#define	RDR_TO_PC_PARAMETERS							0x82
#define	RDR_TO_PC_ESCAPE								0x83
#define	RDR_TO_PC_DATARATEANDCLOCKFREQUENCY				0x84


#define	RDR_TO_PC_NOTIFYSLOTCHANGE						0x50
#define	RDR_TO_PC_HARDWAREERROR							0x51

#define NO_ERROR                   0x81

/*********************************************************************************************************
 * CCID 卡槽状态类型定义
*********************************************************************************************************/
typedef union _CCID_SLOT_STATUS
{
	unsigned char _byte;
	struct
	{
		unsigned char    Slot0CurrentState:1;
		unsigned char 	 Slot0ChangedState:1;
		unsigned char    BulkOutCompeleteFlag:1;                     //BULK OUT传输结束
        unsigned char    BulkInCompeleteFlag:1;                      //BULK IN 传输结束
    }BitCtl;

}CCID_SLOT_STATUS;
/*********************************************************************************************************
 * CCID 非接卡槽状态类型定义
*********************************************************************************************************/
typedef union _CCID_NSLOT_STATUS
{
	unsigned char _Nbyte;
	struct
	{
		unsigned char    NSlot0CurrentState:1;
		unsigned char 	 NSlot0ChangedState:1;
//		unsigned char    BulkOutCompeleteFlag:1;                     //BULK OUT传输结束
//        unsigned char    BulkInCompeleteFlag:1;                      //BULK IN 传输结束
    }NBitCtl;

}CCID_NSLOT_STATUS;

extern volatile CCID_SLOT_STATUS CCIDSlotStatus;
extern volatile CCID_NSLOT_STATUS CCIDNSlotStatus;
#define Reset_CCIDStatus            CCIDSlotStatus._byte &= 0x03
#define Reset_NCCIDStatus            CCIDNSlotStatus._Nbyte &= 0x03
#define bBulkOutCompeleteFlag      CCIDSlotStatus.BitCtl.BulkOutCompeleteFlag
#define Set_bBulkOutCompeleteFlag  bBulkOutCompeleteFlag = 1
#define Clr_bBulkOutCompeleteFlag  bBulkOutCompeleteFlag = 0

#define bBulkInCompeleteFlag       CCIDSlotStatus.BitCtl.BulkInCompeleteFlag
#define Set_bBulkInCompeleteFlag   bBulkInCompeleteFlag = 1
#define Clr_bBulkInCompeleteFlag   bBulkInCompeleteFlag = 0

unsigned char  PC_to_RDR_IccPowerOn(void);
unsigned char PC_to_RDR_IccPowerOff(void);
unsigned char PC_to_RDR_SetParameters(void);
unsigned char PC_to_RDR_XfrBlock(void);
void CCID_CmdNotSupport(void);
void RDR_to_PC_Parameters(unsigned char err);
void RDR_to_PC_DataBlock(unsigned char err);
void RDR_to_PC_SlotStatus(unsigned char err);
extern void CCID_StatePolling(void);
extern void CCID_AnalyseBulkMessage(void);
//extern void memcpy(unsigned char *str,unsigned char *ptr,unsigned char len);


#endif 



