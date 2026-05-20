需要[[开启AFIO时钟]]
需要[[GPIO初始化]]
每个Pin只能对应一个GPIOx，就算是同一个GPIO也不能用|
```
/*AFIO选择中断引脚*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
```

# HAL

在cube