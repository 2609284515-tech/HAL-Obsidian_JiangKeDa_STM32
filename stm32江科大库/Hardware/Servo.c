#include "main.h"                 
#include "PWM.h"

/**
 * @brief 舵机初始化函数
 * @note 调用底层 PWM 初始化配置
 */
void Servo_Init(void)
{
    // 初始化 PWM 输出通道，通常配置为 50Hz (周期 20ms)
    PWM_Init();
}

/**
 * @brief 设置舵机角度
 * @param Angle: 目标角度，范围通常为 0~180 度
 * @note 将角度线性映射为 PWM 比较值（脉冲宽度）
 *       公式推导：
 *       - 舵机信号周期通常为 20ms (50Hz)
 *       - 0度对应脉冲宽度约 0.5ms (500us)
 *       - 180度对应脉冲宽度约 2.5ms (2500us)
 *       - 假设 PWM 计数器频率使得 1us 对应 1 个计数单位（或已按比例缩放）
 *       - 这里 2000 代表 180度对应的脉宽增量 (2.5ms - 0.5ms = 2.0ms = 2000us)
 *       - 500 代表 0度时的基准脉宽 (0.5ms = 500us)
 */
void Servo_SetAngle(float Angle)
{
    // 计算对应的 PWM 比较值并设置
    // Angle / 180 * 2000 + 500 将 0-180 度映射为 500-2500 的计数值
    PWM_SetCompare1(Angle / 180 * 2000 + 500);
}
