#include "ili9488_v3.h"


uint32_t ILI9488_SRAM_BASE;


uint32_t FSMC_ILI9488_CMD;
uint32_t FSMC_ILI9488_DATA;


GPIO_TypeDef*       LCD_BL_PORT;
uint16_t            LCD_BL_PIN;


void USER_ILI9488_Open(SRAM_HandleTypeDef* hsram, char* bl) {
    switch ((*hsram).Init.NSBank) {
        case FSMC_NORSRAM_BANK1: ILI9488_SRAM_BASE = FSMC_SRAM1_BASE; break;
        case FSMC_NORSRAM_BANK2: ILI9488_SRAM_BASE = FSMC_SRAM2_BASE; break;
        case FSMC_NORSRAM_BANK3: ILI9488_SRAM_BASE = FSMC_SRAM3_BASE; break;
        case FSMC_NORSRAM_BANK4: ILI9488_SRAM_BASE = FSMC_SRAM4_BASE; break;
    }

    FSMC_ILI9488_CMD  = ILI9488_SRAM_BASE;
    FSMC_ILI9488_DATA = ILI9488_SRAM_BASE | 0x00000002;

    switch (bl[1]) {
    case 'A': LCD_BL_PORT = (GPIO_TypeDef*)GPIOA_BASE; break;
    case 'B': LCD_BL_PORT = (GPIO_TypeDef*)GPIOB_BASE; break;
    case 'C': LCD_BL_PORT = (GPIO_TypeDef*)GPIOC_BASE; break;
    case 'D': LCD_BL_PORT = (GPIO_TypeDef*)GPIOD_BASE; break;
    case 'E': LCD_BL_PORT = (GPIO_TypeDef*)GPIOE_BASE; break;
    case 'F': LCD_BL_PORT = (GPIO_TypeDef*)GPIOF_BASE; break;
    case 'G': LCD_BL_PORT = (GPIO_TypeDef*)GPIOG_BASE; break;
    case 'H': LCD_BL_PORT = (GPIO_TypeDef*)GPIOH_BASE; break;
    case 'I': LCD_BL_PORT = (GPIO_TypeDef*)GPIOI_BASE; break;
    }
    if (bl[3] == '\0') {
        switch (bl[2]) {
        case '0': LCD_BL_PIN = GPIO_PIN_0; break;
        case '1': LCD_BL_PIN = GPIO_PIN_1; break;
        case '2': LCD_BL_PIN = GPIO_PIN_2; break;
        case '3': LCD_BL_PIN = GPIO_PIN_3; break;
        case '4': LCD_BL_PIN = GPIO_PIN_4; break;
        case '5': LCD_BL_PIN = GPIO_PIN_5; break;
        case '6': LCD_BL_PIN = GPIO_PIN_6; break;
        case '7': LCD_BL_PIN = GPIO_PIN_7; break;
        case '8': LCD_BL_PIN = GPIO_PIN_8; break;
        case '9': LCD_BL_PIN = GPIO_PIN_9; break;
        }
    }
    else {
        switch (bl[3]) {
        case '0': LCD_BL_PIN = GPIO_PIN_10; break;
        case '1': LCD_BL_PIN = GPIO_PIN_11; break;
        case '2': LCD_BL_PIN = GPIO_PIN_12; break;
        case '3': LCD_BL_PIN = GPIO_PIN_13; break;
        case '4': LCD_BL_PIN = GPIO_PIN_14; break;
        case '5': LCD_BL_PIN = GPIO_PIN_15; break;
        }
    }

    HAL_GPIO_WritePin(LCD_BL_PORT, LCD_BL_PIN, GPIO_PIN_SET);
}

void USER_ILI9488_WriteCtrl(uint16_t cmd) {
    *(__IO uint16_t*)FSMC_ILI9488_CMD = cmd;
}

void USER_ILI9488_WriteData(uint16_t dat) {
    *(__IO uint16_t*)FSMC_ILI9488_DATA = dat;
}

uint16_t USER_ILI9488_Read() {
    return *(__IO uint16_t*)FSMC_ILI9488_DATA;
}

uint32_t USER_ILI9488_GetID()
{
	uint16_t buf[4];

	USER_ILI9488_WriteCtrl(0x04);  
	buf[0] = USER_ILI9488_Read();        
	buf[1] = USER_ILI9488_Read()&0x00ff; 
	buf[2] = USER_ILI9488_Read()&0x00ff; 
	buf[3] = USER_ILI9488_Read()&0x00ff; 
	return (buf[1] << 16) + (buf[2] << 8) + buf[3];  
}