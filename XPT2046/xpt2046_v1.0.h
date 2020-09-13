#include "stm32f4xx_hal.h"


/** ���Ŷ��壺
  * TOUCH_INT        PH4
  * TOUCH_SCK        PH2
  * TOUCH_MOSI       PH3
  * TOUCH_MISO       PH5
  * TOUCH_CS         PI8
  *
  * ע�⣺����XPT2046��SPIΪģ��SPI
  *
  * ˵����
  * (1)TOUCH_INT��INPUTģʽ�����ⲿ�ж�ģʽ
  * (2)TOUCH_INT��TOUCH_MISO��Ϊ����ģʽ������Ų�������
  *
  */


#define TOUCH_INT_ACTIVE                0


#define TOUCH_CS_RESET()                HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET)
#define TOUCH_CS_SET()                  HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET)

#define TOUCH_SCK_RESET()               HAL_GPIO_WritePin(TOUCH_SCK_GPIO_Port, TOUCH_SCK_Pin, GPIO_PIN_RESET)
#define TOUCH_SCK_SET()                 HAL_GPIO_WritePin(TOUCH_SCK_GPIO_Port, TOUCH_SCK_Pin, GPIO_PIN_SET)

#define TOUCH_MOSI_RESET()              HAL_GPIO_WritePin(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, GPIO_PIN_RESET)
#define TOUCH_MOSI_SET()                HAL_GPIO_WritePin(TOUCH_MOSI_GPIO_Port, TOUCH_MOSI_Pin, GPIO_PIN_SET)

#define TOUCH_MISO_READ()               HAL_GPIO_ReadPin(TOUCH_MISO_GPIO_Port, TOUCH_MISO_Pin)
#define TOUCH_INT_READ()                HAL_GPIO_ReadPin(TOUCH_INT_GPIO_Port, TOUCH_INT_Pin)

#define TOUCH_CHANNEL_X                 0x90
#define TOUCH_CHANNEL_Y                 0xd0


//�û�����
void        USER_TOUCH_Init();                                          //��ʼ��
uint16_t    USER_TOUCH_ReadADC(uint8_t ch);                             //��һ��ͨ��ADC

//�ڲ�����
void        USER_TOUCH_Delay(uint32_t us);                              //���΢����ʱ
void        USER_TOUCH_WriteByte(uint8_t byte);                         //дһ���ֽ�
uint16_t    USER_TOUCH_ReadByte();                                      //SPI��һ���ֽ