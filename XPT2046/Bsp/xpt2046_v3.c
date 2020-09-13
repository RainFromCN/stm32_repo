#include "stm32f4xx_hal.h"
#include "xpt2046_v3.h"


GPIO_TypeDef*       XPT2046_CS_PORT;
uint16_t            XPT2046_CS_PIN;

GPIO_TypeDef*       XPT2046_SCK_PORT;
uint16_t            XPT2046_SCK_PIN;

GPIO_TypeDef*       XPT2046_MOSI_PORT;
uint16_t            XPT2046_MOSI_PIN;

GPIO_TypeDef*       XPT2046_MISO_PORT;
uint16_t            XPT2046_MISO_PIN;



void USER_XPT2046_Open(char* cs, char* sck, char* mosi, char* miso) {
    
    switch (cs[1]) {
    case 'A': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOA_BASE; break;
    case 'B': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOB_BASE; break;
    case 'C': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOC_BASE; break;
    case 'D': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOD_BASE; break;
    case 'E': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOE_BASE; break;
    case 'F': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOF_BASE; break;
    case 'G': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOG_BASE; break;
    case 'H': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOH_BASE; break;
    case 'I': XPT2046_CS_PORT = (GPIO_TypeDef *)GPIOI_BASE; break;
    }
    if (cs[3] == '\0') {
        switch (cs[2]) {
        case '0': XPT2046_CS_PIN = GPIO_PIN_0; break;
        case '1': XPT2046_CS_PIN = GPIO_PIN_1; break;
        case '2': XPT2046_CS_PIN = GPIO_PIN_2; break;
        case '3': XPT2046_CS_PIN = GPIO_PIN_3; break;
        case '4': XPT2046_CS_PIN = GPIO_PIN_4; break;
        case '5': XPT2046_CS_PIN = GPIO_PIN_5; break;
        case '6': XPT2046_CS_PIN = GPIO_PIN_6; break;
        case '7': XPT2046_CS_PIN = GPIO_PIN_7; break;
        case '8': XPT2046_CS_PIN = GPIO_PIN_8; break;
        case '9': XPT2046_CS_PIN = GPIO_PIN_9; break;
        }
    }
    else {
        switch (cs[3]) {
        case '0': XPT2046_CS_PIN = GPIO_PIN_10; break;
        case '1': XPT2046_CS_PIN = GPIO_PIN_11; break;
        case '2': XPT2046_CS_PIN = GPIO_PIN_12; break;
        case '3': XPT2046_CS_PIN = GPIO_PIN_13; break;
        case '4': XPT2046_CS_PIN = GPIO_PIN_14; break;
        case '5': XPT2046_CS_PIN = GPIO_PIN_15; break;
        }
    }

    switch (sck[1]) {
    case 'A': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOA_BASE; break;
    case 'B': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOB_BASE; break;
    case 'C': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOC_BASE; break;
    case 'D': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOD_BASE; break;
    case 'E': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOE_BASE; break;
    case 'F': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOF_BASE; break;
    case 'G': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOG_BASE; break;
    case 'H': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOH_BASE; break;
    case 'I': XPT2046_SCK_PORT = (GPIO_TypeDef *)GPIOI_BASE; break;
    }
    if (sck[3] == '\0') {
        switch (sck[2]) {
        case '0': XPT2046_SCK_PIN = GPIO_PIN_0; break;
        case '1': XPT2046_SCK_PIN = GPIO_PIN_1; break;
        case '2': XPT2046_SCK_PIN = GPIO_PIN_2; break;
        case '3': XPT2046_SCK_PIN = GPIO_PIN_3; break;
        case '4': XPT2046_SCK_PIN = GPIO_PIN_4; break;
        case '5': XPT2046_SCK_PIN = GPIO_PIN_5; break;
        case '6': XPT2046_SCK_PIN = GPIO_PIN_6; break;
        case '7': XPT2046_SCK_PIN = GPIO_PIN_7; break;
        case '8': XPT2046_SCK_PIN = GPIO_PIN_8; break;
        case '9': XPT2046_SCK_PIN = GPIO_PIN_9; break;
        }
    }
    else {
        switch (sck[3]) {
        case '0': XPT2046_SCK_PIN = GPIO_PIN_10; break;
        case '1': XPT2046_SCK_PIN = GPIO_PIN_11; break;
        case '2': XPT2046_SCK_PIN = GPIO_PIN_12; break;
        case '3': XPT2046_SCK_PIN = GPIO_PIN_13; break;
        case '4': XPT2046_SCK_PIN = GPIO_PIN_14; break;
        case '5': XPT2046_SCK_PIN = GPIO_PIN_15; break;
        }
    }

    switch (mosi[1]) {
    case 'A': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOA_BASE; break;
    case 'B': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOB_BASE; break;
    case 'C': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOC_BASE; break;
    case 'D': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOD_BASE; break;
    case 'E': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOE_BASE; break;
    case 'F': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOF_BASE; break;
    case 'G': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOG_BASE; break;
    case 'H': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOH_BASE; break;
    case 'I': XPT2046_MOSI_PORT = (GPIO_TypeDef *)GPIOI_BASE; break;
    }
    if (mosi[3] == '\0') {
        switch (mosi[2]) {
        case '0': XPT2046_MOSI_PIN = GPIO_PIN_0; break;
        case '1': XPT2046_MOSI_PIN = GPIO_PIN_1; break;
        case '2': XPT2046_MOSI_PIN = GPIO_PIN_2; break;
        case '3': XPT2046_MOSI_PIN = GPIO_PIN_3; break;
        case '4': XPT2046_MOSI_PIN = GPIO_PIN_4; break;
        case '5': XPT2046_MOSI_PIN = GPIO_PIN_5; break;
        case '6': XPT2046_MOSI_PIN = GPIO_PIN_6; break;
        case '7': XPT2046_MOSI_PIN = GPIO_PIN_7; break;
        case '8': XPT2046_MOSI_PIN = GPIO_PIN_8; break;
        case '9': XPT2046_MOSI_PIN = GPIO_PIN_9; break;
        }
    }
    else {
        switch (mosi[3]) {
        case '0': XPT2046_MOSI_PIN = GPIO_PIN_10; break;
        case '1': XPT2046_MOSI_PIN = GPIO_PIN_11; break;
        case '2': XPT2046_MOSI_PIN = GPIO_PIN_12; break;
        case '3': XPT2046_MOSI_PIN = GPIO_PIN_13; break;
        case '4': XPT2046_MOSI_PIN = GPIO_PIN_14; break;
        case '5': XPT2046_MOSI_PIN = GPIO_PIN_15; break;
        }
    }

    switch (miso[1]) {
    case 'A': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOA_BASE; break;
    case 'B': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOB_BASE; break;
    case 'C': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOC_BASE; break;
    case 'D': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOD_BASE; break;
    case 'E': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOE_BASE; break;
    case 'F': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOF_BASE; break;
    case 'G': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOG_BASE; break;
    case 'H': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOH_BASE; break;
    case 'I': XPT2046_MISO_PORT = (GPIO_TypeDef *)GPIOI_BASE; break;
    }
    if (miso[3] == '\0') {
        switch (miso[2]) {
        case '0': XPT2046_MISO_PIN = GPIO_PIN_0; break;
        case '1': XPT2046_MISO_PIN = GPIO_PIN_1; break;
        case '2': XPT2046_MISO_PIN = GPIO_PIN_2; break;
        case '3': XPT2046_MISO_PIN = GPIO_PIN_3; break;
        case '4': XPT2046_MISO_PIN = GPIO_PIN_4; break;
        case '5': XPT2046_MISO_PIN = GPIO_PIN_5; break;
        case '6': XPT2046_MISO_PIN = GPIO_PIN_6; break;
        case '7': XPT2046_MISO_PIN = GPIO_PIN_7; break;
        case '8': XPT2046_MISO_PIN = GPIO_PIN_8; break;
        case '9': XPT2046_MISO_PIN = GPIO_PIN_9; break;
        }
    }
    else {
        switch (miso[3]) {
        case '0': XPT2046_MISO_PIN = GPIO_PIN_10; break;
        case '1': XPT2046_MISO_PIN = GPIO_PIN_11; break;
        case '2': XPT2046_MISO_PIN = GPIO_PIN_12; break;
        case '3': XPT2046_MISO_PIN = GPIO_PIN_13; break;
        case '4': XPT2046_MISO_PIN = GPIO_PIN_14; break;
        case '5': XPT2046_MISO_PIN = GPIO_PIN_15; break;
        }
    }

    XPT2046_CS_RESET();
}

void USER_XPT2046_Delay(volatile uint32_t us) {
    for (uint32_t i = 0; i < us; i++) {
        uint8_t t = 12;
        while (t--);
    }
}

void USER_XPT2046_WriteByte(uint8_t byte) {
    XPT2046_MOSI_RESET();
    XPT2046_SCK_RESET();

    for (uint8_t i = 0; i < 8; i++) {
        ((byte >> (7 - i)) & 0x01) ? XPT2046_MOSI_SET() : XPT2046_MOSI_RESET();
        USER_XPT2046_Delay(5);
        XPT2046_SCK_SET();
        XPT2046_SCK_RESET();
    }
}

uint16_t USER_XPT2046_ReadByte(void) {
    uint16_t tmp, buf;

    XPT2046_MOSI_RESET();
    XPT2046_SCK_SET();

    for (uint8_t i = 0; i < 12; i++) {
        XPT2046_SCK_RESET();
        tmp = XPT2046_MISO_READ();
        buf |= tmp << (11 - i);
        XPT2046_SCK_SET();
    }

    return buf;
}

uint16_t USER_XPT2046_ReadADC(uint8_t ch) {
    USER_XPT2046_WriteByte(ch);
    return USER_XPT2046_ReadByte();
}