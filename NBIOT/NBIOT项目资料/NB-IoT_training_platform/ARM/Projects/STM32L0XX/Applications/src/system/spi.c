#include "main.h"

extern SPI_HandleTypeDef SpiHandle;

void spi_gpio_init(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
  
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	SPIx_SCK_GPIO_CLK_ENABLE();
	SPIx_MISO_GPIO_CLK_ENABLE();
	SPIx_MOSI_GPIO_CLK_ENABLE();
	SPIx_NSS_GPIO_CLK_ENABLE();
	
	/* Enable SPI clock */
	SPIx_CLK_ENABLE(); 
  
	/* -2- Configure PC_5 IO in output push-pull mode */
	GPIO_InitStruct.Pin = SPIx_NSS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
	HAL_GPIO_Init(SPIx_NSS_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SPIx_NSS_GPIO_PORT,SPIx_NSS_PIN,GPIO_PIN_SET);//Ƭѡ�ź��ø�	
	
	/*##-2- Configure peripheral GPIO ##########################################*/  
	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = SPIx_SCK_AF;
  
	HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);
    
	/* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.Pin = SPIx_MISO_PIN;
	GPIO_InitStruct.Alternate = SPIx_MISO_AF;
  
	HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
  
	/* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
	GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    
	HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

	/*##-3- Configure the NVIC for SPI #########################################*/
	/* NVIC for SPI */
	HAL_NVIC_SetPriority(SPIx_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(SPIx_IRQn);
}


void spi_gpio_deinit(SPI_HandleTypeDef *hspi)
{
  /*##-1- Reset peripherals ##################################################*/
  SPIx_FORCE_RESET();
  SPIx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure SPI SCK as alternate function  */
  HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
  /* Configure SPI MISO as alternate function  */
  HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
  /* Configure SPI MOSI as alternate function  */
  HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
  
  /*##-3- Disable the NVIC for SPI ###########################################*/
  HAL_NVIC_DisableIRQ(SPIx_IRQn);
}

/**
  * @brief  TxRx Transfer completed callback
  * @param  hspi: SPI handle. 
  * @note   This example shows a simple way to report end of Interrupt TxRx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
 /*...*/
}

/**
  * @brief  SPI error callbacks
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
 void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{

}


/**
  * @brief  This function handles SPI interrupt request.  
  * @param  None
  * @retval None  
  */
void SPIx_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&SpiHandle);
}


