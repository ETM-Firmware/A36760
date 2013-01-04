#ifndef __IOPorts_H
#define __IOPorts_H


#if defined(__dsPIC30F__)
#include <p30fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__PIC24H__)
#include <p24Hxxxx.h>
#endif


#if defined(__dsPIC30F6014A__)
#define _PORTA
#define _PORTB
#define _PORTC
#define _PORTD
#define _PORTF
#define _PORTG
#endif 




#define TRIS_DIGITAL_OUTPUT  0
#define TRIS_DIGITAL_INPUT   1

#define _PORT_PORTA 0xA0
#define _PORT_PORTB 0xB0
#define _PORT_PORTC 0xC0
#define _PORT_PORTD 0xD0
#define _PORT_PORTE 0xE0
#define _PORT_PORTF 0xF0
#define _PORT_PORTG 0x00

#define _PIN_NOT_CONNECTED 0x20

#define _PIN_RA0 _PORT_PORTA | 0x00
#define _PIN_RA1 _PORT_PORTA | 0x01
#define _PIN_RA3 _PORT_PORTA | 0x03
#define _PIN_RA4 _PORT_PORTA | 0x04
#define _PIN_RA5 _PORT_PORTA | 0x05
#define _PIN_RA6 _PORT_PORTA | 0x06
#define _PIN_RA7 _PORT_PORTA | 0x07
#define _PIN_RA8 _PORT_PORTA | 0x08
#define _PIN_RA9 _PORT_PORTA | 0x09
#define _PIN_RA10 _PORT_PORTA | 0x0A
#define _PIN_RA11 _PORT_PORTA | 0x0B
#define _PIN_RA12 _PORT_PORTA | 0x0C
#define _PIN_RA13 _PORT_PORTA | 0x0D
#define _PIN_RA14 _PORT_PORTA | 0x0E
#define _PIN_RA15 _PORT_PORTA | 0x0F


#define _PIN_RB0 _PORT_PORTB | 0x00
#define _PIN_RB1 _PORT_PORTB | 0x01
#define _PIN_RB3 _PORT_PORTB | 0x03
#define _PIN_RB4 _PORT_PORTB | 0x04
#define _PIN_RB5 _PORT_PORTB | 0x05
#define _PIN_RB6 _PORT_PORTB | 0x06
#define _PIN_RB7 _PORT_PORTB | 0x07
#define _PIN_RB8 _PORT_PORTB | 0x08
#define _PIN_RB9 _PORT_PORTB | 0x09
#define _PIN_RB10 _PORT_PORTB | 0x0A
#define _PIN_RB11 _PORT_PORTB | 0x0B
#define _PIN_RB12 _PORT_PORTB | 0x0C
#define _PIN_RB13 _PORT_PORTB | 0x0D
#define _PIN_RB14 _PORT_PORTB | 0x0E
#define _PIN_RB15 _PORT_PORTB | 0x0F


#define _PIN_RC0 _PORT_PORTC | 0x00
#define _PIN_RC1 _PORT_PORTC | 0x01
#define _PIN_RC3 _PORT_PORTC | 0x03
#define _PIN_RC4 _PORT_PORTC | 0x04
#define _PIN_RC5 _PORT_PORTC | 0x05
#define _PIN_RC6 _PORT_PORTC | 0x06
#define _PIN_RC7 _PORT_PORTC | 0x07
#define _PIN_RC8 _PORT_PORTC | 0x08
#define _PIN_RC9 _PORT_PORTC | 0x09
#define _PIN_RC10 _PORT_PORTC | 0x0A
#define _PIN_RC11 _PORT_PORTC | 0x0B
#define _PIN_RC12 _PORT_PORTC | 0x0C
#define _PIN_RC13 _PORT_PORTC | 0x0D
#define _PIN_RC14 _PORT_PORTC | 0x0E
#define _PIN_RC15 _PORT_PORTC | 0x0F

