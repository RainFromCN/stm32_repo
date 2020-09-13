#include "servo_v4.h"


/* 创建一个舵机 */
SERVO_TypeDef SERVO_Open(TIM_HandleTypeDef* servo_tim, uint32_t pwm_channel) {
	SERVO_TypeDef tmp;
	int k;
	tmp.servo_tim = servo_tim;
	tmp.pwm_channel = pwm_channel;
	k = (*servo_tim).Init.Period + 1;
	tmp.base = (int)(k * 0.025);
	tmp.period = (int)(k * 0.1);

	HAL_TIM_Base_Start(servo_tim);
	__HAL_TIM_SET_COMPARE(servo_tim, pwm_channel, SERVO_Mapping(&tmp, 90) );
	HAL_TIM_PWM_Start(servo_tim, pwm_channel);
    
    return tmp;
}

/* 将角度映射成占空比 */
int SERVO_Mapping(SERVO_TypeDef* S, float angle) {
	return (int)((angle / 180) * S->period + S->base);
}

/* 控制舵机旋转到angle角度 */
void SERVO_Rotate(SERVO_TypeDef* S, float angle) {
	__HAL_TIM_SET_COMPARE(S->servo_tim, S->pwm_channel, SERVO_Mapping(S, angle) );
	S->cur_angle = angle;
}