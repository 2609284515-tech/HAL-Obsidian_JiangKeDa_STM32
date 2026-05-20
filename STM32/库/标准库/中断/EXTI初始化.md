需要[[AFIO选择中断引脚]]
这里的Line就是AFIO选择中断引脚的PinSource。如果抖动，上拉下拉没啥区别
```
/*EXTI初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;    //这里的Line就是配置AFIO的PinSource
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
```


# HAL

在cube