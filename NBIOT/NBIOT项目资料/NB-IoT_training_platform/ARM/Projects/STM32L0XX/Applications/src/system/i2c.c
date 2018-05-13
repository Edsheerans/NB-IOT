#include "main.h"

I2C_HandleTypeDef I2CxHandle_hmc5883l;
I2C_HandleTypeDef I2C1Handle_bh1750fvi;

void i2c_gpio_init(I2C_HandleTypeDef *hi2c)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  I2Cx_SCL_GPIO_CLK_ENABLE();
  I2Cx_SDA_GPIO_CLK_ENABLE();
  /* Enable I2C1 clock */
  I2Cx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* I2C TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH  ;
  GPIO_InitStruct.Alternate = I2Cx_SCL_AF;
  
  HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);
    
  /* I2C RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = I2Cx_SDA_PIN;
  GPIO_InitStruct.Alternate = I2Cx_SDA_AF;
    
  HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for I2C #########################################*/   
  /* NVIC for I2C1 */
  HAL_NVIC_SetPriority(I2Cx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(I2Cx_IRQn);
}

void i2c_gpio_deinit(I2C_HandleTypeDef *hi2c)
{
  /*##-1- Reset peripherals ##################################################*/
  I2Cx_FORCE_RESET();
  I2Cx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure I2C Tx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
  /* Configure I2C Rx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);
  
  /*##-3- Disable the NVIC for I2C ###########################################*/
  HAL_NVIC_DisableIRQ(I2Cx_IRQn);
}

void i2c_init()
{
#ifdef HMC5883L	
		I2CxHandle_hmc5883l.Instance              = I2Cx;
		I2CxHandle_hmc5883l.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
		I2CxHandle_hmc5883l.Init.Timing           = I2C_TIMING_100KHZ;
		I2CxHandle_hmc5883l.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
		I2CxHandle_hmc5883l.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
		I2CxHandle_hmc5883l.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
		I2CxHandle_hmc5883l.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
		I2CxHandle_hmc5883l.Init.OwnAddress1      = HMC5883L_I2C_ADDRESS;
//  	I2CxHandle_hmc5883l.Init.OwnAddress2      = 0xFE;

		HAL_I2C_Init(&I2CxHandle_hmc5883l);
#endif

#ifdef BH1750FVI
		I2C1Handle_bh1750fvi.Instance              = I2Cx;
		I2C1Handle_bh1750fvi.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
		I2C1Handle_bh1750fvi.Init.Timing           = I2C_TIMING_100KHZ;
		I2C1Handle_bh1750fvi.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
		I2C1Handle_bh1750fvi.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
		I2C1Handle_bh1750fvi.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
		I2C1Handle_bh1750fvi.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
		I2C1Handle_bh1750fvi.Init.OwnAddress1      = BH1750FVI_I2C_ADDRESS;
//  	I2C1Handle_bh1750fvi.Init.OwnAddress2      = 0xFE;

		HAL_I2C_Init(&I2C1Handle_bh1750fvi);
#endif

}

void I2Cx_IRQHandler(void)
{
#ifdef HMC5883L	
  HAL_I2C_EV_IRQHandler(&I2CxHandle_hmc5883l);
  HAL_I2C_ER_IRQHandler(&I2CxHandle_hmc5883l);
#endif
	
#ifdef BH1750FVI	
  HAL_I2C_EV_IRQHandler(&I2C1Handle_bh1750fvi);
  HAL_I2C_ER_IRQHandler(&I2C1Handle_bh1750fvi);
#endif
}

