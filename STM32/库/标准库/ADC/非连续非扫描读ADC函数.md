需要[[校准ADC]]
```
//获取ADC
uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}
```

可以把[[配置ADC规则组通道]]放在这

# HAL库

```
uint16_t AD_GetValue(void)
{
    uint16_t adcValue;

    HAL_ADC_Start(&hadc1);

    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    HAL_ADC_Stop(&hadc1);

    return adcValue;
}
```