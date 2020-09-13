#include "stm32f4xx_hal.h"

/*************  TIM *************/

/** motor_tim��������������ĸ߼���ʱ��
  *
  *��1��ʱ��Դ          �ڲ�ʱ��
  *��2��ͨ��            ����PWM
  *��3��PSC             0
  *��4��ARR             8399
  *��5��OSSR            ʹ��
  *��6������            low
  *
  */

/** encoder_tim�����ڶԽӱ�����
  *
  *��1�����ͨ��        ������
  *��2��ͨ��            ͨ��һ����PWM
  *��3��PSC             0
  *��4��ARR             0xffff
  *��5��������ģʽ      TI1 and TI2
  *��6������            both edge
  *
  */

/************* ���� *************/

/** channel:ѡ����һ��ͨ����Ϊ����PWM���
  *
  */

/** PPR:�� Pulse Per Revolution���������������
  *
  */

/** expect:PID���趨ֵ
  *
  */

/** k1,k2,k3:PID����������
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


//���ڽ�������޷�
#define MOTOR_INF               1e6         //����������ޣ����ø���
#define MOTOR_SPEED_EPS         0.5         //�����ٶ�������ޣ���λ��RPM�����Ը���
#define MOTOR_LOC_EPS           10          //���������λ��������ޣ����Ը���


//��ʼ��
void    USER_MOTOR_Open(TIM_HandleTypeDef* motor_tim, uint32_t channel);    //��ʼ�����
void    USER_MOTOR_UseEncoder(int PPR, TIM_HandleTypeDef* encoder_tim);     //ʹ�ܱ�����
void    USER_MOTOR_UseSpeedLoop();                                          //ʹ���ٶȻ�
void    USER_MOTOR_UseLocLoop();                                            //ʹ��λ�û�

//����PID
void    USER_PID_SetExpect(double expect);                          //����PID����ֵ
void    USER_PID_SetParams(double k1, double k2, double k3);        //����PID����

//�ڲ�����
void    USER_MOTOR_PeriodUpdate();                                  //��������ʹ��
void    USER_MOTOR_EncoderUpdate(TIM_HandleTypeDef* htim);          //���������
int     USER_MOTOR_PID();                                           //PID���㺯��
