/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Target board general functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"
#include "utilities.h"
#include "delay.h"
#include "gpio.h"
#include "printf.h"
#include "i2c.h"
#include "uart.h"
#include "SHT2x.h"
#include "gps.h"
#include "gps-board.h"
#include "uart-board.h"
#include "nb.h"
#include "softuart.h"
#include "config.h"
#include "neul_bc95.h"
#include "iwdog.h"
#include "tim-board.h"
#include "timer.h"

#if defined( USE_USB_CDC )
#include "uart-usb-board.h"
#endif

/*!
 * Define indicating if an external IO expander is to be used
 */
//#define BOARD_IOE_EXT

/*!
 * Generic definition
 */
#ifndef SUCCESS
#define SUCCESS                                     1
#endif

#ifndef FAIL
#define FAIL                                        0
#endif

/*!
 * Board MCU pins definitions
 */
#define OSC_LSE_IN                                  PC_14
#define OSC_LSE_OUT                                 PC_15

#define OSC_HSE_IN                                  PH_0
#define OSC_HSE_OUT                                 PH_1

// Used for sht2x
#define I2C_SCL                                     PB_8
#define I2C_SDA                                     PB_9

// Used for neul-95
#define UART_TX                                     PA_9
#define UART_RX                                     PA_10

// Printf
#define PRINTF_TX                                   PA_2
#define PRINTF_RX                                   PA_3

// Used for gps
#define SORFUART_TX                                 PB_1
#define SORFUART_RX                                 PB_2
#define GPS_PPS                                     PB_0
#define SORFUART_RX_IRQ                             IOE_2

/*!
 * MCU objects
 */
extern I2c_t I2c;

extern Gpio_t GpsPps;
extern Gpio_t GpsRx;
extern Gpio_t GpsTx;
extern Gpio_t GpsPowerEn;

/*!
 * Possible power sources
 */
enum BoardPowerSource
{
    USB_POWER = 0,
    BATTERY_POWER
};

/*!
 * \brief Disable interrupts
 *
 * \remark IRQ nesting is managed
 */
void BoardDisableIrq(void);

/*!
 * \brief Enable interrupts
 *
 * \remark IRQ nesting is managed
 */
void BoardEnableIrq(void);

/*!
 * \brief Initializes the target board peripherals.
 */
void BoardInitMcu(void);

/*!
 * \brief Initializes the boards peripherals.
 */
void BoardInitPeriph(void);

/*!
 * \brief De-initializes the target board peripherals to decrease power
 *        consumption.
 */
void BoardDeInitMcu(void);

/*!
 * \brief Get the current battery level
 *
 * \retval value  battery level ( 0: very low, 254: fully charged )
 */
uint8_t BoardGetBatteryLevel(void);

/*!
 * \brief Get the current battery voltage
 *
 * \retval value  battery voltage
 */
uint16_t BoardGetPowerSupply(void);

/*!
 * Returns a pseudo random seed generated using the MCU Unique ID
 *
 * \retval seed Generated pseudo random seed
 */
uint32_t BoardGetRandomSeed(void);

/*!
 * \brief Gets the board 64 bits unique ID
 *
 * \param [IN] id Pointer to an array that will contain the Unique ID
 */
void BoardGetUniqueId(uint8_t *id);

/*!
 * \brief Get the board power source
 *
 * \retval value  power source ( 0: USB_POWER,  1: BATTERY_POWER )
 */
uint8_t GetBoardPowerSource(void);

#endif // __BOARD_H__
