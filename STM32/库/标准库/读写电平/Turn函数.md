是[[检测电平]]后[[改变电平]]
```
//是检测电平后变相反电平
void xx_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
}
```

# HAL库

```
//是检测电平后变相反电平
void LED_Turn(void)
{
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	}
}
```

