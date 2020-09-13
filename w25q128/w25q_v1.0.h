#include "stm32f4xx_hal.h"


/* ���Ŷ���
 * FLASH_SCK	PI1
 * FLASH_MOSI	PI3
 * FLASH_MISO	PC2
 * FLASH_CS		PI0
 *
 *
 * SPIģʽ
 * �ߵ�ƽ��Ч
 * �����ض�ȡ����
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


//��ʼ��
uint8_t     USER_FLASH_Init();

//��д����
void        USER_FLASH_PageWrite(uint32_t add, uint8_t* pbuf, uint16_t len); //дһ��Page
void        USER_FLASH_Read(uint32_t add, uint8_t* pbuf, uint16_t len);      //������
void        USER_FLASH_Write(uint32_t add, uint8_t* pbuf, uint16_t len);     //����д

//��������
void        USER_FLASH_EraseSector(uint32_t add); //��һ������
void        USER_FLASH_EraseChip();               //��������оƬ

//��Դ����
void        USER_FLASH_SleepIn();                 //��˯ģʽ
void        USER_FLASH_SleepOut();                //����FLASH

//�ڲ�����
uint8_t     USER_FLASH_RWByte(uint8_t byte);      //��дһ���ֽ�
void        USER_FLASH_WaitBusy();                //æ�ȴ�
void        USER_FLASH_WriteEnable();             //дʹ��
uint32_t    USER_FLASH_ReadJudecID();             //��ȡJudecID
uint8_t     USER_FLASH_ReadDevID();               //��ȡDevID