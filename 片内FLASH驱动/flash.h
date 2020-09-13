#include "stm32f4xx_hal.h"

#define USER_Sector_Number 				FLASH_SECTOR_11    // flash 扇区
#define USER_Sector_Address				0x080E0000	// flash 扇区地址

void USER_FLASH_WriteByteArray(unsigned char* byte, unsigned int len);		//写字节流函数
void USER_FLASH_WriteWordArray(unsigned int* word, unsigned int len);		//写字流函数
uint32_t* USER_FLASH_ArrayHead(void);					//返回 flash 头