#include "stm32f4xx_hal.h"


/************* UART *************/

/** huart：xmodem通信使用的huart
  *
  *（1）是否使用中断     使用中断
  *（2）是否使用DMA      使用DMA
  *
  */

/*************  DMA *************/

/** hdma：作为xmodem协议接收缓冲
  *
  *（1）目标            串口接收Rx
  *（2）模式            非循环模式
  *（3）极性            高电平
  *（4）方式            字节对字节 
  *
  */


#pragma pack(1)
typedef struct {
    uint8_t NUM;
    uint8_t XNUM;
    uint8_t DATA[128];
    uint8_t SUM;
} XMODEM_TypeDef;
#pragma pack()


#define XMODEM_SIZE             131

#define XMODEM_SOH              0x01
#define XMODEM_EOT              0X04
#define XMODEM_ACK              0X06
#define XMODEM_NAK              0X15
#define XMODEM_CAN              0X18

#define XMODEM_STATE_IDLE       0
#define XMODEM_STATE_BUSY       1


/* 用户调用 */
void        XMODEM_Open(UART_HandleTypeDef* huart, DMA_HandleTypeDef* hdma);         //初始化
void        XMODEM_Start();        //开始进行XMODEM传输
uint8_t     XMODEM_CheckBusy();    //检查XMODEM是否忙
uint32_t    XMODEM_CheckBagCnt();  //检查接收到的包的个数

/* 内部调用 */
void        XMODEM_SendCtrl(uint8_t cmd);  //发送命令
void        XMODEM_Parse();                //解析数据包
void        XMODEM_UARTHandler(UART_HandleTypeDef* huart); //串口接收中断服务函数


