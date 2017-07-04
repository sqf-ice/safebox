
#define Dummy_Byte 0xA5

/* Select SPI FLASH: ChipSelect pin low  */
#define Select_Flash()     GPIO_ResetBits(GPIOC, GPIO_Pin_4)
/* Deselect SPI FLASH: ChipSelect pin high */
#define NotSelect_Flash()    GPIO_SetBits(GPIOC, GPIO_Pin_4)



void SPI_Flash_Init(void);	         //SPI��ʼ��
u8 SPI_Flash_ReadByte(void);		//flash����������������һ���ֽ�
u8 SPI_Flash_SendByte(u8 byte);		//	FLASH������������������һ���ֽ�


void FlashWaitBusy(void);			    //Flashæ���
void FlashReadID(void);		//��ȡflashID�ĸ��ֽ�
	

