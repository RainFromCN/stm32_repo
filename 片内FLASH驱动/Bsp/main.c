#include "string.h"
#include "flash.h"

void SystemClock_Config(void);

uint8_t table[10];					// flash ����

int main(void)
{
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_USART1_UART_Init();
	
	for (int i = 0; i < 10; i ++) table[i] = '0' + i;		//�� flash ����д��
	
	USER_FLASH_WriteByteArray(table, 10);		//�� flash ����д�� flash
	
	memset(table, 0, sizeof(table) );			//���� flash ����

	while (1) {
		uint8_t* p = (uint8_t*)USER_FLASH_ArrayHead();	//��ȡ flash ��ַͷ
		for (int i = 0; i < 10; i ++) printf("%c ", p[i])��	//�ӵ�ַͷ��ʼ�����ݲ���ӡ
		printf("\r\n\n");				
		HAL_Delay(1000);
	}
}