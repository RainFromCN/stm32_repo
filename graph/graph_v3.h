#ifndef __GRAPH_H
#define __GRAPH_H


#include "stm32f4xx_hal.h"


/* 用户调用 */
void        Graph_TxPoint(int dat)；             //向硬石上位机发送一个数据点
void        Graph_TxCurve(int* pbuf, int len)    //向硬石上位机发送许多数据点

#endif