#define _PIN_RD0 _PORT_PORTD | 0x00
#define _PIN_RD1 _PORT_PORTD | 0x01
#define _PIN_RD3 _PORT_PORTD | 0x03
#define _PIN_RD4 _PORT_PORTD | 0x04
#define _PIN_RD5 _PORT_PORTD | 0x05
#define _PIN_RD6 _PORT_PORTD | 0x06
#define _PIN_RD7 _PORT_PORTD | 0x07
#define _PIN_RD8 _PORT_PORTD | 0x08
#define _PIN_RD9 _PORT_PORTD | 0x09
#define _PIN_RD10 _PORT_PORTD | 0x0A
#define _PIN_RD11 _PORT_PORTD | 0x0B
#define _PIN_RD12 _PORT_PORTD | 0x0C
#define _PIN_RD13 _PORT_PORTD | 0x0D
#define _PIN_RD14 _PORT_PORTD | 0x0E
#define _PIN_RD15 _PORT_PORTD | 0x0F

#define _PIN_RE0 _PORT_PORTE | 0x00
#define _PIN_RE1 _PORT_PORTE | 0x01
#define _PIN_RE3 _PORT_PORTE | 0x03
#define _PIN_RE4 _PORT_PORTE | 0x04
#define _PIN_RE5 _PORT_PORTE | 0x05
#define _PIN_RE6 _PORT_PORTE | 0x06
#define _PIN_RE7 _PORT_PORTE | 0x07
#define _PIN_RE8 _PORT_PORTE | 0x08
#define _PIN_RE9 _PORT_PORTE | 0x09
#define _PIN_RE10 _PORT_PORTE | 0x0A
#define _PIN_RE11 _PORT_PORTE | 0x0B
#define _PIN_RE12 _PORT_PORTE | 0x0C
#define _PIN_RE13 _PORT_PORTE | 0x0D
#define _PIN_RE14 _PORT_PORTE | 0x0E
#define _PIN_RE15 _PORT_PORTE | 0x0F


#define _PIN_RF0 _PORT_PORTF | 0x00
#define _PIN_RF1 _PORT_PORTF | 0x01
#define _PIN_RF3 _PORT_PORTF | 0x03
#define _PIN_RF4 _PORT_PORTF | 0x04
#define _PIN_RF5 _PORT_PORTF | 0x05
#define _PIN_RF6 _PORT_PORTF | 0x06
#define _PIN_RF7 _PORT_PORTF | 0x07
#define _PIN_RF8 _PORT_PORTF | 0x08
#define _PIN_RF9 _PORT_PORTF | 0x09
#define _PIN_RF10 _PORT_PORTF | 0x0A
#define _PIN_RF11 _PORT_PORTF | 0x0B
#define _PIN_RF12 _PORT_PORTF | 0x0C
#define _PIN_RF13 _PORT_PORTF | 0x0D
#define _PIN_RF14 _PORT_PORTF | 0x0E
#define _PIN_RF15 _PORT_PORTF | 0x0F



#define _PIN_RG0 _PORT_PORTG | 0x00
#define _PIN_RG1 _PORT_PORTG | 0x01
#define _PIN_RG3 _PORT_PORTG | 0x03
#define _PIN_RG4 _PORT_PORTG | 0x04
#define _PIN_RG5 _PORT_PORTG | 0x05
#define _PIN_RG6 _PORT_PORTG | 0x06
#define _PIN_RG7 _PORT_PORTG | 0x07
#define _PIN_RG8 _PORT_PORTG | 0x08
#define _PIN_RG9 _PORT_PORTG | 0x09
#define _PIN_RG10 _PORT_PORTG | 0x0A
#define _PIN_RG11 _PORT_PORTG | 0x0B
#define _PIN_RG12 _PORT_PORTG | 0x0C
#define _PIN_RG13 _PORT_PORTG | 0x0D
#define _PIN_RG14 _PORT_PORTG | 0x0E
#define _PIN_RG15 _PORT_PORTG | 0x0F


void PinSetValue(unsigned char pin, unsigned char pin_value);

void PinSetTris(unsigned char pin, unsigned char tris_state);


#endif
