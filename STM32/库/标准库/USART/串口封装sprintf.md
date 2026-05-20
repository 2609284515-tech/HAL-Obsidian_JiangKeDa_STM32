需要[[开启USART]]
```
/*串口封装sprintf*/
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}
```

```
#include <stdio.h>
#include <stdarg.h>
```