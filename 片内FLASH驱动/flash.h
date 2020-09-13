#include "stm32f4xx_hal.h"

#define USER_Sector_Number 				FLASH_SECTOR_11    // flash ����
#define USER_Sector_Address				0x080E0000	// flash ������ַ

void USER_FLASH_WriteByteArray(unsigned char* byte, unsigned int len);		//д�ֽ�������
void USER_FLASH_WriteWordArray(unsigned int* word, unsigned int len);		//д��������
uint32_t* USER_FLASH_ArrayHead(void);					//���� flash ͷ