#include "main.h"

int16_t Encoder_Count; // 全局变量，用于存储旋转编码器的累计计数值

/**
 * @brief GPIO外部中断回调函数
 * @details 
 *          - 该函数由HAL库在检测到GPIO中断时自动调用。
 *          - 用于处理旋转编码器 A相 (Encoder_A_Pin) 和 B相 (Encoder_B_Pin) 的信号变化。
 *          - **原理**：通过检测某一相的边沿触发中断，并读取另一相的电平状态来判断旋转方向。
 *          - **方向定义**：
 *            - 顺时针旋转：Count 增加 (+)
 *            - 逆时针旋转：Count 减少 (-)
 * @param GPIO_Pin 触发中断的GPIO引脚号
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 判断是否是 A相 (Line0) 触发的中断
    if (GPIO_Pin == Encoder_A_Pin)
    {
        // 当 A相发生跳变时，检查 B相 的电平
        // 根据硬件特性：若 B相 为低电平 (RESET)，通常对应一个方向的旋转
        if (HAL_GPIO_ReadPin(GPIOB, Encoder_B_Pin) == GPIO_PIN_RESET)
        {
            Encoder_Count--; // 计数器减1 (例如：逆时针)
        }
        // 注意：如果 B相 为高电平，则对应相反方向，此处未处理或隐含在else逻辑中，
        // 但当前代码结构仅处理了 B为低 的情况，需结合具体硬件接线确认逻辑完整性。
        // *修正提示*：通常完整的正交解码逻辑会包含 else { Encoder_Count++; }
    }
    // 判断是否是 B相 (Line1) 触发的中断
    else if (GPIO_Pin == Encoder_B_Pin)
    {
        // 当 B相发生跳变时，检查 A相 的电平
        if (HAL_GPIO_ReadPin(GPIOB, Encoder_A_Pin) == GPIO_PIN_RESET)
        {
            Encoder_Count++; // 计数器加1 (例如：顺时针)
        }
    }
}

/**
 * @brief 获取当前编码器计数值
 * @retval int16_t 当前的累计计数值
 */
int16_t Encoder_GetCount(void)
{
    return Encoder_Count; // 返回全局计数变量
}