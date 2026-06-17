#include "main.h"
#include "cmsis_os2.h"
#include "Types/LED_Type.h"
#include "FreeRTOS.h"
#include "Serial.h"

//声明外部 FreeRTOS 对象句柄
extern osMessageQueueId_t LEDQueueHandle;   // LED 消息队列句用于任务间通信
extern osSemaphoreId_t KeySemaphoreHandle;  // 按键信号量用于中断与任务同步

/**
 * @brief 按键处理任务入口函数
 * @param argument 任务参数（未使用）
 * 
 * 功能说明：
 * 1. 等待按键中断触发的信号量
 * 2. 软件消抖
 * 3. 检测按键上升沿（按下动作）
 * 4. 切换 LED 状态并通过消息队列发送控制指令
 */
void StartKeyTask(void *argument)
{
    // 初始化 LED 状态变量，初始为关闭
    LED_State_Type LED_State_Value = LED_State_Off;
    
    // 记录上一次按键的电平状态，用于边缘检测
    uint8_t Key_Last_State = 0;

    for(;;) // 无限循环，任务主体
    {
        // 阻塞等待信号量：当按键中断发生时，HAL_GPIO_EXTI_Callback 会释放此信号量
        // osWaitForever 表示如果没有信号量，任务将一直挂起，不占用 CPU 资源
        osSemaphoreAcquire(KeySemaphoreHandle, osWaitForever);
        
        // 软件消抖：等待 20ms 以避开按键机械抖动产生的不稳定电平
        osDelay(20);
        
        // 读取当前按键引脚的实际电平状态
        // 假设 Key_2_Pin 连接到 GPIOB，高电平表示按键未按下或已释放（具体取决于硬件电路设计，此处依据逻辑推断）
        uint8_t Current_Key_State = HAL_GPIO_ReadPin(GPIOB, Key_2_Pin);

        // 检测上升沿：当前为高电平(1) 且 上次为低电平(0)
        // 注意：这里的具体逻辑取决于硬件电路。如果按键按下接地，则通常检测下降沿。
        // 根据代码逻辑，这里假设 1 代表有效触发状态（或者按键松开触发，需结合硬件确认）。
        // 通常按键按下检测逻辑为：if (Current == 0 && Last == 1) 对于低电平有效的按键。
        // 此处保留原代码逻辑：检测到从 0 变 1 的过程
        if (Current_Key_State == 1 && Key_Last_State == 0)
        {
            // 更新上次状态标记
            Key_Last_State = 1;

            // 翻转 LED 的逻辑状态
            LED_State_Value = !LED_State_Value;
            
            // 动态分配内存用于存储 LED 消息结构体
            // 使用 pvPortMalloc 而不是 malloc，因为这是 FreeRTOS 推荐的堆管理方式
            LED_MsgStructure *pLED_Msg_Put = pvPortMalloc(sizeof(LED_MsgStructure));
            
            // 检查内存分配是否成功（生产代码中建议添加此检查）
            if (pLED_Msg_Put != NULL)
            {
                // 填充消息内容
                pLED_Msg_Put->LED_Color = LED_Color_Red; // 设置 LED 颜色为红色
                pLED_Msg_Put->LED_State = LED_State_Value; // 设置 LED 开关状态

                // 将消息指针放入队列
                // LED 任务将从队列中取出该指针，处理完后负责释放内存
                osMessageQueuePut(LEDQueueHandle, &pLED_Msg_Put, 0, osWaitForever);
            }
        }
        else if (Current_Key_State == 0)
        {
            // 如果当前电平为 0，更新上次状态为 0
            // 这确保了只有当状态从 0 变为 1 时才会触发上面的 if 分支
            Key_Last_State = 0;
        }
    }
}

/**
 * @brief GPIO 外部中断回调函数
 * @param GPIO_Pin 触发中断的引脚号
 * 
 * 功能说明：
 * 当配置的 GPIO 引脚发生电平变化（中断）时，由 HAL 库自动调用。
 * 此处释放信号量，唤醒 StartKeyTask 任务进行按键处理。
 * 注意：中断回调中应只执行最简短的操作，避免耗时操作。
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 释放信号量，通知按键任务有事件发生
    // 如果有多个任务等待此信号量，最高优先级的任务将被解锁
    osSemaphoreRelease(KeySemaphoreHandle);
}