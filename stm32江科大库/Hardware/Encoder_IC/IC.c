#include "main.h"
#include "stm32f1xx_hal_tim.h"

// 声明外部定时器句柄，通常由 CubeMX 生成，对应 TIM3
extern TIM_HandleTypeDef htim3;

/**
 * @brief 初始化编码器接口
 * @note 启动 TIM3 的编码器模式。
 *       在编码器模式下，TIM3 的通道1 (CH1) 和通道2 (CH2) 分别连接编码器的 A相 和 B相。
 *       定时器硬件会自动根据 A/B 相的相位关系进行计数（正转加，反转减）。
 */
void IC_Init(void)
{
    // 启动 TIM3 通道1 的编码器接口功能
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
    
    // 启动 TIM3 通道2 的编码器接口功能
    // 注意：在 HAL 库中，通常只需要调用一次 HAL_TIM_Encoder_Start 并指定一个通道即可同时启用两个通道，
    // 但显式写出两个通道有助于理解硬件连接逻辑，或者在某些特定配置下确保双通道均被正确使能。
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);
}

/**
 * @brief 读取编码器当前计数值（位置/相位）
 * @return int16_t 有符号的计数值。
 *         - 正值表示正转（顺时针，具体取决于接线和极性配置）
 *         - 负值表示反转（逆时针）
 * @note 
 *       1. __HAL_TIM_GET_COUNTER 返回的是 uint16_t (无符号)。
 *       2. 此处直接将其强制转换为 int16_t (有符号)。
 *          利用补码特性：
 *          - 当 CNT < 32768 时，视为正数 (0 ~ 32767)
 *          - 当 CNT >= 32768 时，视为负数 (-32768 ~ -1)
 *          这要求定时器的自动重装载寄存器 (ARR) 设置为 65535 (即 16位最大值)，
 *          且初始计数器值通常设为 0 或中间值，以便通过溢出/下溢来判断方向变化范围。
 *          如果 ARR 设置较小，这种转换可能无法正确反映长距离旋转的累积位置，仅适用于小范围角度检测或相对变化。
 */
int16_t Encoder_Get(void)
{
    // 获取定时器当前的计数寄存器 (CNT) 值
    // 宏定义展开后大致为: (uint16_t)(htim3->Instance->CNT)
    return (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
}