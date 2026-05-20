需要[[校准ADC]]
需要[[DMA初始化]]
```
ADC_DMACmd(ADC1, ENABLE);
ADC_SoftwareStartConvCmd(ADC1, ENABLE);
```
# HAL

cmd自动执行
而start改成
```
HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcValue, 2);
```