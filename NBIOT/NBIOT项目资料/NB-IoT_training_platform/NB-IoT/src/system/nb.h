#ifndef __NB_H_
#define __NB_H_
#include "board.h"

#if defined( UART_PRINTF )
    #if LOGLEVEL >= LOG_DEBUG
    #define log(format, ...)  printf(format, ##__VA_ARGS__)
    #else
    #define log(format, ...)
    #endif
#else
#define log(format, ...)
#endif


#define RESPONSE_STRING_SIZE 100
#define COMMAND_STRING_SIZE  100

//The maximum number that can be created
#define SOCKET_MAX           7

//The user payload size
#define PAYLOAD_SIZE         64

//The head of udp packet 
#define PACKET_HEAD_SIZE     40

//Defines nb node id size
#define NODE_ID_SIZE         4

//AT command list
typedef enum{
    NBAND = 0,
    NRB = 1,
    NCONFIG = 2,
    CFUN = 3,
    CSQ = 4,
    CGATT = 5,
    CEREG = 6,
    CSCON = 7,
    NSOCR = 8,
    NSOST = 9,
    NSONMI = 10,
    AT = 11
}tCommand;

//Indicate how to confirm response
typedef enum{
    CONFIRM_PARA,
    CONFIRM_OK_ONLY
}tConfirm;

//At command type
typedef enum{
    T_READ,
    T_SET
}tType;

//Command response type
typedef struct{
    uint8_t cmd[15];
    uint8_t para1[15];
    uint8_t para2[15];
}tResponse;

/*!
 * \brief   Initialize uart1 and nb mudule
 *
 *
 * \retval  If nb is ready to work,the function returns 'true'
 *          otherwise the function return 'false'.This function 
 *          must call first when call other function.
 */
bool nb_init(void);

/*!
 * \brief   Creat an udp socket
 *
 *
 * \retval  If socket create success,the function returns 'true'
 */
bool udp_creat_socket(void);

/*!
 * \brief   Send an udp packet with specified ip:port
 *
 *
 * \retval  If success,the function returns 'true'
 */
 bool udp_send(uint8_t *data, const char *ip, const char*port);

#endif
