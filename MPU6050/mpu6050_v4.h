#include "stm32f4xx_hal.h"


/*************  IIC *************/

/** iic��ʹ��IIC��MPU����ͨ��
*
*��1��ģʽ            ��׼ģʽ
*��2����ַ����        7-bit
*��3���豸��ַ        0x00
*��4����ʽ            �ֽڶ��ֽ�
*
*/


#pragma pack(4)

typedef struct {
    short x;
    short y;
    short z;
} Gyro_TypeDef;

typedef struct {
    float x;
    float y;
    float z;
} Accel_TypeDef;

typedef struct {
    float roll;  //�����
    float pitch; //������
    float yaw;   //ƫ����
} Angle_TypeDef;

#pragma pack()


//�Ĵ���
#define MPU6050_SMPLRT_DIV                  0x19 //�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define MPU6050_CONFIG                      0x1A //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define MPU6050_GYRO_CONFIG                 0x1B //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define MPU6050_ACCEL_CONFIG                0x1C //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define MPU6050_ACCEL_XOUT_H                0x3B
#define MPU6050_ACCEL_XOUT_L                0x3C
#define MPU6050_ACCEL_YOUT_H                0X3D
#define MPU6050_ACCEL_YOUT_L                0x3E
#define MPU6050_ACCEL_ZOUT_H                0x3F
#define MPU6050_ACCEL_ZOUT_L                0x40
#define MPU6050_TEMP_OUT_H                  0x41
#define MPU6050_TEMP_OUT_L                  0x42
#define MPU6050_GYRO_XOUT_H	                0x43
#define MPU6050_GYRO_XOUT_L                 0x44
#define MPU6050_GYRO_YOUT_H	                0X45
#define MPU6050_GYRO_YOUT_L                 0x46
#define MPU6050_GYRO_ZOUT_H	                0x47
#define MPU6050_GYRO_ZOUT_L	                0x48
#define MPU6050_PWR_MGMT_1                  0x6B //��Դ��������ֵ��0x00(��������)
#define MPU6050_WHO_AM_I                    0x75 //IIC��ַ(Ĭ��0x68,ֻ��)
#define MPU6050_SLAVEADDR                   0xD0 //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ


/* �û����� */
uint8_t     MPU6050_Open(I2C_HandleTypeDef* iic);       //��ʼ��
void        MPU6050_SetSamplePeriod(uint8_t period);    //���ò���ʱ�䣬����ֵ5(ms)
void        MPU6050_Update();                           //�δ����
void        MPU6050_Callback();                         //��ѯ����
float       MPU6050_GetPitch();                         //���ظ�����
float       MPU6050_GetRoll();                          //���غ����
float       MPU6050_GetYaw();                           //����ƫ����


/* �ڲ����� */
float       MPU6050_GetAccelX();
float       MPU6050_GetAccelY();
float       MPU6050_GetAccelZ();
short       MPU6050_GetGyroX();
short       MPU6050_GetGyroY();
short       MPU6050_GetGyroZ();