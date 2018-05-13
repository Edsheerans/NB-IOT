/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Generic low level driver for GPS receiver

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "board.h"

/*!
 * FIFO buffers size
 */
#define FIFO_TX_SIZE                                128
#define FIFO_RX_SIZE                                128

uint8_t TxBuffer[FIFO_TX_SIZE];
uint8_t RxBuffer[FIFO_RX_SIZE];

#define NMEASTRING_SIZE 128
/*!
 * \brief Buffer holding the  raw data received from the gps
 */
uint8_t NmeaString[NMEASTRING_SIZE];

/*!
 * \brief Maximum number of data byte that we will accept from the GPS
 */
uint8_t NmeaStringSize = 0;

PpsTrigger_t PpsTrigger;


void GpsMcuOnPpsSignal(void)
{
    bool parseData = false;

    GpsPpsHandler(&parseData);

    if(parseData == true)
    {
          softuart_turn_rx_on();
        //UartInit(&Uart2, UART_2, PRINTF_TX, PRINTF_RX);
//        UartConfig( &Uart1, RX_ONLY, 115200, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL );
        //UartConfig(&Uart2, RX_ONLY, 9600, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL);
    }
}

void GpsMcuInvertPpsTrigger(void)
{
    // There is no need to invert the PPS signal on SensorNode platform.
}

uint8_t GpsMcuGetPpsTrigger(void)
{
    return(PpsTrigger);
}

void GpsMcuInit(void)
{
    NmeaStringSize = 0;
    PpsTrigger = PpsTriggerIsFalling;

    //FifoInit(&Uart2.FifoTx, TxBuffer, FIFO_TX_SIZE);
    //FifoInit(&Uart2.FifoRx, RxBuffer, FIFO_RX_SIZE);
    //Uart2.IrqNotify = GpsMcuIrqNotify;
    softuart_init();
    softuart_set_notify(GpsMcuIrqNotify);
    softuart_turn_rx_off();

    GpsMcuStart();
    GpioInit(&GpsPps, GPS_PPS, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0);
    GpioSetInterrupt(&GpsPps, IRQ_FALLING_EDGE, IRQ_VERY_LOW_PRIORITY, &GpsMcuOnPpsSignal);
}

void GpsMcuStart(void)
{
    // GpsPowerEn pin is unused and not initialization
    //GpioWrite(&GpsPowerEn, 0);      // power up the GPS
}

void GpsMCuStop(void)
{
    // GpsPowerEn pin is unused and not initialization
    //GpioWrite(&GpsPowerEn, 1);      // power down the GPS
}

void GpsMcuIrqNotify(UartNotifyId_t id)
{
    uint8_t data;
    if(id == UART_NOTIFY_RX)
    {
        if(softuart_getchar(&data) == 0)
        {
            if((data == '$') || (NmeaStringSize >= NMEASTRING_SIZE - 1))
            {
                NmeaStringSize = 0;
            }

            NmeaString[NmeaStringSize++] = (int8_t)data;

            if(data == '\n')
            {
                NmeaString[NmeaStringSize] = '\0';
                // make sure gps data is correct
                if(GpsParseGpsData((int8_t*)NmeaString, NmeaStringSize) == SUCCESS)
                {
                    softuart_turn_rx_off();
                }
                NmeaStringSize = 0;
            }
        }
    }
}
