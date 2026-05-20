需要[[OC初始化]]
```
//改PSC
void PWM_SetPrescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);
}
```

# HAL库

```
void PWM_SetPrescaler(uint16_t Prescaler)
{
	__HAL_TIM_SET_PRESCALER(&htim2, Prescaler);
}
```