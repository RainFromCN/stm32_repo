#include "stm32f4xx_hal.h"

/*************  TIM *************/

/** motor_tim：用于驱动电机的高级定时器
  *
  *（1）时钟源          内部时钟
  *（2）通道            互补PWM
  *（3）PSC             0
  *（4）ARR             8399
  *（5）OSSR            使能
  *（6）极性            low
  *
  */

/** encoder_tim：用于对接编码器
  *
  *（1）混合通道        编码器
  *（2）通道            通道一互补PWM
  *（3）PSC             0
  *（4）ARR             0xffff
  *（5）编码器模式      TI1 and TI2
  *（6）极性            both edge
  *
  */

/************* 其他 *************/

/** channel:选择哪一个通道作为互补PWM输出
  *
  */

/** PPR:即 Pulse Per Revolution，编码器物理参数
  *
  */

/** expect:PID的设定值
  *
  */

/** k1,k2,k3:PID的三个参数
  *
  */

#pragma pack(1)
typedef struct {
    int duty;
    int period;
} PWM_TypeDef;

typedef struct {
    double expect_speed;
    double cur_err;
    double last_err;
    double prev_err;
    double A;
    double B;
    double C;
} SPDPID_TypeDef;

typedef struct {
    int expect_loc;
    int sum_err;
    int cur_err;
    int last_err;
    double kp;
    double ki;
    double kd;
} LOCPID_TypeDef;

typedef struct {
    int beg;
    int end;
    int overflow;
    int PPR;
} Encoder_TypeDef;

typedef struct {
    PWM_TypeDef PWM;
    SPDPID_TypeDef speed_PID;
    LOCPID_TypeDef loc_PID;
    Encoder_TypeDef Encoder;

    double physical_speed;
    double physical_dir;
    int    physical_loc;
} Moter_TypeDef;
#pragma pack()


//用于进行误差限幅
#define MOTOR_INF               1e6         //定义误差上限，不用更改
#define MOTOR_SPEED_EPS         0.5         //定义速度误差下限，单位是RPM，可以更改
#define MOTOR_LOC_EPS           10          //定义编码器位置误差下限，可以更改


//初始化
void    USER_MOTOR_Open(TIM_HandleTypeDef* motor_tim, uint32_t channel);    //初始化电机
void    USER_MOTOR_UseEncoder(int PPR, TIM_HandleTypeDef* encoder_tim);     //使能编码器
void    USER_MOTOR_UseSpeedLoop();                                          //使能速度环
void    USER_MOTOR_UseLocLoop();                                            //使能位置环

//设置PID
void    USER_PID_SetExpect(double expect);                          //设置PID期望值
void    USER_PID_SetParams(double k1, double k2, double k3);        //设置PID参数

//内部调用
void    USER_MOTOR_PeriodUpdate();                                  //供编码器使用
void    USER_MOTOR_EncoderUpdate(TIM_HandleTypeDef* htim);          //编码器溢出
int     USER_MOTOR_PID();                                           //PID计算函数
