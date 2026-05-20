需要[[IC初始化]]
需要[[配置TRGI输入源]]
需要[[配置从模式复位]]
```
//测周法读PWM频率
uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
```

测频法：在闸门时间T内，对上升沿计次，得到N，则频率 
$f_x = N / T$
测周法：两个上升沿内，以标准频率fc计次，得到N ，则频率
$f_x=f_c / N$
中界频率：测频法的相对误差与测周法相对误差相等的频率点$f_m=\sqrt{f_c / T}$
频率大用测频，频率小用测周

在测周法里，频率相对误差难算，于是要用周期相对误差近似，我们并不是“对周期差取倒数”来求频率差，而是利用**微分关系**：
$$
f = \frac{1}{T} \quad \Rightarrow \quad \mathrm{d}f = -\frac{1}{T^2}\,\mathrm{d}T
$$
因此，由周期测量误差 $\Delta T$（小量）引起的频率误差近似为：
$$
\Delta f \approx -\frac{1}{T^2} \Delta T
$$
取绝对值，并两边除以 $f = 1/T$，得到：
$$
\frac{|\Delta f|}{f} \approx \frac{|\Delta T|}{T}
$$
**频率相对误差 ≈ 周期相对误差。** 这个关系成立的前提是 $\Delta T$ 相对于 $T$很小，从而一阶近似足够精确。

在测周法中，$\Delta T = \pm 1/f_0$（$f_0$ 是时标频率，通常高达 MHz 甚至 GHz），而 $T$ 是被测信号的周期（比如 ms 到 μs 级）。比值 $\Delta T / T$ 确实很小，所以微分近似非常精确。

测周法的最小频率就是一个周期CNT刚好记满65535，最大频率取决于需要的相对误差，如千分之一就是一个周期要记大于等于1000个数（忽略误差）

# HAL库

```
//测周法读PWM频率
uint32_t IC_GetFreq(void)
{
	return 1000000 / (HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1) + 1);
}
```