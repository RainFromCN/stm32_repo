#include "stm32f4xx_hal.h"


/************* GPIO *************/

/** cs��W25QƬѡ�ź�
  * 
  *��1��ģʽ            �������
  *��2��������          ��������
  *��3���ٶ�            �ǳ���
  *
  */

/*************  SPI *************/

/** spi����W25Q����ͨ��
  *
  *��1������            �� 
  *��2������            �ڶ�����
  *��3���ٶ�            20Mb/s����
  *
  */


/* �豸���� */
#define W25Q_ID	                0xEF4018 
#define W25Q_PAGESIZE           256
#define W25Q_SECTORSIZE         4096


/* ����ָ�� */
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


/* ��ʼ������ */
uint8_t     W25Q_Open(SPI_HandleTypeDef* spi, char* cs);

/* ��д���� */
void        W25Q_PageWrite(uint32_t add, uint8_t* pbuf, uint16_t len); //дһ��Page
void        W25Q_Read(uint32_t add, uint8_t* pbuf, uint16_t len);      //������
void        W25Q_Write(uint32_t add, uint8_t* pbuf, uint16_t len);     //����д

/* �������� */
void        W25Q_EraseSector(uint32_t add); //��һ������
void        W25Q_EraseChip();               //��������оƬ

/* ��Դ���� */
void        W25Q_SleepIn();                 //��˯ģʽ
void        W25Q_SleepOut();                //����W25Q

/* �ڲ����� */
uint8_t     W25Q_RWByte(uint8_t byte);      //��дһ���ֽ�
void        W25Q_WaitBusy();                //æ�ȴ�
void        W25Q_WriteEnable();             //дʹ��
uint32_t    W25Q_ReadJudecID();             //��ȡJudecID
uint8_t     W25Q_ReadDevID();               //��ȡDevID