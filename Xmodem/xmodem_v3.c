#include "stm32f4xx_hal.h"
#include "string.h"
#include "xmodem_v3.h"


UART_HandleTypeDef* XMODEM_UART;
DMA_HandleTypeDef*  XMODEM_DMA;


static uint8_t XMODEM_CMD;                          //命令字节缓冲
static uint8_t XMODEM_BUF[XMODEM_SIZE];             //DMA数据包缓冲（不包含帧头）

static uint32_t XMODEM_BAGCNT = 0;                  //记录数据包个数

static uint8_t XMODEM_STATE = XMODEM_STATE_IDLE;    //记录XMODEM状态


/* XMOEDM初始化函数 */
void XMODEM_Open(UART_HandleTypeDef* huart, DMA_HandleTypeDef* hdma) {

    XMODEM_UART = huart;
    XMODEM_DMA  = hdma;

    /* 标记XMODEM状态为空闲 */
    XMODEM_STATE = XMODEM_STATE_IDLE;
}

/* 开始传输 */
void XMODEM_Start() {

    /* 初始化 */
    XMODEM_CMD = 0XFF;
    memset(XMODEM_BUF, 0, sizeof(XMODEM_BUF) );
    XMODEM_BAGCNT = 0;
    XMODEM_STATE = XMODEM_STATE_BUSY;

    /* 使能接收命令 */
    HAL_UART_Receive_IT(XMODEM_UART, &XMODEM_CMD, 1);

    /* 发送传输请求 */
    XMODEM_SendCtrl(XMODEM_NAK);
}

/* 返回本次传输接收数据包的个数 */
uint32_t XMODEM_CheckBagCnt() {
    return XMODEM_BAGCNT;
}

/* 检查XMODEM是否处于忙碌状态 */
uint8_t XMODEM_CheckBusy() { return XMODEM_STATE; }

/* 发送指令 */
void XMODEM_SendCtrl(uint8_t cmd) {
    HAL_UART_Transmit(XMODEM_UART, &cmd, 1, 0xffff);
}

/* 解析数据 */
void XMODEM_Parse() {
    XMODEM_TypeDef* xp = (XMODEM_TypeDef*)XMODEM_BUF;
    
    ...用户修改（1）//说明：向目标储存介质中写入XMODEM串口读出来的数据包
    //FLASH_Write(xp->DATA, XMODEM_BAGCNT * 128, 128);
}

...用户修改（2）//需要将该函数放在 HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)中断回调函数中
void XMODEM_UARTHandler(UART_HandleTypeDef* huart) {
    if (huart == XMODEM_UART) {

        /* 如果是帧头 */
        if (XMODEM_CMD == XMODEM_SOH) {
            XMODEM_CMD = 0xff;
            HAL_UART_Receive_DMA(XMODEM_UART, XMODEM_BUF, XMODEM_SIZE);
        }
        
        /* 所有数据传输完成 */
        else if (XMODEM_CMD == XMODEM_EOT) {
            XMODEM_CMD = 0xff;
            XMODEM_STATE = XMODEM_STATE_IDLE;
            HAL_UART_Receive_IT(XMODEM_UART, &XMODEM_CMD, 1);
            XMODEM_SendCtrl(XMODEM_ACK);
        }
        
        /* DMA传输完成 */
        else if (__HAL_DMA_GET_COUNTER(XMODEM_DMA) == 0) {
            
            /* 计算校验位 */
            XMODEM_TypeDef* xp = (XMODEM_TypeDef*)XMODEM_BUF;
            for (uint8_t i = 0; i < 128; i ++) xp->SUM -= xp->DATA[i];
            
            /* 校验位正确，处理数据 */
            if (xp->SUM == 0) {
                
                /* 数据处理 */
                XMODEM_Parse();

                XMODEM_BAGCNT += 1;
                XMODEM_SendCtrl(XMODEM_ACK);
            }
            
            /* 否则请求重发 */
            else XMODEM_SendCtrl(XMODEM_NAK);
            
            /* 使能接收命令 */
            HAL_UART_Receive_IT(XMODEM_UART, &XMODEM_CMD, 1);
        }
    }
}