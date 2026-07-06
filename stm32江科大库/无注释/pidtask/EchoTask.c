#include "main.h"
#include "cmsis_os2.h"
#include "Echo.h"
#include "OLED.h"

uint8_t Distance;

void StartEchoTask(void *argument)
{
    // OLED_ShowString(4, 1, "Dis:");
    for(;;)
    {
        // Echo_Trig();
        // Distance = Echo_Get();
        // OLED_ShowNum(4, 5, Distance, 3);
        osDelay(100);
    }
}