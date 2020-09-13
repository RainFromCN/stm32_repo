#include "stm32f4xx_hal.h"
#include "w25x.h"


uint8_t USER_FLASH_Init() {
    USER_FLASH_ReadDevID();
    if (USER_FLASH_ReadJudecID() != FLASH_ID) return 0;
    return 1;
}

uint8_t USER_FLASH_RWByte(uint8_t byte) {
    uint8_t tmp;
    if (HAL_SPI_TransmitReceive(&hspi2, &byte, &tmp, 1, 0xff) != HAL_OK);
    return tmp;
}

void USER_FLASH_WriteEnable() {
    FLASH_CS_RESET();
    USER_FLASH_RWByte(WRITE_ENABLE);
    FLASH_CS_SET();
}

void USER_FLASH_WaitBusy() {
    uint8_t tmp = 0;
    while (1) {
        FLASH_CS_RESET();
        USER_FLASH_RWByte(READ_STATUS);
        tmp = USER_FLASH_RWByte(0xff);
        FLASH_CS_SET();
        if ((tmp & 0x01) == 0) return ;
    }
}

void USER_FLASH_EraseSector(uint32_t add) {
    USER_FLASH_WriteEnable();
    USER_FLASH_WaitBusy();
	
    FLASH_CS_RESET();
    USER_FLASH_RWByte(SECTOR_ERASE);
    USER_FLASH_RWByte((add >> 16) & 0xff);
    USER_FLASH_RWByte((add >> 8) & 0xff);
    USER_FLASH_RWByte(add & 0xff);
    FLASH_CS_SET();

    USER_FLASH_WaitBusy();
}

void USER_FLASH_EraseChip() {
    USER_FLASH_WriteEnable();

    FLASH_CS_RESET();
    USER_FLASH_RWByte(CHIP_ERASE);
    FLASH_CS_SET();

    USER_FLASH_WaitBusy();
}

void USER_FLASH_PageWrite(uint32_t add, uint8_t* pbuf, uint16_t len) {
    if (len > FLASH_PAGESIZE) return ;

    USER_FLASH_WriteEnable();

    FLASH_CS_RESET();
    USER_FLASH_RWByte(PAGE_PROGRAM);
    USER_FLASH_RWByte((add >> 16) & 0xff);
    USER_FLASH_RWByte((add >> 8) & 0xff);
    USER_FLASH_RWByte(add & 0xff);
    for (uint16_t i = 0; i < len; i ++) USER_FLASH_RWByte(pbuf[i]);

    FLASH_CS_SET();

    USER_FLASH_WaitBusy();
}

void USER_FLASH_Write(uint32_t add, uint8_t* pbuf, uint16_t len) {
    uint16_t page_cnt = len / FLASH_PAGESIZE;
    len %= FLASH_PAGESIZE;

    for (uint16_t i = 0; i < page_cnt; i ++) {
        USER_FLASH_PageWrite(add, pbuf, FLASH_PAGESIZE);
        add += FLASH_PAGESIZE;
        pbuf += FLASH_PAGESIZE;
    }
    USER_FLASH_PageWrite(add, pbuf, len);
}

void USER_FLASH_Read(uint32_t add, uint8_t* pbuf, uint16_t len) {
    FLASH_CS_RESET();
    USER_FLASH_RWByte(READ_DATA);
    USER_FLASH_RWByte((add >> 16) & 0xff);
    USER_FLASH_RWByte((add >> 8) & 0xff);
    USER_FLASH_RWByte(add & 0xff);
    for (uint16_t i = 0; i < len; i ++) pbuf[i] = USER_FLASH_RWByte(0xff);
    FLASH_CS_SET();
}

uint8_t USER_FLASH_ReadDevID() {
    uint8_t tmp = 0;

    FLASH_CS_RESET();
    USER_FLASH_RWByte(DEV_ID);
    USER_FLASH_RWByte(0xff);
    USER_FLASH_RWByte(0xff);
    USER_FLASH_RWByte(0xff);
    tmp = USER_FLASH_RWByte(0xff);
    FLASH_CS_SET();

    return tmp;
}

uint32_t USER_FLASH_ReadJudecID() {
    uint32_t tmp;

    FLASH_CS_RESET();
    USER_FLASH_RWByte(JUDEC_ID);
    for (int i = 0; i < 3; i ++) {
        tmp <<= 8;
        tmp |= USER_FLASH_RWByte(0xff);
    }
    FLASH_CS_SET();

    return tmp;
}

void USER_FLASH_SleepIn() {
    FLASH_CS_RESET();
    USER_FLASH_RWByte(PWR_DOWN);
    FLASH_CS_SET();
}

void USER_FLASH_SleepOut() {
    FLASH_CS_RESET();
    USER_FLASH_RWByte(RELEASE_PWR_DOWN);
    FLASH_CS_SET();
}