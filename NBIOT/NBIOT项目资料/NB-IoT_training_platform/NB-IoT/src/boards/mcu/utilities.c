/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Helper functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "utilities.h"

/*!
 * Redefinition of rand() and srand() standard C functions.
 * These functions are redefined in order to get the same behavior across
 * different compiler toolchains implementations.
 */
// Standard random functions redefinition start
#define RAND_LOCAL_MAX 2147483647L
#define STRING_MAX     64

static uint8_t String[STRING_MAX];

static uint32_t next = 1;

int32_t rand1( void )
{
    return ( ( next = next * 1103515245L + 12345L ) % RAND_LOCAL_MAX );
}

void srand1( uint32_t seed )
{
    next = seed;
}
// Standard random functions redefinition end

int32_t randr( int32_t min, int32_t max )
{
    return ( int32_t )rand1( ) % ( max - min + 1 ) + min;
}

void memcpy1( uint8_t *dst, const uint8_t *src, uint16_t size )
{
    while( size-- )
    {
        *dst++ = *src++;
    }
}

void memcpyr( uint8_t *dst, const uint8_t *src, uint16_t size )
{
    dst = dst + ( size - 1 );
    while( size-- )
    {
        *dst-- = *src++;
    }
}

void memset1( uint8_t *dst, uint8_t value, uint16_t size )
{
    while( size-- )
    {
        *dst++ = value;
    }
}

int8_t Nibble2HexChar( uint8_t a )
{
    if( a < 10 )
    {
        return '0' + a;
    }
    else if( a < 16 )
    {
        return 'A' + ( a - 10 );
    }
    else
    {
        return '?';
    }
}

uint16_t str2int(const char *str)
{
    int number = 0;

    while(*str != 0)
    {
        if ((*str < '0') || (*str > '9')) 
        {                      
            break;
        }
        number = number * 10 + (*str - '0');
        str++;     
    }
    return number;
}
uint8_t * HexToStr(uint8_t *pHex, uint16_t len)
{
    char lb, hb;
    int i;
    
    for (i = 0; i < len; i++)
    {
        hb = (pHex[i] & 0xf0)>>4;
        if(hb<=9 )  
            hb += 0x30;  
        else if( hb>=10 && hb<=15 )  
            hb = hb -10 + 'A';  
        else  
            return NULL;
        
        lb = pHex[i] & 0x0f;  
        if(lb<=9 )  
            lb += 0x30;  
        else if( lb>=10 && lb<=15 )  
            lb = lb - 10 + 'A';  
        else  
            return NULL; 
        String[i*2]   = hb;  
        String[i*2+1] = lb;

    }
    String[i*2] = '\0';
    return String;
}



