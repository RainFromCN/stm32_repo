#include "stm32f4xx_hal.h"
#include <stdint.h>


/** ʹ�ø�XMODEMģ��֮ǰ
  * ��Ҫ��USER_XMODEM_IdleHandler
  * ��������ŵ� stm32fxxx_hal.c
  * �ļ��е�USARTx_IRQHandler������
  *
  * ע�⣬��ģ��ʹ��DMA���д���
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


//�û�����
void        USER_XMODEM_Init(); //��ʼ��XMODEM����
void        USER_XMODEM_Start_ByteArray(uint8_t* file, uint32_t len); //��ȡXMODEM�����ֽ�������
//void      USER_XMODEM_Start_FLASH() //��XMODEM����SPI FLASH��
uint8_t     USER_XMODEM_CheckBusy(); //���XMODEM�����Ƿ����ڶ�д

//�ڲ�����
void        USER_XMODEM_IdleHandler(); //�����жϷ�����
void        USER_XMODEM_SendCmd(uint8_t cmd); //����ָ��
void        USER_XMODEM_Parse(uint8_t* pbuf, uint16_t len); //��������