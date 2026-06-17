#include "main.h"
#include <stdint.h>

// 声明外部定时器句柄，该变量通常在 main.c 或 stm32f1xx_hal_msp.c 中定义
extern TIM_HandleTypeDef htim2;

// 全局变量，用于记录定时器溢出（中断）发生的次数
uint16_t Num = 0;

/**
 * @brief 定时器初始化函数
 * @note 启动 TIM2 并开启更新中断 (Update Interrupt)
 */
void Timer_Init(void)
{
    // 以中断模式启动 TIM2 基定时器
    // 当计数器达到自动重装载值 (ARR) 时，将触发中断并调用回调函数
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief HAL 库定时器周期 elapsed 回调函数
 * @param htim: 指向定时器句柄的指针
 * @note 此函数由 HAL 库在中断服务程序 (ISR) 中自动调用
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // 判断当前触发中断的定时器实例是否为 TIM2
    if (htim->Instance == TIM2)
    {
        // 如果是 TIM2，则全局计数变量加 1
        Num++;
    }
}

/**
 * @brief 获取当前中断计数值
 * @return uint16_t 返回 Num 的当前值
 */
uint16_t GetNum(void)
{
    return Num;
}