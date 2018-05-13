#ifndef __I2C_H__
#define __I2C_H__

#include "main.h"

#define HMC5883L_I2C_ADDRESS		0x3C
#define BH1750FVI_I2C_ADDRESS       0x46

/* Timing samples with SYSCLK 32MHz set in SystemClock_Config() */ 
#define I2C_TIMING_100KHZ       0x10A13E56 /* Analog Filter ON, Rise Time 400ns, Fall Time 100ns */ 
#define I2C_TIMING_400KHZ       0x00B1112E /* Analog Filter ON, Rise Time 250ns, Fall Time 100ns */ 

#define I2Cx                              I2C1
#define I2Cx_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SCL_AF                     GPIO_AF4_I2C1
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SDA_AF                     GPIO_AF4_I2C1

/* Definition for I2Cx's NVIC */
#define I2Cx_IRQn                    I2C1_IRQn
#define I2Cx_IRQHandler              I2C1_IRQHandler

void i2c_gpio_init(I2C_HandleTypeDef *hi2c);
void i2c_gpio_deinit(I2C_HandleTypeDef *hi2c);
void i2c_init(void);

#endif
