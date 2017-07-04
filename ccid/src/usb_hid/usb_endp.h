typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;

/******************************************************************************
 * ���� ����ֵ
******************************************************************************/
#define USB_REQ_ERROR                           0
#define USB_REQ_SUCCESS                         2
#define USB_REQ_UNSUPPORT                       4

/******************************************************************************
 * UBS �˵����ýṹ����
******************************************************************************/
typedef struct
{
    unsigned int    data_remainsize;                                // ʣ������ݳ���
    unsigned int    data_completesize;                              // ��ɵ����ݳ���
    unsigned char   *pEndpDatbuf;                                   // ��������ָ��
    unsigned char   data_thanse_complate ;                           //�������
}ENDP_STATUS_T;

extern uint8_t USB_SendData(uint8_t bEpNum,uint8_t *pDatBuf,uint16_t wTxLen);


