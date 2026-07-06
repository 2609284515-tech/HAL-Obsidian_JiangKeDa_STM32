#include "main.h"

// 声明外部 DMA 句柄，用于内存到内存 (M2M) 的传输
extern DMA_HandleTypeDef hdma_memtomem_dma1_channel1;

/**
  * @brief  执行 DMA 内存到内存的数据转运
  * @param  AddrA: 源数据地址
  * @param  AddrB: 目标数据地址
  * @param  MyDMA_Size: 需要传输的数据量（单位取决于 DMA 配置，通常为字、半字或字节）
  * @retval None
  */
void MyDMA_Transfer(uint32_t AddrA, uint32_t AddrB, uint32_t MyDMA_Size)
{
    // 启动 DMA 传输，指定源地址、目标地址和数据大小
    HAL_DMA_Start(&hdma_memtomem_dma1_channel1, AddrA, AddrB, MyDMA_Size);

    // 等待 DMA 传输完成
    // HAL_DMA_FULL_TRANSFER: 等待整个传输过程结束
    // HAL_MAX_DELAY: 超时时间为无限等待，直到传输完成
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
}