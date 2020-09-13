#ifndef __SERVO_H
#define __SERVO_H


#include "stm32h7xx_hal.h"


#define SERVO_IDLE				0
#define SERVO_BUSY				1


/* �������ṹ�� */
typedef struct {
	TIM_HandleTypeDef* servo_tim;
	uint32_t pwm_channel;

	float cur_angle;

	int base;
	int period;

} SERVO_TypeDef;


/* �û����� */
SERVO_TypeDef	SERVO_Open(TIM_HandleTypeDef* servo_tim, uint32_t pwm_channel);
int				SERVO_Mapping(SERVO_TypeDef* S, float angle);
void			SERVO_Rotate(SERVO_TypeDef* S, float angle);


#endif