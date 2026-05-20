#include "main.h"
#include <stdint.h>

// 声明外部 ADC 句柄，通常由 CubeMX 在 main.c 或 adc.c 中定义
extern ADC_HandleTypeDef hadc1;

/**
 * @brief ADC 初始化及校准
 * @note 在执行 ADC 转换之前进行校准可以提高转换精度。
 *       HAL_ADCEx_Calibration_Start 会执行单次校准过程。
 *       注意：校准期间 ADC 必须处于禁用状态（即未启动转换）。
 */
void AD_Init(void)
{
    // 启动 ADC1 的校准程序
    // 此函数是阻塞式的，直到校准完成才返回
    HAL_ADCEx_Calibration_Start(&hadc1);
}

/**
 * @brief 获取单次 ADC 转换值
 * @return uint16_t ADC 转换结果（例如 12位 ADC 返回 0~4095）
 * @note 这是一个阻塞式函数。它会启动转换，等待转换完成，读取数据，然后停止 ADC。
 *       适用于低频采样或对实时性要求不高的场景。
 *       如果在高频循环中调用，可能会占用大量 CPU 时间。
 */
uint16_t AD_GetValue(void)
{
    uint16_t adcValue; // 用于存储最终的 ADC 转换结果

    // 1. 启动 ADC 转换
    // 对于单次模式，这会触发一次转换序列
    HAL_ADC_Start(&hadc1);

    // 2. 等待转换完成
    // HAL_MAX_DELAY 表示无限等待，直到转换结束标志位置位
    // 如果硬件出现故障导致标志位永不置位，程序将在此处死锁
    // 在实际工程中，建议设置一个合理的超时时间（如 10ms）
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    
    // 3. 读取转换结果
    // 从 ADC 数据寄存器 (DR) 中读取已转换完成的数值
    adcValue = HAL_ADC_GetValue(&hadc1);

    // 4. 停止 ADC
    // 关闭 ADC 以节省功耗，并为下一次启动做准备
    // 在单次非连续模式下，每次读取后停止是好习惯
    HAL_ADC_Stop(&hadc1);

    return adcValue;
}