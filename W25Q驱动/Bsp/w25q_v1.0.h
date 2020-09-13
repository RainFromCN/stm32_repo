#include "stm32f4xx_hal.h"


/* 引脚定义
 * FLASH_SCK	PI1
 * FLASH_MOSI	PI3
 * FLASH_MISO	PC2
 * FLASH_CS		PI0
 *
 *
 * SPI模式
 * 高电平有效
 * 上升沿读取数据
 * 
 */


extern SPI_HandleTypeDef hspi2;


#define FLASH_ID                 0xEF4018 
#define FLASH_PAGESIZE           256
#define FLASH_SECTORSIZE         096
#define FLASH_BLOCKSIZE          65535
#define FLASH_CHIPSIZE           16776960


#define WRITE_ENABLE             0x06
#define WRITE_DISABLE            0x04
#define READ_STATUS              0x05
#define WRITE_STATUS             0x01
#define READ_DATA                0x03
#define FAST_READ                0x0B
#define PAGE_PROGRAM             0x02
#define BLOCK_ERASE              0XD8
#define SECTOR_ERASE             0x20
#define CHIP_ERASE               0xC7
#define PWR_DOWN                 0xB9
#define RELEASE_PWR_DOWN         0xAB
#define JUDEC_ID                 0X9F
#define DEV_ID                   0xAB


#define FLASH_CS_RESET()         HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
#define FLASH_CS_SET()           HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)


//初始化
uint8_t     USER_FLASH_Init();

//读写操作
void        USER_FLASH_PageWrite(uint32_t add, uint8_t* pbuf, uint16_t len); //写一个Page
void        USER_FLASH_Read(uint32_t add, uint8_t* pbuf, uint16_t len);      //连续读
void        USER_FLASH_Write(uint32_t add, uint8_t* pbuf, uint16_t len);     //连续写

//擦除数据
void        USER_FLASH_EraseSector(uint32_t add); //擦一个扇区
void        USER_FLASH_EraseChip();               //擦除整个芯片

//电源管理
void        USER_FLASH_SleepIn();                 //沉睡模式
void        USER_FLASH_SleepOut();                //唤醒FLASH

//内部调用
uint8_t     USER_FLASH_RWByte(uint8_t byte);      //读写一个字节
void        USER_FLASH_WaitBusy();                //忙等待
void        USER_FLASH_WriteEnable();             //写使能
uint32_t    USER_FLASH_ReadJudecID();             //读取JudecID
uint8_t     USER_FLASH_ReadDevID();               //读取DevID