#include "stm32f4xx_hal.h"
#include "w25q_v3.h"


SPI_HandleTypeDef*  W25Q_SPI;
GPIO_TypeDef*       W25Q_CS_PORT;
uint16_t            W25Q_CS_PIN;



uint8_t W25Q_Open(SPI_HandleTypeDef* spi, char* cs) {
    
    /* 使用哪一个SPI */
    W25Q_SPI = spi;
    
    /* 使用哪一个CS引脚 */
    switch(cs[1]) {
        case 'A': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOA_BASE; break;
        case 'B': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOB_BASE; break;
        case 'C': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOC_BASE; break;
        case 'D': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOD_BASE; break;
        case 'E': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOE_BASE; break;
        case 'F': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOF_BASE; break;
        case 'G': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOG_BASE; break;
        case 'H': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOH_BASE; break;
        case 'I': W25Q_CS_PORT = (GPIO_TypeDef *)GPIOI_BASE; break;
    }
    if (cs[3] == '\0') {
        switch(cs[2]) {
            case '0': W25Q_CS_PIN = GPIO_PIN_0; break;
            case '1': W25Q_CS_PIN = GPIO_PIN_1; break;
            case '2': W25Q_CS_PIN = GPIO_PIN_2; break;
            case '3': W25Q_CS_PIN = GPIO_PIN_3; break;
            case '4': W25Q_CS_PIN = GPIO_PIN_4; break;
            case '5': W25Q_CS_PIN = GPIO_PIN_5; break;
            case '6': W25Q_CS_PIN = GPIO_PIN_6; break;
            case '7': W25Q_CS_PIN = GPIO_PIN_7; break;
            case '8': W25Q_CS_PIN = GPIO_PIN_8; break;
            case '9': W25Q_CS_PIN = GPIO_PIN_9; break;
        }
    }
    else {
        switch (cs[3]) {
            case '0': W25Q_CS_PIN = GPIO_PIN_10; break;
            case '1': W25Q_CS_PIN = GPIO_PIN_11; break;
            case '2': W25Q_CS_PIN = GPIO_PIN_12; break;
            case '3': W25Q_CS_PIN = GPIO_PIN_13; break;
            case '4': W25Q_CS_PIN = GPIO_PIN_14; break;
            case '5': W25Q_CS_PIN = GPIO_PIN_15; break;
        }
    }

    /* FLASH唤醒 */
    W25Q_SleepOut();
    if (W25Q_ReadJudecID() != W25Q_ID) return 0;
    return 1;
}

uint8_t W25Q_RWByte(uint8_t byte) {
    uint8_t tmp;
    if (HAL_SPI_TransmitReceive(W25Q_SPI, &byte, &tmp, 1, 0xff) != HAL_OK);
    return tmp;
}

void W25Q_WriteEnable() {
    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_WRITE_ENABLE);
    W25Q_CS_SET();
}

void W25Q_WaitBusy() {
    uint8_t tmp = 0;
    while (1) {
        W25Q_CS_RESET();
        W25Q_RWByte(W25Q_READ_STATUS);
        tmp = W25Q_RWByte(0xff);
        W25Q_CS_SET();
        if ((tmp & 0x01) == 0) return;
    }
}

void W25Q_EraseSector(uint32_t add) {
    W25Q_WriteEnable();
    W25Q_WaitBusy();

    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_SECTOR_ERASE);
    W25Q_RWByte((add >> 16) & 0xff);
    W25Q_RWByte((add >> 8) & 0xff);
    W25Q_RWByte(add & 0xff);
    W25Q_CS_SET();

    W25Q_WaitBusy();
}

void W25Q_EraseChip() {
    W25Q_WriteEnable();

    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_CHIP_ERASE);
    W25Q_CS_SET();

    W25Q_WaitBusy();
}

void W25Q_PageWrite(uint32_t add, uint8_t* pbuf, uint16_t len) {
    if (len > W25Q_PAGESIZE) return;

    W25Q_WriteEnable();

    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_PAGE_PROGRAM);
    W25Q_RWByte((add >> 16) & 0xff);
    W25Q_RWByte((add >> 8) & 0xff);
    W25Q_RWByte(add & 0xff);
    for (uint16_t i = 0; i < len; i++) W25Q_RWByte(pbuf[i]);

    W25Q_CS_SET();

    W25Q_WaitBusy();
}

void W25Q_Write(uint32_t add, uint8_t* pbuf, uint16_t len) {
    uint16_t page_cnt = len / W25Q_PAGESIZE;
    len %= W25Q_PAGESIZE;

    for (uint16_t i = 0; i < page_cnt; i++) {
        W25Q_PageWrite(add, pbuf, W25Q_PAGESIZE);
        add += W25Q_PAGESIZE;
        pbuf += W25Q_PAGESIZE;
    }
    W25Q_PageWrite(add, pbuf, len);
}

void W25Q_Read(uint32_t add, uint8_t* pbuf, uint16_t len) {
    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_READ_DATA);
    W25Q_RWByte((add >> 16) & 0xff);
    W25Q_RWByte((add >> 8) & 0xff);
    W25Q_RWByte(add & 0xff);
    for (uint16_t i = 0; i < len; i++) pbuf[i] = W25Q_RWByte(0xff);
    W25Q_CS_SET();
}

uint8_t W25Q_ReadDevID() {
    uint8_t tmp = 0;

    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_DEV_ID);
    W25Q_RWByte(0xff);
    W25Q_RWByte(0xff);
    W25Q_RWByte(0xff);
    tmp = W25Q_RWByte(0xff);
    W25Q_CS_SET();

    return tmp;
}

uint32_t W25Q_ReadJudecID() {
    uint32_t tmp;

    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_JUDEC_ID);
    for (int i = 0; i < 3; i++) {
        tmp <<= 8;
        tmp |= W25Q_RWByte(0xff);
    }
    W25Q_CS_SET();

    return tmp;
}

void W25Q_SleepIn() {
    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_PWR_DOWN);
    W25Q_CS_SET();
}

void W25Q_SleepOut() {
    W25Q_CS_RESET();
    W25Q_RWByte(W25Q_RELEASE_PWR_DOWN);
    W25Q_CS_SET();
}