#include "main.h"
#include "stm32f1xx_hal_tim.h"
#include "OLED.h"

#define TIM_Handler_1 htim2
#define TIM_Handler_2 htim4

extern TIM_HandleTypeDef TIM_Handler_1;
extern TIM_HandleTypeDef TIM_Handler_2;

void IC_Init(void)
{
    HAL_TIM_Encoder_Start(&TIM_Handler_1, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&TIM_Handler_1, TIM_CHANNEL_2);
    HAL_TIM_Encoder_Start(&TIM_Handler_2, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&TIM_Handler_2, TIM_CHANNEL_2);
}

//读编码器相位
//强制把CNT的无符号转成了有符号数
int16_t Encoder_Get_1(void)
{
	return __HAL_TIM_GET_COUNTER(&TIM_Handler_1);
}

int16_t Encoder_Get_2(void)
{
    return __HAL_TIM_GET_COUNTER(&TIM_Handler_2);
}

void Encoder_Reset(void)
{
    __HAL_TIM_SET_COUNTER(&TIM_Handler_1, 0);
    __HAL_TIM_SET_COUNTER(&TIM_Handler_2, 0);
}

void Encoder_Test(void)
{ 
    OLED_ShowSignedNum(1, 1, Encoder_Get_1(), 5, OLED_6X8);
    OLED_ShowSignedNum(2, 1, Encoder_Get_2(), 5, OLED_6X8);
}