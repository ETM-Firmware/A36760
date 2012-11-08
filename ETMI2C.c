#include "ETMI2C.h"


unsigned int WaitForI2CBusIdle(unsigned char i2c_port) {
  
  I2C_TIMER_REGISTER = 0;
  I2C_TIMER_PERIOD = I2C_TIMEOUT_CYCLES;
  I2C_TIMER_INT_FLAG = 0;
  I2C_TIMER_CON_BITS.TON = 1;
  I2C_TIMER_CON_BITS.TCKPS = I2C_TIMER_PRESCALE_1_1;
  
#if defined(_I2CMD)
  if ((i2c_port == 0) || (i2c_port == 1)) {
    while (I2CSTATbits.TRSTAT && !I2C_TIMER_INT_FLAG);		                 //Wait for bus Idle
  }
#endif

#if defined(_I2C1MD)
  if (i2c_port == 1) {
    while (I2C1STATbits.TRSTAT && !I2C_TIMER_INT_FLAG);		                 //Wait for bus Idle
  }
#endif

#if defined(_I2C2MD)
  if (i2c_port == 2) {
    while (I2C2STATbits.TRSTAT && !I2C_TIMER_INT_FLAG);		                 //Wait for bus Idle
  }
#endif
    
  if (I2C_TIMER_INT_FLAG) {
    return 0xFA00;
  } else {
    return 0x0000;
  }
}
  




unsigned int GenerateI2CStart(unsigned char i2c_port) {
  I2C_TIMER_REGISTER = 0;
  I2C_TIMER_PERIOD = I2C_TIMEOUT_CYCLES;
  I2C_TIMER_INT_FLAG = 0;
  I2C_TIMER_CON_BITS.TON = 1;
  I2C_TIMER_CON_BITS.TCKPS = I2C_TIMER_PRESCALE_1_1;
  
#if defined(_I2CMD)
  if ((i2c_port == 0) || (i2c_port == 1)) {
    I2CCONbits.SEN = 1;		                         //Generate Start COndition
    while (I2CCONbits.SEN && !I2C_TIMER_INT_FLAG);	 //Wait for Start COndition
  }
#endif

#if defined(_I2C1MD)
  if (i2c_port == 1) {
    I2C1CONbits.SEN = 1;		                         //Generate Start COndition
    while (I2C1CONbits.SEN && !I2C_TIMER_INT_FLAG);	 //Wait for Start COndition
  }
#endif

#if defined(_I2C2MD)
  if (i2c_port == 2) {
    I2C2CONbits.SEN = 1;		                         //Generate Start COndition
    while (I2C2CONbits.SEN && !I2C_TIMER_INT_FLAG);	 //Wait for Start COndition
  }
#endif
    
  if (I2C_TIMER_INT_FLAG) {
    return 0xFA00;
  } else {
    return 0x0000;
  }
}



unsigned int GenerateI2CRestart(unsigned char i2c_port) {
  I2C_TIMER_REGISTER = 0;
  I2C_TIMER_PERIOD = I2C_TIMEOUT_CYCLES;
  I2C_TIMER_INT_FLAG = 0;
  I2C_TIMER_CON_BITS.TON = 1;
  I2C_TIMER_CON_BITS.TCKPS = I2C_TIMER_PRESCALE_1_1;
  
#if defined(_I2CMD)
  if ((i2c_port == 0) || (i2c_port == 1)) {
    I2CCONbits.RSEN = 1;	                         //Generate Re-Start COndition
    while (I2CCONbits.RSEN && !I2C_TIMER_INT_FLAG);	 //Wait for Re-Start COndition
  }
#endif

#if defined(_I2C1MD)
  if (i2c_port == 1) {
    I2C1CONbits.RSEN = 1;		                 //Generate Re-Start COndition
    while (I2C1CONbits.RSEN && !I2C_TIMER_INT_FLAG);	 //Wait for Re-Start COndition
  }
#endif

#if defined(_I2C2MD)
  if (i2c_port == 2) {
    I2C2CONbits.RSEN = 1;		                 //Generate Re-Start COndition
    while (I2C2CONbits.RSEN && !I2C_TIMER_INT_FLAG);	 //Wait for Re-Start COndition
  }
#endif
    
  if (I2C_TIMER_INT_FLAG) {
    return 0xFA00;
  } else {
    return 0x0000;
  }
}



