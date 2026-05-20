#include "main.h"
#include "Delay.h"
#include <stdint.h>

uint16_t CountSensor_Count = 0; // 全局变量，用于存储传感器的累计计数值

/**
 * @brief GPIO外部中断回调函数
 * @details 
 *          - 该函数由HAL库在检测到GPIO中断时自动调用。
 *          - 用于处理计数传感器信号引脚 (CountSensor_Pin) 的状态变化。
 *          - **注意**：在中断服务程序中使用延时函数 [Delay_ms] 是一种简单的软件消抖方法，
 *            但会阻塞中断上下文，导致其他低优先级中断或主循环任务暂时无法执行。
 *            在实际工程中，更推荐使用定时器硬件滤波或状态机方式消抖。
 * @param GPIO_Pin 触发中断的GPIO引脚号
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 判断是否是计数传感器引脚触发的中断
    if (GPIO_Pin == CountSensor_Pin)
    {
        Delay_ms(20); // 延时20ms，等待机械抖动结束，确保信号稳定
        
        // 再次读取引脚电平，确认是否为有效的低电平（下降沿触发后的稳定状态）
        // 如果此时引脚仍为低电平，说明是一次有效的按键/传感器触发
        if (HAL_GPIO_ReadPin(GPIOB, CountSensor_Pin) == 0)    
        {
            CountSensor_Count++; // 计数器加1
        }
    }
}

/**
 * @brief 获取当前传感器计数值
 * @retval uint16_t 当前的累计计数值
 */
uint16_t CountSensor_GetCount(void)
{
    return CountSensor_Count; // 返回全局计数变量
}