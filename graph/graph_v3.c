#include "graph_v3.h"


void Graph_TxPoint(int dat) {
    uint8_t cx = 0x88;
    uint8_t* pbuf = &dat;
    printf("%c%c", 0xaa, 0x88);
    for (uint8_t i = 0; i < 4; i++) {
        printf("%c", pbuf[i]);
        cx += pbuf[i];
    }
    for (uint8_t i = 0; i < 8; i++) {
        printf("%c", 0x55);
        cx += 0x55;
    }
    printf("%c%c", cx, 0x2f);
}


void Graph_TxCurve(int* pbuf, int len) {
    for (int i = 0; i < len; i++) {
        printf("%c", pbuf[i]);
        HAL_Delay(100);
    }
}