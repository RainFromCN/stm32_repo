#include "mathlab_v3.h"
#include "math.h"


/* ��Ծ�ź� */
void MATHLAB_Step(uint32_t* head, uint32_t len, uint8_t type, uint32_t step_index, float init_val, float final_val) {
    
    if (type == MATHLAB_FLOAT) {
        for (uint32_t i = 0; i < step_index; i++)   ((float*)head)[i] = init_val;
        for (uint32_t i = step_index; i < len; i++) ((float*)head)[i] = final_val;
    }
    
    else if (type == MATHLAB_INT) {
        for (uint32_t i = 0; i < step_index; i++)   ((int*)head)[i] = (int)(init_val);
        for (uint32_t i = step_index; i < len; i++) ((int*)head)[i] = (int)(final_val);
    }
}

/* �����ź� */
void MATHLAB_Sine(uint32_t* head, uint32_t len, uint8_t type, float floar, float ceiling, uint32_t T) {
    float offset = (floar + ceiling) / 2;
    float amp = (ceiling - floar) / 2;
    
    if (type == MATHLAB_FLOAT) {
        int cnt = len / T;
        float step = 2 * MATHLAB_PI / T;
        for (int i = 0; i < cnt; i++) {
            for (int j = 0; j < T; j++) {
                ((float*)head)[i*T + j] = amp * sin(step*j) + offset;
            }
        }
        for (int i = cnt * T; i < len; i++) {
            ((float*)head)[i] = amp * sin(step*i) + offset;
        }
    }
    
    else if (type == MATHLAB_INT) {
        int cnt = len / T;
        float step = 2 * MATHLAB_PI / T;
        for (int i = 0; i < cnt; i++) {
            for (int j = 0; j < T; j++) {
                ((int*)head)[i*T + j] = (int)(amp * sin(step*j) + offset);
            }
        }
        for (int i = cnt * T; i < len; i++) {
            ((int*)head)[i] = (int)(amp * sin(step*i) + offset);
        }
    }
}

...�û��޸ģ�1��//���ʹ�����ݲɼ����ܣ����˺����Ž��δ�ʱ����
/* ���������δ���� */
void MATHLAB_UpdateEx(Collect_TypeDef* C, uint32_t len) {
    
    /* ȡ��һ���������������Ϊ��������� */
    C[0].tick += 1;
    if (C[0].tick >= C[0].sample_period) C[0].tick = 0;
    else return ;
    
    /* �ɼ���� */
    if (C[0].state == MATHLAB_IDLE) return ;
    
    /* �ɼ����� */
    for (uint32_t i = 0; i < len; i++) {
        if (C[i].data_type == MATHLAB_FLOAT) ((float*)C[i].head)[C[i].index++] = *((float*)C[i].data_source);
        else if (C[i].data_type == MATHLAB_INT) ((int*)C[i].head)[C[i].index++] = *((int*)C[i].data_source);
    }
    
    /* �ж��Ƿ�ɼ���� */
    if (C[0].index == C[0].len) {
        for (uint32_t i = 0; i < len; i ++) C[i].state = MATHLAB_IDLE;
    }
}

...�û��޸ģ�2��//���ʹ�����ݲɼ����ܣ����˺����Ž��δ�ʱ����
/* �Ե�������δ���� */
void MATHLAB_Update(Collect_TypeDef* C) {
    
    (*C).tick += 1;
    if ((*C).tick > (*C).sample_period) (*C).tick = 0;
    else return ;
    
    /* �ɼ���� */
    if ((*C).state == MATHLAB_IDLE) return ;
    
    /* �ɼ����� */
    if ((*C).data_type == MATHLAB_FLOAT) ((float*)(*C).head)[(*C).index++] = *((float*)(*C).data_source);
    else if ((*C).data_type == MATHLAB_INT) ((int*)(*C).head)[(*C).index++] = *((int*)(*C).data_source);
    
    /* �ж��Ƿ�ɼ���� */
    if ((*C).index == (*C).len) (*C).state = MATHLAB_IDLE;
}

/* ���ݲɼ� */
Collect_TypeDef MATHLAB_Collect(uint32_t* head, uint32_t len, uint8_t type, uint32_t* data_source, uint32_t sample_period) {
    
    Collect_TypeDef tmp;
    
    /* ��ʼ�� */
    tmp.data_type = type;
    tmp.head = head;
    tmp.index = 0;
    tmp.len = len;
    tmp.sample_period = sample_period;
    tmp.tick = 0;
    tmp.data_source = data_source;
    tmp.state = MATHLAB_BUSY;
    
    return tmp;
}