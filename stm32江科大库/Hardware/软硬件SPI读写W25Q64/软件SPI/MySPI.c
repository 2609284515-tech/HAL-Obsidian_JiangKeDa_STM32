#include "main.h"

/**
  * @brief  写片选信号 (SS/CS)
  * @param  BitValue: 0 为选中 (低电平有效), 1 为取消选中
  * @retval 无
  */
void MySPI_W_SS(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, W25Q64_SS_Pin, (GPIO_PinState)BitValue);
}

/**
  * @brief  写时钟信号 (SCK)
  * @param  BitValue: 0 为低电平, 1 为高电平
  * @retval 无
  */
void MySPI_W_SCK(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, W25Q64_SCK_Pin, (GPIO_PinState)BitValue);
}

/**
  * @brief  写主机输出从机输入信号 (MOSI)
  * @param  BitValue: 0 为低电平, 1 为高电平
  * @retval 无
  */
void MySPI_W_MOSI(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, W25Q64_MOSI_Pin, (GPIO_PinState)BitValue);
}

/**
  * @brief  读主机输入从机输出信号 (MISO)
  * @retval 引脚当前电平状态 (0 或 1)
  */
uint8_t MySPI_R_MISO(void)
{
	return HAL_GPIO_ReadPin(GPIOA, W25Q64_MISO_Pin);
}

/**
  * @brief  SPI 起始信号 (拉低片选)
  * @note   W25Q64 的片选信号低电平有效，拉低表示开始通信
  * @retval 无
  */
void MySPI_Start(void)
{
	MySPI_W_SS(0); // 拉低 SS，选中 W25Q64
}

/**
  * @brief  SPI 停止信号 (拉高片选)
  * @note   拉高片选表示结束本次通信
  * @retval 无
  */
void MySPI_Stop(void)
{
	MySPI_W_SS(1); // 拉高 SS，取消选中 W25Q64
}

/**
  * @brief  SPI 交换一个字节数据 (同时发送和接收)
  * @param  ByteSend: 要发送的 8 位数据
  * @retval 接收到的 8 位数据
  * @note   模式：CPOL=0 (空闲时 SCK 为低), CPHA=0 (第一个边沿采样/第二个边沿变化)
  *         通常 W25Q64 支持 Mode 0 或 Mode 3，此处实现符合 Mode 0 (上升沿采样)
  *         数据高位先发 (MSB First)
  */
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i, ByteReceive = 0x00;
	
	for (i = 0; i < 8; i ++)
	{
		// 1. 主机将数据位放到 MOSI 线上
		// !!(ByteSend & (0x80 >> i)) 确保结果为标准的 0 或 1
		MySPI_W_MOSI(!!(ByteSend & (0x80 >> i)));
		
		// 2. SCK 拉高 (上升沿)
		// 在上升沿时刻，从机(W25Q64)会读取 MOSI 上的数据
		// 同时，主机可以在此时读取 MISO 上的数据 (取决于具体器件时序，通常在此处或下降沿读取)
		MySPI_W_SCK(1);
		
		// 3. 主机从 MISO 读取数据位
		// 如果 MISO 为高，则将接收变量的对应位置 1
		if (MySPI_R_MISO())
		{
			ByteReceive |= (0x80 >> i);
		}
		
		// 4. SCK 拉低 (下降沿)
		// 准备下一位数据，MOSI 可以在 SCK 低电平时变化
		MySPI_W_SCK(0);
	}
	
	return ByteReceive;
}