需要[[EXTI初始化]]
需要[[NVIC配置]]
这是一个[[中断函数]]
如果不是15~10就写作 EXTIx_IRQHandler
不是15~10也要判断标志位
如果15~10有多个通道，就并列if在函数里
如果想要上下沿都触发就把电平检测删了，只保留延时
# 消抖版本
```
//因为抖动所以手写了一个下降沿触发，延时消除抖动
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		Delay_ms(20);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)    //下降沿触发
		{
			xx_Count ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
```

# 不需要消抖的版本
```
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		xx_Count ++;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
```

# 旋转编码器版本
旋转编码器A，B均从高电位开始，正转A提前90度，反转B提前90度

```
//是旋转编码器中断，其中的Line0是A，Line1是B，顺时针转B的下降沿对应A的低电位，逆时针转A的下降沿对应B的低电位，Count正对应顺时针
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			Encoder_Count --;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			Encoder_Count ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
```

# HAL库

# 消抖版本

```
//因为抖动所以手写了一个下降沿触发，延时消除抖动
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == CountSensor_Pin)
    {
        Delay_ms(20);
		if (HAL_GPIO_ReadPin(GPIOB, CountSensor_Pin) == 0)    //下降沿触发
		{
			CountSensor_Count ++;
		}
    }
}
```

# 不需要消抖的版本

```
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == CountSensor_Pin)
    {
		CountSensor_Count ++;
    }
}
```

# 旋转编码器版本

```
//是旋转编码器中断，其中的Line0是A，Line1是B，顺时针转B的下降沿对应A的低电位，逆时针转A的下降沿对应B的低电位，Count正对应顺时针
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == Encoder_A_Pin)
    {
        if (HAL_GPIO_ReadPin(GPIOB, Encoder_B_Pin) == GPIO_PIN_RESET)
        {
            Encoder_Count--;
        }
    }
    else if (GPIO_Pin == Encoder_B_Pin)
    {
        if (HAL_GPIO_ReadPin(GPIOB, Encoder_A_Pin) == GPIO_PIN_RESET)
        {
            Encoder_Count++;
        }
    }
}
```