#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "main.h"

#define BUZZER_CTL						GPIO_PIN_9
#define BUZZER_GPIO_PORT              	GPIOC
#define BUZZER_GPIO_CLK_ENABLE()       	__HAL_RCC_GPIOC_CLK_ENABLE()

void buzzer_init(void);

void buzzer_on(void);
	
void buzzer_off(void);

#endif

