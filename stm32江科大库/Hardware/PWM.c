#include "main.h"
#include "stm32f1xx_hal_tim.h"

// 声明外部定时器句柄，通常在 main.c 或 stm32f1xx_hal_msp.c 中定义
extern TIM_HandleTypeDef htim2;

/**
 * @brief PWM 初始化函数
 * @note 启动 TIM2 通道 1 的 PWM 输出
 */
void PWM_Init(void)
{
    // 以 PWM 模式启动 TIM2 的通道 1
    // 注意：定时器的基本配置（如频率、ARR、PSC等）通常在 CubeMX 生成的初始化代码中完成
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

/**
 * @brief 设置 PWM 通道 1 的比较值 (CCR)
 * @param Compare: 新的比较值，决定占空比
 * @note 修改 CCR 寄存器可以动态改变 PWM 波的占空比，从而控制电机速度或舵机角度
 */
void PWM_SetCompare1(uint16_t Compare)
{
    // 使用 HAL 库宏直接设置 TIM2 通道 1 的捕获/比较寄存器值
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Compare);
}

/**
 * @brief 设置 PWM 定时器的预分频值 (PSC)
 * @param Prescaler: 新的预分频值
 * @note 修改 PSC 会改变定时器的计数频率，进而改变 PWM 波的频率
 *       公式: 计数频率 = 定时器时钟频率 / (Prescaler + 1)
 * @warning 动态修改预分频器可能会导致 PWM 波形短暂异常，建议在初始化时设定好频率
 */
void PWM_SetPrescaler(uint16_t Prescaler)
{
    // 使用 HAL 库宏直接设置 TIM2 的预分频寄存器值
    __HAL_TIM_SET_PRESCALER(&htim2, Prescaler);
}