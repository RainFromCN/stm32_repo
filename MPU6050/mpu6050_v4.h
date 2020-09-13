#include "stm32f4xx_hal.h"


/*************  IIC *************/

/** iic：使用IIC与MPU进行通信
*
*（1）模式            标准模式
*（2）地址长度        7-bit
*（3）设备地址        0x00
*（4）方式            字节对字节
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
    float roll;  //横滚角
    float pitch; //俯仰角
    float yaw;   //偏航角
} Angle_TypeDef;

#pragma pack()


//寄存器
#define MPU6050_SMPLRT_DIV                  0x19 //陀螺仪采样率，典型值：0x07(125Hz)
#define MPU6050_CONFIG                      0x1A //低通滤波频率，典型值：0x06(5Hz)
#define MPU6050_GYRO_CONFIG                 0x1B //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define MPU6050_ACCEL_CONFIG                0x1C //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
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
#define MPU6050_PWR_MGMT_1                  0x6B //电源管理，典型值：0x00(正常启用)
#define MPU6050_WHO_AM_I                    0x75 //IIC地址(默认0x68,只读)
#define MPU6050_SLAVEADDR                   0xD0 //IIC写入时的地址字节数据，+1为读取


/* 用户调用 */
uint8_t     MPU6050_Open(I2C_HandleTypeDef* iic);       //初始化
void        MPU6050_SetSamplePeriod(uint8_t period);    //设置采样时间，典型值5(ms)
void        MPU6050_Update();                           //滴答更新
void        MPU6050_Callback();                         //轮询更新
float       MPU6050_GetPitch();                         //返回俯仰角
float       MPU6050_GetRoll();                          //返回横滚角
float       MPU6050_GetYaw();                           //返回偏航角


/* 内部调用 */
float       MPU6050_GetAccelX();
float       MPU6050_GetAccelY();
float       MPU6050_GetAccelZ();
short       MPU6050_GetGyroX();
short       MPU6050_GetGyroY();
short       MPU6050_GetGyroZ();