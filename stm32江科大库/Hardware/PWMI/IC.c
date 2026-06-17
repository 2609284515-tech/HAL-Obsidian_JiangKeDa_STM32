#include "main.h"

// 声明外部定时器句柄，通常由 CubeMX 生成的 main.c 或 tim.c 中定义
extern TIM_HandleTypeDef htim3;

/**
 * @brief 初始化输入捕获功能
 * @note 启动 TIM3 的通道1和通道2用于输入捕获
 *       - 通道1通常用于捕获PWM信号的周期（上升沿到下一个上升沿）
 *       - 通道2通常用于捕获PWM信号的高电平时间（上升沿到下降沿，取决于具体配置）
 */
void IC_Init(void)
{
    // 启动定时器3通道1的输入捕获中断/DMA（此处为非中断模式，直接读取寄存器）
    HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_1);
    
    // 启动定时器3通道2的输入捕获
    HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_2);
}

/**
 * @brief 使用测周法获取PWM信号的频率
 * @return uint32_t 频率值，单位：Hz
 * @note 计算公式：Frequency = Timer_Clock_Frequency / (Capture_Value + 1)
 *       假设定时器时钟频率为 1MHz (即每个计数值为 1us)，则频率 = 1,000,000 / 周期计数值
 *       HAL_TIM_ReadCapturedValue 返回的是捕获寄存器的值，代表两个上升沿之间的计数值
 */
uint32_t IC_GetFreq(void)
{
    // 获取通道1捕获的值（周期对应的计数值）
    // +1 是为了修正从0开始计数带来的误差，或者根据具体硬件配置调整
    uint32_t period_count = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1) + 1;
    
    // 防止除以零错误
    if (period_count == 0) return 0;

    // 假设定时器计数频率为 1MHz (1us/step)，计算频率
    return 1000000 / period_count;
}

/**
 * @brief 获取PWM信号的占空比
 * @return uint32_t 占空比，单位：% (0-100)
 * @note 计算公式：Duty = (High_Level_Time / Total_Period_Time) * 100%
 *       通道2捕获高电平时间计数值，通道1捕获总周期计数值
 */
uint32_t IC_Getduty(void)
{
    // 获取通道2捕获的值（高电平时间对应的计数值）
    uint32_t high_level_count = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2) + 1;
    
    // 获取通道1捕获的值（总周期对应的计数值）
    uint32_t period_count = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1) + 1;
    
    // 防止除以零错误
    if (period_count == 0) return 0;

    // 计算占空比百分比
    // 注意：整数运算可能导致精度丢失，若需更高精度可先乘后除或使用浮点数
    return (high_level_count * 100) / period_count;
}