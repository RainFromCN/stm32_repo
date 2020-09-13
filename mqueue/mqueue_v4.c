#include "mqueue_v4.h"
#include "string.h"


/* 创建消息队列 */
MessageQueue_TypeDef MQUEUE_Open(UART_HandleTypeDef* mq_uart) {
    
    MessageQueue_TypeDef tmp;
    
    memset(&tmp, 0, sizeof(tmp));

    tmp.mq_uart = mq_uart;

    return tmp;
}

/* 开始接收消息 */
void MQUEUE_Enable(MessageQueue_TypeDef* M) {
    HAL_UART_Receive_IT(M->mq_uart, (uint8_t*)&M->buff, 1);
}

...用户修改（1）//放在UART接收完成中断里
/* 更新消息队列 */
void MQUEUE_Update(UART_HandleTypeDef* huart, MessageQueue_TypeDef* M) {
    
    if (M->mq_uart != huart) {
        HAL_UART_Receive_IT(M->mq_uart, (uint8_t*)&M->buff, 1);
        return;
    }
    
    /* 接收数据 */
    if (M->mark == 1) {
        M->value_buf[M->value_len++] = M->buff;
        M->cx += M->buff;
        if (M->value_len >= 4) M->mark = 2;
    }
    
    /* 接收到帧头 */
    else if (M->buff == '@') {
        M->topic_len = 0;
        M->value_len = 0;
        M->mark = 0;
        M->cx = 0;
    }

    /* 接收到间隔符 */
    else if (M->buff == '#') {
        M->mark = 1;
    }

    /* 接收到结束符 */
    else if (M->buff == '$') {

        /* 校验位正确 */
        if (M->cx == M->cx_rec && M->topic_len < 20) {

            M->topic_buf[M->topic_len++] = '\0';

            /* 赋值 */
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

        /* 重置 */
        M->topic_len = 0;
        M->value_len = 0;
        M->mark = 0;
        M->cx = 0;
    }

    /* 接收到topic */
    else if (M->mark == 0) {

        /* 出错则重置 */
        if (M->topic_len >= 20) {
            M->topic_len = 0;
            M->value_len = 0;
            M->mark = 0;
            M->cx = 0;
        }

        M->cx += M->buff;
        M->topic_buf[M->topic_len++] = M->buff;
    }

    /* 接收到cx */
    else if (M->mark == 2) {
        M->cx_rec = M->buff;
    }

    HAL_UART_Receive_IT(M->mq_uart, (uint8_t*)&M->buff, 1);
}

/* 订阅一个话题 */
void MQUEUE_Subscribe(MessageQueue_TypeDef* M, char* topic, uint32_t* to, uint8_t data_type) {
    Message_TypeDef tmp;

    for (uint8_t i = 0; i <= strlen(topic); i++) {
        tmp.topic[i] = topic[i];
    }

    tmp.to = to;
    tmp.data_type = data_type;

    M->message[M->queue_size++] = tmp;
}

/* 发布一个话题 */
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