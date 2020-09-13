
/************* 杂项 *************/

/** lp_const：低通滤波器系数
  *
  *（1）取值范围：0-1，该系数越大，滤波效果越强，延时越大
  *
  */


#ifndef __FILTER_H
#define __FILTER_H


#define FILTER_FLOAT                0
#define FILTER_INT                  1


#include "stm32f1xx_hal.h"


/* 滤波器对象结构体 */
typedef struct {

    uint8_t   data_type;
    uint32_t* from;
    uint32_t* to;

    /* 低通滤波系数 */
    float lp_const;

    /* 采样周期 */
    uint32_t sample_period;
    uint32_t tick;
    uint8_t  tick_fresh;

} FILTER_TypeDef;


/* 用户调用 */
FILTER_TypeDef  FILTER_Open(uint8_t data_type, uint32_t* from, uint32_t* to, float lp_const, uint32_t sample_period);
void            FILTER_Update(FILTER_TypeDef* F);
void            FILTER_Callback(FILTER_TypeDef* F);


#endif