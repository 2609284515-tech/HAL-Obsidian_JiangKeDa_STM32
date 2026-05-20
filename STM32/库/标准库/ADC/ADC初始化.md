需要[[开启ADC时钟]]
需要[[ADC分频]]
需要[[配置ADC规则组通道]]
```
/*ADC初始化*/
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitStructure);
```

连续模式是读完了继续读，扫描是读整个序列
如果连续，触发ADC就放在[[Init函数]]最后

```
/*触发ADC*/
ADC_SoftwareStartConvCmd(ADC1, ENABLE);
```

# HAL

在cube