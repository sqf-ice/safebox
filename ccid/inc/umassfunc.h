

extern unsigned int USB_SIL_Write(unsigned char bEpAddr, unsigned char * pBufferPointer, unsigned int wBufferSize);
extern  unsigned int USB_SIL_Read(unsigned char bEpAddr, unsigned char* pBufferPointer);
extern void  API_MakeData(void);
extern void Read_Memory(unsigned char lun, unsigned int Memory_Offset, unsigned int Transfer_Length);
extern void Write_Memory (unsigned char lun, unsigned int Memory_Offset, unsigned int Transfer_Length);




