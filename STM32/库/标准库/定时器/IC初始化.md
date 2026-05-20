需要[[启动定时器]]
需要[[GPIO初始化]]
CH引脚要查[[引脚表.png]]
Indirect改的不是CCR是引脚
```
/*IC初始化*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
```

IC即输入捕获，当引脚接收跳变，将CNT存到CCR，但是作编码器接口就不会

默认初始化
```
TIM_ICStructInit(&TIM_ICInitStructure);
```

# HAL

在cube