需要[[开启AFIO时钟]]
PA15,PB3/4默认是调试口不能直接重映射，要解除调试复用
```
/*AFIO重映射引脚*/
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    //解除调试复用
```

# HAL

在cube