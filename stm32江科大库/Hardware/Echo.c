#include "main.h"
#include "delay.h"
#include "OLED.h"
#include "cmsis_os2.h"

/**
  * @brief  触发超声波模块发送声波
  * @note   HC-SR00/HC-SR04 需要至少 10us 的高电平触发信号
  */
void Echo_Trig(void)
{
    // 拉高 Trig 引脚
    HAL_GPIO_WritePin(HC_TRIG_GPIO_Port, HC_TRIG_Pin, GPIO_PIN_SET);
    
    // 延时 1ms (1000us)，远大于所需的 10us，确保触发成功
    // 注意：osDelay 的单位通常是毫秒(ms)
    osDelay(1); 
    
    // 拉低 Trig 引脚，完成触发脉冲
    HAL_GPIO_WritePin(HC_TRIG_GPIO_Port, HC_TRIG_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  获取超声波测量的距离
  * @retval 距离值，单位：厘米(cm)
  * @note   当前实现使用软件轮询+粗略延时，精度较低且阻塞线程
  */
uint32_t Echo_Get(void)
{
    uint16_t i = 0;
    
    // 1. 等待 Echo 引脚变为高电平（声波发出）
    // 注意：如果传感器故障或未连接，此处可能死循环，建议增加超时机制
    while (HAL_GPIO_ReadPin(HC_ECHO_GPIO_Port, HC_ECHO_Pin) == GPIO_PIN_RESET);
    
    // 2. 测量高电平持续时间
    // 原理：声音在空气中速度约 340m/s。
    // 距离(cm) = (时间(us) * 34000 cm/s) / 2 / 1000000 us/s 
    //          = 时间(us) / 58
    
    // 当前逻辑缺陷：
    // osDelay(1) 通常最小精度为 1ms (1000us)。
    // 超声波往返 1cm 大约需要 58us。
    // 使用 1ms 的步长意味着最小分辨率为 ~17cm (1000/58 ≈ 17.2)，精度极差。
    while (HAL_GPIO_ReadPin(HC_ECHO_GPIO_Port, HC_ECHO_Pin) == GPIO_PIN_SET)
    {
        i++;
        osDelay(1); // 每次循环延时 1ms
    }
    
    // 计算距离
    // i 是毫秒数，转换为微秒需 *1000
    // 公式：Distance(cm) = Time(us) / 58
    uint32_t Distance = i * 1000 / 58; 
    
    return Distance;
}

/**
  * @brief  测试函数：触发测距并在 OLED 上显示结果
  */
void Echo_Test(void)
{
    Echo_Trig();              // 触发测距
    // 在 OLED 第4列第5行显示距离，最大3位数，使用6x8字体
    OLED_ShowNum(4, 5, Echo_Get(), 3, OLED_6X8);
}