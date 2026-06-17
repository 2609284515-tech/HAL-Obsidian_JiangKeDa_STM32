#include "main.h"
#include "delay.h"
#include "OLED.h"
#include "cmsis_os2.h"

void Echo_Trig(void)
{
    HAL_GPIO_WritePin(HC_TRIG_GPIO_Port, HC_TRIG_Pin, GPIO_PIN_SET);
    osDelay(1);
    HAL_GPIO_WritePin(HC_TRIG_GPIO_Port, HC_TRIG_Pin, GPIO_PIN_RESET);
}

uint32_t Echo_Get(void)
{
    uint16_t i = 0;
    while (HAL_GPIO_ReadPin(HC_ECHO_GPIO_Port, HC_ECHO_Pin) == GPIO_PIN_RESET);
    osDelay(1);
    while (HAL_GPIO_ReadPin(HC_ECHO_GPIO_Port, HC_ECHO_Pin) == GPIO_PIN_SET)
    {
        i++;
        osDelay(1);
    }
    uint32_t Distance = i * 1000 / 58; //步长是17，不满17会舍去
    
    return Distance;
}

void Echo_Test(void)
{
    Echo_Trig();
    OLED_ShowNum(4, 5, Echo_Get(), 3, OLED_6X8);
}