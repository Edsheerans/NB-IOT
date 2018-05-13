#include <stdio.h>
#include <rt_misc.h>
#include "main.h"
#include "printf.h"

#pragma import(__use_no_semihosting_swi)

///* UART handler declaration */
extern UART_HandleTypeDef UartHandle_uart2;

//uint8_t printf_TxBuffer[PRINTF_FIFO_TX_SIZE];
//uint8_t printf_RxBuffer[PRINTF_FIFO_RX_SIZE];



void printf_init()
{
	uart2_gpio_init(&UartHandle_uart2);
	
	uart2_init();

//    FifoInit(&USARTx_PRINTF.FifoTx, printf_TxBuffer, PRINTF_FIFO_TX_SIZE);
//    FifoInit(&USARTx_PRINTF.FifoRx, printf_RxBuffer, PRINTF_FIFO_RX_SIZE);

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
	uart2_put_char((char)ch);
	return 1;
}

int fgetc(FILE *f)
{
    uint8_t data;
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
