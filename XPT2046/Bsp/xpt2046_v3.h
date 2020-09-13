#include "stm32f4xx_hal.h"


/************* GPIO *************/

/** cs：XPT2046片选
  *
  *（1）模式            推挽输出
  *（2）上下拉          不上下拉
  *（3）速度            非常高
  *
  */

/** sck：XPT2046的SPI时钟脚
  *
  *（1）模式            推挽输出
  *（2）上下拉          不上下拉
  *（3）速度            非常高
  *
  */

/** mosi：XPT2046的SPI主机输出脚
  *
  *（1）模式            推挽输出
  *（2）上下拉          不上下拉
  *（3）速度            非常高
  *
  */

/** miso：XPT2046的SPI主机输入脚
  *
  *（1）模式            输入模式
  *（2）上下拉          上拉
  *（3）速度            非常高
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


//用户调用
void        USER_XPT2046_Open(char* cs, char* sck, char* mosi, char* miso);   //初始化
uint16_t    USER_XPT2046_ReadADC(uint8_t ch);                                 //读一个通道ADC

//内部调用
void        USER_XPT2046_Delay(uint32_t us);                                  //软件微妙延时
void        USER_XPT2046_WriteByte(uint8_t byte);                             //写一个字节
uint16_t    USER_XPT2046_ReadByte();                                          //SPI读一个字节