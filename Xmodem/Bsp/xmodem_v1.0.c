#include "xmodem.h"
#include "string.h"
#include "stm32f4xx_hal.h"


#define XMODEM_IDLE             0
#define XMODEM_BUSY             1


static uint8_t  XMODEM_STATE = XMODEM_IDLE;
static uint8_t* XMODEM_FILE;
static uint32_t XMODEM_FILELEN;


static uint8_t XMODEM_BUFFER[MAX_BUF_SIZE];


/** 初始化函数
  * 功能：使能UART空闲中断，打开DMA传输
  * 
  *
  */
void USER_XMODEM_Init() {
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, XMODEM_BUFFER, MAX_BUF_SIZE);
    
    XMODEM_STATE = XMODEM_IDLE;
}


/** 空闲中断函数
  * 功能：当UART空闲的时候，运行该中断
  * 注意：此函数需要放到 stm32fxxx_it.c 文件的 USARTx_IRQHandler 函数内
  *
  */
void USER_XMODEM_IdleHandler() {
    uint16_t len = 0;
    
    //判断是否传输完成一个XMODEM包
    if (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_IDLE) ) { 
        
        //清除UART空闲中断标志  
        __HAL_UART_CLEAR_IDLEFLAG(&huart1); 
        
        //停止DMA传输
        HAL_UART_DMAStop(&huart1); 
        
        //进行数据解析
        len = MAX_BUF_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
        USER_XMODEM_Parse(XMODEM_BUFFER, len); 
        
        //重新使能DMA传输
        HAL_UART_Receive_DMA(&huart1, XMODEM_BUFFER, MAX_BUF_SIZE);
    }
}


/** 开始传输函数
  * 功能：当发送端有发送文件请求的时候，运行该函数后开始传输
  * 
  * 
  */
void USER_XMODEM_Start_ByteArray(uint8_t* file, uint32_t len) {
  
    //设置此时XMODEM状态为忙
    XMODEM_STATE = XMODEM_BUSY;
    
    //设置接受的字符串
    XMODEM_FILE     = file;
    XMODEM_FILELEN  = len;
    
    //发送NAK指令，开始传输
    USER_XMODEM_SendCmd(XMODEM_NAK);
}


/** 发送指令函数
  * 功能：发送指令
  * 参数 cmd：需要发送的指令
  * 
  */
void USER_XMODEM_SendCmd(uint8_t cmd) {
  
    //发送一个字节的指令
    HAL_UART_Transmit(&huart1, &cmd, 1, 0xff);
}


/** 忙检查函数
  * 功能：检查串口是否正在工作
  * 
  * 
  */
uint8_t USER_XMODEM_CheckBusy() {
    return XMODEM_STATE;
}


/** 解析数据函数
  * 功能：在一个包传输完成后，进行数据解析
  * 参数 pbuf：缓冲首地址
  * 参数 len：缓冲长度
  */
void USER_XMODEM_Parse(uint8_t* pbuf, uint16_t len) {
    XMODEM_TypeDef* px = (XMODEM_TypeDef*) pbuf;
    
    //如果接收到的是一个数据包
    if (len == sizeof(XMODEM_TypeDef) ) {
      
        //计算校验位
        for (uint8_t i = 0; i < 128; i ++) px -> SUM -= (px -> DATA)[i];
        
        //如果校验位正确，那么拷贝数据到文件内，并返回ACK
        if (px -> SUM == 0) {
            if (XMODEM_FILELEN >= (px -> NUM) * 128) {
                memcpy(&XMODEM_FILE[(px->NUM-1)*128], px->DATA, 128); 
            }
            USER_XMODEM_SendCmd(XMODEM_ACK);
        }
        
        //否则返回NAK，请求重发该数据包
        else USER_XMODEM_SendCmd(XMODEM_NAK);
    }
    
    //如果接收到的是结束指令，那么返回ACK
    else if (pbuf[0] == XMODEM_EOT) {
        XMODEM_STATE = XMODEM_IDLE;
        USER_XMODEM_SendCmd(XMODEM_ACK);
    }
}

