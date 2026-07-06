#include "main.h"
#include "cmsis_os2.h"
#include "MPU6050.h"

void StartAngleTask(void *argument)
{
    for(;;)
    {
        MPU6050_Angle();
        osDelay(10);
    }
}