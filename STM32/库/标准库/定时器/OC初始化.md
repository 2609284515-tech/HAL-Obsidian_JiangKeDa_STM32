需要[[启动定时器]]
需要[[GPIO初始化]]
这里的OC通道引脚要查[[引脚表.png]]
```
/*OC初始化*/
	TIM_OCInitTypeDef TIM_OCInitstructure;
	TIM_OCStructInit(&TIM_OCInitstructure);
	TIM_OCInitstructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitstructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitstructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitstructure.TIM_Pulse = 90;
	TIM_OC1Init(TIM2, &TIM_OCInitstructure);
```

模式翻译
	TIM_OCMode_Timing 冻结
	TIM_OCMode_Active 匹配时置有效电平
	TIM_OCMode_Inactive 匹配时置无效电平
	TIM_OCMode_Toggle 匹配时电平翻转
	TIM_OCMode_PWM1 PWM模式1
	TIM_OCMode_PWM2 PWM模式2

输出比较即OC，比较CNT与CCR从而输出PWM波形，不能在基本定时器上使用
PWM用来在惯性系统中通过控制数字量脉冲获得模拟量，或者作为通信协议
PWM参数：频率，占空比（高电平占比），分辨率（占空比变化步距）

| 模式       | 描述                                  |
| -------- | ----------------------------------- |
| 冻结       | CNT=CCR时，REF保持为原状态                  |
| 匹配时置有效电平 | CNT=CCR时，REF置有效电平                   |
| 匹配时置无效电平 | CNT=CCR时，REF置无效电平                   |
| 匹配时电平翻转  | CNT=CCR时，REF电平翻转                    |
| 强制为无效电平  | CNT与CCR无效，REF强制为无效电平                |
| 强制为有效电平  | CNT与CCR无效，REF强制为有效电平                |
| PWM模式1   | CNT<CCR时，REF置有效电平，CNT≥CCR时，REF置无效电平 |
| PWM模式2   | CNT<CCR时，REF置无效电平，CNT≥CCR时，REF置有效电平 |
PWM频率：	Freq = CK_PSC / (PSC + 1) / (ARR + 1)
PWM占空比：	Duty = CCR / (ARR + 1)
PWM分辨率：	Reso = 1 / (ARR + 1)

比如呼吸灯，PSC=720-1，ARR=100，这样频率1kHz
舵机，频率50Hz，PSC=72-1，ARR=20000-1，这样PCC为微秒
电机，PSC=36-1，ARR=100-1，这样20kHz

# HAL

在cube