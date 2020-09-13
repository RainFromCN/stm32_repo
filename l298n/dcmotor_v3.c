#include "dcmotor_v3.h"
#include "string.h"


static unsigned short tick = 0;


/* ��������ʵ�� */
DCMOTOR_TypeDef DCMOTOR_Open(TIM_HandleTypeDef* adv_tim, uint32_t pwm_pwmn_channel) {
    DCMOTOR_TypeDef tmp;
    memset(&tmp, 0, sizeof(DCMOTOR_TypeDef));

    tmp.PWM.period = (*adv_tim).Init.Period + 1;
    tmp.PWM.period = (int)(tmp.PWM.period * 0.95);
    tmp.PWM.channel = pwm_pwmn_channel;

    tmp.PWM.ADV_TIM = adv_tim;
    
    /* TIMʱ������ */
    HAL_TIM_Base_Start(adv_tim);
    __HAL_TIM_SET_COMPARE(adv_tim, pwm_pwmn_channel, 0);
    HAL_TIM_PWM_Start(adv_tim, pwm_pwmn_channel);
    
    return tmp;
} 

/* ���������� */
void DCMOTOR_UseEncoder(DCMOTOR_TypeDef* M, TIM_HandleTypeDef* enc_tim, uint32_t sample_period, int ppr) {
    (*M).Encoder.pwr = DCMOTOR_ON;
    (*M).Encoder.sample_period = sample_period;
    (*M).Encoder.ENC_TIM = enc_tim;
    (*M).Encoder.PPR = ppr;
    
    /* TIMʱ������ */
    HAL_TIM_Base_Start(enc_tim);
    __HAL_TIM_SET_COUNTER(enc_tim, 10000);
    HAL_TIM_Encoder_Start(enc_tim, TIM_CHANNEL_ALL);
}

...�û��޸ģ�1��//���ʹ�ñ��������轫�˺����Ž��δ�ʱ����ÿ1ms����һ��
void DCMOTOR_EncUpdate(DCMOTOR_TypeDef* M) {

    tick += 1;
    if (tick >= (*M).Encoder.sample_period) tick = 0;
    else return;

    /* �����ٶȺ�λ�� */
    int tmp = __HAL_TIM_GET_COUNTER((*M).Encoder.ENC_TIM) - 10000;
    (*M).location += tmp;
    tmp *= (1000 / (*M).Encoder.sample_period);
    (*M).speed = (double)tmp / (*M).Encoder.PPR;
    
    /* ���ñ����� */
    __HAL_TIM_SET_COUNTER((*M).Encoder.ENC_TIM, 10000);
}

/* ����ռ�ձȣ�ռ�ձȿ����Ǹ��� */
void DCMOTOR_SetDuty(DCMOTOR_TypeDef* M, int duty) {
    if (duty < 0) {
        HAL_TIM_PWM_Stop((*M).PWM.ADV_TIM, (*M).PWM.channel);
        HAL_TIMEx_PWMN_Start((*M).PWM.ADV_TIM, (*M).PWM.channel);
        duty = -duty;
    }
    else {
        HAL_TIMEx_PWMN_Stop((*M).PWM.ADV_TIM, (*M).PWM.channel);
        HAL_TIM_PWM_Start((*M).PWM.ADV_TIM, (*M).PWM.channel);
    }
    if (duty > (*M).PWM.period) duty = (*M).PWM.period;

    __HAL_TIM_SET_COMPARE((*M).PWM.ADV_TIM, (*M).PWM.channel, duty);
}