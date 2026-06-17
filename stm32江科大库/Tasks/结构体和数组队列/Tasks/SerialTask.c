#include "main.h"
#include "FreeRTOS.h"
#include "Serial.h"
#include "cmsis_os2.h"
#include <string.h>

// 声明外部消息队列句柄，用于向其他任务（如处理任务）发送串口接收数据
extern osMessageQueueId_t SerialQueueHandle;

/**
 * @brief  串口数据处理任务入口函数
 * @param  argument: 任务创建时传递的参数（此处未使用）
 * @retval None
 */
void StartSerialTask(void *argument)
{
    // 初始化串口硬件及中断配置
    Serial_Init();

    for(;;) // 无限循环，持续监测串口接收状态
    {
        // 检查串口接收标志位
        // Serial_GetRxFlag() 返回 1 表示接收到一包完整的数据
        if (Serial_GetRxFlag() == 1)
        {
            // 动态分配内存，大小为4字节（假设数据包固定长度为4）
            // 注意：实际工程中应检查返回值是否为 NULL
            uint8_t *Serial_RxArray_Put = pvPortMalloc(sizeof(uint8_t[4]));
            
            // 将全局缓冲区 Serial_RxPacket 中的数据复制到新分配的内存中
            // 使用 memcpy 避免直接传递全局变量指针，防止数据在传输过程中被新的中断数据覆盖
            memcpy(Serial_RxArray_Put, Serial_RxPacket, sizeof(uint8_t[4]));

            // 将指向数据包的指针放入消息队列
            // &Serial_RxArray_Put: 发送指针的地址
            // 0: 优先级
            // osWaitForever: 如果队列满则等待
            osMessageQueuePut(SerialQueueHandle, &Serial_RxArray_Put, 0, osWaitForever);
        }
        else
        {
            // 如果没有接收到数据，延时 10ms
            // 作用：降低 CPU 占用率，给其他低优先级任务执行机会
            osDelay(10);
        }
    }
}