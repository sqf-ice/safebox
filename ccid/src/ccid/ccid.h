#ifndef _CCID_H_
#define _CCID_H_
//extern  uint32_t bDeviceState; /* USB device status */
extern unsigned char UsbBulkMsgBuf[271];
extern unsigned char send_flag;
extern unsigned char  slot1;
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



/*********************************************************************************************************
 * CCID 卡槽状态类型定义
*********************************************************************************************************/
typedef union _CCID_SLOT_STATUS
{
	unsigned char _byte;
	struct
	{
		unsigned char 	Slot0CurrentState:1;
		unsigned char 	Slot0ChangedState:1;
		unsigned char    BulkOutCompeleteFlag:1;                     //BULK OUT传输结束
        unsigned char    BulkInCompeleteFlag:1;                      //BULK IN 传输结束
    }BitCtl;

}CCID_SLOT_STATUS;
extern volatile CCID_SLOT_STATUS CCIDSlotStatus;

typedef struct
{
	unsigned char		FiDi;											//FiDi 参数
	unsigned char 	T01ConvChecksum;								//正向 反向以及CheckSUM标志
	unsigned char 	GuardTimeInteger;								//保护时间
	unsigned char 	WaitingTimeInteger;								//等待时间(WWT CWT BWT)
	unsigned char 	ClockStop;										//时钟停止
	unsigned char 	Ifsc;
	unsigned char 	Nad;
}ISO7816_param_T01_t;



#define Reset_CCIDStatus            CCIDSlotStatus._byte &= 0x03

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
extern void memcpy(unsigned char *str,unsigned char *ptr,unsigned char len);


/************************************************************************/
/* 定义错误代码
*************************************************************************/
#define SLOT_NO_ERROR						0x81
#define ISO7816_NO_ERROR					0x81				// 无错误
#define ISO7816_VOLTAGE_ERROR				0x07				// 电压参数错误


#define ISO7816_CMD_ABORT					0xff				// 指令取消
#define ISO7816_ICC_MUTE					0xfe				// ICC 无响应 超时
#define ISO7816_XFR_PARITY					0xFD				// 奇偶校验错误
#define ISO7816_XFR_OVERRUN					0xFC				// 缓冲区溢出


#define ISO7816_HW_ERROR					0xfb				// 硬件错误
#define ISO7816_BAD_ATR_TS					0xf8				// ATR 起始字符错误
#define ISO7816_BAD_ATR_TCK					0xf7				// ATR TCK字符错误
#define ISO7816_ICC_PROTOCOL_NOT_SUPPORT	0xf6				// 类型不支持
#define ISO7816_ICC_CLASS_NOT_SUPPORT		0xf5				// 类型不支持
#define ERROR_PROCEDURE_BYTE_CONFILT		0xf4				//过程字节冲突

#define ISO7816_CMD_NOT_SUPPORTED           0x00                //命令不支持

#define ERROR_BAD_DWLENGTH					0x01			//dwLength域错误
#define ERROR_BAD_BSLOT						0x05			//卡槽号 错误
#define ERROR_BAD_BSEQ						0x06			//指令序号错误
#define ERROR_BAD_ABRFU_3					0x07			//3字节保留数据错误
#define ERROR_BAD_ABRFU_2					0x08			//2字节保留数据错误
#define ERROR_BAD_BPROTOCOLNUM				0x07			//协议类型错误
#define ERROR_BAD_POWERSELECT				0x07			//电压参数错误


#define ERROR_BAD_FIDI						0x0a			//FIDI参数错误
#define ERROR_BAD_TCCKS						0x0b			//TCKS参数错误
#define ERROR_BAD_GUARDTIME					0x0c			//GUARD参数错误
#define ERROR_BAD_WAITINGINTEGER			0x0d			//WAITINGINTEGER参数错误
#define ERROR_BAD_CLOCKSTOP					0x0e			//CLKSTOP参数错误
#define ERROR_BAD_IFSC						0x0f			//IFSC参数错误
#define ERROR_BAD_NAD						0x10			//NAD参数错误

#define ERROR_BAD_WLEVELPARAMETER			0x08			//WLEVELPARAMETER参数错误
#endif 



