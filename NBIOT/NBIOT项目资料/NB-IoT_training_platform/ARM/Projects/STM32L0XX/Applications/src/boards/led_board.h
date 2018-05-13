#ifndef __LED_BOARD_H__
#define __LED_BOARD_H__

#include "main.h"

typedef enum 
{
	LED1 = 0,
	LED2 ,
	LED3 ,
	LED4 ,
	LED5 ,
	LED6 ,
	LED7 ,
	LED8 ,
	RGB 	
} led_typedef;

#define LED_SUM								9								//9×éLEDµÆ

#define LED1_PIN							GPIO_PIN_5
#define	LED1_GPIO_PORT						GPIOA
#define LED1_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOA_CLK_ENABLE()  
#define LED1_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOA_CLK_DISABLE()

#define LED2_PIN							GPIO_PIN_4
#define	LED2_GPIO_PORT						GPIOC
#define LED2_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED2_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOC_CLK_DISABLE()

#define LED3_PIN							GPIO_PIN_3
#define	LED3_GPIO_PORT						GPIOC
#define LED3_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOC_CLK_DISABLE()

#define LED4_PIN							GPIO_PIN_2
#define	LED4_GPIO_PORT						GPIOC
#define LED4_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOC_CLK_DISABLE()

#define LED5_PIN							GPIO_PIN_1
#define	LED5_GPIO_PORT						GPIOC
#define LED5_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED5_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOC_CLK_DISABLE()

#define LED6_PIN							GPIO_PIN_0
#define	LED6_GPIO_PORT						GPIOC
#define LED6_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED6_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOC_CLK_DISABLE()

#define LED7_PIN							GPIO_PIN_11
#define	LED7_GPIO_PORT						GPIOC
#define LED7_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED7_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOC_CLK_DISABLE()

#define LED8_PIN							GPIO_PIN_10
#define	LED8_GPIO_PORT						GPIOC
#define LED8_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOC_CLK_ENABLE()  
#define LED8_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOC_CLK_DISABLE()

#define RGB_B_PIN							GPIO_PIN_14
#define	RGB_GPIO_PORT						GPIOB
#define RGB_GPIO_CLK_ENABLE()           	__HAL_RCC_GPIOB_CLK_ENABLE()  
#define RGB_GPIO_CLK_DISABLE()          	__HAL_RCC_GPIOB_CLK_DISABLE()

#define RGB_R_PIN							GPIO_PIN_13

#define RGB_G_PIN							GPIO_PIN_12

void led_init(led_typedef led);
void led_on(led_typedef led);
void led_off(led_typedef led);
void led_toggle(led_typedef led);
void rgb_red(led_typedef led);
void rgb_green(led_typedef led);
void rgb_blue(led_typedef led);
void rgb_off(led_typedef led);

#endif
