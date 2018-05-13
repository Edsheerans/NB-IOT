#include "main.h"

/* UART handler declaration */
UART_HandleTypeDef UartHandle_uart1;
UART_HandleTypeDef UartHandle_uart2;

__IO ITStatus UartReady = RESET;

void uart1_gpio_init(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USART1_TX_GPIO_CLK_ENABLE();
  USART1_RX_GPIO_CLK_ENABLE();
  /* Enable USART2 clock */
  USART1_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART1_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USART1_TX_AF;
  
  HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USART1_RX_PIN;
  GPIO_InitStruct.Alternate = USART1_RX_AF;
    
  HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void uart1_gpio_deinit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USART1_FORCE_RESET();
  USART1_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USART1_TX_GPIO_PORT, USART1_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USART1_RX_GPIO_PORT, USART1_RX_PIN);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USART1_IRQn);
}

void uart1_init()
{
	/*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle_uart1.Instance        = USART1;
  UartHandle_uart1.Init.BaudRate   = 9600;
  UartHandle_uart1.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle_uart1.Init.StopBits   = UART_STOPBITS_1;
  UartHandle_uart1.Init.Parity     = UART_PARITY_NONE;
  UartHandle_uart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle_uart1.Init.Mode       = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle_uart1);

}

void uart1_transmit(UART_HandleTypeDef *huart, uint8_t *tx_buffer, uint16_t size)
{
	HAL_UART_Transmit_IT(&UartHandle_uart1, (uint8_t *)tx_buffer, size);
	while (UartReady != SET)
	{
	}
	UartReady = RESET;
}

void uart1_receive(UART_HandleTypeDef *huart, uint8_t *rx_buffer, uint16_t size)
{
	HAL_UART_Receive_IT(&UartHandle_uart1, (uint8_t *)rx_buffer, size);
	while (UartReady != SET)
	{
	}
	UartReady = RESET;
}

void uart2_gpio_init(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USART2_TX_GPIO_CLK_ENABLE();
  USART2_RX_GPIO_CLK_ENABLE();
  /* Enable USART2 clock */
  USART2_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART2_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USART2_TX_AF;
  
  HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USART2_RX_PIN;
  GPIO_InitStruct.Alternate = USART2_RX_AF;
    
  HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void uart2_gpio_deinit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USART2_FORCE_RESET();
  USART2_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USART2_TX_GPIO_PORT, USART2_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USART2_RX_GPIO_PORT, USART2_RX_PIN);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USART2_IRQn);
}

void uart2_put_char(char ch)
{
	uint8_t *tx_buffer;
	*tx_buffer = ch;

	HAL_UART_Transmit_IT(&UartHandle_uart2, (uint8_t*)tx_buffer, 1);
			
	while (UartReady != SET)
	{
	}
			
	UartReady = RESET;
}

void uart2_transmit(uint8_t *tx_buffer, uint16_t size)
{
	HAL_UART_Transmit_IT(&UartHandle_uart2, (uint8_t*)tx_buffer, size);
			
	while (UartReady != SET)
	{
	}
			
	UartReady = RESET;

}

void uart2_init()
{
/*##-1- Configure the UART peripheral ######################################*/
/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
/* UART1 configured as follow:
				- Word Length = 8 Bits
				- Stop Bit = One Stop bit
				- Parity = None
				- BaudRate = 9600 baud
				- Hardware flow control disabled (RTS and CTS signals) */
				
	UartHandle_uart2.Instance        = USART2;
	UartHandle_uart2.Init.BaudRate   = 115200;
//	UartHandle_uart2.Init.BaudRate   = 9600;
	UartHandle_uart2.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle_uart2.Init.StopBits   = UART_STOPBITS_1;
	UartHandle_uart2.Init.Parity     = UART_PARITY_NONE;
	UartHandle_uart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle_uart2.Init.Mode       = UART_MODE_TX_RX;
	
	HAL_UART_Init(&UartHandle_uart2);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	/* Set transmission flag: trasfer complete*/
	UartReady = SET;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	/* Set transmission flag: trasfer complete*/
	UartReady = SET;
}


void USART1_IRQHandler(void)
{
  	HAL_UART_IRQHandler(& UartHandle_uart1);
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(& UartHandle_uart2);
}

