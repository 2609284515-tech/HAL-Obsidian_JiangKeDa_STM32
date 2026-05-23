#include "main.h"
#include "Delay.h"

#define SCL_PORT		GPIOB
#define SDA_PORT		GPIOB
#define DTIME			10

void MyI2C_W_SCL(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SCL_PORT, SCL_Pin, (GPIO_PinState)BitValue);
	Delay_us(DTIME);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SDA_PORT, SDA_Pin, (GPIO_PinState)BitValue);
	Delay_us(DTIME);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin(SDA_PORT, SDA_Pin);
	Delay_us(DTIME);
	return BitValue;
}

uint8_t MyI2C_R_SCL(void)
{
	uint8_t BitValue;
	BitValue = HAL_GPIO_ReadPin(SCL_PORT, SCL_Pin);
	Delay_us(DTIME);
	return BitValue;
}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
