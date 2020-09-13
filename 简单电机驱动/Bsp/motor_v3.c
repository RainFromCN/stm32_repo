#include "motor_v3.h"
#include "string.h"


uint32_t                MOTOR_CHANNEL;
TIM_HandleTypeDef*      MOTOR_TIM;
TIM_HandleTypeDef*      Encoder_TIM;
extern Moter_TypeDef    MOTOR;


int MOTOR_UseEncoder   = 0;
int MOTOR_UseSpeedLoop = 0;
int MOTOR_UseLocLoop   = 0;


void USER_MOTOR_Open(TIM_HandleTypeDef* htim, uint32_t channel) {
    memset(&MOTOR, 0, sizeof(Moter_TypeDef) );

    MOTOR_TIM = htim;
    MOTOR_CHANNEL = channel;
    MOTOR.PWM.period = (*MOTOR_TIM).Init.Period;

    __HAL_TIM_SET_COMPARE(MOTOR_TIM, MOTOR_CHANNEL, 0);
    HAL_TIM_PWM_Start(MOTOR_TIM, MOTOR_CHANNEL);
}

//ʹ�ܱ�����
void USER_MOTOR_UseEncoder(int PPR, TIM_HandleTypeDef* htim) {
    MOTOR.Encoder.PPR      = PPR;
    MOTOR_UseEncoder       = 1;
    Encoder_TIM = htim;
}

//ʹ���ٶȱջ�
void USER_MOTOR_UseSpeedLoop() {
    MOTOR_UseSpeedLoop = 1;
}

//ʹ��λ�ñջ�
void USER_MOTOR_UseLocLoop() {
    MOTOR_UseLocLoop = 1;
}

//����PID����ֵ
void USER_PID_SetExpect(double expect) {
    if (MOTOR_UseSpeedLoop) MOTOR.speed_PID.expect_speed = expect;
    else if (MOTOR_UseLocLoop) MOTOR.loc_PID.expect_loc = (int)expect;
}

//����PID����
void USER_PID_SetParams(double k1, double k2, double k3) {
    if (MOTOR_UseLocLoop) {
        MOTOR.loc_PID.kp = k1;
        MOTOR.loc_PID.ki = k2;
        MOTOR.loc_PID.kd = k3;
    }
    else if (MOTOR_UseSpeedLoop) {
        MOTOR.speed_PID.A = k1;
        MOTOR.speed_PID.B = k2;
        MOTOR.speed_PID.C = k3;
    }
}

//���ڵδ�ʱ����ʮ��������һ��
void USER_MOTOR_PeriodUpdate() {

    //����ʵ���ٶ�
    MOTOR.Encoder.end     = __HAL_TIM_GET_COUNTER(Encoder_TIM);
    MOTOR.physical_speed  = MOTOR.Encoder.end - MOTOR.Encoder.beg + MOTOR.Encoder.overflow * 65536;
    MOTOR.physical_speed /= MOTOR.Encoder.PPR;
    MOTOR.physical_speed *= 6000;

    //����ʵ��λ��
    MOTOR.physical_loc    = MOTOR.Encoder.end - MOTOR.Encoder.beg + MOTOR.Encoder.overflow * 65536;

    //����ʵ�ʷ���
    MOTOR.physical_dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(Encoder_TIM);

    //���ñ�����
    memset(&MOTOR.Encoder, 0, sizeof(Encoder_TypeDef) );
    MOTOR.Encoder.beg = __HAL_TIM_GET_COUNTER(Encoder_TIM);

    //����PID
    if (MOTOR_UseSpeedLoop) {
        MOTOR.PWM.duty += USER_MOTOR_PID();

        //����޷�
        if (MOTOR.PWM.duty > MOTOR.PWM.period) MOTOR.PWM.duty = MOTOR.PWM.period;
        if (MOTOR.PWM.duty < 0) MOTOR.PWM.duty = 0;

        __HAL_TIM_SET_COMPARE(MOTOR_TIM, MOTOR_CHANNEL, MOTOR.PWM.duty);
    }
    else if (MOTOR_UseLocLoop) {
        MOTOR.PWM.duty = USER_MOTOR_PID();

        //����޷�
        if (MOTOR.PWM.duty > MOTOR.PWM.period) MOTOR.PWM.duty = MOTOR.PWM.period;
        if (MOTOR.PWM.duty < 0) MOTOR.PWM.duty = 0;

        __HAL_TIM_SET_COMPARE(MOTOR_TIM, MOTOR_CHANNEL, MOTOR.PWM.duty);
    }
}

//���ʹ�ñ���������ô���˺������ڶ�ʱ�����»ص�������
void USER_MOTOR_EncoderUpdate(TIM_HandleTypeDef* htim) {
    if (htim != Encoder_TIM) return ;

    //������TIM���
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(Encoder_TIM) ) 
        MOTOR.Encoder.overflow -= 1;
    else MOTOR.Encoder.overflow += 1;
}

int USER_MOTOR_PID() {

    //�ٶȻ�������ʽPID
    if (MOTOR_UseSpeedLoop) {

        //���㵱ǰ���
        MOTOR.speed_PID.cur_err = MOTOR.speed_PID.expect_speed - MOTOR.physical_speed;
        
        //����޷�
        if (MOTOR.speed_PID.cur_err > MOTOR_INF || MOTOR.speed_PID.cur_err < -MOTOR_INF)
            MOTOR.speed_PID.cur_err = 0;
        if (MOTOR.speed_PID.cur_err < MOTOR_SPEED_EPS && MOTOR.speed_PID.cur_err > -MOTOR_SPEED_EPS)
            MOTOR.speed_PID.cur_err = 0;

        //��������
        double dout = MOTOR.speed_PID.A * MOTOR.speed_PID.cur_err - MOTOR.speed_PID.B * MOTOR.speed_PID.last_err + MOTOR.speed_PID.C * MOTOR.speed_PID.prev_err;

        return (int)dout;
    }

    //λ�û���λ��ʽPID
    else if (MOTOR_UseLocLoop) {

        //���㵱ǰ���
        MOTOR.loc_PID.cur_err = MOTOR.loc_PID.expect_loc - MOTOR.physical_loc;

        //����޷�
        if (MOTOR.loc_PID.cur_err > MOTOR_INF || MOTOR.loc_PID.cur_err < -MOTOR_INF)
            MOTOR.loc_PID.cur_err = 0;
        if (MOTOR.loc_PID.cur_err < MOTOR_LOC_EPS && MOTOR.loc_PID.cur_err > -MOTOR_LOC_EPS)
            MOTOR.loc_PID.cur_err = 0;

        //�����ۼ����
        MOTOR.loc_PID.sum_err += MOTOR.loc_PID.cur_err;

        //���������
        double out = MOTOR.loc_PID.cur_err * MOTOR.loc_PID.kp + MOTOR.loc_PID.sum_err * MOTOR.loc_PID.ki + (MOTOR.loc_PID.cur_err - MOTOR.loc_PID.last_err) * MOTOR.loc_PID.kd;
    
        return (int)out;
    }
    
    return 0;
}

