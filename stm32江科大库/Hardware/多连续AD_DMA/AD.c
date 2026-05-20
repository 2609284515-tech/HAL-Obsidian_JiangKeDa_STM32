#include "main.h"
#include <stdint.h>

// 声明外部 ADC 句柄，通常由 CubeMX 在 main.c 或 adc.c中定义
extern ADC_HandleTypeDef hadc1;

// 声明 ADC 数据缓冲区数组
// 用于存储 DMA 传输过来的 ADC 转换结果
// 数组大小应至少等于 HAL_ADC_Start_DMA 中指定的长度
uint16_t AD_Value[2];

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
 * @brief 启动 ADC 并通过 DMA 获取数据
 * @note 配置 ADC1 以连续/扫描模式运行，并将转换结果通过 DMA 直接传输到内存数组。
 * 
 * @param hadc1: ADC 句柄指针
 * @param (uint32_t *)AD_Value: 目标内存地址，用于存储转换结果。
 *                              注意：HAL 库要求此处传入 uint32_t* 类型指针，
 *                              即使实际数据存储为 uint16_t。DMA 会自动处理数据宽度。
 * @param 2: 要转换的数据长度（即通道数量或序列长度）。
 *           这里表示期望接收 2 个 ADC 转换结果（例如两个不同通道的值，或同一通道的两次采样）。
 * 
 * @warning 确保 AD_Value 数组的大小至少为 2，否则会导致内存溢出。
 *          通常在 CubeMX 配置中，ADC 需设置为 "Scan Conversion Mode" (扫描模式) 
 *          和 "Continuous Conversion Mode" (连续转换模式)，并启用 DMA Continuous Requests。
 */
void AD_StartGetValue_DMA(void)
{
    // 启动 ADC 并启用 DMA 传输
    // 数据将自动存入 AD_Value[0] 和 AD_Value[1]
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)AD_Value, 2);
}