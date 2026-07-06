#include "main.h"
#include <stdio.h>
#include <stdarg.h>

// 声明外部 UART 句柄，通常在 main.c 或 stm32xxxx_hal_msp.c 中定义
extern UART_HandleTypeDef huart1;

// 全局变量：存储接收到的单个字节数据
uint8_t Serial_RxData;
// 全局变量：接收完成标志位，1表示收到数据，0表示未收到或已读取
uint8_t Serial_RxFlag;

/**
  * @brief  串口初始化函数
  * @note   开启串口中断接收模式，准备接收第一个字节
  * @param  无
  * @retval 无
  */
void Serial_Init(void)
{
    // 启动中断接收，每次接收1个字节存入 Serial_RxData
    HAL_UART_Receive_IT(&huart1, &Serial_RxData, 1);
}

/**
  * @brief  发送一个字节
  * @param  Byte: 要发送的字节数据
  * @retval 无
  */
void Serial_SendByte(uint8_t Byte)
{
    // 使用阻塞模式发送1个字节
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY);
}

/**
  * @brief  发送字节数组
  * @param  Array: 指向要发送的数据数组的指针
  * @param  Length: 数组长度
  * @retval 无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	HAL_UART_Transmit(&huart1, Array, Length, HAL_MAX_DELAY);
}

/**
  * @brief  发送字符串
  * @param  String: 指向以 '\0' 结尾的字符串的指针
  * @retval 无
  */
void Serial_SendString(char *String)
{
	uint16_t len = strlen(String);
    HAL_UART_Transmit(&huart1, (uint8_t *)String, len, HAL_MAX_DELAY);
}

/**
  * @brief  计算 X 的 Y 次幂
  * @param  X: 底数
  * @param  Y: 指数
  * @retval X^Y 的结果
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  发送指定长度的数字
  * @note   例如 Number=123, Length=5, 则发送 "00123"
  * @param  Number: 要发送的数字
  * @param  Length: 显示的总位数（不足高位补0）
  * @retval 无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
        // 通过除法和取模运算提取每一位数字，并转换为 ASCII 码发送
        // Serial_Pow(10, Length - 1 - i) 计算当前位的权重
		Serial_SendByte(Number / Serial_Pow(10, Length - 1 - i) % 10 + '0');
	}
}

/**
  * @brief  格式化打印输出 (类似 printf)
  * @param  format: 格式化字符串
  * @param  ...: 可变参数
  * @retval 无
  */
void Serial_Printf(char *format, ...)
{
	char String[100]; // 缓冲区，最大支持100字符
	va_list arg;
	va_start(arg, format);
    // 将格式化内容写入缓冲区
	vsprintf(String, format, arg);
	va_end(arg);
    // 发送缓冲区内容
	Serial_SendString(String);
}

/**
  * @brief  获取接收标志位
  * @note   如果标志位为1，则自动清零并返回1，否则返回0
  * @param  无
  * @retval 1: 收到新数据, 0: 未收到新数据
  */
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0; // 清除标志位，防止重复读取
		return 1;
	}
	return 0;
}

/**
  * @brief  获取接收到的数据
  * @param  无
  * @retval 接收到的字节数据
  */
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

/**
  * @brief  UART 接收完成回调函数
  * @note   当 HAL_UART_Receive_IT 接收到指定长度的数据后，HAL 库会自动调用此函数
  * @param  huart: UART 句柄指针
  * @retval 无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // 判断是否是 USART1 且当前没有未处理的数据 (防重叠/丢包简单处理)
    if (huart->Instance == USART1 && Serial_RxFlag == 0)
    {
        Serial_RxFlag = 1; // 置位接收标志，通知主循环有新数据
        
        // 重要：重新开启中断接收，以便接收下一个字节
        // 如果不加这一行，只能接收一次数据
        HAL_UART_Receive_IT(&huart1, &Serial_RxData, 1);
    }
}