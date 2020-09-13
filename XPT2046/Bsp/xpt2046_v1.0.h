#include "stm32f4xx_hal.h"


/** 引脚定义：
  * TOUCH_INT        PH4
  * TOUCH_SCK        PH2
  * TOUCH_MOSI       PH3
  * TOUCH_MISO       PH5
  * TOUCH_CS         PI8
  *
  * 注意：驱动XPT2046的SPI为模拟SPI
  *
  * 说明：
  * (1)TOUCH_INT是INPUT模式而非外部中断模式
  * (2)TOUCH_INT和TOUCH_MISO均为上拉模式，其余脚不上下拉
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


//用户调用
void        USER_TOUCH_Init();                                          //初始化
uint16_t    USER_TOUCH_ReadADC(uint8_t ch);                             //读一个通道ADC

//内部调用
void        USER_TOUCH_Delay(uint32_t us);                              //软件微妙延时
void        USER_TOUCH_WriteByte(uint8_t byte);                         //写一个字节
uint16_t    USER_TOUCH_ReadByte();                                      //SPI读一个字节