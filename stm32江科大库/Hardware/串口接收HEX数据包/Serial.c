#include "main.h"
#include <stdio.h>
#include <stdarg.h>

// 声明外部 UART 句柄，通常在 main.c 中定义
extern UART_HandleTypeDef huart1;

// 全局变量：临时存储当前接收到的单个字节
uint8_t Serial_RxData;
// 全局变量：数据包接收完成标志位
uint8_t Serial_RxFlag;
// 全局变量：接收缓冲区，用于存储解析出的4字节有效数据
uint8_t Serial_RxPacket[4];
// 全局变量：发送缓冲区，用户在此填入要发送的4字节数据
uint8_t Serial_TxPacket[4];

/**
  * @brief  串口初始化函数
  * @note   开启中断接收模式，准备接收第一个字节
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
    // 阻塞式发送1个字节
    HAL_UART_Transmit(&huart1, &Byte, 1, HAL_MAX_DELAY);
}

/**
  * @brief  发送字节数组
  * @param  Array: 数据指针
  * @param  Length: 数据长度
  * @retval 无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	HAL_UART_Transmit(&huart1, Array, Length, HAL_MAX_DELAY);
}

/**
  * @brief  发送字符串
  * @param  String: 以 '\0' 结尾的字符串指针
  * @retval 无
  */
void Serial_SendString(char *String)
{
	uint16_t len = strlen(String);
    HAL_UART_Transmit(&huart1, (uint8_t *)String, len, HAL_MAX_DELAY);
}

/**
  * @brief  计算幂运算 (X^Y)
  * @param  X: 底数
  * @param  Y: 指数
  * @retval 计算结果
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
  * @brief  发送指定宽度的数字
  * @note   例如 Number=123, Length=5 -> 发送 "00123"
  * @param  Number: 数值
  * @param  Length: 显示位数
  * @retval 无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
        // 提取每一位数字并转换为 ASCII 发送
		Serial_SendByte(Number / Serial_Pow(10, Length - 1 - i) % 10 + '0');
	}
}

/**
  * @brief  格式化打印 (类似 printf)
  * @param  format: 格式字符串
  * @param  ...: 可变参数
  * @retval 无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

/**
  * @brief  获取接收完成标志
  * @note   读取后会自动清除标志位
  * @param  无
  * @retval 1: 有完整数据包, 0: 无
  */
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0; // 清除标志
		return 1;
	}
	return 0;
}

/**
  * @brief  发送 HEX 数据包
  * @note   协议格式: 0xFF + 4字节数据 + 0xFE
  *         数据内容需先写入全局变量 Serial_TxPacket
  * @param  无
  * @retval 无
  */
void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);             // 发送帧头
	Serial_SendArray(Serial_TxPacket, 4); // 发送4字节有效数据
	Serial_SendByte(0xFE);             // 发送帧尾
}

/**
  * @brief  UART 接收中断回调函数
  * @note   实现状态机解析 HEX 数据包
  *         协议格式: 0xFF [Data1] [Data2] [Data3] [Data4] 0xFE
  * @param  huart: UART 句柄
  * @retval 无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // 静态变量：保持状态和索引
    static uint8_t RxState = 0;   // 状态机状态: 0-等待帧头, 1-接收数据, 2-等待帧尾
    static uint8_t pRxPacket = 0; // 数据包缓冲区索引 (0-3)

    if (huart->Instance == USART1)
    {
        // --- 状态机逻辑开始 ---
        
        // 状态 0: 等待帧头 0xFF
        if (RxState == 0)
		{
            // 只有当收到 0xFF 且当前没有未处理的数据包时，才开始接收新包
			if (Serial_RxData == 0xFF && Serial_RxFlag == 0)
			{
				RxState = 1;      // 切换到接收数据状态
				pRxPacket = 0;    // 重置缓冲区索引
			}
		}
        // 状态 1: 正在接收4字节有效数据
		else if (RxState == 1)
		{
            // 将数据存入缓冲区
			Serial_RxPacket[pRxPacket] = Serial_RxData;
			pRxPacket ++;         // 索引递增
			
            // 判断是否收满了4个字节
			if (pRxPacket >= 4)
			{
				RxState = 2;      // 切换到等待帧尾状态
			}
		}
        // 状态 2: 等待帧尾 0xFE
		else if (RxState == 2)
		{
            // 检查是否收到帧尾 0xFE
			if (Serial_RxData == 0xFE)
			{
				RxState = 0;      // 复位状态机，准备接收下一帧
				Serial_RxFlag = 1;// 置位标志，通知主循环数据已就绪
			}
            // 如果收到的不是 0xFE，说明数据包损坏或不同步。
            // 严谨做法应在此处 else { RxState = 0; } 重置状态以重新寻找帧头
		}
        
        // --- 状态机逻辑结束 ---
        
        // 重要：重新开启中断接收，以便接收下一个字节
        HAL_UART_Receive_IT(&huart1, &Serial_RxData, 1);
    }
}