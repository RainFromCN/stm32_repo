#ifndef __MQUEUE_H
#define __MQUEUE_H


#include "stm32f4xx_hal.h"


#define MQUEUE_FLOAT                0
#define MQUEUE_INT                  1


/* 消息对象 */
typedef struct {

    uint32_t* to;
    uint8_t   data_type;
    uint8_t   topic[20];

} Message_TypeDef;

/* 消息队列对象 */
typedef struct {

    Message_TypeDef message[20];
    uint8_t   queue_size;

    uint8_t   topic_buf[20];
    uint8_t   value_buf[20];
    uint8_t   topic_len;
    uint8_t   value_len;
    uint8_t   mark;
    uint8_t   cx;
    uint8_t   cx_rec;
    uint8_t   buff;

    UART_HandleTypeDef* mq_uart;

} MessageQueue_TypeDef;


/* 用户调用 */
MessageQueue_TypeDef    MQUEUE_Open(UART_HandleTypeDef* mq_uart);                                                   //创建消息队列实例
void                    MQUEUE_Update(UART_HandleTypeDef* huart, MessageQueue_TypeDef* M);                          //更新消息队列
void                    MQUEUE_Enable(MessageQueue_TypeDef* M);                                                     //使能消息队列
void                    MQUEUE_Subscribe(MessageQueue_TypeDef* M, char* topic, uint32_t* to, uint8_t data_type);    //订阅消息
void                    MQUEUE_Publish(UART_HandleTypeDef* huart, char* topic, uint32_t* from);                     //发布消息


#endif 