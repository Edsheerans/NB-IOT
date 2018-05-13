#include "main.h"
#include "hmc5883l.h"

/* I2C handler declaration */
extern I2C_HandleTypeDef I2CxHandle_hmc5883l;

int16_t x_data = 0;
int16_t y_data = 0;
int16_t z_data = 0;
double angle = 0;

void hmc5883l_init()
{
	uint8_t tx_buffer[3];

	i2c_gpio_init(&I2CxHandle_hmc5883l);
	
	i2c_init();
	
	tx_buffer[0] = 0x00;//寄存器A
	tx_buffer[1] = 0x70;
	tx_buffer[2] = 0x20;//寄存器B的值，地址自增
	
	HAL_I2C_Master_Transmit_IT(&I2CxHandle_hmc5883l,HMC5883L_I2C_ADDRESS,tx_buffer,3);
	
	while (HAL_I2C_GetState(&I2CxHandle_hmc5883l) != HAL_I2C_STATE_READY)
	{
	}
 
}

void hmc5883l_single_read()
{
	uint8_t rx_buffer[6];
	uint8_t tx_buffer[2];
	
	tx_buffer[0] = 0x02;//模式寄存器
	tx_buffer[1] = 0x01;
	
	HAL_I2C_Master_Transmit_IT(&I2CxHandle_hmc5883l,HMC5883L_I2C_ADDRESS,tx_buffer,2);
	
	while (HAL_I2C_GetState(&I2CxHandle_hmc5883l) != HAL_I2C_STATE_READY)
	{
	} 
	
	DelayMs(10);//数据准备，数据手册标准时间：6ms
	
	HAL_I2C_Master_Receive_IT(&I2CxHandle_hmc5883l,HMC5883L_I2C_ADDRESS,rx_buffer,6);
	
	while (HAL_I2C_GetState(&I2CxHandle_hmc5883l) != HAL_I2C_STATE_READY)
	{
	} 

	x_data = convert_data(rx_buffer[0],rx_buffer[1]);
	y_data = convert_data(rx_buffer[4],rx_buffer[5]);
	z_data = convert_data(rx_buffer[2],rx_buffer[3]);
}

float hmc5883l_get_data(int16_t *data)
{
	hmc5883l_single_read();
	
	data[0] = x_data;
	data[1] = y_data;
	data[2] = z_data;
	
	angle = (atan2((float)y_data,(float)x_data) * (180 / 3.14159265) + 180);
	
	return(angle);
}

int16_t convert_data(uint8_t up,uint8_t low)
{
	int16_t out;
	uint16_t out_tmp;
	out_tmp = up;
	out_tmp = out_tmp << 8;
	out_tmp |= low;
	out = (int16_t)out_tmp;
	return(out);
}

