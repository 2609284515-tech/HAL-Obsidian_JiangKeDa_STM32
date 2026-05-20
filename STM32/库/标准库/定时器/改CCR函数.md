需要[[OC初始化]]
```
//改CCR
void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}
```

# HAL库

```
//改CCR
void PWM_SetCompare1(uint16_t Compare)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Compare);
}
```