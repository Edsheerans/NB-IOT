#include "main.h"

GPIO_TypeDef* LED_PORT[LED_SUM] = {LED1_GPIO_PORT,LED2_GPIO_PORT,LED3_GPIO_PORT,
								   LED4_GPIO_PORT,LED5_GPIO_PORT,LED6_GPIO_PORT,
								   LED7_GPIO_PORT,LED8_GPIO_PORT,RGB_GPIO_PORT
							      };
const uint16_t LED_PIN[LED_SUM-1] = {LED1_PIN,LED2_PIN,LED3_PIN,
								   LED4_PIN,LED5_PIN,LED6_PIN,
								   LED7_PIN,LED8_PIN
								  };

void led_init(led_typedef led)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	switch(led)
	{
		case (LED1):
			{
				LED1_GPIO_CLK_ENABLE(); 
				GPIO_InitStruct.Pin = LED1_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], LED1_PIN, GPIO_PIN_SET); 
			}
			break;
		case (LED2):
			{
				LED2_GPIO_CLK_ENABLE(); 
				GPIO_InitStruct.Pin = LED2_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], LED2_PIN, GPIO_PIN_SET); 
			}
			break;
		case (LED3):
			{
				LED3_GPIO_CLK_ENABLE(); 
				GPIO_InitStruct.Pin = LED3_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], LED3_PIN, GPIO_PIN_SET); 
			}
			break;
		case (LED4):
			{
				LED4_GPIO_CLK_ENABLE();
				GPIO_InitStruct.Pin = LED4_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], LED4_PIN, GPIO_PIN_SET); 
			}
			break;
		case (LED5):
			{
				LED5_GPIO_CLK_ENABLE();
				GPIO_InitStruct.Pin = LED5_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], LED5_PIN, GPIO_PIN_SET); 
			}
			break;
		case (LED6):
			{
				LED6_GPIO_CLK_ENABLE();
				GPIO_InitStruct.Pin = LED6_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], LED6_PIN, GPIO_PIN_SET); 
			}
			break;
		case (LED7):
			{
				LED7_GPIO_CLK_ENABLE();
				GPIO_InitStruct.Pin = LED7_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], LED7_PIN, GPIO_PIN_SET); 
			}
			break;
		case (LED8):
			{
				LED8_GPIO_CLK_ENABLE();
				GPIO_InitStruct.Pin = LED8_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], LED8_PIN, GPIO_PIN_SET); 
			}
			break;
		case (RGB):
			{
				RGB_GPIO_CLK_ENABLE();
				GPIO_InitStruct.Pin = RGB_B_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], RGB_B_PIN, GPIO_PIN_SET);

				GPIO_InitStruct.Pin = RGB_R_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], RGB_B_PIN, GPIO_PIN_SET);
				
				GPIO_InitStruct.Pin = RGB_G_PIN;
				HAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
				HAL_GPIO_WritePin(LED_PORT[led], RGB_B_PIN, GPIO_PIN_SET);
			}
			break;
		default:
			break;
	}
}

void led_on(led_typedef led)
{
	HAL_GPIO_WritePin(LED_PORT[led], LED_PIN[led], GPIO_PIN_RESET); 
}

void led_off(led_typedef led)
{
	HAL_GPIO_WritePin(LED_PORT[led], LED_PIN[led], GPIO_PIN_SET); 
}

void led_toggle(led_typedef led)
{
	HAL_GPIO_TogglePin(LED_PORT[led], LED_PIN[led]);
}

void rgb_red(led_typedef led)
{
	HAL_GPIO_WritePin(LED_PORT[led], RGB_R_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT[led], RGB_B_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT[led], RGB_G_PIN, GPIO_PIN_SET);
}

void rgb_green(led_typedef led)
{
	HAL_GPIO_WritePin(LED_PORT[led], RGB_G_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT[led], RGB_R_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT[led], RGB_B_PIN, GPIO_PIN_SET);
}

void rgb_blue(led_typedef led)
{
	HAL_GPIO_WritePin(LED_PORT[led], RGB_B_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT[led], RGB_G_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT[led], RGB_R_PIN, GPIO_PIN_SET);
}

void rgb_off(led_typedef led)
{
	HAL_GPIO_WritePin(LED_PORT[led], RGB_R_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT[led], RGB_G_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_PORT[led], RGB_B_PIN, GPIO_PIN_SET);
}
