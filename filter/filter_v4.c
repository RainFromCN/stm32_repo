#include "filter_v4.h"


/* �����˲��� */
FILTER_TypeDef FILTER_Open(uint8_t data_type, uint32_t* from, uint32_t* to, float lp_const, uint32_t sample_period) {
    FILTER_TypeDef tmp;

    tmp.data_type = data_type;
    tmp.from = from;
    tmp.to = to;
    tmp.lp_const = lp_const;
    tmp.sample_period = sample_period;

    return tmp;
}


...�û��޸ģ�1��//�δ���£�����1ms��ʱ���ж���
void FILTER_Update(FILTER_TypeDef* F) {

    F->tick += 1;
    if (F->tick >= F->sample_period) F->tick = 0;
    else return;

    F->tick_fresh= 1;

}

...�û��޸ģ�2��//��ѯ���£�����while�л��߽����δ����֮��
void FILTER_Callback(FILTER_TypeDef* F) {
    
    if (F->tick_fresh== 0) return;
    F->tick_fresh= 0;

    if (F->data_type == FILTER_FLOAT) {
        *(float*)(F->to) = (1 - F->lp_const) * (*(float*)(F->to)) + F->lp_const * (*(float*)(F->from));
    }
    else if (F->data_type == FILTER_INT) {
        *(int*)(F->to) = (int)((1 - F->lp_const) * (*(int*)(F->to)) + F->lp_const * (*(int*)(F->from)));
    }
}