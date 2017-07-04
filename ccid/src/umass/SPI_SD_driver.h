#ifndef _SPI_SD_DRIVER_H_
#define _SPI_SD_DRIVER_H_

/* Private define ------------------------------------------------------------*/
/* SD卡类型定义 */
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

/* SPI总线速度设置*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

/* SD传输数据结束后是否释放总线宏定义 */
#define NO_RELEASE      0
#define RELEASE         1

/* SD卡指令表 */
#define CMD0    0       //卡复位
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define ACMD23  23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define ACMD41  41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00

/* Private macro -------------------------------------------------------------*/
//SD卡CS片选使能端操作： PB12
#define SD_CS_ENABLE()      GPIO_ResetBits(GPIOC,(u16)0x20)  //选中SD卡
#define SD_CS_DISABLE()     GPIO_SetBits(GPIOC,(u16)0x20)     //不选中SD卡
#define SD_DET()           1
                                                                    //1-有 0-无

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
