#include "main.h"

/* I2C handler declaration */
extern I2C_HandleTypeDef I2C1Handle_bh1750fvi;

/*初始化后，需要预热一定时间，详见数据手册*/
void bh1750fvi_init()
{
	uint8_t tx_buffer[1];

	i2c_gpio_init(&I2C1Handle_bh1750fvi);
	
	i2c_init();
	
	tx_buffer[0] = 0x01;//power

	HAL_I2C_Master_Transmit_IT(&I2C1Handle_bh1750fvi,BH1750FVI_I2C_ADDRESS,tx_buffer,1);
	
	while (HAL_I2C_GetState(&I2C1Handle_bh1750fvi) != HAL_I2C_STATE_READY)
	{
	}
	
	tx_buffer[0] = 0x10;//H-resolution mode

	HAL_I2C_Master_Transmit_IT(&I2C1Handle_bh1750fvi,BH1750FVI_I2C_ADDRESS,tx_buffer,1);
	
	while (HAL_I2C_GetState(&I2C1Handle_bh1750fvi) != HAL_I2C_STATE_READY)
	{
	}
}

uint16_t bh1750fvi_read()
{
	uint8_t rx_buffer[2];
	uint16_t light_data = 0;
	HAL_I2C_Master_Receive_IT(&I2C1Handle_bh1750fvi, (uint16_t)BH1750FVI_I2C_ADDRESS, (uint8_t *)rx_buffer, 2);
	
	while (HAL_I2C_GetState(&I2C1Handle_bh1750fvi) != HAL_I2C_STATE_READY)
	{
	}
	
	light_data = rx_buffer[0];
	light_data = (light_data << 8) + rx_buffer[1]; //合成16位数据，精度1Lx
	
	return(light_data);	
}
