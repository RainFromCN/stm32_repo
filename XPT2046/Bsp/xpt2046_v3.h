#include "stm32f4xx_hal.h"


/************* GPIO *************/

/** cs��XPT2046Ƭѡ
  *
  *��1��ģʽ            �������
  *��2��������          ��������
  *��3���ٶ�            �ǳ���
  *
  */

/** sck��XPT2046��SPIʱ�ӽ�
  *
  *��1��ģʽ            �������
  *��2��������          ��������
  *��3���ٶ�            �ǳ���
  *
  */

/** mosi��XPT2046��SPI���������
  *
  *��1��ģʽ            �������
  *��2��������          ��������
  *��3���ٶ�            �ǳ���
  *
  */

/** miso��XPT2046��SPI���������
  *
  *��1��ģʽ            ����ģʽ
  *��2��������          ����
  *��3���ٶ�            �ǳ���
  *
  */


#define XPT2046_CS_RESET()                HAL_GPIO_WritePin(XPT2046_CS_PORT, XPT2046_CS_PIN, GPIO_PIN_RESET)
#define XPT2046_CS_SET()                  HAL_GPIO_WritePin(XPT2046_CS_PORT, XPT2046_CS_PIN, GPIO_PIN_SET)

#define XPT2046_SCK_RESET()               HAL_GPIO_WritePin(XPT2046_SCK_PORT, XPT2046_SCK_PIN, GPIO_PIN_RESET)
#define XPT2046_SCK_SET()                 HAL_GPIO_WritePin(XPT2046_SCK_PORT, XPT2046_SCK_PIN, GPIO_PIN_SET)

#define XPT2046_MOSI_RESET()              HAL_GPIO_WritePin(XPT2046_MOSI_PORT, XPT2046_MOSI_PIN, GPIO_PIN_RESET)
#define XPT2046_MOSI_SET()                HAL_GPIO_WritePin(XPT2046_MOSI_PORT, XPT2046_MOSI_PIN, GPIO_PIN_SET)

#define XPT2046_MISO_READ()               HAL_GPIO_ReadPin(XPT2046_MISO_PORT, XPT2046_MISO_PIN)

#define XPT2046_CHANNEL_X                 0x90
#define XPT2046_CHANNEL_Y                 0xd0


//�û�����
void        USER_XPT2046_Open(char* cs, char* sck, char* mosi, char* miso);   //��ʼ��
uint16_t    USER_XPT2046_ReadADC(uint8_t ch);                                 //��һ��ͨ��ADC

//�ڲ�����
void        USER_XPT2046_Delay(uint32_t us);                                  //���΢����ʱ
void        USER_XPT2046_WriteByte(uint8_t byte);                             //дһ���ֽ�
uint16_t    USER_XPT2046_ReadByte();                                          //SPI��һ���ֽ