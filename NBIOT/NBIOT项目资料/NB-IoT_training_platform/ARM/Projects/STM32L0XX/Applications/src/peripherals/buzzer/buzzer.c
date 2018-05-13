#include "main.h"

void buzzer_init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = BUZZER_CTL;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	BUZZER_GPIO_CLK_ENABLE();
	HAL_GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BUZZER_GPIO_PORT,BUZZER_CTL,GPIO_PIN_RESET);
}

void buzzer_on()
{
	HAL_GPIO_WritePin(BUZZER_GPIO_PORT,BUZZER_CTL,GPIO_PIN_SET);
}

void buzzer_off()
{
	HAL_GPIO_WritePin(BUZZER_GPIO_PORT,BUZZER_CTL,GPIO_PIN_RESET);
}
