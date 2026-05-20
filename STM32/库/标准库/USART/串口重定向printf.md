需要[[开启USART]]
```
//串口重定向printf
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}
```

```
#include <stdio.h>
#include <stdarg.h>
```

# HAL

麻烦，直接放弃