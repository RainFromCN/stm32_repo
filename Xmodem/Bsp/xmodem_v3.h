#include "stm32f4xx_hal.h"


/************* UART *************/

/** huart��xmodemͨ��ʹ�õ�huart
  *
  *��1���Ƿ�ʹ���ж�     ʹ���ж�
  *��2���Ƿ�ʹ��DMA      ʹ��DMA
  *
  */

/*************  DMA *************/

/** hdma����ΪxmodemЭ����ջ���
  *
  *��1��Ŀ��            ���ڽ���Rx
  *��2��ģʽ            ��ѭ��ģʽ
  *��3������            �ߵ�ƽ
  *��4����ʽ            �ֽڶ��ֽ� 
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


/* �û����� */
void        XMODEM_Open(UART_HandleTypeDef* huart, DMA_HandleTypeDef* hdma);         //��ʼ��
void        XMODEM_Start();        //��ʼ����XMODEM����
uint8_t     XMODEM_CheckBusy();    //���XMODEM�Ƿ�æ
uint32_t    XMODEM_CheckBagCnt();  //�����յ��İ��ĸ���

/* �ڲ����� */
void        XMODEM_SendCtrl(uint8_t cmd);  //��������
void        XMODEM_Parse();                //�������ݰ�
void        XMODEM_UARTHandler(UART_HandleTypeDef* huart); //���ڽ����жϷ�����


