#include "main.h"
#include "MyI2C.h"
#include "MPU6050.h"
#include "MPU6050_Reg.h"
#include "OLED.h"
#include <math.h>

#define MPU6050_ADDRESS		0xD0

float AngleGyroX;
float AngleX;

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	return Data;
}

void MPU6050_Init(void)
{
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//开陀螺仪x时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x07);		//10分频
	MPU6050_WriteReg(MPU6050_CONFIG, 0x00);			//最高滤波
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


void MPU6050_Angle(void)
{
	int16_t AccX, AccY, AccZ, 
		GyroX, GyroY, GyroZ;

	float AngleY, AngleZ;
	

	MPU6050_GetData(&AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);
	AngleX = atan2(AccY, AccZ) / 3.14159 * 180 - 1;

	GyroX += 32;

	float Alpha = 0.99;
	AngleGyroX = AngleGyroX * Alpha + AngleX * (1 - Alpha) + GyroX / 32768.0 * 2000 * 0.01;

	OLED_ShowFloatNum(0, 49, AngleX, 3, 0, OLED_6X8);
	OLED_ShowFloatNum(25, 49, AngleGyroX, 3, 0, OLED_6X8);
	OLED_ShowSignedNum(57, 49, GyroX, 3, OLED_6X8);
}

void MPU6050_Test(void)
{
	uint8_t ID = MPU6050_GetID();
	
	OLED_ShowHexNum(1, 1, ID, 2, OLED_6X8);

	int16_t AX, AY, AZ, GX, GY, GZ;
	
	MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
	OLED_ShowSignedNum(2, 1, AX, 5, OLED_6X8);
	OLED_ShowSignedNum(3, 1, AY, 5, OLED_6X8);
	OLED_ShowSignedNum(4, 1, AZ, 5, OLED_6X8);
	OLED_ShowSignedNum(2, 8, GX, 5, OLED_6X8);
	OLED_ShowSignedNum(3, 8, GY, 5, OLED_6X8);
	OLED_ShowSignedNum(4, 8, GZ, 5, OLED_6X8);
}