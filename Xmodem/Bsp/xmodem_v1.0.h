#include "stm32f4xx_hal.h"
#include <stdint.h>


/** 使用该XMODEM模块之前
  * 需要将USER_XMODEM_IdleHandler
  * 这个函数放到 stm32fxxx_hal.c
  * 文件中的USARTx_IRQHandler函数中
  *
  * 注意，本模块使用DMA进行传输
  *
  *
  */


extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;


#pragma pack(1)

typedef struct {
    uint8_t SOH;
    uint8_t NUM;
    uint8_t XNUM;
    uint8_t DATA[128];
    uint8_t SUM;
} XMODEM_TypeDef;

#pragma pack()


#define MAX_BUF_SIZE            256


#define XMODEM_SOH              0x01
#define XMODEM_EOT              0X04
#define XMODEM_ACK              0X06
#define XMODEM_NAK              0X15
#define XMODEM_CAN              0X18


//用户调用
void        USER_XMODEM_Init(); //初始化XMODEM串口
void        USER_XMODEM_Start_ByteArray(uint8_t* file, uint32_t len); //读取XMODEM流到字节数组中
//void      USER_XMODEM_Start_FLASH() //读XMODEM流到SPI FLASH中
uint8_t     USER_XMODEM_CheckBusy(); //检查XMODEM串口是否正在读写

//内部调用
void        USER_XMODEM_IdleHandler(); //空闲中断服务函数
void        USER_XMODEM_SendCmd(uint8_t cmd); //发送指令
void        USER_XMODEM_Parse(uint8_t* pbuf, uint16_t len); //处理数据