需要[[USART初始化]]
永远放在[[Init函数]]最后
```
USART_Cmd(USART1, ENABLE);
```

# HAL

不用开启，在初始化后就自动开启