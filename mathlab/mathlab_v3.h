#ifndef __MATHLAB_H
#define __MATHLAB_H


#include "stm32f1xx_hal.h"


#define MATHLAB_FLOAT               0
#define MATHLAB_DOUBLE              1
#define MATHLAB_INT                 2


#define MATHLAB_IDLE                0
#define MATHLAB_BUSY                1

#define MATHLAB_PI                  3.1416


/* �ɼ�����ṹ�� */
typedef struct {
    int         sample_period;  //�����ļ��
    uint32_t*   head;           //����ɼ����ݵ�ַ
    uint32_t    len;            //�ɼ����ݳ���
    uint8_t     data_type;      //�ɼ����ݵ�����
    uint32_t*   data_source;    //���ɼ������ݵ�ַ
    uint32_t    index;          //�����òɼ�һ��
    uint32_t    tick;           //�δ��ʱ
    uint8_t     state;          //�ɼ������״̬
} Collect_TypeDef;


/* �û����� */
void              MATHLAB_Step(uint32_t* head, uint32_t len, uint8_t type, uint32_t step_index, float init_val, float final_val);     //��Ծ�ź�
void              MATHLAB_Sine(uint32_t* head, uint32_t len, uint8_t type, float floar, float ceiling, uint32_t T);                   //�����ź�
void              MATHLAB_UpdateEx(Collect_TypeDef* C, uint32_t len);                                                                 //һ�θ��¶���βɼ�����
void              MATHLAB_Update(Collect_TypeDef* C);                                                                                 //���µ����ɼ�����
Collect_TypeDef   MATHLAB_Collect(uint32_t* head, uint32_t len, uint8_t type, uint32_t* data_source, uint32_t sample_period);         //�����ɼ�����ʵ��



#endif