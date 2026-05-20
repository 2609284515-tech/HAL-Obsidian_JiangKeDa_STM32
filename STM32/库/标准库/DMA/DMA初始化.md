
```
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStructure.DMA_BufferSize = Size;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
```

硬件触发通道要查[[DMA硬件触发.png]]
软件触发时不能自动重装
M2M就是软件触发，就是开启DMA会立刻转运
转运ADC1->DR时不用自增
BufferSize会在转运时自减

# HAL库

在cube
要把NVIC的Force DMA channels Interrupts关了