#include <stdio.h>
#include <rt_misc.h>
#include "printf.h"

#pragma import(__use_no_semihosting_swi)

uint8_t printf_TxBuffer[PRINTF_FIFO_TX_SIZE];
uint8_t printf_RxBuffer[PRINTF_FIFO_RX_SIZE];

void printf_init()
{
    UartInit(&USARTx_PRINTF, UART_2, PRINTF_TX, PRINTF_RX);
    UartConfig(&USARTx_PRINTF, RX_TX, USART_BAUDRATE, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL);

    FifoInit(&USARTx_PRINTF.FifoTx, printf_TxBuffer, PRINTF_FIFO_TX_SIZE);
    FifoInit(&USARTx_PRINTF.FifoRx, printf_RxBuffer, PRINTF_FIFO_RX_SIZE);

    printf("Printf init\n");
}

struct __FILE
{
    int handle;                 // Add whatever you need here
};
FILE __stdout;
FILE __stdin;
int fputc(int ch, FILE *f)
{
    return (UartPutChar(&USARTx_PRINTF, (uint8_t) ch));
}

int fgetc(FILE *f)
{
    uint8_t data;
    //UartGetChar(&Uart2, &data);
    return data;
}
void _ttywrch(int ch)
{
    return ;
}
int ferror(FILE *f)                              // Your implementation of ferror
{
    return EOF;
}
void _sys_exit(int return_code)
{
label:
    goto label;           // endless loop
}
