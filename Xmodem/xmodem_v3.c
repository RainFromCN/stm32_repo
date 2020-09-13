#include "stm32f4xx_hal.h"
#include "string.h"
#include "xmodem_v3.h"


UART_HandleTypeDef* XMODEM_UART;
DMA_HandleTypeDef*  XMODEM_DMA;


static uint8_t XMODEM_CMD;                          //�����ֽڻ���
static uint8_t XMODEM_BUF[XMODEM_SIZE];             //DMA���ݰ����壨������֡ͷ��

static uint32_t XMODEM_BAGCNT = 0;                  //��¼���ݰ�����

static uint8_t XMODEM_STATE = XMODEM_STATE_IDLE;    //��¼XMODEM״̬


/* XMOEDM��ʼ������ */
void XMODEM_Open(UART_HandleTypeDef* huart, DMA_HandleTypeDef* hdma) {

    XMODEM_UART = huart;
    XMODEM_DMA  = hdma;

    /* ���XMODEM״̬Ϊ���� */
    XMODEM_STATE = XMODEM_STATE_IDLE;
}

/* ��ʼ���� */
void XMODEM_Start() {

    /* ��ʼ�� */
    XMODEM_CMD = 0XFF;
    memset(XMODEM_BUF, 0, sizeof(XMODEM_BUF) );
    XMODEM_BAGCNT = 0;
    XMODEM_STATE = XMODEM_STATE_BUSY;

    /* ʹ�ܽ������� */
    HAL_UART_Receive_IT(XMODEM_UART, &XMODEM_CMD, 1);

    /* ���ʹ������� */
    XMODEM_SendCtrl(XMODEM_NAK);
}

/* ���ر��δ���������ݰ��ĸ��� */
uint32_t XMODEM_CheckBagCnt() {
    return XMODEM_BAGCNT;
}

/* ���XMODEM�Ƿ���æµ״̬ */
uint8_t XMODEM_CheckBusy() { return XMODEM_STATE; }

/* ����ָ�� */
void XMODEM_SendCtrl(uint8_t cmd) {
    HAL_UART_Transmit(XMODEM_UART, &cmd, 1, 0xffff);
}

/* �������� */
void XMODEM_Parse() {
    XMODEM_TypeDef* xp = (XMODEM_TypeDef*)XMODEM_BUF;
    
    ...�û��޸ģ�1��//˵������Ŀ�괢�������д��XMODEM���ڶ����������ݰ�
    //FLASH_Write(xp->DATA, XMODEM_BAGCNT * 128, 128);
}

...�û��޸ģ�2��//��Ҫ���ú������� HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)�жϻص�������
void XMODEM_UARTHandler(UART_HandleTypeDef* huart) {
    if (huart == XMODEM_UART) {

        /* �����֡ͷ */
        if (XMODEM_CMD == XMODEM_SOH) {
            XMODEM_CMD = 0xff;
            HAL_UART_Receive_DMA(XMODEM_UART, XMODEM_BUF, XMODEM_SIZE);
        }
        
        /* �������ݴ������ */
        else if (XMODEM_CMD == XMODEM_EOT) {
            XMODEM_CMD = 0xff;
            XMODEM_STATE = XMODEM_STATE_IDLE;
            HAL_UART_Receive_IT(XMODEM_UART, &XMODEM_CMD, 1);
            XMODEM_SendCtrl(XMODEM_ACK);
        }
        
        /* DMA������� */
        else if (__HAL_DMA_GET_COUNTER(XMODEM_DMA) == 0) {
            
            /* ����У��λ */
            XMODEM_TypeDef* xp = (XMODEM_TypeDef*)XMODEM_BUF;
            for (uint8_t i = 0; i < 128; i ++) xp->SUM -= xp->DATA[i];
            
            /* У��λ��ȷ���������� */
            if (xp->SUM == 0) {
                
                /* ���ݴ��� */
                XMODEM_Parse();

                XMODEM_BAGCNT += 1;
                XMODEM_SendCtrl(XMODEM_ACK);
            }
            
            /* ���������ط� */
            else XMODEM_SendCtrl(XMODEM_NAK);
            
            /* ʹ�ܽ������� */
            HAL_UART_Receive_IT(XMODEM_UART, &XMODEM_CMD, 1);
        }
    }
}