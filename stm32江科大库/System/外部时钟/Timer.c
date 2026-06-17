#include "main.h"
#include <stdint.h>

// 声明外部定时器句柄，通常在 main.c 或 stm32xxxx_hal_msp.c 中定义
extern TIM_HandleTypeDef htim2;

// 全局变量，用于记录定时器溢出（中断）的次数
uint16_t Num = 0;

/**
 * @brief 定时器初始化函数
 * @note 启动 TIM2 并开启更新中断
 */
void Timer_Init(void)
{
    // 以中断模式启动 TIM2 基定时器
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief HAL 库定时器周期 elapsed 回调函数
 * @param htim: 定时器句柄指针
 * @note 当定时器计数达到自动重装载值（ARR）时触发此回调
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // 判断是否是 TIM2 触发的中断
    if (htim->Instance == TIM2)
    {
        // 每次中断发生，计数器加 1
        Num++;
    }
}

/**
 * @brief 获取当前定时器中断计数值
 * @return uint16_t: 返回 Num 的值
 */
uint16_t GetNum(void)
{
    return Num;
}