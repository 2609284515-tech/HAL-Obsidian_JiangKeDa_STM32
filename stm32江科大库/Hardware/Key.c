#include "main.h"

/**
 * @brief 获取按键键码
 * @details 
 *          - 返回值为1或2，分别对应Key_1和Key_2。
 *          - 如果没有按键按下，返回0。
 *          - 包含软件消抖处理（前后各延时20ms）。
 *          - **注意**：这是一个阻塞式函数。如果按住按键不放，程序会卡在 while 循环中，直到按键松开才会返回。
 * @retval uint8_t 按键键码 (0: 无按键, 1: Key_1, 2: Key_2)
 */
uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0; // 初始化键码为0，表示默认无按键按下

    // 检测 Key_1 是否按下 (低电平有效)
    if (HAL_GPIO_ReadPin(GPIOB, Key_1_Pin) == 0)
    {
        HAL_Delay(20); // 延时20ms进行软件消抖，消除按键闭合时的机械抖动
        
        // 等待按键释放 (阻塞等待)
        // 只要引脚仍为低电平，就在此处循环，直到用户松手
        while (HAL_GPIO_ReadPin(GPIOB, Key_1_Pin) == 0);
        
        HAL_Delay(20); // 松开后再次延时20ms，消除断开时的机械抖动
        
        KeyNum = 1; // 设置键码为1
    }

    // 检测 Key_2 是否按下 (低电平有效)
    if (HAL_GPIO_ReadPin(GPIOB, Key_2_Pin) == 0)
    {
        HAL_Delay(20); // 延时20ms进行软件消抖
        
        // 等待按键释放 (阻塞等待)
        while (HAL_GPIO_ReadPin(GPIOB, Key_2_Pin) == 0);
        
        HAL_Delay(20); // 松开后再次延时20ms，消除断开时的机械抖动
        
        KeyNum = 2; // 设置键码为2
    }
    
    return KeyNum; // 返回最终的键码
}