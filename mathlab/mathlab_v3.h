#ifndef __MATHLAB_H
#define __MATHLAB_H


#include "stm32f1xx_hal.h"


#define MATHLAB_FLOAT               0
#define MATHLAB_DOUBLE              1
#define MATHLAB_INT                 2


#define MATHLAB_IDLE                0
#define MATHLAB_BUSY                1

#define MATHLAB_PI                  3.1416


/* 采集对象结构体 */
typedef struct {
    int         sample_period;  //采样的间隔
    uint32_t*   head;           //保存采集数据地址
    uint32_t    len;            //采集数据长度
    uint8_t     data_type;      //采集数据的类型
    uint32_t*   data_source;    //待采集的数据地址
    uint32_t    index;          //间隔多久采集一次
    uint32_t    tick;           //滴答计时
    uint8_t     state;          //采集对象的状态
} Collect_TypeDef;


/* 用户调用 */
void              MATHLAB_Step(uint32_t* head, uint32_t len, uint8_t type, uint32_t step_index, float init_val, float final_val);     //阶跃信号
void              MATHLAB_Sine(uint32_t* head, uint32_t len, uint8_t type, float floar, float ceiling, uint32_t T);                   //正弦信号
void              MATHLAB_UpdateEx(Collect_TypeDef* C, uint32_t len);                                                                 //一次更新多个次采集对象
void              MATHLAB_Update(Collect_TypeDef* C);                                                                                 //更新单个采集对象
Collect_TypeDef   MATHLAB_Collect(uint32_t* head, uint32_t len, uint8_t type, uint32_t* data_source, uint32_t sample_period);         //创建采集对象实例



#endif