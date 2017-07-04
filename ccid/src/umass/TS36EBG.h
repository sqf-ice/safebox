


extern void TS36ebg_UartInit(void);
extern unsigned char TS36EBG_CONNECT(unsigned char *);


extern void AS606_FPC1020_UartInit(void);
extern unsigned  char  AS606_FPC1020_UartSendData(unsigned char * buf,unsigned int len );
extern unsigned  char  AS606_FPC1020_UartRecieveData_OnePackage(unsigned char * buf,unsigned int *len );
extern unsigned  short AS606_FPC1020_UartRecieveData_ContinuePackages(unsigned char * buf,unsigned int * len);
extern unsigned  int   AS606_FPC1020_length;
extern unsigned  int   AS606_FPC1020_index;
extern unsigned  char	  AS606_FPC1020_GetRecievedPackage(unsigned char * buf, unsigned short numpackage,unsigned short* len);

extern void WEL_Init(void);
extern unsigned char	WEL_SendData(unsigned char * buf,  unsigned int len);
extern unsigned char	WEL_RecieveData(unsigned char * buf,  unsigned int len, unsigned char flag);
extern unsigned char	WEL_TransferData(unsigned char * sbuf, unsigned char * rbuf, unsigned int slen, unsigned int * rlen);
extern unsigned  char WEL_CheckFingerExist(void);

extern unsigned char FingerPrintDevice;



