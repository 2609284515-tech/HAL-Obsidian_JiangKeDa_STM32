EXTI通道如果不是15~10就写作 EXTIx_IRQn，TIM通道写作 TIMx_IRQn。如果想配多个通道不用再建结构体，直接覆盖原结构体即可
```
/*NVIC配置*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
```

响应优先级(SubPriority)是抢占优先级(PreemptionPriority)的子优先级，在抢占相同时才比较响应。这个分组整个工程只能分一次

| 分组方式 | 抢占优先级      | 响应优先级      |
| ---- | ---------- | ---------- |
| 分组0  | 0位，取值为0    | 4位，取值为0~15 |
| 分组1  | 1位，取值为0~1  | 3位，取值为0~7  |
| 分组2  | 2位，取值为0~3  | 2位，取值为0~3  |
| 分组3  | 3位，取值为0~7  | 1位，取值为0~1  |
| 分组4  | 4位，取值为0~15 | 0位，取值为0    |
# HAL

在cube