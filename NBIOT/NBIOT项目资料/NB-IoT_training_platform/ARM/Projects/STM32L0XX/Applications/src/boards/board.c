#include "main.h"

__IO ITStatus data_Ready = RESET;

__IO ITStatus key1_flag = RESET;
__IO ITStatus key2_flag = RESET;
__IO ITStatus key3_flag = RESET;

/* ADC handle declaration */
ADC_HandleTypeDef             AdcHandle;

/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef        sConfig;

/* Variable used to get converted value */
//__IO uint32_t adc1_converted_value = 0;

/* Timer handler declaration */
extern TIM_HandleTypeDef        TimHandle;

void system_clock_config(void)
{
  	RCC_ClkInitTypeDef RCC_ClkInitStruct;
 	RCC_OscInitTypeDef RCC_OscInitStruct;

#ifdef US100
	/* Configure the system clock to 32 Mhz */
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
  
	/* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSI Oscillator and activate PLL with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
	RCC_OscInitStruct.HSICalibrationValue = 0x10;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);  
  
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
#else
	/* Configure the system clock to 2 Mhz */
  	/* Enable Power Control clock */
  	__HAL_RCC_PWR_CLK_ENABLE();
  
  	/* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
 	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  
  	/* Enable MSI Oscillator */
  	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  	RCC_OscInitStruct.MSICalibrationValue=0x00;
  	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  	{
    	/* Initialization Error */
  	}
    
  	/* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  	{
    	/* Initialization Error */
  	}
#endif
}

void io_interrupt_config(void)
{
  	GPIO_InitTypeDef   GPIO_InitStructure;

  	/* Enable GPIOC clock */
  	__HAL_RCC_GPIOC_CLK_ENABLE();
  
  	/* Configure PC13 pin as input floating */
  	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

  	/* Enable and set EXTI4_15 Interrupt to the lowest priority */
  	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 3, 0);
 	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void adc1_gpio_init(ADC_HandleTypeDef *hadc)
{
	GPIO_InitTypeDef                 GPIO_InitStruct;
  
  	/*##-1- Enable peripherals and GPIO Clocks #################################*/
 	/* Enable GPIO clock ****************************************/
 	__HAL_RCC_GPIOA_CLK_ENABLE();
  	/* ADC1 Periph clock enable */
 	__HAL_RCC_ADC1_CLK_ENABLE();
  
  	/*##- 2- Configure peripheral GPIO #########################################*/
  	/* ADC3 Channel8 GPIO pin configuration */
 	GPIO_InitStruct.Pin = GPIO_PIN_0;
  	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  	GPIO_InitStruct.Pull = GPIO_NOPULL;
  	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  	/*##-3- Configure the NVIC #################################################*/
  	/* NVIC configuration for ADC EOC interrupt */
  	HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
  	HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);	
}

void adc1_init()
{
	/* ### - 1 - Initialize ADC peripheral #################################### */
  	/*
   	*  Instance                  = ADC1.
   	*  OversamplingMode          = Disabled
   	*  ClockPrescaler            = PCLK clock with no division.
   	*  LowPowerAutoPowerOff      = Disabled (For this exemple continuous mode is enabled with software start)
   	*  LowPowerFrequencyMode     = Enabled (To be enabled only if ADC clock is lower than 2.8MHz) 
   	*  LowPowerAutoWait          = Disabled (New conversion starts only when the previous conversion is completed)       
   	*  Resolution                = 12 bit (increased to 16 bit with oversampler)
   	*  SamplingTime              = 7.5 cycles od ADC clock.
   	*  ScanConvMode              = forward
   	*  DataAlign                 = Right
   	*  ContinuousConvMode        = Enabled
   	*  DiscontinuousConvMode     = Enabled
   	*  ExternalTrigConvEdge      = None (Software start)
   	*  EOCSelection              = End Of Conversion event
   	*  DMAContinuousRequests     = DISABLE
   	*/

  	AdcHandle.Instance = ADC1;
  
  	AdcHandle.Init.OversamplingMode      = DISABLE;
  
  	AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
  	AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
  	AdcHandle.Init.LowPowerFrequencyMode = ENABLE;
  	AdcHandle.Init.LowPowerAutoWait      = DISABLE;
    
  	AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  	AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_7CYCLES_5;
  	AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  	AdcHandle.Init.ContinuousConvMode    = ENABLE;
  	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  	AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  	AdcHandle.Init.DMAContinuousRequests = DISABLE;

	adc1_gpio_init(&AdcHandle);
 
  	/* Initialize ADC peripheral according to the passed parameters */
  	HAL_ADC_Init(&AdcHandle);

}

void adc1_set_channel0()
{
	/* Select Channel 0 to be converted */
  	sConfig.Channel = ADC_CHANNEL_0;    
  	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);
}

void adc1_start()
{
	HAL_ADC_Start_IT(&AdcHandle);
}

void adc1_stop()
{
	HAL_ADC_Stop_IT(&AdcHandle);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  	if(GPIO_Pin == GPIO_PIN_1)
  	{
		key1_flag = SET;
 	}
		
	if(GPIO_Pin == GPIO_PIN_6)
  	{
		data_Ready = SET;
 	}
	
	if(GPIO_Pin == GPIO_PIN_12)
  	{
		key3_flag = SET;
 	}
	
	if(GPIO_Pin == GPIO_PIN_13)
  	{
		key2_flag = SET;
 	}
	
}

/**
  * @brief  This function handles ADC interrupt request.
  * @param  None
  * @retval None
  */
void ADC1_COMP_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&AdcHandle);
}

void EXTI0_1_IRQHandler(void)
{
  	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

/**
  * @brief  This function handles External lines 15 to 4 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6) != RESET) 
	{ 
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_6);
	}
	
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET) 
	{ 
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_12);
	}
	
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET) 
	{ 
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_13);
	}
	
}

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
//   while (1)
//  {
//  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
	
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
	
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
	
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
}

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIMx_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TimHandle);
}

