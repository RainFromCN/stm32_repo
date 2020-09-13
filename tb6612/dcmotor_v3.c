#include "dcmotor_v3.h"
#include "string.h"


/* 创建电机实例 */
DCMOTOR_TypeDef DCMOTOR_Open(TIM_HandleTypeDef* pwm_tim, uint32_t pwm_channel, char* in1, char* in2) {
    DCMOTOR_TypeDef tmp;
    memset(&tmp, 0, sizeof(DCMOTOR_TypeDef));

    switch (in1[1]) {
    case 'A': tmp.in1_port = (GPIO_TypeDef *)GPIOA_BASE; break;
    case 'B': tmp.in1_port = (GPIO_TypeDef *)GPIOB_BASE; break;
    case 'C': tmp.in1_port = (GPIO_TypeDef *)GPIOC_BASE; break;
    case 'D': tmp.in1_port = (GPIO_TypeDef *)GPIOD_BASE; break;
    case 'E': tmp.in1_port = (GPIO_TypeDef *)GPIOE_BASE; break;
    }
    if (in1[3] == '\0') {
        switch (in1[2]) {
        case '0': tmp.in1_pin = GPIO_PIN_0; break;
        case '1': tmp.in1_pin = GPIO_PIN_1; break;
        case '2': tmp.in1_pin = GPIO_PIN_2; break;
        case '3': tmp.in1_pin = GPIO_PIN_3; break;
        case '4': tmp.in1_pin = GPIO_PIN_4; break;
        case '5': tmp.in1_pin = GPIO_PIN_5; break;
        case '6': tmp.in1_pin = GPIO_PIN_6; break;
        case '7': tmp.in1_pin = GPIO_PIN_7; break;
        case '8': tmp.in1_pin = GPIO_PIN_8; break;
        case '9': tmp.in1_pin = GPIO_PIN_9; break;
        }
    }
    else {
        switch (in1[3]) {
        case '0': tmp.in1_pin = GPIO_PIN_10; break;
        case '1': tmp.in1_pin = GPIO_PIN_11; break;
        case '2': tmp.in1_pin = GPIO_PIN_12; break;
        case '3': tmp.in1_pin = GPIO_PIN_13; break;
        case '4': tmp.in1_pin = GPIO_PIN_14; break;
        case '5': tmp.in1_pin = GPIO_PIN_15; break;
        }
    }

    switch (in2[1]) {
    case 'A': tmp.in2_port = (GPIO_TypeDef *)GPIOA_BASE; break;
    case 'B': tmp.in2_port = (GPIO_TypeDef *)GPIOB_BASE; break;
    case 'C': tmp.in2_port = (GPIO_TypeDef *)GPIOC_BASE; break;
    case 'D': tmp.in2_port = (GPIO_TypeDef *)GPIOD_BASE; break;
    case 'E': tmp.in2_port = (GPIO_TypeDef *)GPIOE_BASE; break;
    }
    if (in2[3] == '\0') {
        switch (in2[2]) {
        case '0': tmp.in2_pin = GPIO_PIN_0; break;
        case '1': tmp.in2_pin = GPIO_PIN_1; break;
        case '2': tmp.in2_pin = GPIO_PIN_2; break;
        case '3': tmp.in2_pin = GPIO_PIN_3; break;
        case '4': tmp.in2_pin = GPIO_PIN_4; break;
        case '5': tmp.in2_pin = GPIO_PIN_5; break;
        case '6': tmp.in2_pin = GPIO_PIN_6; break;
        case '7': tmp.in2_pin = GPIO_PIN_7; break;
        case '8': tmp.in2_pin = GPIO_PIN_8; break;
        case '9': tmp.in2_pin = GPIO_PIN_9; break;
        }
    }
    else {
        switch (in2[3]) {
        case '0': tmp.in2_pin = GPIO_PIN_10; break;
        case '1': tmp.in2_pin = GPIO_PIN_11; break;
        case '2': tmp.in2_pin = GPIO_PIN_12; break;
        case '3': tmp.in2_pin = GPIO_PIN_13; break;
        case '4': tmp.in2_pin = GPIO_PIN_14; break;
        case '5': tmp.in2_pin = GPIO_PIN_15; break;
        }
    }

    tmp.PWM.period = (*pwm_tim).Init.Period + 1;
    tmp.PWM.period = (int)(tmp.PWM.period * 0.95);
    tmp.PWM.channel = pwm_channel;

    tmp.PWM.pwm_tim = pwm_tim;

    /* TIM时基启动 */
    HAL_TIM_Base_Start(pwm_tim);
    __HAL_TIM_SET_COMPARE(pwm_tim, pwm_channel, 0);
    HAL_TIM_PWM_Start(pwm_tim, pwm_channel);

    return tmp;
}

