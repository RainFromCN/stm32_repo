#include "stm32f4xx_hal.h"


/************* FSMC *************/

/** Bank1 NOR/PSRAM4
  *
  *��1��Ƭѡ            NE4
  *��2���洢������      SRAM
  *��3����ַλ��        1bit
  *��4������λ��        16bits
  *��5��NWE            WRдʹ��
  *��6��NOE            RD��ʹ��
  *��7��A0             RSдָ��/����ѡ��
  *��8��D0-D15         DB0-DB15
  *
  */

/************* GPIO *************/

/** bl��LCD��ʾ�������ź�
  *
  *��1��ģʽ            �������
  *��2��������          ��������
  *��3���ٶ�            �ǳ���
  *��4��Ĭ�ϵ�ƽ        �ߵ�ƽ
  *
  */


#define FSMC_SRAM1_BASE                0X60000000
#define FSMC_SRAM2_BASE                0X64000000
#define FSMC_SRAM3_BASE                0X68000000
#define FSMC_SRAM4_BASE                0X6C000000


//ILI9488��ʼ��
uint32_t    USER_ILI9488_GetID(); //��ȡ�豸ID
void        USER_ILI9488_Open(SRAM_HandleTypeDef* hsram, char* bl); //��ʼ��


//ILI9488��д
void        USER_ILI9488_WriteCtrl(uint16_t cmd); //дָ��
void        USER_ILI9488_WriteData(uint16_t dat); //д����
uint16_t    USER_ILI9488_Read();                  //������