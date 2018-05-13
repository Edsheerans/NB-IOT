#include "main.h"

#ifdef PRINTF
#include "printf.h"
#endif

#ifdef ADXL362
#include "adxl362.h"
#endif

#ifdef HMC5883L
#include "hmc5883l.h"
#endif

#ifdef BH1750FVI
#include "bh1750fvi.h"
#endif

#ifdef US100
#include "us100.h"
#endif

#ifdef BUTTON
extern __IO ITStatus key1_flag;
extern __IO ITStatus key2_flag;
extern __IO ITStatus key3_flag;
#endif

#ifdef ADC_1
__IO uint32_t adc1_converted_value = 0;
__IO ITStatus adc1_flag = RESET;
float voltage = 0;
#endif

#ifdef ADXL362
#ifdef OSCILLOSCOPE
uint8_t oscilloscope_buffer[11];
uint8_t xyzValues[6] = {0, 0, 0, 0, 0, 0};
uint8_t sum_num = 0;
#else
int16_t x_axis,y_axis,z_axis;
#endif
extern SPI_HandleTypeDef SpiHandle;
extern __IO ITStatus data_Ready;
uint8_t status_data = 0;
#endif

#ifdef HMC5883L
int16_t hmc5883l_data[3];
double hmc5883l_angle = 0;
#endif

#ifdef BH1750FVI
uint16_t light_data = 0;
#endif

#ifdef US100
float us100_data = 0;
#endif

#ifdef ADXL362 
void adxl362_config()
{
	spi_gpio_init(&SpiHandle);																						//spi intt
	
	ADXL362_Init();
	ADXL362_SoftwareReset();
   	ADXL362_SetPowerMode(0);						    													 	    //standby mode
	ADXL362_SetRange(ADXL362_RANGE_2G);																				//measurement range:2g
    ADXL362_SetOutputRate(ADXL362_ODR_400_HZ);																		//sampling frequency:400HZ
    ADXL362_SetRegisterValue(ADXL362_ACT_INACT_CTL_ACT_EN,ADXL362_REG_ACT_INACT_CTL,1);									
	ADXL362_FifoSetup(ADXL362_FIFO_DISABLE,0,0);
	ADXL362_INTMAP1(); 																								//data_ready interrupt
    ADXL362_SetPowerMode(1);
}
#endif

#ifdef ADC_1
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	adc1_stop();
  	/* Get the converted value of regular channel */
  	adc1_converted_value = HAL_ADC_GetValue(AdcHandle);

	adc1_flag = SET;
}
#endif

int main(void)
{
	HAL_Init();

	system_clock_config();

#ifdef	PRINTF	
	printf_init();
#endif
	
#ifdef	RGB_LED
	led_init(RGB);
#endif
	
#ifdef	NORMAL_LED
	led_init(LED1);
	led_init(LED2);
	led_init(LED3);
	led_init(LED4);
	led_init(LED5);
	led_init(LED6);
	led_init(LED7);
	led_init(LED8);
#endif

#ifdef BUTTON
	button_init(KEY1);
	button_init(KEY2);
	button_init(KEY3);
#endif

#ifdef BUZZER
	buzzer_init();
#endif
	
#ifdef ADC_1
	adc1_init();
	adc1_set_channel0();
	adc1_start();
#endif
	
#ifdef ADXL362  	
	io_interrupt_config();																							//adxl362_init after I/O interrupt 
	adxl362_config();

#ifdef OSCILLOSCOPE	
	oscilloscope_buffer[0] = 0x55;
	oscilloscope_buffer[1] = 0x51;
	oscilloscope_buffer[8] = 0x00;
	oscilloscope_buffer[9] = 0x00;
#endif

#endif
   
#ifdef HMC5883L
	hmc5883l_init();	
#endif

#ifdef BH1750FVI
	bh1750fvi_init();
	DelayMs(180);
#endif
	
#ifdef US100
	us100_init();
#endif
	
	while (1)
	{
#ifdef RGB_LED
		rgb_red(RGB);
		DelayMs(100);
		rgb_green(RGB);
		DelayMs(100);
		rgb_blue(RGB);
		DelayMs(100);
#endif

#ifdef	NORMAL_LED
		led_toggle(LED4);
		led_toggle(LED5);
		led_toggle(LED6);
		led_toggle(LED7);
		led_toggle(LED8);
#endif
		
#ifdef	BUTTON
		if(key1_flag == SET)
		{
			led_toggle(LED1);
			key1_flag = RESET;
		}
		if(key2_flag == SET)
		{
			led_toggle(LED2);
			key2_flag = RESET;
		}
		if(key3_flag == SET)
		{
			led_toggle(LED3);
			key3_flag = RESET;
		}
#endif
		
#ifdef BUZZER
		buzzer_on();
		DelayMs(300);
		buzzer_off();
#endif
		
#ifdef ADC_1
		if(adc1_flag == SET)
		{	
			voltage =  (float)adc1_converted_value * 3.3 / 4096;
#ifdef PRINTF	
			printf("ADC1:%.2fV\n",voltage);
#endif
			DelayMs(100);
			adc1_flag = RESET;
			adc1_start();
		}
#endif
	
#ifdef ADXL362 
#ifdef OSCILLOSCOPE
		while(data_Ready)
		{
			data_Ready = RESET;	
			ADXL362_GetRegisterValue(xyzValues, ADXL362_REG_XDATA_L, 6);
				
			oscilloscope_buffer[2] = xyzValues[0];
			oscilloscope_buffer[3] = xyzValues[1];
			oscilloscope_buffer[4] = xyzValues[2];
			oscilloscope_buffer[5] = xyzValues[3];
			oscilloscope_buffer[6] = xyzValues[4];
			oscilloscope_buffer[7] = xyzValues[5];

			for(sum_num = 0;sum_num < 10; sum_num++)
			{
				oscilloscope_buffer[10] += oscilloscope_buffer[sum_num];
			}
				
			uart2_transmit(oscilloscope_buffer,11);
		}
#else
		ADXL362_GetXyz(&x_axis,&y_axis,&z_axis);
#ifdef PRINTF
		printf("x_axis=%d y_axis=%d z_axis=%d\n",x_axis,y_axis,z_axis);
		DelayMs(200);		
#endif
#endif
#endif
		
#ifdef HMC5883L
		DelayMs(1000);		
		hmc5883l_angle = hmc5883l_get_data(hmc5883l_data);
#ifdef PRINTF	
		printf("x:%d y:%d z:%d	angle:%.2f\n",hmc5883l_data[0],hmc5883l_data[1],hmc5883l_data[2],hmc5883l_angle);
#endif
#endif
		
#ifdef BH1750FVI
		DelayMs(1000);
		light_data = bh1750fvi_read();
#ifdef PRINTF
		printf("light_data:%d\n",light_data);
#endif
#endif
		
#ifdef US100
		us100_data = us100_get_distance();
		DelayMs(50);
		if(us100_data >= 25 && us100_data <= 4500)
		{
#ifdef PRINTF
			printf("distance:%.1fmm\n",us100_data);
#endif
		}
		else
		{
#ifdef PRINTF
			printf("error\n");
#endif
		}
#endif
	}
}
