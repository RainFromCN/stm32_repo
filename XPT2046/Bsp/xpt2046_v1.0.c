#include "stm32f4xx_hal.h"
#include "xpt2046_v1.0.h"


void USER_TOUCH_Init() {
    TOUCH_CS_RESET();
}

void USER_TOUCH_Delay(volatile uint32_t us) {
    for (uint32_t i = 0; i < us; i++) {
        uint8_t t = 12;
        while (t--);
    }
}

void USER_TOUCH_WriteByte(uint8_t byte) {
    TOUCH_MOSI_RESET();
    TOUCH_SCK_RESET();

    for (uint8_t i = 0; i < 8; i++) {
        ((byte >> (7 - i)) & 0x01) ? TOUCH_MOSI_SET() : TOUCH_MOSI_RESET();
        USER_TOUCH_Delay(5);
        TOUCH_SCK_SET();
        TOUCH_SCK_RESET();
    }
}

uint16_t USER_TOUCH_ReadByte(void) {
    uint16_t tmp, buf;

    TOUCH_MOSI_RESET();
    TOUCH_SCK_SET();

    for (uint8_t i = 0; i < 12; i++) {
        TOUCH_SCK_RESET();
        tmp = TOUCH_MISO_READ();
        buf |= tmp << (11 - i);
        TOUCH_SCK_SET();
    }

    return buf;
}

uint16_t USER_TOUCH_ReadADC(uint8_t ch) {
    USER_TOUCH_WriteByte(ch);
    return USER_TOUCH_ReadByte();
}