#include "main.h"

/* TIM handle declaration */
TIM_HandleTypeDef    TimHandle;

/* Prescaler declaration */
uint32_t uwPrescalerValue = 0;

volatile uint32_t timer_counter = 0;
volatile uint8_t timer_flag = 0;

void us100_init()
{
	us100_gpio_init();
	us100_timer_init();
}

float us100_get_distance()
{
	float distance = 0;
	
	timer_counter = 0;
	timer_flag = 0;

	HAL_GPIO_WritePin(US100_GPIO_PORT, TRIG, GPIO_PIN_SET);
	DelayMs(1);
	HAL_GPIO_WritePin(US100_GPIO_PORT, TRIG, GPIO_PIN_RESET);
	
	while(HAL_GPIO_ReadPin(US100_GPIO_PORT,ECHO) == GPIO_PIN_RESET);
	
	HAL_TIM_Base_Start_IT(&TimHandle);
	
	while(timer_flag == 0);
	
	distance = (340*((float)timer_counter)*10*1/1000000)/2*1000;	//mm
	
	return(distance);
}

void us100_gpio_init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  	/* Enable GPIOB clock */
  	__HAL_RCC_GPIOB_CLK_ENABLE();
  
  	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = TRIG;
	HAL_GPIO_Init(US100_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(US100_GPIO_PORT, TRIG, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Pin = ECHO;
	HAL_GPIO_Init(US100_GPIO_PORT, &GPIO_InitStruct);
}

void us100_timer_init()
{
	/* Compute the prescaler value to have TIMx counter clock equal to 1 MHz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock / 1000000)-1);
	
	/*##-1- Configure the TIM peripheral #######################################*/ 
	/* Set TIMx instance */
	TimHandle.Instance = TIMx;
    
	/* Initialize TIMx peripheral as follow:
       + Period = 10 - 1
       + Prescaler = SystemCoreClock/10000 Note that APB clock = TIMx clock if
                     APB prescaler = 1.
       + ClockDivision = 0
       + Counter direction = Up
	*/
	TimHandle.Init.Period = 10-1;	//10us
	TimHandle.Init.Prescaler = uwPrescalerValue;
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	HAL_TIM_Base_Init(&TimHandle);
}

/**
  * @brief TIM MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	TIMx_CLK_ENABLE();
  
	/* The used GPIO (LED2 port) will be configured in the main program through
	LED2 initialization method */

	/*##-2- Configure the NVIC for TIMx ########################################*/
	/* Set the TIMx priority */
	HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);
  
	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(TIMx_IRQn);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	timer_counter++;
	if(HAL_GPIO_ReadPin(US100_GPIO_PORT,ECHO) != GPIO_PIN_SET)
	{
		HAL_TIM_Base_Stop_IT(&TimHandle);
		timer_flag = 1;
	}
}
