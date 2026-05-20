需要[[开启GPIO时钟]]
多个Pin用|，或者All。传感器默认是灯亮，低电阻
```
/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
```
模式翻译
	GPIO_Mode_AIN 模拟
	GPIO_Mode_IN_FLOATING 浮空
	GPIO_Mode_IPD 下拉
	GPIO_Mode_IPU 上拉
	GPIO_Mode_Out_OD 开漏
	GPIO_Mode_Out_PP 推挽
	AF复用（内部外设控制接管ODR）
		GPIO_Mode_AF_OD
		GPIO_Mode_AF_PP

# HAL

在cube