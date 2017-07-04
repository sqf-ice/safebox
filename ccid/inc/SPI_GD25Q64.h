

#define Select_Flash()     GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define NotSelect_Flash()    GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define Dummy_Byte 0

extern void SPI_Flash_Init(void);
extern unsigned char SPI_ExFlash_GetCapcity(void);
extern void Spi_Chip_Erase(void);
extern unsigned char  SPI_ExFlash_WriteData(unsigned long PageAddr, unsigned char  *pbString, unsigned short Len);
extern void SPI_ExFlash_ReadData(unsigned long ReadAddr,unsigned char * DataAddr,unsigned int n);
extern void SPI_ExFlash_EraseBlock(unsigned long BlockAddr); 
extern void SPI_ExFlash_FlashReadID(void);

