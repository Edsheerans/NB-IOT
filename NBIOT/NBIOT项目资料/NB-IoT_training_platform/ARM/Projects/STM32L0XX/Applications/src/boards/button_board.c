#include "main.h"

GPIO_TypeDef* BUTTON_PORT[BUTTON_SUM] = {KEY1_BUTTON_GPIO_PORT,KEY2_BUTTON_GPIO_PORT,KEY3_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN[BUTTON_SUM] = {KEY1_BUTTON_PIN,KEY2_BUTTON_PIN,KEY3_BUTTON_PIN}; 
const uint8_t BUTTON_IRQn[BUTTON_SUM] = {KEY1_BUTTON_EXTI_IRQn,KEY2_BUTTON_EXTI_IRQn,KEY3_BUTTON_EXTI_IRQn};

void button_init(button_typedef button)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  
	GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	
	switch(button)
	{
		case (KEY1):
			{
				KEY1_BUTTON_GPIO_CLK_ENABLE();  
				GPIO_InitStruct.Pin = KEY1_BUTTON_PIN;
				HAL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct);
				NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[button]), 0x03);
				HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[button]));				
			}
			break;
		case (KEY2):
			{
				KEY2_BUTTON_GPIO_CLK_ENABLE();  
				GPIO_InitStruct.Pin = KEY2_BUTTON_PIN;
				HAL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct);
				NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[button]), 0x03);
				HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[button]));
			}
			break;
		case (KEY3):
			{
				KEY3_BUTTON_GPIO_CLK_ENABLE();  
				GPIO_InitStruct.Pin = KEY3_BUTTON_PIN;
				HAL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct);
				NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[button]), 0x03);
				HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[button]));
			}
			break;
		default:
			break;
	}
}

