typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;

/******************************************************************************
 * 定义 返回值
******************************************************************************/
#define USB_REQ_ERROR                           0
#define USB_REQ_SUCCESS                         2
#define USB_REQ_UNSUPPORT                       4

/******************************************************************************
 * UBS 端点配置结构类型
******************************************************************************/
typedef struct
{
    unsigned int    data_remainsize;                                // 剩余的数据长度
    unsigned int    data_completesize;                              // 完成的数据长度
    unsigned char   *pEndpDatbuf;                                   // 传输数据指针
    unsigned char   data_thanse_complate ;                           //传输完成
}ENDP_STATUS_T;

extern uint8_t USB_SendData(uint8_t bEpNum,uint8_t *pDatBuf,uint16_t wTxLen);


