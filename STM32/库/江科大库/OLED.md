4行16列，清除某一处只要写入空格，需要更改.c的引脚配置和引脚初始化

| 函数                                    | 作用         |
| ------------------------------------- | ---------- |
| OLED_Init();                          | 初始化        |
| OLED_Clear();                         | 清屏         |
| OLED_ShowChar(1, 1, 'A');             | 显示一个字符     |
| OLED_ShowString(1, 3, "HelloWorld!"); | 显示字符串      |
| OLED_ShowNum(2, 1, 12345, 5);         | 显示十进制数字    |
| OLED_ShowSignedNum(2, 7, -66, 2);     | 显示有符号十进制数字 |
| OLED_ShowHexNum(3, 1, 0xAA55, 4);     | 显示十六进制数字   |
| OLED_ShowBinNum(4, 1, 0xAA55, 16);    | 显示二进制数字    |

# HAL库
把GPIO在Cube设置
把型号的include改成main的include
把引脚配置改成HAL的
```
#define OLED_W_SCL(x)       HAL_GPIO_WritePin(GPIOB, OLED_SCL_Pin, (GPIO_PinState)(x))

#define OLED_W_SDA(x)       HAL_GPIO_WritePin(GPIOB, OLED_SDA_Pin, (GPIO_PinState)(x))
```
把init的时钟和gpio初始化全删了