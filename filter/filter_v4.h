
/************* ���� *************/

/** lp_const����ͨ�˲���ϵ��
  *
  *��1��ȡֵ��Χ��0-1����ϵ��Խ���˲�Ч��Խǿ����ʱԽ��
  *
  */


#ifndef __FILTER_H
#define __FILTER_H


#define FILTER_FLOAT                0
#define FILTER_INT                  1


#include "stm32f1xx_hal.h"


/* �˲�������ṹ�� */
typedef struct {

    uint8_t   data_type;
    uint32_t* from;
    uint32_t* to;

    /* ��ͨ�˲�ϵ�� */
    float lp_const;

    /* �������� */
    uint32_t sample_period;
    uint32_t tick;
    uint8_t  tick_fresh;

} FILTER_TypeDef;


/* �û����� */
FILTER_TypeDef  FILTER_Open(uint8_t data_type, uint32_t* from, uint32_t* to, float lp_const, uint32_t sample_period);
void            FILTER_Update(FILTER_TypeDef* F);
void            FILTER_Callback(FILTER_TypeDef* F);


#endif