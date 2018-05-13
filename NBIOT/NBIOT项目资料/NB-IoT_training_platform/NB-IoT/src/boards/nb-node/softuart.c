#include "board.h"

#define SU_TRUE    1
#define SU_FALSE   0

// startbit and stopbit parsed internally (see ISR)
#define RX_NUM_OF_BITS (8)
volatile char           inbuf[SOFTUART_IN_BUF_SIZE];
volatile static unsigned char  qin;
static unsigned char           qout;
volatile static unsigned char  flag_rx_off;
volatile static unsigned char  flag_rx_ready;
// 1 Startbit, 8 Databits, 1 Stopbit = 10 Bits/Frame
#define TX_NUM_OF_BITS (10)
volatile static unsigned char  flag_tx_busy;
volatile static unsigned char  bits_left_in_tx;
volatile static unsigned short internal_tx_buffer; /* ! mt: was type uchar - this was wrong */

#define set_tx_pin_high()      ( GpioWrite(&tx,1) )
#define set_tx_pin_low()       ( GpioWrite(&tx,0) )
#define get_rx_pin_status()    ( GpioRead(&rx) )

#define start_tx()             (HAL_TIM_Base_Start_IT(&tim6_handle))
#define stop_tx()              (HAL_TIM_Base_Stop_IT(&tim6_handle))
#define start_rx()             (HAL_TIM_Base_Start_IT(&tim6_handle))
#define stop_rx()              {HAL_TIM_Base_Stop_IT(&tim6_handle);TIM6->CNT = 0;}
#define nop()                  {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}

Gpio_t tx;
Gpio_t rx;
TIM_HandleTypeDef TimHandle;
Irq_notify softuart_notify = NULL;

static void rx_irqhandler(void);
static void rx_off(void);

static void delay()
{
    Softuart_baud baud;
    uint16_t i = 0;

    baud = SOFTUART_BAUD_RATE;
    switch(baud)
    {
    case BAUD_9600:
        i = 50;
        break;
    case BAUD_115200:
        break;
    default:
        return;
    }
    while(i--)
        nop();
}

void softuart_turn_rx_on( void )
{
    GpioSetInterrupt(&rx, IRQ_FALLING_EDGE, IRQ_LOW_PRIORITY, rx_irqhandler);
}

void softuart_turn_rx_off( void )
{
    GpioSetInterrupt(&rx, IRQ_FALLING_EDGE, IRQ_LOW_PRIORITY, rx_off);
    stop_rx();
}

static unsigned char softuart_transmit_busy( void ) 
{
    return ( flag_tx_busy == SU_TRUE ) ? 1 : 0;
}

static void rx_irqhandler(void)
{
    if(flag_rx_ready == SU_FALSE)
    {
        // wait some time so that measure the middle part of signal
        delay();
        flag_rx_ready = SU_TRUE;
        start_rx();
    }
}

static void rx_off(void)
{

}

static void io_init(void)
{
    GpioInit(&tx, SORFUART_TX, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 1);
    GpioInit(&rx, SORFUART_RX, PIN_INPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1);
    GpioSetInterrupt(&rx, IRQ_FALLING_EDGE, IRQ_LOW_PRIORITY, rx_irqhandler);
}

void softuart_tim_cb(void)
{
    static unsigned char flag_rx_waiting_for_stop_bit = SU_FALSE;
    static unsigned char rx_mask = 1;
    static unsigned char rx_bit_cnt = 0;
    static unsigned char internal_rx_buffer = 0;

    unsigned char flag_in;

    // Transmitter Section
    if ( flag_tx_busy == SU_TRUE ) {
        if ( internal_tx_buffer & 0x01 ) {
            set_tx_pin_high();
        }
        else {
            set_tx_pin_low();
        }
        internal_tx_buffer >>= 1;
        if ( --bits_left_in_tx == 0 ) {
            flag_tx_busy = SU_FALSE;
            stop_tx();
        }
    }

    // Receiver Section
    if ( flag_rx_off == SU_FALSE ) {
        if ( flag_rx_waiting_for_stop_bit ) {
            inbuf[qin] = internal_rx_buffer;
            internal_rx_buffer = 0;
            if(softuart_notify != NULL) {
                softuart_notify(UART_NOTIFY_RX);
            }
            stop_rx();
            if ( ++qin >= SOFTUART_IN_BUF_SIZE ) {
                // overflow - reset inbuf-index
                qin = 0;
            }
            flag_rx_ready = SU_FALSE;
            flag_rx_waiting_for_stop_bit = SU_FALSE;
        }
        else {  // rx_test_busy
            if ( flag_rx_ready == SU_TRUE ) {
                // rcv
                flag_in = get_rx_pin_status();
                if ( flag_in ) {
                    internal_rx_buffer |= rx_mask;
                }
                rx_mask <<= 1;
                if ( ++rx_bit_cnt == RX_NUM_OF_BITS ) {
                    flag_rx_waiting_for_stop_bit = SU_TRUE;
                    rx_bit_cnt = 0;
                    rx_mask = 1;
                }
            }
        }
    }
}

void softuart_init( void )
{
    flag_tx_busy  = SU_FALSE;
    flag_rx_ready = SU_FALSE;
    flag_rx_off   = SU_FALSE;

    io_init();
    set_tx_pin_high(); /* mt: set to high to avoid garbage on init */
    tim_init(tim6);
    set_tim_cb(tim6, softuart_tim_cb);
}

void softuart_set_notify(Irq_notify notify)
{
    softuart_notify = notify;
}

uint8_t softuart_getchar( uint8_t *data )
{
    if(qout != qin){
    	*data = inbuf[qout];
    	if ( ++qout >= SOFTUART_IN_BUF_SIZE ) {
            qout = 0;
    	}
        return 0;
    }
    return 1;
}

void softuart_putchar( const char ch )
{
    while ( softuart_transmit_busy() == SU_TRUE) {
        ; // wait for transmitter ready
        // add watchdog-reset here if needed;
    }

    // invoke_UART_transmit
    bits_left_in_tx    = TX_NUM_OF_BITS;
    internal_tx_buffer = ( ch << 1 ) | 0x200;
    flag_tx_busy       = SU_TRUE;
    start_tx();
}
	
void softuart_puts( const char *s )
{
    while ( *s ) {
        softuart_putchar( *s++ );
    }
}
