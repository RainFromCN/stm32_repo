#ifndef __DCMOTOR_H
#define __DCMOTOR_H


/************* TIM *************/

/** advanced tim：产生PWM波的TIM
*
*（1）预分频           0
*（2）OSSR            使能
*（3）CH极性          如果电路中有高速光耦，那么极性相反（low）
*/

/** encoder tim：编码器接口定时器
*
*（1）预分频           0
*（2）ARR             0xffff
*（3）模式             Encoder Mode TI1 and TI2
*/

/*************  其他 *************/

/** pwm_pwmn_channel：高级定时器的PWM互补输出通道
*/

/** sample_period：编码器采样周期
*/

/** ppr：编码器线数(pulse per round)
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

/* 电机对象结构体 */
typedef struct {

    float speed;    //物理速度
    int location;   //物理位置

    PWM_TypeDef PWM;            //PWM对象
    Encoder_TypeDef Encoder;    //编码器对象

} DCMOTOR_TypeDef;


/* 用户调用 */
DCMOTOR_TypeDef     DCMOTOR_Open(TIM_HandleTypeDef* adv_tim, uint32_t pwm_pwmn_channel);
void                DCMOTOR_UseEncoder(DCMOTOR_TypeDef* M, TIM_HandleTypeDef* enc_tim, uint32_t sample_period, int ppr);
void                DCMOTOR_EncUpdate(DCMOTOR_TypeDef* M);
void                DCMOTOR_SetDuty(DCMOTOR_TypeDef* M, int duty);


#endif