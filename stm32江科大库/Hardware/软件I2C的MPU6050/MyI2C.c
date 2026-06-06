#include "main.h"
#include "Delay.h"

// 定义 I2C 使用的 GPIO 端口和引脚
// 注意：需要在 main.h 或相关头文件中定义 SCL_Pin 和 SDA_Pin
#define SCL_PORT		GPIOB
#define SDA_PORT		GPIOB

// 延时时间，单位微秒(us)，用于满足 I2C 时序要求
// 根据 CPU 主频调整，通常标准模式需要 >= 4.7us (高电平) 和 >= 4.0us (低电平)
#define DTIME			10

/**
  * @brief  写 SCL 引脚电平
  * @param  BitValue: 1为高电平，0为低电平
  * @retval 无
  */
void MyI2C_W_SCL(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SCL_PORT, SCL_Pin, (GPIO_PinState)BitValue);
	Delay_us(DTIME); // 保持电平稳定一段时间
}

/**
  * @brief  写 SDA 引脚电平
  * @param  BitValue: 1为高电平，0为低电平
  * @retval 无
  */
void MyI2C_W_SDA(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SDA_PORT, SDA_Pin, (GPIO_PinState)BitValue);
	Delay_us(DTIME); // 保持电平稳定一段时间
}

/**
  * @brief  读 SDA 引脚电平
  * @retval 引脚当前电平状态 (0 或 1)
  */
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin(SDA_PORT, SDA_Pin);
	Delay_us(DTIME); // 读取后稍作延时，确保时序间隔
	return BitValue;
}

/**
  * @brief  读 SCL 引脚电平 (本驱动中主要用于调试或特定时序，常规发送/接收主要依赖写SCL)
  * @retval 引脚当前电平状态 (0 或 1)
  */
uint8_t MyI2C_R_SCL(void)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin(SCL_PORT, SCL_Pin);
	Delay_us(DTIME);
	return BitValue;
}

/**
  * @brief  产生 I2C 起始信号 (Start Condition)
  * @note   SCL 高电平时，SDA 由高变低
  * @retval 无
  */
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1); // SDA 拉高
	MyI2C_W_SCL(1); // SCL 拉高
	MyI2C_W_SDA(0); // SDA 拉低 (产生下降沿，即起始信号)
	MyI2C_W_SCL(0); // SCL 拉低，准备传输数据
}

/**
  * @brief  产生 I2C 停止信号 (Stop Condition)
  * @note   SCL 高电平时，SDA 由低变高
  * @retval 无
  */
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0); // SDA 拉低
	MyI2C_W_SCL(1); // SCL 拉高
	MyI2C_W_SDA(1); // SDA 拉高 (产生上升沿，即停止信号)
}

/**
  * @brief  通过 I2C 发送一个字节数据
  * @param  Byte: 要发送的 8 位数据
  * @retval 无
  * @note   高位先发 (MSB First)
  */
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		// 判断第 i 位 (从最高位开始) 是 0 还是 1
		// 0x80 >> i: 依次生成 10000000, 01000000, ..., 00000001
		MyI2C_W_SDA(Byte & (0x80 >> i)); 
		MyI2C_W_SCL(1); // SCL 拉高，从机读取 SDA 数据
		MyI2C_W_SCL(0); // SCL 拉低，允许 SDA 变化以准备下一位
	}
}

/**
  * @brief  通过 I2C 接收一个字节数据
  * @retval 接收到的 8 位数据
  * @note   高位先收 (MSB First)
  */
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1); // 释放 SDA 线，设置为输入模式 (内部上拉或外部上拉生效)
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1); // SCL 拉高，主机读取 SDA 数据
		if (MyI2C_R_SDA() == 1)
		{
			Byte |= (0x80 >> i); // 如果 SDA 为高，则将对应位置 1
		}
		MyI2C_W_SCL(0); // SCL 拉低，准备读取下一位
	}
	return Byte;
}

/**
  * @brief  发送应答信号 (ACK) 或非应答信号 (NACK)
  * @param  AckBit: 0 表示应答 (ACK), 1 表示非应答 (NACK)
  * @retval 无
  */
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit); // 设置 SDA 电平
	MyI2C_W_SCL(1);      // SCL 拉高，从机读取应答位
	MyI2C_W_SCL(0);      // SCL 拉低，结束应答位传输
}

/**
  * @brief  接收应答信号 (ACK/NACK)
  * @retval 0 表示收到应答 (ACK), 1 表示收到非应答 (NACK)
  */
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1); // 释放 SDA 线，准备接收从机的应答
	MyI2C_W_SCL(1); // SCL 拉高，读取 SDA 状态
	AckBit = MyI2C_R_SDA(); // 读取应答位
	MyI2C_W_SCL(0); // SCL 拉低，结束应答位传输
	return AckBit;
}