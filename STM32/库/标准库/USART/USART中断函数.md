需要[[配置串口接收中断]]
需要[[NVIC配置]]
这是一个[[中断函数]]
```
//因为主时钟足够快，所以会在下一次中断前清零标志位，并把Data转移
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET && Serial_RxFlag == 0)
	{
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
```

```
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}
```

# HAL

```
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1 && Serial_RxFlag == 0)
    {
        Serial_RxFlag = 1;
        
        HAL_UART_Receive_IT(&huart1, &Serial_RxData, 1);
    }
}
```

```
void Serial_Init(void)
{
    HAL_UART_Receive_IT(&huart1, &Serial_RxData, 1);
}
```