#ifndef __UART_H__
#define __UART_H__

#include "main.h"

/*uart1 define*/
#define USART1_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USART1_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USART1_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USART1_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USART1 Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA  
#define USART1_TX_AF                     GPIO_AF4_USART1
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA 
#define USART1_RX_AF                     GPIO_AF4_USART1

/*uart2 define*/
#define USART2_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE();
#define USART2_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USART2_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USART2_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USART2 Pins */
#define USART2_TX_PIN                    GPIO_PIN_2
#define USART2_TX_GPIO_PORT              GPIOA  
#define USART2_TX_AF                     GPIO_AF4_USART2
#define USART2_RX_PIN                    GPIO_PIN_3
#define USART2_RX_GPIO_PORT              GPIOA 
#define USART2_RX_AF                     GPIO_AF4_USART2

void uart1_gpio_init(UART_HandleTypeDef *huart);
void uart1_gpio_deinit(UART_HandleTypeDef *huart);
void uart1_transmit(UART_HandleTypeDef *huart, uint8_t *tx_buffer, uint16_t Size);
void uart1_receive(UART_HandleTypeDef *huart, uint8_t *rx_buffer, uint16_t Size);
void uart1_init(void);

void uart2_gpio_init(UART_HandleTypeDef *huart);
void uart2_gpio_deinit(UART_HandleTypeDef *huart);
void uart2_put_char(char ch);
void uart2_transmit(uint8_t *tx_buffer, uint16_t size);
void uart2_init(void);

#endif
