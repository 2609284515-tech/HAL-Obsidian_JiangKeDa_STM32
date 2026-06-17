#include "main.h"
#include "cmsis_os2.h"
#include "Types/LED_Type.h"
#include "FreeRTOS.h"
#include <string.h>

// 声明外部消息队列句柄
extern osMessageQueueId_t SerialQueueHandle; // 用于接收来自 SerialTask 的原始串口数据
extern osMessageQueueId_t LEDQueueHandle;    // 用于向 LED/OLED 任务发送解析后的控制指令

/**
 * @brief  串口数据转换与转发任务入口函数
 * @param  argument: 任务创建时传递的参数（此处未使用）
 * @retval None
 */
void StartTransTask(void *argument)
{
    // 定义指针变量，用于接收从串口队列中取出的数据指针
    uint8_t *Serial_RxArray_Get;

    for(;;) // 无限循环，处理接收到的串口数据
    {
        // 1. 接收串口数据
        // 从 SerialQueueHandle 获取数据指针
        // osWaitForever: 如果没有串口数据，任务在此阻塞，不占用 CPU
        osMessageQueueGet(SerialQueueHandle, &Serial_RxArray_Get, 0, osWaitForever);

        // 2. 准备 LED 控制消息
        // 动态分配内存用于存储解析后的 LED 状态结构体
        // 注意：应检查 pvPortMalloc 返回值，防止内存分配失败
        LED_MsgStructure *pLED_Msg_Put = pvPortMalloc(sizeof(LED_MsgStructure));
        
        // 初始化默认值（可选，防止 switch 未匹配到情况时的随机值）
        pLED_Msg_Put->LED_Color = LED_Color_Red; 
        pLED_Msg_Put->LED_State = LED_State_Off;

        // 3. 解析协议 - 颜色字段 (假设数据包第1个字节表示颜色)
        switch(Serial_RxArray_Get[0])
        {
            case 0x00:
                pLED_Msg_Put->LED_Color = LED_Color_Red;   // 0x00 对应红色
                break;
            case 0x01:
                pLED_Msg_Put->LED_Color = LED_Color_Green; // 0x01 对应绿色
                break;
            case 0x02:
                pLED_Msg_Put->LED_Color = LED_Color_Blue;  // 0x02 对应蓝色
                break;         
            // 可以添加 default 分支处理非法颜色代码
        }

        // 4. 解析协议 - 状态字段 (假设数据包第2个字节表示开关状态)
        switch(Serial_RxArray_Get[1])
        {
            case 0x00:
                pLED_Msg_Put->LED_State = LED_State_Off;   // 0x00 对应关闭
                break;
            case 0x01:
                pLED_Msg_Put->LED_State = LED_State_On;    // 0x01 对应开启
                break;
            // 可以添加 default 分支处理非法状态代码
        }

        // 5. 释放串口数据内存
        // Serial_RxArray_Get 是由 SerialTask 分配的，本任务使用完后必须释放，防止内存泄漏
        vPortFree(Serial_RxArray_Get);

        // 6. 发送 LED 控制消息
        // 将解析好的 LED 结构体指针放入 LED 队列，供 OLEDTask 或其他 LED 控制任务处理
        osMessageQueuePut(LEDQueueHandle, &pLED_Msg_Put, 0, osWaitForever);
    }
}