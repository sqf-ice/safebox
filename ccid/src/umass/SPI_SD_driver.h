#ifndef _SPI_SD_DRIVER_H_
#define _SPI_SD_DRIVER_H_

/* Private define ------------------------------------------------------------*/
/* SD�����Ͷ��� */
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

/* SPI�����ٶ�����*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

/* SD�������ݽ������Ƿ��ͷ����ߺ궨�� */
#define NO_RELEASE      0
#define RELEASE         1

/* SD��ָ��� */
#define CMD0    0       //����λ
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define ACMD41  41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00

/* Private macro -------------------------------------------------------------*/
//SD��CSƬѡʹ�ܶ˲����� PB12
#define SD_CS_ENABLE()      GPIO_ResetBits(GPIOC,(u16)0x20)  //ѡ��SD��
#define SD_CS_DISABLE()     GPIO_SetBits(GPIOC,(u16)0x20)     //��ѡ��SD��
#define SD_DET()           1
                                                                    //1-�� 0-��

/* Private function prototypes -----------------------------------------------*/
void SPI_Configuration(void);
void SPI_SetSpeed(unsigned char SpeedSet);
unsigned char SPI_ReadWriteByte(unsigned char TxData);
unsigned char SD_WaitReady(void);
unsigned char SD_SendCommand(unsigned char cmd, unsigned long arg, unsigned char crc);
unsigned char SD_SendCommand_NoDeassert(unsigned char cmd, unsigned long arg, unsigned char crc);
unsigned char SD_Init(void);
unsigned char SD_ReadSingleBlock(unsigned long sector, unsigned char *buffer);
unsigned char SD_ReadMultiBlock( unsigned long sector, unsigned char *buffer, unsigned char count);
unsigned char SD_WriteSingleBlock(unsigned long sector, const unsigned char *data);
unsigned char SD_WriteMultiBlock(unsigned long sector, const unsigned char *data, unsigned char count);
unsigned long SD_GetCapacity(void);
#endif
