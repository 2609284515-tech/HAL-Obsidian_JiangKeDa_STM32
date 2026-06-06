#include "main.h"

// 声明外部定义的 SPI 句柄，通常在 main.c 或 spi.c 中由 CubeMX 生成
extern SPI_HandleTypeDef hspi1;

/**
  * @brief  写片选信号 (SS/CS)
  * @param  BitValue: 0 为选中 (低电平有效), 1 为取消选中
  * @retval 无
  * @note   即使使用硬件 SPI，片选信号通常也由软件手动控制，以确保时序灵活性
  */
void MySPI_W_SS(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, SPI1_SS_Pin, (GPIO_PinState)BitValue);
}

/**
  * @brief  写时钟信号 (SCK) - 软件模拟用
  * @param  BitValue: 0 为低电平, 1 为高电平
  * @retval 无
  * @note   在使用 HAL_SPI_TransmitReceive 时，此函数不会被调用，SCK 由硬件外设自动控制
  */
void MySPI_W_SCK(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, SPI1_SCK_Pin, (GPIO_PinState)BitValue);
}

/**
  * @brief  写主机输出从机输入信号 (MOSI) - 软件模拟用
  * @param  BitValue: 0 为低电平, 1 为高电平
  * @retval 无
  * @note   在使用 HAL_SPI_TransmitReceive 时，此函数不会被调用，MOSI 由硬件外设自动控制
  */
void MySPI_W_MOSI(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, SPI1_MOSI_Pin, (GPIO_PinState)BitValue);
}

/**
  * @brief  读主机输入从机输出信号 (MISO) - 软件模拟用
  * @retval 引脚当前电平状态 (0 或 1)
  * @note   在使用 HAL_SPI_TransmitReceive 时，此函数不会被调用，MISO 由硬件外设自动读取
  */
uint8_t MySPI_R_MISO(void)
{
	return HAL_GPIO_ReadPin(GPIOA, SPI1_MISO_Pin);
}

/**
  * @brief  SPI 起始信号 (拉低片选)
  * @note   选中 W25Q64，开始通信帧
  * @retval 无
  */
void MySPI_Start(void)
{
	MySPI_W_SS(0); // 拉低 SS，选中设备
}

/**
  * @brief  SPI 停止信号 (拉高片选)
  * @note   取消选中 W25Q64，结束通信帧
  * @retval 无
  */
void MySPI_Stop(void)
{
	MySPI_W_SS(1); // 拉高 SS，取消选中设备
}

/**
  * @brief  通过硬件 SPI 交换一个字节数据
  * @param  ByteSend: 要发送的 8 位数据
  * @retval 接收到的 8 位数据
  * @note   使用 STM32 硬件 SPI 外设进行全双工通信
  *         此函数阻塞直到传输完成 (HAL_MAX_DELAY)
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ByteReceive = 0;
	
	// 调用 HAL 库硬件 SPI 收发函数
	// &hspi1: SPI 句柄
	// &ByteSend: 发送数据缓冲区指针
	// &ByteReceive: 接收数据缓冲区指针
	// 1: 数据长度 (1 字节)
	// HAL_MAX_DELAY: 超时时间 (无限等待，直到传输完成)
	HAL_SPI_TransmitReceive(&hspi1, &ByteSend, &ByteReceive, 1, HAL_MAX_DELAY);
	
	return ByteReceive;
}