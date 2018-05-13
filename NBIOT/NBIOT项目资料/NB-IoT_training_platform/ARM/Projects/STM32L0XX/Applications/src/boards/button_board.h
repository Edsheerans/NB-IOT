#ifndef __BUTTON_BOARD_H__
#define __BUTTON_BOARD_H__

#include "main.h"

typedef enum 
{
	KEY1 = 0,
	KEY2,
	KEY3
} button_typedef;

#define BUTTON_SUM								3

#define KEY1_BUTTON_PIN                         GPIO_PIN_1
#define KEY1_BUTTON_GPIO_PORT                   GPIOA
#define KEY1_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()   
#define KEY1_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOA_CLK_DISABLE()  
#define KEY1_BUTTON_EXTI_LINE                   GPIO_PIN_1
#define KEY1_BUTTON_EXTI_IRQn                   EXTI0_1_IRQn

#define KEY2_BUTTON_PIN                         GPIO_PIN_13
#define KEY2_BUTTON_GPIO_PORT                   GPIOC
#define KEY2_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()   
#define KEY2_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()  
#define KEY2_BUTTON_EXTI_LINE                   GPIO_PIN_13
#define KEY2_BUTTON_EXTI_IRQn                   EXTI4_15_IRQn

#define KEY3_BUTTON_PIN                         GPIO_PIN_12
#define KEY3_BUTTON_GPIO_PORT                   GPIOC
#define KEY3_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()   
#define KEY3_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()  
#define KEY3_BUTTON_EXTI_LINE                   GPIO_PIN_12
#define KEY3_BUTTON_EXTI_IRQn                   EXTI4_15_IRQn

void button_init(button_typedef button);

#endif
