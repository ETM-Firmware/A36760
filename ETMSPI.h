#ifndef __ETMSPI_H
#define __ETMSPI_H



#if defined(__dsPIC30F__)
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24Hxxxx.h>
#endif



unsigned long SendAndReceiveSPI(unsigned int data_word, unsigned char spi_port);
/*
  If the SPI operation is successful, the function will return 0x0000dddd (dddd being the 16 bits read from the SPI port) 
  If the SPI operation fails, the function will return 0x11110000
*/


// A 16 bit timer needs to be dedicated to SPI error checking.
// These will need to modified for your design
// The timer also needs to be configured elsewhere

#define SPI_TIMER_PRESCALE_1_1   0b00
#define SPI_TIMER_PRESCALE_1_8   0b01
#define SPI_TIMER_PRESCALE_1_16  0b10
#define SPI_TIMER_PRESCALE_1_256 0b11



#define SPI_TIMER_REGISTER   TMR4
#define SPI_TIMER_CON_BITS   T4CONbits
#define SPI_TIMER_INT_FLAG   _T4IF
#define SPI_TIMER_INT_EN     _T4IE
#define SPI_TIMER_INT_PRI    _T4IP
#define SPI_TIMER_PERIOD     PR4
#define SPI_TIMER_PRESCALE   SPI_TIMER_PRESCALE_1_1
#define SPI_TIMEOUT_CYCLES   4000


#define SPI_BUS_ACTIVE      0
#define SPI_DATA_SENT       1
#define SPI_DATA_RECEIVED   2 
#define SPI_BUS_TIMEOUT     3
#define SPI_BUS_OVERFLOW    4

#define SPI_PORT            0
#define SPI_PORT_1          1
#define SPI_PORT_2          2


#endif