unsigned int WriteByteI2C(unsigned char data, unsigned char i2c_port) {
  I2C_TIMER_REGISTER = 0;
  I2C_TIMER_PERIOD = I2C_TIMEOUT_CYCLES;
  I2C_TIMER_INT_FLAG = 0;
  I2C_TIMER_CON_BITS.TON = 1;
  I2C_TIMER_CON_BITS.TCKPS = I2C_TIMER_PRESCALE_1_1;
  
#if defined(_I2CMD)
  if ((i2c_port == 0) || (i2c_port == 1)) {
    I2CTRN = (data);                                            //Load data to the transmit buffer
    while (!I2CSTATbits.TRSTAT && !I2C_TIMER_INT_FLAG);         //Set when transmit process starts
    while (I2CSTATbits.TRSTAT && !I2C_TIMER_INT_FLAG);          //Cleared at end of Slave ACK
  }
#endif

#if defined(_I2C1MD)
  if (i2c_port == 1) {
    I2C1TRN = (data);                                           //Load data to the transmit buffer
    while (!I2C1STATbits.TRSTAT && !I2C_TIMER_INT_FLAG);        //Set when transmit process starts
    while (I2C1STATbits.TRSTAT && !I2C_TIMER_INT_FLAG);	        //Cleared at end of Slave ACK
  }
#endif

#if defined(_I2C2MD)
  if (i2c_port == 2) {
    I2C2TRN = (data);                                           //Load data to the transmit buffer
    while (!I2C2STATbits.TRSTAT && !I2C_TIMER_INT_FLAG);        //Set when transmit process starts
    while (I2C2STATbits.TRSTAT && !I2C_TIMER_INT_FLAG);	        //Cleared at end of Slave ACK
  }
#endif
    
  if (I2C_TIMER_INT_FLAG) {
    return 0xFA00;
  } else {
    return 0x0000;
  }
}

unsigned int ReadByteI2C(unsigned char i2c_port) {
  unsigned char return_data;

  I2C_TIMER_REGISTER = 0;
  I2C_TIMER_PERIOD = I2C_TIMEOUT_CYCLES;
  I2C_TIMER_INT_FLAG = 0;
  I2C_TIMER_CON_BITS.TON = 1;
  I2C_TIMER_CON_BITS.TCKPS = I2C_TIMER_PRESCALE_1_1;
  
#if defined(_I2CMD)
  if ((i2c_port == 0) || (i2c_port == 1)) {
    I2CCONbits.RCEN = 1;			                 //Start Master receive
    while(I2CCONbits.RCEN && !I2C_TIMER_INT_FLAG);               //Wait for data transfer
    while(!I2CSTATbits.RBF && !I2C_TIMER_INT_FLAG);              //Wait for receive bufer to be full
    return_data = I2CRCV;
  }
#endif

#if defined(_I2C1MD)
  if (i2c_port == 1) {
    I2C1CONbits.RCEN = 1;			                 //Start Master receive
    while(I2C1CONbits.RCEN && !I2C_TIMER_INT_FLAG);               //Wait for data transfer
    while(!I2C1STATbits.RBF && !I2C_TIMER_INT_FLAG);              //Wait for receive bufer to be full
    return_data = I2C1RCV;
  }
#endif

#if defined(_I2C2MD)
  if (i2c_port == 2) {
    I2C2CONbits.RCEN = 1;			                 //Start Master receive
    while(I2C2CONbits.RCEN && !I2C_TIMER_INT_FLAG);               //Wait for data transfer
    while(!I2C2STATbits.RBF && !I2C_TIMER_INT_FLAG);              //Wait for receive bufer to be full
    return_data = I2C2RCV;
  }
#endif
    
  if (I2C_TIMER_INT_FLAG) {
    return 0xFA00;
  } else {
    return (return_data & 0x00FF);
  }
}



unsigned int GenerateI2CStop(unsigned char i2c_port) {
  I2C_TIMER_REGISTER = 0;
  I2C_TIMER_PERIOD = I2C_TIMEOUT_CYCLES;
  I2C_TIMER_INT_FLAG = 0;
  I2C_TIMER_CON_BITS.TON = 1;
  I2C_TIMER_CON_BITS.TCKPS = I2C_TIMER_PRESCALE_1_1;
  
#if defined(_I2CMD)
  if ((i2c_port == 0) || (i2c_port == 1)) {
    I2CCONbits.PEN = 1;	                                 //Generate Stop COndition
    while (I2CCONbits.PEN && !I2C_TIMER_INT_FLAG);	 //Wait for Stop COndition
  }
#endif

#if defined(_I2C1MD)
  if (i2c_port == 1) {
    I2C1CONbits.PEN = 1;		                 //Generate stop COndition
    while (I2C1CONbits.PEN && !I2C_TIMER_INT_FLAG);	 //Wait for stop COndition
  }
#endif

#if defined(_I2C2MD)
  if (i2c_port == 2) {
    I2C2CONbits.PEN = 1;		                 //Generate stop COndition
    while (I2C2CONbits.PEN && !I2C_TIMER_INT_FLAG);	 //Wait for stop COndition
  }
#endif
    
  if (I2C_TIMER_INT_FLAG) {
    return 0xFA00;
  } else {
    return 0x0000;
  }
}


