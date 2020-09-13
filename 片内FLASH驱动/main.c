#include "string.h"
#include "flash.h"

void SystemClock_Config(void);

uint8_t table[10];					// flash 缓冲

int main(void)
{
	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_USART1_UART_Init();
	
	for (int i = 0; i < 10; i ++) table[i] = '0' + i;		//向 flash 缓冲写入
	
	USER_FLASH_WriteByteArray(table, 10);		//将 flash 缓冲写入 flash
	
	memset(table, 0, sizeof(table) );			//重置 flash 缓冲

	while (1) {
		uint8_t* p = (uint8_t*)USER_FLASH_ArrayHead();	//获取 flash 地址头
		for (int i = 0; i < 10; i ++) printf("%c ", p[i])；	//从地址头开始读数据并打印
		printf("\r\n\n");				
		HAL_Delay(1000);
	}
}