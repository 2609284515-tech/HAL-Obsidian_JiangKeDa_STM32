是[[检测电平]]
包含消除抖动，[[检测电平]]前后延时，在只有一个时名字变成Get，如果不需要turn则只需要return Read就行
```
//按键获取键码1~2，不按为0，包含消除抖动，检测电平前后延时
//是阻塞式操作，按住不放时会卡住，直到松手
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
		Delay_ms(20);
		KeyNum = 1;
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20);
		KeyNum = 2;
	}
	
	return KeyNum;
}
```

而且通常在main里又建一个变量，为了只调用这个代码一次
```
uint8_t KeyNum;
```

```
KeyNum = Key_GetNum();
```

# HAL库

```
uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0; 

    if (HAL_GPIO_ReadPin(GPIOB, Key_1_Pin) == 0)
    {
        HAL_Delay(20);
        while (HAL_GPIO_ReadPin(GPIOB, Key_1_Pin) == 0);
        
        HAL_Delay(20);
        KeyNum = 1;
    }

    if (HAL_GPIO_ReadPin(GPIOB, Key_2_Pin) == 0)
    {
        HAL_Delay(20);
        while (HAL_GPIO_ReadPin(GPIOB, Key_2_Pin) == 0);
        
        HAL_Delay(20);
        KeyNum = 2;
    }
    
    return KeyNum;
}
```