/* 启动编码器 */
void DCMOTOR_UseEncoder(DCMOTOR_TypeDef* M, TIM_HandleTypeDef* enc_tim, uint32_t sample_period, int ppr) {
    (*M).ENC.pwr = DCMOTOR_ON;
    (*M).ENC.sample_period = sample_period;
    (*M).ENC.enc_tim = enc_tim;
    (*M).ENC.PPR = ppr;

    /* TIM时基启动 */
    HAL_TIM_Base_Start(enc_tim);
    __HAL_TIM_SET_COUNTER(enc_tim, 10000);
    HAL_TIM_Encoder_Start(enc_tim, TIM_CHANNEL_ALL);
}

...用户修改（1）//如果使用编码器，需将此函数放进滴答定时器中每1ms运行一次
/* 将多个对象进行滴答更新 */
void DCMOTOR_UpdateEx(DCMOTOR_TypeDef* M, uint32_t len) {

    static int tmp[20];
    
    /* 取第一个对象采样周期作为组采样周期 */
    M[0].ENC.tick += 1;
    if (M[0].ENC.tick >= M[0].ENC.sample_period) M[0].ENC.tick = 0;
    else return;

    /* 获取编码器数据并重置编码器 */
    for (uint8_t i = 0; i < len; i++) {
        tmp[i] = __HAL_TIM_GET_COUNTER(M[i].ENC.enc_tim) - 10000;
        __HAL_TIM_SET_COUNTER(M[i].ENC.enc_tim, 10000);
    }
    
    /* 更新速度和位置 */
    for (uint8_t i = 0; i < len; i++) {
        M[i].location += tmp[i];
        tmp[i] *= (1000 / M[i].ENC.sample_period);
        M[i].speed = (double)tmp[i] / M[i].ENC.PPR;
    }
}

...用户修改（2）//如果使用编码器，需将此函数放进滴答定时器中每1ms运行一次
/* 将单个对象进行滴答更新 */
void DCMOTOR_Update(DCMOTOR_TypeDef* M) {

    (*M).ENC.tick += 1;
    if ((*M).ENC.tick >= (*M).ENC.sample_period) (*M).ENC.tick = 0;
    else return;

    /* 更新速度和位置 */
    int tmp = __HAL_TIM_GET_COUNTER((*M).ENC.enc_tim) - 10000;
    (*M).location += tmp;
    tmp *= (1000 / (*M).ENC.sample_period);
    (*M).speed = (double)tmp / (*M).ENC.PPR;

    /* 重置编码器 */
    __HAL_TIM_SET_COUNTER((*M).ENC.enc_tim, 10000);
}

/* 设置电机旋转方向 */
void DCMOTOR_SetDir(DCMOTOR_TypeDef* M, uint8_t dir) {
    if (dir == DCMOTOR_FORWARD) {
        HAL_GPIO_WritePin((*M).in1_port, (*M).in1_pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin((*M).in2_port, (*M).in2_pin, GPIO_PIN_RESET);
    }
    else if (dir == DCMOTOR_BACKWARD) {
        HAL_GPIO_WritePin((*M).in1_port, (*M).in1_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin((*M).in2_port, (*M).in2_pin, GPIO_PIN_SET);
    }
}

/* 设置占空比，占空比可以是负数 */
void DCMOTOR_SetDuty(DCMOTOR_TypeDef* M, int duty) {
    if (duty < 0) {
        DCMOTOR_SetDir(M, DCMOTOR_BACKWARD);
        duty = -duty;
    }
    else {
        DCMOTOR_SetDir(M, DCMOTOR_FORWARD);
    }
    if (duty >(*M).PWM.period) duty = (*M).PWM.period;

    __HAL_TIM_SET_COMPARE((*M).PWM.pwm_tim, (*M).PWM.channel, duty);
}

