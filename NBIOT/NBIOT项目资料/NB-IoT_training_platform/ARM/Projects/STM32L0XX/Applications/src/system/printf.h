#ifndef __PRINTF_H__
#define __PRINTF_H__

/* Includes ------------------------------------------------------------------*/
//#include "board.h"

/*!
 * FIFO buffers size
 */
#define PRINTF_FIFO_TX_SIZE                     128
#define PRINTF_FIFO_RX_SIZE                     128

#define USARTx_PRINTF                           Uart2
//#define USART_BAUDRATE													115200

extern void printf_init(void);


/*!
 * Log level
 */
#define  LOG_ERR    0
#define	 LOG_WARN   1
#define  LOG_INFO   2
#define	 LOG_DEBUG  3

/* Log level setting */
#ifndef LOGLEVEL
#define LOGLEVEL LOG_ERR
#endif

#if LOGLEVEL >= LOG_ERR
#define log_err(format, ...) printf("ERR:");printf(format, ##__VA_ARGS__)
#else
#define log_err(format, ...)
#endif

#if LOGLEVEL >= LOG_WARN
#define log_warn(format, ...) printf("WARN:");printf(format, ##__VA_ARGS__)
#else
#define log_warn(format, ...)
#endif

#if LOGLEVEL >= LOG_INFO
#define log_info(format, ...) printf("INFO:");printf(format, ##__VA_ARGS__)
#else
#define log_info(format, ...)
#endif

#if LOGLEVEL >= LOG_DEBUG
#define log_debug(format, ...) printf("DBG:");printf(format, ##__VA_ARGS__)
#else
#define log_debug(format, ...)
#endif



#endif /* __PRINTF_H */

