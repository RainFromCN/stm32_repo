#ifndef __DCMOTOR_H
#define __DCMOTOR_H


#include "stm32f1xx_hal.h"


#define DCMOTOR_OFF             0
#define DCMOTOR_ON              1

#define DCMOTOR_FORWARD         0
#define DCMOTOR_BACKWARD        1


typedef struct {
    int duty;
    int period;
    uint32_t channel;
    TIM_HandleTypeDef* pwm_tim;
} PWM_TypeDef;

typedef struct {
    uint32_t tick;
    int pwr;
    int PPR;
    int sample_period;
    TIM_HandleTypeDef* enc_tim;
} ENC_TypeDef;

/* �������ṹ�� */
typedef struct {
    PWM_TypeDef PWM;
    ENC_TypeDef ENC;

    /* IN1�ڶ��� */ 
    GPIO_TypeDef* in1_port;
    uint16_t      in1_pin;

    /* IN2�ڶ��� */
    GPIO_TypeDef* in2_port;
    uint16_t      in2_pin;

    float speed;    //����ת��
    int location;   //����λ��
} DCMOTOR_TypeDef;


/* �û����� */
DCMOTOR_TypeDef     DCMOTOR_Open(TIM_HandleTypeDef* pwm_tim, uint32_t pwm_channel, char* in1, char* in2);
void                DCMOTOR_UseEncoder(DCMOTOR_TypeDef* M, TIM_HandleTypeDef* enc_tim, uint32_t sample_period, int ppr);
void                DCMOTOR_UpdateEx(DCMOTOR_TypeDef* M, uint32_t len);   
void                DCMOTOR_Update(DCMOTOR_TypeDef* M);
void                DCMOTOR_SetDir(DCMOTOR_TypeDef* M, uint8_t dir);    //���õ����ת����
void                DCMOTOR_SetDuty(DCMOTOR_TypeDef* M, int duty);      //���õ��ռ�ձ�


#endif