#include "xmodem.h"
#include "string.h"
#include "stm32f4xx_hal.h"


#define XMODEM_IDLE             0
#define XMODEM_BUSY             1


static uint8_t  XMODEM_STATE = XMODEM_IDLE;
static uint8_t* XMODEM_FILE;
static uint32_t XMODEM_FILELEN;


static uint8_t XMODEM_BUFFER[MAX_BUF_SIZE];


/** ��ʼ������
  * ���ܣ�ʹ��UART�����жϣ���DMA����
  * 
  *
  */
void USER_XMODEM_Init() {
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, XMODEM_BUFFER, MAX_BUF_SIZE);
    
    XMODEM_STATE = XMODEM_IDLE;
}


/** �����жϺ���
  * ���ܣ���UART���е�ʱ�����и��ж�
  * ע�⣺�˺�����Ҫ�ŵ� stm32fxxx_it.c �ļ��� USARTx_IRQHandler ������
  *
  */
void USER_XMODEM_IdleHandler() {
    uint16_t len = 0;
    
    //�ж��Ƿ������һ��XMODEM��
    if (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_IDLE) ) { 
        
        //���UART�����жϱ�־  
        __HAL_UART_CLEAR_IDLEFLAG(&huart1); 
        
        //ֹͣDMA����
        HAL_UART_DMAStop(&huart1); 
        
        //�������ݽ���
        len = MAX_BUF_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
        USER_XMODEM_Parse(XMODEM_BUFFER, len); 
        
        //����ʹ��DMA����
        HAL_UART_Receive_DMA(&huart1, XMODEM_BUFFER, MAX_BUF_SIZE);
    }
}


/** ��ʼ���亯��
  * ���ܣ������Ͷ��з����ļ������ʱ�����иú�����ʼ����
  * 
  * 
  */
void USER_XMODEM_Start_ByteArray(uint8_t* file, uint32_t len) {
  
    //���ô�ʱXMODEM״̬Ϊæ
    XMODEM_STATE = XMODEM_BUSY;
    
    //���ý��ܵ��ַ���
    XMODEM_FILE     = file;
    XMODEM_FILELEN  = len;
    
    //����NAKָ���ʼ����
    USER_XMODEM_SendCmd(XMODEM_NAK);
}


/** ����ָ���
  * ���ܣ�����ָ��
  * ���� cmd����Ҫ���͵�ָ��
  * 
  */
void USER_XMODEM_SendCmd(uint8_t cmd) {
  
    //����һ���ֽڵ�ָ��
    HAL_UART_Transmit(&huart1, &cmd, 1, 0xff);
}


/** æ��麯��
  * ���ܣ���鴮���Ƿ����ڹ���
  * 
  * 
  */
uint8_t USER_XMODEM_CheckBusy() {
    return XMODEM_STATE;
}


/** �������ݺ���
  * ���ܣ���һ����������ɺ󣬽������ݽ���
  * ���� pbuf�������׵�ַ
  * ���� len�����峤��
  */
void USER_XMODEM_Parse(uint8_t* pbuf, uint16_t len) {
    XMODEM_TypeDef* px = (XMODEM_TypeDef*) pbuf;
    
    //������յ�����һ�����ݰ�
    if (len == sizeof(XMODEM_TypeDef) ) {
      
        //����У��λ
        for (uint8_t i = 0; i < 128; i ++) px -> SUM -= (px -> DATA)[i];
        
        //���У��λ��ȷ����ô�������ݵ��ļ��ڣ�������ACK
        if (px -> SUM == 0) {
            if (XMODEM_FILELEN >= (px -> NUM) * 128) {
                memcpy(&XMODEM_FILE[(px->NUM-1)*128], px->DATA, 128); 
            }
            USER_XMODEM_SendCmd(XMODEM_ACK);
        }
        
        //���򷵻�NAK�������ط������ݰ�
        else USER_XMODEM_SendCmd(XMODEM_NAK);
    }
    
    //������յ����ǽ���ָ���ô����ACK
    else if (pbuf[0] == XMODEM_EOT) {
        XMODEM_STATE = XMODEM_IDLE;
        USER_XMODEM_SendCmd(XMODEM_ACK);
    }
}

