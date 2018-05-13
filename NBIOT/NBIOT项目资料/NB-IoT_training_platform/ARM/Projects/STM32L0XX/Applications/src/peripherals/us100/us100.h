#ifndef __US100_H__
#define __US100_H__

#include "main.h"

#define TRIG						GPIO_PIN_10
#define ECHO						GPIO_PIN_11
#define US100_GPIO_PORT				GPIOB

#define TIMx                       	TIM6
#define TIMx_CLK_ENABLE             __HAL_RCC_TIM6_CLK_ENABLE

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                   TIM6_DAC_IRQn
#define TIMx_IRQHandler             TIM6_DAC_IRQHandler

void us100_init(void);

float us100_get_distance(void);

void us100_gpio_init(void);

void us100_timer_init(void);

#endif
