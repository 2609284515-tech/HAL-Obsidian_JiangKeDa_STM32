#include "main.h"

extern SPI_HandleTypeDef hspi1;

void MySPI_W_SS(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, SPI1_SS_Pin, (GPIO_PinState)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, SPI1_SCK_Pin, (GPIO_PinState)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOA, SPI1_MOSI_Pin, (GPIO_PinState)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
	return HAL_GPIO_ReadPin(GPIOA, SPI1_MISO_Pin);
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ByteReceive = 0;
	
	HAL_SPI_TransmitReceive(&hspi1, &ByteSend, &ByteReceive, 1, HAL_MAX_DELAY);
	
	return ByteReceive;
}
