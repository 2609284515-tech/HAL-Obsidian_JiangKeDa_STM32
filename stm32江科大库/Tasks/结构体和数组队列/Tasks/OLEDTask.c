#include "main.h"
#include "cmsis_os2.h"
#include "Types/LED_Type.h"
#include "FreeRTOS.h"
#include "OLED.h"

// 声明外部消息队列句柄，用于从 KeyTask 或其他任务接收 LED 控制消息
extern osMessageQueueId_t LEDQueueHandle;

/**
 * @brief  OLED 显示任务入口函数
 * @param  argument: 任务创建时传递的参数（此处未使用）
 * @retval None
 */
void StartOLEDTask(void *argument)
{
    // 初始化 OLED 屏幕硬件
    OLED_Init();

    // 定义指针变量，用于接收从队列中取出的消息指针
    // 注意：这里只声明了指针，内存是由发送方（KeyTask）分配的
    LED_MsgStructure *pLED_Msg_Get;

    // 在 OLED 上显示静态标签文本
    // 第1行第1列显示 "LED_Color:"
    OLED_ShowString(1, 1, "LED_Color:");
    // 第2行第1列显示 "LED_State:"
    OLED_ShowString(2, 1, "LED_State:");

    for(;;) // 无限循环，等待并处理消息
    {
        // 从消息队列中获取消息
        // LEDQueueHandle: 消息队列句柄
        // &pLED_Msg_Get: 存放接收到的消息指针的地址（即二级指针）
        // 0: 优先级为 0
        // osWaitForever: 如果队列为空，则永久阻塞等待，直到有新消息到达
        osMessageQueueGet(LEDQueueHandle, &pLED_Msg_Get, 0, osWaitForever);

        // 根据接收到的消息中的颜色字段，更新 OLED 第一行的显示内容
        switch(pLED_Msg_Get->LED_Color)
        {
            case LED_Color_Blue:
                // 显示蓝色，末尾加空格以覆盖之前可能更长的字符（如 Green）
                OLED_ShowString(1, 11, "Blue ");
                break;
            case LED_Color_Green:
                OLED_ShowString(1, 11, "Green");
                break;
            case LED_Color_Red:
                // 显示红色，末尾加空格以对齐
                OLED_ShowString(1, 11, "Red  ");
                break;
            // 可以根据需要添加 default 分支处理异常颜色值
        }

        // 根据接收到的消息中的状态字段，更新 OLED 第二行的显示内容
        // 如果 LED_State 为真（非0），显示 "On "；否则显示 "Off"
        OLED_ShowString(2, 11, pLED_Msg_Get->LED_State ? "On " : "Off");

        // 释放动态分配的内存
        // 因为内存是在发送方（KeyTask）通过 pvPortMalloc 分配的，
        // 所以接收方在使用完数据后必须负责释放，以防止内存泄漏
        vPortFree(pLED_Msg_Get);
    }
}