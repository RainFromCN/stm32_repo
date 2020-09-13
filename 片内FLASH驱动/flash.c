#include "flash.h"

void USER_FLASH_WriteByteArray(unsigned char* byte, unsigned int len) {
	HAL_FLASH_Unlock();								//����
	FLASH_Erase_Sector(USER_Sector_Number, FLASH_VOLTAGE_RANGE_1);				//����������ʹ����͵�ѹ
	uint64_t tmp;									
	for (int i = 0; i < len; i ++) {
		tmp = byte[i];								//������������ת���� uint64_t
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, USER_Sector_Address + i, tmp);		//��������
	}
	HAL_FLASH_Lock();									//����
}

void USER_FLASH_WriteWordArray(unsigned int* word, unsigned int len) {
	HAL_FLASH_Unlock();								//����
	FLASH_Erase_Sector(USER_Sector_Number, FLASH_VOLTAGE_RANGE_1);				//��������
	uint64_t tmp;									
	for (int i = 0; i < len; i ++) {				
		tmp = word[i];								//������������ת���� uint64_t
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, USER_Sector_Address + i * 4, tmp);	//��������
	}
	HAL_FLASH_Lock();									//����
}

uint32_t* USER_FLASH_ArrayHead(void) {
	return (uint32_t*) USER_Sector_Address; 							//���� flash ������ַ
}