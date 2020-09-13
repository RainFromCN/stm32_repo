#include "flash.h"

void USER_FLASH_WriteByteArray(unsigned char* byte, unsigned int len) {
	HAL_FLASH_Unlock();								//解锁
	FLASH_Erase_Sector(USER_Sector_Number, FLASH_VOLTAGE_RANGE_1);				//擦出扇区，使用最低电压
	uint64_t tmp;									
	for (int i = 0; i < len; i ++) {
		tmp = byte[i];								//将待发送数据转换成 uint64_t
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, USER_Sector_Address + i, tmp);		//发送数据
	}
	HAL_FLASH_Lock();									//上锁
}

void USER_FLASH_WriteWordArray(unsigned int* word, unsigned int len) {
	HAL_FLASH_Unlock();								//解锁
	FLASH_Erase_Sector(USER_Sector_Number, FLASH_VOLTAGE_RANGE_1);				//擦出扇区
	uint64_t tmp;									
	for (int i = 0; i < len; i ++) {				
		tmp = word[i];								//将待发送数据转换成 uint64_t
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, USER_Sector_Address + i * 4, tmp);	//发送数据
	}
	HAL_FLASH_Lock();									//上锁
}

uint32_t* USER_FLASH_ArrayHead(void) {
	return (uint32_t*) USER_Sector_Address; 							//返回 flash 扇区地址
}