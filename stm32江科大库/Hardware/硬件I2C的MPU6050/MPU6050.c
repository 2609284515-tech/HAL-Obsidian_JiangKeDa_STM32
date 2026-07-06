#include "main.h"
#include "MPU6050_Reg.h"
#include "stm32f1xx_hal_i2c.h"

// 声明外部定义的 I2C 句柄，通常在 main.c 或 i2c.c 中由 CubeMX 生成
extern I2C_HandleTypeDef hi2c2;

// MPU6050 I2C 地址 (7位地址为0x68, HAL库函数通常自动处理读写位，但此处宏定义为0xD0需注意HAL库行为)
// 注意：HAL_I2C_Mem_Write/Read 函数的第二个参数是 DeviceAddress。
// 如果使用的是标准7位地址，应传入 0x68。如果传入 0xD0，HAL库可能会再次左移，导致地址错误。
// 请根据实际 HAL 库版本和配置确认地址格式。通常建议定义为 0x68 << 1 或仅 0x68 并让 HAL 处理。
// 在此上下文中，假设用户已验证 0xD0 有效（某些旧版本或特定配置下可能需要完整8位地址，但标准HAL通常期望7位）。
#define MPU6050_ADDRESS		0xD0 
#define I2C_TIMEOUT         100 // I2C 操作超时时间 (ms)

/**
  * @brief  向 MPU6050 指定寄存器写入数据
  * @param  RegAddress: 寄存器地址
  * @param  Data: 要写入的数据
  * @retval HAL_StatusTypeDef: HAL_OK 表示成功，其他表示错误
  */
HAL_StatusTypeDef MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    // 使用 HAL 库内存写入函数
    // &hi2c2: I2C 句柄
    // MPU6050_ADDRESS: 设备地址
    // RegAddress: 内存地址(寄存器地址)
    // I2C_MEMADD_SIZE_8BIT: 内存地址宽度为8位
    // &Data: 数据指针
    // 1: 数据长度
    // I2C_TIMEOUT: 超时时间
    return HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDRESS, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, I2C_TIMEOUT);
}

/**
  * @brief  从 MPU6050 指定寄存器读取数据
  * @param  RegAddress: 寄存器地址
  * @retval 读取到的数据
  */
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0;
    // 使用 HAL 库内存读取函数
    HAL_I2C_Mem_Read(&hi2c2, MPU6050_ADDRESS, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, I2C_TIMEOUT);
    return Data;
}

/**
  * @brief  初始化 MPU6050
  * @note   配置电源管理、采样率、滤波及量程
  * @retval 无
  */
void MPU6050_Init(void)
{
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		// 唤醒 MPU6050，选择内部 8MHz 晶振作为时钟源
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);		// 使能所有轴 (加速度计和陀螺仪)
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		// 设置采样率分频系数为 9，采样率 = 1kHz / (1+9) = 100Hz
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			// 设置数字低通滤波器 (DLPF)，带宽约为 5Hz
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	// 设置陀螺仪量程为 ±2000 dps
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	// 设置加速度计量程为 ±16g
}

/**
  * @brief  获取 MPU6050 的设备 ID
  * @retval 设备 ID (正常应为 0x68)
  */
uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/**
  * @brief  获取加速度计和陀螺仪的原始数据
  * @param  AccX, AccY, AccZ: 加速度计 X/Y/Z 轴数据指针
  * @param  GyroX, GyroY, GyroZ: 陀螺仪 X/Y/Z 轴数据指针
  * @retval 无
  * @note   数据为有符号 16 位整数，高位在前
  */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
					int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	// 读取加速度计 X 轴数据
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;		// 合并高低字节
	
	// 读取加速度计 Y 轴数据
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;
	
	// 读取加速度计 Z 轴数据
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	// 读取陀螺仪 X 轴数据
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	// 读取陀螺仪 Y 轴数据
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	// 读取陀螺仪 Z 轴数据
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}