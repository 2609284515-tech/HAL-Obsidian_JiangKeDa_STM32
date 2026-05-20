cube直接官网下
vs:https://www.bilibili.com/video/BV1QfbpzGENy

1. 打开 .ioc 文件，进入 **Pinout & Configuration** -> **System Core** -> **RCC**。
2. **High Speed Clock (HSE)** 选择 **Crystal/Ceramic Resonator** (假设你有外部晶振，通常是 8MHz)。
3. 点击顶部的 **Clock Configuration** 标签页。
4. 在 **HCLK (MHz)** 输入框中输入 **72**。
5. 按下回车，CubeMX 会自动计算 PLL 参数（例如 PLLMul x9），并确保所有总线频率合法。
    - _此时，你的系统主频变成了 72MHz。_

下一步[[安装Keil5]]