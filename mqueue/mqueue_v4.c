#include "mqueue_v4.h"
#include "string.h"


/* ������Ϣ���� */
MessageQueue_TypeDef MQUEUE_Open(UART_HandleTypeDef* mq_uart) {
    
    MessageQueue_TypeDef tmp;
    
    memset(&tmp, 0, sizeof(tmp));

    tmp.mq_uart = mq_uart;

    return tmp;
}

/* ��ʼ������Ϣ */
void MQUEUE_Enable(MessageQueue_TypeDef* M) {
    HAL_UART_Receive_IT(M->mq_uart, (uint8_t*)&M->buff, 1);
}

...�û��޸ģ�1��//����UART��������ж���
/* ������Ϣ���� */
void MQUEUE_Update(UART_HandleTypeDef* huart, MessageQueue_TypeDef* M) {
    
    if (M->mq_uart != huart) {
        HAL_UART_Receive_IT(M->mq_uart, (uint8_t*)&M->buff, 1);
        return;
    }
    
    /* �������� */
    if (M->mark == 1) {
        M->value_buf[M->value_len++] = M->buff;
        M->cx += M->buff;
        if (M->value_len >= 4) M->mark = 2;
    }
    
    /* ���յ�֡ͷ */
    else if (M->buff == '@') {
        M->topic_len = 0;
        M->value_len = 0;
        M->mark = 0;
        M->cx = 0;
    }

    /* ���յ������ */
    else if (M->buff == '#') {
        M->mark = 1;
    }

    /* ���յ������� */
    else if (M->buff == '$') {

        /* У��λ��ȷ */
        if (M->cx == M->cx_rec && M->topic_len < 20) {

            M->topic_buf[M->topic_len++] = '\0';

            /* ��ֵ */
            for (uint8_t i = 0; i < M->queue_size; i++) {

                if (strcmp((char*)(M->message[i]).topic, (char*)M->topic_buf) == 0) {
                    if ((M->message[i]).data_type == MQUEUE_FLOAT) {
                        *(float*)(M->message[i]).to = *(float*)M->value_buf;
                    }
                    else if ((M->message[i]).data_type == MQUEUE_INT) {
                        *(int*)(M->message[i]).to = *(int*)M->value_buf;
                    }
                }
            }
        }

        /* ���� */
        M->topic_len = 0;
        M->value_len = 0;
        M->mark = 0;
        M->cx = 0;
    }

    /* ���յ�topic */
    else if (M->mark == 0) {

        /* ���������� */
        if (M->topic_len >= 20) {
            M->topic_len = 0;
            M->value_len = 0;
            M->mark = 0;
            M->cx = 0;
        }

        M->cx += M->buff;
        M->topic_buf[M->topic_len++] = M->buff;
    }

    /* ���յ�cx */
    else if (M->mark == 2) {
        M->cx_rec = M->buff;
    }

    HAL_UART_Receive_IT(M->mq_uart, (uint8_t*)&M->buff, 1);
}

/* ����һ������ */
void MQUEUE_Subscribe(MessageQueue_TypeDef* M, char* topic, uint32_t* to, uint8_t data_type) {
    Message_TypeDef tmp;

    for (uint8_t i = 0; i <= strlen(topic); i++) {
        tmp.topic[i] = topic[i];
    }

    tmp.to = to;
    tmp.data_type = data_type;

    M->message[M->queue_size++] = tmp;
}

/* ����һ������ */
void MQUEUE_Publish(UART_HandleTypeDef* huart, char* topic, uint32_t* from) {
    
    uint8_t head = '@';
    uint8_t div  = '#';
    uint8_t end  = '$';

    uint8_t cx = 0;
    uint8_t* p = (uint8_t*)from;

    HAL_UART_Transmit(huart, (uint8_t*)&head, 1, 0xffff);
    HAL_UART_Transmit(huart, (uint8_t*)topic, strlen(topic), 0xffff);
    HAL_UART_Transmit(huart, (uint8_t*)&div, 1, 0xffff);
    HAL_UART_Transmit(huart, (uint8_t*)from, 4, 0xffff);

    for (uint8_t i = 0; i < strlen(topic); i++) cx += topic[i];
    for (uint8_t i = 0; i < 4; i++) cx += p[i];

    HAL_UART_Transmit(huart, (uint8_t*)&cx, 1, 0xffff);
    HAL_UART_Transmit(huart, (uint8_t*)&end, 1, 0xffff);
}