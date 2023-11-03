#include "key.h"


uint8_t Floor_Key_Scan()
{
	uint8_t unKey_Val = 0;
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
		unKey_Val = 1;

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13) == GPIO_PIN_RESET)
		unKey_Val = 2;

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14) == GPIO_PIN_RESET)
		unKey_Val = 3;
	
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET)	
		unKey_Val = 4;	
	
	
	return unKey_Val;
}

uint8_t Velocity_Key_Scan()
{
	uint8_t unKey_Val = 0;
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
		unKey_Val = 1;

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
		unKey_Val = 2;

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
		unKey_Val = 3;
	
	return unKey_Val;
}