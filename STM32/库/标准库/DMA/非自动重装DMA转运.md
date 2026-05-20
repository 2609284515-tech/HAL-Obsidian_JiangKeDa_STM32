需要[[DMA初始化]]
```
//DMA转运
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
```

# HAL库

```
//DMA转运
void MyDMA_Transfer(uint32_t AddrA, uint32_t AddrB, uint32_t MyDMA_Size)
{
    HAL_DMA_Start(&hdma_memtomem_dma1_channel1, AddrA, AddrB, MyDMA_Size);

    HAL_DMA_PollForTransfer(&hdma_memtomem_dma1_channel1, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
}
```