#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "stm32l0xx_hal.h"
#include "delay.h"
#include "uart.h"
#include "spi.h"
#include "board.h"
#include "i2c.h"
#include "adxl362.h"
#include "us100.h"
#include "led_board.h"
#include "button_board.h"
#include "buzzer.h"

#define	PRINTF

#define NORMAL_LED
#define	RGB_LED
#define BUTTON
#define BUZZER

//#define	ADC_1
//#define	ADXL362
//#define BH1750FVI
//#define	HMC5883L
//#define	US100

#ifdef ADXL362
#define ADXL362_READ_XYZ_REG 					//must be enabled
//#define OSCILLOSCOPE
#endif

/*!
 * Generic definition
 */
#ifndef SUCCESS
#define SUCCESS          1
#endif

#ifndef FAIL
#define FAIL             0
#endif

#endif

