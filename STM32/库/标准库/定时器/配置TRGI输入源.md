需要[[启动定时器]]，并把启动放在配置TRGI输入源之后
```
/*配置TRGI输入源*/
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
```

在TIxFPx中只有TI1FP1和TI2FP2可以触发TRGI

# HAL

在cube