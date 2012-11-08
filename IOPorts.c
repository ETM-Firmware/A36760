#include "IOPorts.h"


void PinSetTris(unsigned char pin, unsigned char tris_state) {
  /*
    This is a very ineffecient way of setting the TRIS, but it is usefull because it 
    1) Allows a framework for pins to passed to function
    2) Is easy for others to use as a simple integer for the pin can be passed to functions
  */
  
  unsigned char port;
  unsigned char bit;
  
  port = pin & 0xF0;
  bit = pin & 0x0F;
  
  switch (port) {
    
#if defined(_PORTA)
  case _PORT_PORTA:
    if (tris_state) {
      TRISA = TRISA | (0x01 << bit);
    } else {
      TRISA = TRISA & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTB)
  case _PORT_PORTB:
    if (tris_state) {
      TRISB = TRISB | (0x01 << bit);
    } else {
      TRISB = TRISB & (~(0x01 << bit));
    }
    break;
#endif
    
#if defined(_PORTC)
  case _PORT_PORTC:
    if (tris_state) {
      TRISC = TRISC | (0x01 << bit);
    } else {
      TRISC = TRISC & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTD)
  case _PORT_PORTD:
    if (tris_state) {
      TRISD = TRISD | (0x01 << bit);
    } else {
      TRISD = TRISD & (~(0x01 << bit));
    }
    break;
#endif


#if defined(_PORTE)    
  case _PORT_PORTE:
    if (tris_state) {
      TRISE = TRISE | (0x01 << bit);
    } else {
      TRISE = TRISE & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTF)    
  case _PORT_PORTF:
    if (tris_state) {
      TRISF = TRISF | (0x01 << bit);
    } else {
      TRISF = TRISF & (~(0x01 << bit));
    }
    break;
#endif
    
#if defined(_PORTG)
  case _PORT_PORTG:
    if (tris_state) {
      TRISG = TRISG | (0x01 << bit);
    } else {
      TRISG = TRISG & (~(0x01 << bit));
    }
    break;
#endif
    
  }
}

void PinSetValue(unsigned char pin, unsigned char pin_value) {
  /*
    This is a very ineffecient way of setting pins, but it is usefull because it 
    1) Allows a framework for pins to passed to function
    2) Is easy for others to use as a simple integer for the pin can be passed to functions
    Note that is changes the LATCH registers not, the output registers.  DO NOT SET the output registers, use the Latches instead
  */
  
  unsigned char port;
  unsigned char bit;
  
  port = pin & 0xF0; 
  bit = pin & 0x0F;
  
  switch (port) {
 
#if defined(_PORTA)
  case _PORT_PORTA:
    if (pin_value) {
      LATA = LATA | (0x01 << bit);
    } else {
      LATA = LATA & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTB)
  case _PORT_PORTB:
    if (pin_value) {
      LATB = LATB | (0x01 << bit);
    } else {
      LATB = LATB & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTC)
  case _PORT_PORTC:
    if (pin_value) {
      LATB = LATB | (0x01 << bit);
    } else {
      LATB = LATB & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTD)
  case _PORT_PORTD:
    if (pin_value) {
      LATD = LATD | (0x01 << bit);
    } else {
      LATD = LATD & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTE)
  case _PORT_PORTE:
    if (pin_value) {
      LATE = LATE | (0x01 << bit);
    } else {
      LATE = LATE & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTF)
  case _PORT_PORTF:
    if (pin_value) {
      LATF = LATF | (0x01 << bit);
    } else {
      LATF = LATF & (~(0x01 << bit));
    }
    break;
#endif

#if defined(_PORTG)
  case _PORT_PORTG:
    if (pin_value) {
      LATG = LATG | (0x01 << bit);
    } else {
      LATG = LATG & (~(0x01 << bit));
    }
    break;
#endif

  }
}



