#include "main.h"
#include "PWM.h"
#include "Delay.h"


void Motor_Init(void)
{
	PWM_Init();
}

//电机速度
void Motor_SetSpeed_1(int16_t Speed)
{
	if (Speed >= 0)
	{
		HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
		PWM_SetCompare4(Speed);
	}
	else 
	{
		HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
		PWM_SetCompare4(-Speed);
	}
}

void Motor_SetSpeed_2(int16_t Speed)
{
	if (Speed >= 0)
	{
		HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
		PWM_SetCompare1(Speed);
	}
	else
	{
		HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
		PWM_SetCompare1(-Speed);
	}
}

void Motor_Test(void)
{
	static uint16_t Flag;
	if (Flag)
	{
		;
	}
	else
	{
		Motor_SetSpeed_1(10);
		Motor_SetSpeed_2(10);
		Delay_ms(100);
		Motor_SetSpeed_1(0);
		Motor_SetSpeed_2(0);
		Flag = 1;
	}
}