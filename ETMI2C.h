#ifndef __ETMI2C_H
#define __ETMI2C_H

#if defined(__dsPIC30F__)
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24Hxxxx.h>
#endif


unsigned int WaitForI2CBusIdle(unsigned char i2c_port);
/*
  This will wait for bus idle on the specified I2C Bus
  
  The result will be 0x0000 if everything works properly
  The result will be 0xFA00 if there is an I2C bus failure
*/

unsigned int GenerateI2CStart(unsigned char i2c_port);
/*
  This will generate a start condition on the specified I2C bus
  
  The result will be 0x0000 if everything works properly
  The result will be 0xFA00 if there is an I2C bus failure
*/

unsigned int GenerateI2CRestart(unsigned char i2c_port);
/*
  This will generate a re-start condition on the specified I2C bus
  
  The result will be 0x0000 if everything works properly
  The result will be 0xFA00 if there is an I2C bus failure
*/

unsigned int WriteByteI2C(unsigned char data, unsigned char i2c_port);
/*
  This will write a byte to the specified I2C bus
  
  The result will be 0x0000 if everything works properly
  The result will be 0xFA00 if there is an I2C bus failure
*/

unsigned int ReadByteI2C(unsigned char i2c_port);
/*
  This will read a byte from the specified I2C bus
  
  The result will be 0x00dd if everything works properly
  The result will be 0xFA00 if there is an I2C bus failure
*/


unsigned int GenerateI2CStop(unsigned char i2c_port);
/*
  This will generate a stop condition on the specified I2C bus
  
  The result will be 0x0000 if everything works properly
  The result will be 0xFA00 if there is an I2C bus failure
*/



// A 16 bit timer needs to be dedicated to I2C error checking.
// These will need to modified for your design
// The timer also needs to be configured elsewhere


#define I2C_TIMER_REGISTER   TMR4
#define I2C_TIMER_CON_BITS   T4CONbits
#define I2C_TIMER_INT_FLAG   _T4IF
#define I2C_TIMER_INT_EN     _T4IE
#define I2C_TIMER_PERIOD     PR4
#define I2C_TIMER_PRESCALE   SPI_TIMER_PRESCALE_1_1
#define I2C_TIMEOUT_CYCLES   16000

#define I2C_TIMER_PRESCALE_1_1   0b00
#define I2C_TIMER_PRESCALE_1_8   0b01
#define I2C_TIMER_PRESCALE_1_16  0b10
#define I2C_TIMER_PRESCALE_1_256 0b11

#define I2C_PORT                 0
#define I2C_PORT_1               1
#define I2C_PORT_2               2


#endif
