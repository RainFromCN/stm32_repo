#include "stm32f4xx_hal.h"


/************* GPIO *************/

/** cs：W25Q片选信号
  * 
  *（1）模式            推挽输出
  *（2）上下拉          不上下拉
  *（3）速度            非常高
  *
  */

/*************  SPI *************/

/** spi：与W25Q进行通信
  *
  *（1）极性            高 
  *（2）边沿            第二边沿
  *（3）速度            20Mb/s以内
  *
  */


/* 设备属性 */
#define W25Q_ID	                0xEF4018 
#define W25Q_PAGESIZE           256
#define W25Q_SECTORSIZE         4096


/* 常用指令 */
#define W25Q_WRITE_ENABLE       0x06
#define W25Q_WRITE_DISABLE      0x04
#define W25Q_READ_STATUS        0x05
#define W25Q_WRITE_STATUS       0x01
#define W25Q_READ_DATA          0x03
#define W25Q_FAST_READ          0x0B
#define W25Q_PAGE_PROGRAM       0x02
#define W25Q_BLOCK_ERASE        0XD8
#define W25Q_SECTOR_ERASE       0x20
#define W25Q_CHIP_ERASE         0xC7
#define W25Q_PWR_DOWN           0xB9
#define W25Q_RELEASE_PWR_DOWN   0xAB
#define W25Q_JUDEC_ID           0X9F
#define W25Q_DEV_ID	            0xAB


#define W25Q_CS_RESET()         HAL_GPIO_WritePin(W25Q_CS_PORT, W25Q_CS_PIN, GPIO_PIN_RESET);
#define W25Q_CS_SET()           HAL_GPIO_WritePin(W25Q_CS_PORT, W25Q_CS_PIN, GPIO_PIN_SET);


/* 初始化函数 */
uint8_t     W25Q_Open(SPI_HandleTypeDef* spi, char* cs);

/* 读写操作 */
void        W25Q_PageWrite(uint32_t add, uint8_t* pbuf, uint16_t len); //写一个Page
void        W25Q_Read(uint32_t add, uint8_t* pbuf, uint16_t len);      //连续读
void        W25Q_Write(uint32_t add, uint8_t* pbuf, uint16_t len);     //连续写

/* 擦除数据 */
void        W25Q_EraseSector(uint32_t add); //擦一个扇区
void        W25Q_EraseChip();               //擦除整个芯片

/* 电源管理 */
void        W25Q_SleepIn();                 //沉睡模式
void        W25Q_SleepOut();                //唤醒W25Q

/* 内部调用 */
uint8_t     W25Q_RWByte(uint8_t byte);      //读写一个字节
void        W25Q_WaitBusy();                //忙等待
void        W25Q_WriteEnable();             //写使能
uint32_t    W25Q_ReadJudecID();             //读取JudecID
uint8_t     W25Q_ReadDevID();               //读取DevID