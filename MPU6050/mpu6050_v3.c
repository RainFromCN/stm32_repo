#include "mpu6050_v3.h"
#include "math.h"


I2C_HandleTypeDef* MPU6050_IIC;

static uint32_t tick = 0;

static Angle_TypeDef Angle; //��̬��
static Accel_TypeDef Accel; //������ٶ�
static Gyro_TypeDef  Gyro;  //������ٶ�

static uint8_t Sample_period; //��������


/* ���ò������� */
void MPU6050_SetSamplePeriod(uint8_t period) {
    Sample_period = period;
}

/* ��ʼ�� */
uint8_t MPU6050_Open(I2C_HandleTypeDef* iic) {

    MPU6050_IIC = iic;

    uint8_t tmp = 0x00;

    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 0xffffffff);
    if (tmp != 0x68) return 0;

    tmp = 0x00;
    HAL_I2C_Mem_Write(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 0xffffffff);
    tmp = 0x07;
    HAL_I2C_Mem_Write(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_SMPLRT_DIV, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 0xffffffff);
    tmp = 0x06;
    HAL_I2C_Mem_Write(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_CONFIG, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 0xffffffff);
    tmp = 0x18;
    HAL_I2C_Mem_Write(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_GYRO_CONFIG, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 0xffffffff);
    tmp = 0x01;
    HAL_I2C_Mem_Write(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_ACCEL_CONFIG, I2C_MEMADD_SIZE_8BIT, &tmp, 1, 0xffffffff);

    return 1;
}

float MPU6050_GetAccelX() {
    uint8_t H, L;
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, &H, 1, 0xffffffff);
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_ACCEL_XOUT_L, I2C_MEMADD_SIZE_8BIT, &L, 1, 0xffffffff);

    return (short)((H << 8) + L) / 16384.0;
}

float MPU6050_GetAccelY() {
    uint8_t H, L;
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_ACCEL_YOUT_H, I2C_MEMADD_SIZE_8BIT, &H, 1, 0xffffffff);
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_ACCEL_YOUT_L, I2C_MEMADD_SIZE_8BIT, &L, 1, 0xffffffff);

    return (short)((H << 8) + L) / 16384.0;
}

float MPU6050_GetAccelZ() {
    uint8_t H, L;
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_ACCEL_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &H, 1, 0xffffffff);
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_ACCEL_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &L, 1, 0xffffffff);

    return (short)((H << 8) + L) / 16384.0;
}

short MPU6050_GetGyroX() {
    uint8_t H, L;
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_GYRO_XOUT_H, I2C_MEMADD_SIZE_8BIT, &H, 1, 0xffffffff);
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_GYRO_XOUT_L, I2C_MEMADD_SIZE_8BIT, &L, 1, 0xffffffff);

    return (short)((H << 8) + L);
}

short MPU6050_GetGyroY() {
    uint8_t H, L;
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_GYRO_YOUT_H, I2C_MEMADD_SIZE_8BIT, &H, 1, 0xffffffff);
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_GYRO_YOUT_L, I2C_MEMADD_SIZE_8BIT, &L, 1, 0xffffffff);

    return (short)((H << 8) + L);
}

short MPU6050_GetGyroZ() {
    uint8_t H, L;
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_GYRO_ZOUT_H, I2C_MEMADD_SIZE_8BIT, &H, 1, 0xffffffff);
    HAL_I2C_Mem_Read(MPU6050_IIC, MPU6050_SLAVEADDR, MPU6050_GYRO_ZOUT_L, I2C_MEMADD_SIZE_8BIT, &L, 1, 0xffffffff);

    return (short)((H << 8) + L);
}

...�û��޸ģ�1��//�δ���£�����1ms��ʱ���ж���
void MPU6050_Update() {

    tick += 1;
    if (tick >= Sample_period) tick = 0;
    else return;

    /* ���������ٶ� */
    Accel.x = MPU6050_GetAccelX();
    Accel.y = MPU6050_GetAccelY();
    Accel.z = MPU6050_GetAccelZ();

    /* ���������ٶ� */
    Gyro.x = MPU6050_GetGyroX();
    Gyro.y = MPU6050_GetGyroY();
    Gyro.z = MPU6050_GetGyroZ();

    /* ��̬�ں�--�����˲� */
    Angle.pitch = 57.3 * atan2(Accel.y, Accel.z) + 0.003 * Sample_period * Gyro.x;
    Angle.roll = 57.3 * atan2(Accel.x, Accel.z) + 0.003 * Sample_period * Gyro.y;
}



float MPU6050_GetPitch() {
    return Angle.pitch;
}

float MPU6050_GetRoll() {
    return Angle.roll;
}

float MPU6050_GetYaw() {
    return Angle.yaw;
}
