#ifndef __DCMOTOR_H
#define __DCMOTOR_H


/************* TIM *************/

/** advanced tim������PWM����TIM
*
*��1��Ԥ��Ƶ           0
*��2��OSSR            ʹ��
*��3��CH����          �����·���и��ٹ����ô�����෴��low��
*/

/** encoder tim���������ӿڶ�ʱ��
*
*��1��Ԥ��Ƶ           0
*��2��ARR             0xffff
*��3��ģʽ             Encoder Mode TI1 and TI2
*/

/*************  ���� *************/

/** pwm_pwmn_channel���߼���ʱ����PWM�������ͨ��
*/

/** sample_period����������������
*/

/** ppr������������(pulse per round)
*/


#include "stm32f4xx_hal.h"


#define DCMOTOR_BUSY        1
#define DCMOTOR_IDLE        0

#define DCMOTOR_OFF         0
#define DCMOTOR_ON          1


typedef struct {
    int duty;
    int period;
    uint32_t channel;
    TIM_HandleTypeDef *ADV_TIM;
} PWM_TypeDef;

typedef struct {
    int PPR;
    uint8_t pwr;
    uint32_t sample_period;
    TIM_HandleTypeDef *ENC_TIM;
} Encoder_TypeDef;

/* �������ṹ�� */
typedef struct {

    float speed;    //�����ٶ�
    int location;   //����λ��

    PWM_TypeDef PWM;            //PWM����
    Encoder_TypeDef Encoder;    //����������

} DCMOTOR_TypeDef;


/* �û����� */
DCMOTOR_TypeDef     DCMOTOR_Open(TIM_HandleTypeDef* adv_tim, uint32_t pwm_pwmn_channel);
void                DCMOTOR_UseEncoder(DCMOTOR_TypeDef* M, TIM_HandleTypeDef* enc_tim, uint32_t sample_period, int ppr);
void                DCMOTOR_EncUpdate(DCMOTOR_TypeDef* M);
void                DCMOTOR_SetDuty(DCMOTOR_TypeDef* M, int duty);


#endif