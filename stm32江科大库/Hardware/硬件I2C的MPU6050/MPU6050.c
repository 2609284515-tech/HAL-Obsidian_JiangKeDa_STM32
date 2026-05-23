#include "main.h"
#include "MPU6050_Reg.h"
#include "stm32f1xx_hal_i2c.h"

extern I2C_HandleTypeDef hi2c2;

#define MPU6050_ADDRESS		0xD0
#define I2C_TIMEOUT         100

HAL_StatusTypeDef MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    return HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDRESS, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, I2C_TIMEOUT);
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0;
    HAL_I2C_Mem_Read(&hi2c2, MPU6050_ADDRESS, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, I2C_TIMEOUT);
    return Data;
}

void MPU6050_Init(void)
{
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//开陀螺仪x时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		//10分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			//最高滤波
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//最大量程
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	//最大量程
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
					int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}
