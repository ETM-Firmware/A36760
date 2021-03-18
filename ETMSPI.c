#include "ETMSPI.h"

#define  SPI_RX_OVFLOW_CLR     0xffbf   /* Clear receive overflow bit.*/

unsigned long SendAndReceiveSPI(unsigned int data_word, unsigned char spi_port) {
  unsigned char spi_bus_status;
  unsigned int return_data;
  unsigned int temp;
  
  spi_bus_status = SPI_BUS_ACTIVE;
  SPI_TIMER_REGISTER = 0;
  SPI_TIMER_PERIOD = SPI_TIMEOUT_CYCLES;
  SPI_TIMER_INT_FLAG = 0;
  SPI_TIMER_CON_BITS.TON = 1;
  SPI_TIMER_CON_BITS.TCKPS = SPI_TIMER_PRESCALE_1_1;
  

#if defined(_SPIIF)
  if ((spi_port == 0) || (spi_port == 1)) {
    _SPIIF = 0;
    SPIBUF = data_word;
    while (spi_bus_status == SPI_BUS_ACTIVE) {
      if (SPI_TIMER_INT_FLAG) {
	// There was a timeout of the data read, this is effectivly a fault
	spi_bus_status = SPI_BUS_TIMEOUT;
      }
      if (SPISTATbits.SPIROV) {
	// There was a data overflow in the buffer, this is a fault
	spi_bus_status = SPI_BUS_OVERFLOW;
      }
      if (!SPISTATbits.SPITBF) {
	spi_bus_status = SPI_DATA_SENT;
      }
    }
    
    while(spi_bus_status == SPI_DATA_SENT) {
      if (SPI_TIMER_INT_FLAG) {
	// There was a timeout of the data read, this is effectivly a fault
	spi_bus_status = SPI_BUS_TIMEOUT;
      }
      if (SPISTATbits.SPIROV) {
	// There was a data overflow in the buffer, this is a fault
	spi_bus_status = SPI_BUS_OVERFLOW;
      }
      if (_SPIIF) {
	// Data  been recieved in the buffer, read the data from the return buffer
	spi_bus_status = SPI_DATA_RECEIVED;
      }    
    }
    return_data = SPIBUF;
  }
#endif


#if defined(_SPI1IF)
  if (spi_port == 1) {
    _SPI1IF = 0;
    SPI1STAT &= SPI_RX_OVFLOW_CLR;
    if(SPI1STATbits.SPIRBF)
    {
      temp = SPI1BUF;
    }
    SPI1BUF = data_word;
    while (spi_bus_status == SPI_BUS_ACTIVE) {
      if (SPI_TIMER_INT_FLAG) {
	// There was a timeout of the data read, this is effectivly a fault
	spi_bus_status = SPI_BUS_TIMEOUT;
      }
      if (SPI1STATbits.SPIROV) {
	// There was a data overflow in the buffer, this is a fault
	spi_bus_status = SPI_BUS_OVERFLOW;
      }
      if (!SPI1STATbits.SPITBF) {
	spi_bus_status = SPI_DATA_SENT;
      }
    }
    
    while(spi_bus_status == SPI_DATA_SENT) {
      if (SPI_TIMER_INT_FLAG) {
	// There was a timeout of the data read, this is effectivly a fault
	spi_bus_status = SPI_BUS_TIMEOUT;
      }
      if (SPI1STATbits.SPIROV) {
	// There was a data overflow in the buffer, this is a fault
	spi_bus_status = SPI_BUS_OVERFLOW;
      }
      //if (SPI1STATbits.SPIRBF) {
      // A byte has been recieved in the buffer, read the data from the return buffer
      if (_SPI1IF) {
	// Data  been recieved in the buffer, read the data from the return buffer
	spi_bus_status = SPI_DATA_RECEIVED;
      }    
    }
    return_data = SPI1BUF;
  }
#endif



#if defined(_SPI2IF)
  if (spi_port == 2) {

    _SPI2IF = 0;
    SPI2STAT &= SPI_RX_OVFLOW_CLR;
    if(SPI2STATbits.SPIRBF)
    {
      temp = SPI2BUF;
    }
    SPI2BUF = data_word;
    while (spi_bus_status == SPI_BUS_ACTIVE) {
      if (SPI_TIMER_INT_FLAG) {
	// There was a timeout of the data read, this is effectivly a fault
	spi_bus_status = SPI_BUS_TIMEOUT;
      }
      if (SPI2STATbits.SPIROV) {
	// There was a data overflow in the buffer, this is a fault
	spi_bus_status = SPI_BUS_OVERFLOW;
      }
      if (!SPI2STATbits.SPITBF) {
	spi_bus_status = SPI_DATA_SENT;  // Data has been moved into the shift register for sending out
      }
    }
    
    while(spi_bus_status == SPI_DATA_SENT) {
      if (SPI_TIMER_INT_FLAG) {
	// There was a timeout of the data read, this is effectivly a fault
	spi_bus_status = SPI_BUS_TIMEOUT;
      }
      if (SPI2STATbits.SPIROV) {
	// There was a data overflow in the buffer, this is a fault
	spi_bus_status = SPI_BUS_OVERFLOW;
      }
      if (_SPI2IF) {
	// Data  been recieved in the buffer, read the data from the return buffer
	spi_bus_status = SPI_DATA_RECEIVED;
      }    
    }
    return_data = SPI2BUF;
  }
#endif
  
  if (spi_bus_status == SPI_DATA_RECEIVED) {
    return (0x0000FFFF & return_data);
  } else {
    return (0x11110000);
  }
}
