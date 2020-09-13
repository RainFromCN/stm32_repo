#include "stm32f4xx_hal.h"


/************* FSMC *************/

/** Bank1 NOR/PSRAM4
  *
  *（1）片选            NE4
  *（2）存储器类型      SRAM
  *（3）地址位数        1bit
  *（4）数据位数        16bits
  *（5）NWE            WR写使能
  *（6）NOE            RD读使能
  *（7）A0             RS写指令/数据选择
  *（8）D0-D15         DB0-DB15
  *
  */

/************* GPIO *************/

/** bl：LCD显示屏背光信号
  *
  *（1）模式            推挽输出
  *（2）上下拉          不上下拉
  *（3）速度            非常高
  *（4）默认电平        高电平
  *
  */


#define FSMC_SRAM1_BASE                0X60000000
#define FSMC_SRAM2_BASE                0X64000000
#define FSMC_SRAM3_BASE                0X68000000
#define FSMC_SRAM4_BASE                0X6C000000


//ILI9488初始化
uint32_t    USER_ILI9488_GetID(); //获取设备ID
void        USER_ILI9488_Open(SRAM_HandleTypeDef* hsram, char* bl); //初始化


//ILI9488读写
void        USER_ILI9488_WriteCtrl(uint16_t cmd); //写指令
void        USER_ILI9488_WriteData(uint16_t dat); //写数据
uint16_t    USER_ILI9488_Read();                  //读数据