#ifndef _SOFTUART_H_
#define _SOFTUART_H_

typedef enum{
    BAUD_9600,
    BAUD_115200    
}Softuart_baud;

#define SOFTUART_BAUD_RATE      BAUD_9600

#define SOFTUART_IN_BUF_SIZE     128

#ifndef SOFTUART_BAUD_RATE
#error "Please define SOFTUART_BAUD_RATE!"
#endif
typedef void (*Irq_notify)(UartNotifyId_t id);

// Init the Software Uart
void softuart_init(void);

// Set Software Uart irq notify
void softuart_set_notify(Irq_notify notify);

// Reads a character from the input buffer.
// return [0:OK, 1:BUSY]
uint8_t softuart_getchar( uint8_t *data );

// Writes a character to the serial port.
void softuart_putchar( const char );

// Write a NULL-terminated string from RAM to the serial port
void softuart_puts( const char *s );

void softuart_turn_rx_on( void );

void softuart_turn_rx_off( void );
#endif
