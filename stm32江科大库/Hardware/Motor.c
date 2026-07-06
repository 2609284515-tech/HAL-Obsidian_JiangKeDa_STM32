#include "main.h"
#include "PWM.h"

/**
 * @brief 电机初始化函数
 * @note 调用底层 PWM 初始化，确保电机驱动所需的 PWM 信号已配置
 */
void Motor_Init(void)
{
    // 初始化 PWM 通道，通常用于控制电机速度
    PWM_Init();
}

/**
 * @brief 设置电机速度和方向
 * @param Speed: 电机速度值
 *        - 正值 (0 ~ Max): 正转，数值越大速度越快
 *        - 负值 (-Max ~ 0): 反转，绝对值越大速度越快
 *        - 0: 停止
 * @note 通过控制 AIN1/AIN2 GPIO 电平改变电流方向，通过 PWM 占空比控制平均电压
 */
void Motor_SetSpeed(int16_t Speed)
{
    if (Speed >= 0)
    {
        // --- 正转逻辑 ---
        // AIN1 高电平, AIN2 低电平 -> 电流从 A 流向 B (假设)
        HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_RESET);
        
        // 设置 PWM 占空比，速度值为正，直接传入
        PWM_SetCompare1(Speed);
    }
    else 
    {
        // --- 反转逻辑 ---
        // AIN1 低电平, AIN2 高电平 -> 电流从 B 流向 A (反向)
        HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_RESET);
        
        // 设置 PWM 占空比，速度值为负，取绝对值后传入
        PWM_SetCompare1(-Speed);
    }
}