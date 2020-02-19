#include <xc.h>
#include <libpic30.h>
#include "EEPROM.h"
#include "ETM_CRC.h"


#if defined(__dsPIC30F6014A__)
//#define INTERNAL_EEPROM_SIZE_WORDS  2048
#define INTERNAL_EEPROM_SIZE_WORDS  1024
#endif

#if defined(__dsPIC30F6010A__)
#define INTERNAL_EEPROM_SIZE_WORDS  2048
#endif

#ifndef INTERNAL_EEPROM_SIZE_WORDS
#define INTERNAL_EEPROM_SIZE_WORDS 0
#endif


static unsigned long long alternate_page_data;

typedef struct {
  unsigned int read_internal_error;
  unsigned int read_spi_error;
  unsigned int read_i2c_error;
  unsigned int write_internal_error;
  unsigned int write_spi_error;
  unsigned int write_i2c_error;
  unsigned int page_read_count_internal;
  unsigned int page_read_count_spi;
  unsigned int page_read_count_i2c;
  unsigned int page_write_count_internal;
  unsigned int page_write_count_spi;
  unsigned int page_write_count_i2c;
  unsigned int crc_error;

} TYPE_EEPROM_DEBUG_DATA;

static TYPE_EEPROM_DEBUG_DATA eeprom_debug_data;


unsigned int ETMEEPromPrivateReadSinglePage(unsigned int page_number, unsigned int *page_data);
// This function is used by P1395_CAN_SLAVE

static unsigned int ETMEEPromPrivateWriteSinglePage(unsigned int page_number, unsigned int *page_data);
static unsigned int ETMEEPromPrivateWritePageInternal(unsigned int page_number, unsigned int *data);
static unsigned int ETMEEPromPrivateReadPageInternal(unsigned int page_number, unsigned int *data);


unsigned int ETMEEPromReadPage(unsigned int page_number, unsigned int *page_data) {
  /*
    Data will be returned in *page_data
    If the function return is 0xFFFF the data is valid
    If the function return is 0, the data is NOT valid
  */

  page_number = page_number * 2;
  if (ETMEEPromPrivateReadSinglePage(page_number, &page_data[0])) {
    return 0xFFFF;
  }
  
  if (ETMEEPromPrivateReadSinglePage(page_number + 1, &page_data[0])) {
    return 0xFFFF;
  }

  return 0;
}


unsigned int ETMEEPromWritePage(unsigned int page_number, unsigned int *page_data) {
  /* 
     Will return 0xFFFF if there were no write errors
     Will return 0 if there was a write error to one (or both) registers
  */
  unsigned int error_check = 0xFFFF;

  page_number = page_number * 2;
  
  error_check &= ETMEEPromPrivateWriteSinglePage(page_number, &page_data[0]);
  
  error_check &= ETMEEPromPrivateWriteSinglePage(page_number+1, &page_data[0]);
  
  return error_check;
}


unsigned int ETMEEPromWritePageFast(unsigned int page_number, unsigned int *page_data) {
  unsigned long long alternate_test = 0;
  unsigned int actual_page;
  /*
    Alternate writing between pages
    This should be used for something like pulse counter or off time where it doesn't cause a problem if we miss a single update
    Will return 0xFFFF if there were no write errors
    Will return 0 if there was a write error
  */
  
  actual_page = page_number * 2;

  if (page_number <= 63) {
    alternate_test = alternate_page_data >> page_number;
    alternate_test &= 1;

    if (alternate_test) {
      // use the alternate page register
      actual_page++;
    }
    
    alternate_test = 1;
    alternate_test <<= page_number;
    alternate_page_data ^= alternate_test;
  } 
  
  return (ETMEEPromPrivateWriteSinglePage(actual_page, &page_data[0]));
}


unsigned int ETMEEPromWritePageWithConfirmation(unsigned int page_number, unsigned int *page_data) {
  /*
    Writes to the A and B pages and confirms the values after writing
    This could take a long time to execute
    Will return 0xFFFF if the page data is confirmed in both registers
    Will return 0 if there were any errors (write/read/crc/confirmation)
  */

  unsigned int n;
  unsigned int page_read[16];
  page_number = page_number * 2;
  
  // Write and check page A, abort on error
  if (ETMEEPromPrivateWriteSinglePage(page_number, &page_data[0]) == 0) {
    return 0;
  }
  
  if (ETMEEPromPrivateReadSinglePage(page_number, &page_read[0]) == 0) {
    return 0;
  }
  

  for (n=0; n<16; n++) {
    if (page_data[n] != page_read[n]) {
      return 0;
    }
  }
  
  // Repeat for B register
  if (ETMEEPromPrivateWriteSinglePage(page_number + 1, &page_data[0]) == 0) {
    return 0;
  }
  
  if (ETMEEPromPrivateReadSinglePage(page_number + 1, &page_read[0]) == 0) {
    return 0;
  }


  for (n = 0; n<16; n++) {
    if (page_data[n] != page_read[n]) {
      return 0;
    }
  }


  return 0xFFFF; // The write operation was sucessful
}


unsigned int ETMEEPromWriteWordWithConfirmation(unsigned int register_location, unsigned int data) {
  unsigned int page_read[16];
  unsigned int page_number;

  page_number = (register_location >> 4);
  register_location &= 0x0F;
  
  // Load the existing contents of the register
  if (ETMEEPromReadPage(page_number, &page_read[0]) == 0) {
    return 0;
  }
  
  // Modify the value being changed
  *(&page_read[0] + register_location) = data;
  
  // Write the data back to EEProm
  if (ETMEEPromWritePageWithConfirmation(page_number, &page_read[0])) {
    return 0xFFFF;
  }
  
  return 0;
}


unsigned int ETMEEPromPrivateReadSinglePage(unsigned int page_number, unsigned int *page_data) {
  unsigned int crc_check;

  eeprom_debug_data.page_read_count_internal++;
  if (ETMEEPromPrivateReadPageInternal(page_number, &page_data[0]) == 0) {
    eeprom_debug_data.read_internal_error++;
    return 0;
  } 
  
  crc_check = ETMCRCModbus(&page_data[0], 30);
  if (crc_check != page_data[15]) {
    eeprom_debug_data.crc_error++;
    return 0;
  }
  return 0xFFFF;
}


unsigned int ETMEEPromPrivateWriteSinglePage(unsigned int page_number, unsigned int *page_data) {
  unsigned int crc;
  crc = ETMCRCModbus(&page_data[0], 30);
  page_data[15] = crc;

  
  eeprom_debug_data.page_write_count_internal++;
  if (ETMEEPromPrivateWritePageInternal(page_number, &page_data[0])) {
    return 0xFFFF;
  } else {
    eeprom_debug_data.write_internal_error++;
    return 0;
  } 
  
  return 0;
}




#if INTERNAL_EEPROM_SIZE_WORDS == 0
unsigned int ETMEEPromPrivateWritePageInternal(unsigned int page_number, unsigned int *data) {
  return 0;
}

unsigned int ETMEEPromPrivateReadPageInternal(unsigned int page_number, unsigned int *data) {
  return 0;
}

#else

__eds__ unsigned int internal_eeprom[INTERNAL_EEPROM_SIZE_WORDS] __attribute__ ((space(eedata))) = {};

unsigned int ETMEEPromPrivateWritePageInternal(unsigned int page_number, unsigned int *data) {
  _prog_addressT write_address;
  unsigned int n = 0;
  int data_ram[16];
  unsigned int words_to_write;

  // DPARKER, remove the data_ram variable and write directly with data pointer

  words_to_write = 16;
  
  while (n < words_to_write) {
    data_ram[n] = (int)data[n];
    n++;
  }
  
  if (page_number > (INTERNAL_EEPROM_SIZE_WORDS >> 4)) {
    return 0;
  }
    
  write_address = __builtin_tbladdress(internal_eeprom);
  write_address += page_number << 5;
  _wait_eedata();
  _erase_eedata(write_address, _EE_ROW);
  _wait_eedata();
  _write_eedata_row(write_address, data_ram);
  
  return 0xFFFF;
}


unsigned int ETMEEPromPrivateReadPageInternal(unsigned int page_number, unsigned int *data) {
  unsigned int starting_register;
  unsigned int n;
  unsigned int words_to_read;

  words_to_read = 16;
  starting_register = page_number*16;
  _wait_eedata();
 
  if (page_number > (INTERNAL_EEPROM_SIZE_WORDS >> 4)) {
    return 0;
  }
   
  for (n = 0; n < words_to_read ; n++) {
    data[n] = internal_eeprom[starting_register + n];
  }
  
  return 0xFFFF;
}

#endif


unsigned int ETMEEPromReturnDebugData(unsigned int debug_data_index) {
  unsigned int error_return;
  switch (debug_data_index) {
    
  case ETM_EEPROM_DEBUG_DATA_READ_INTERNAL_ERROR:
    error_return = eeprom_debug_data.read_internal_error;
    break;

  case ETM_EEPROM_DEBUG_DATA_READ_INTERNAL_COUNT:
    error_return = eeprom_debug_data.page_read_count_internal;
    break;

  case ETM_EEPROM_DEBUG_DATA_WRITE_INTERNAL_ERROR:
    error_return = eeprom_debug_data.write_internal_error;
    break;

  case ETM_EEPROM_DEBUG_DATA_WRITE_INTERNAL_COUNT:
    error_return = eeprom_debug_data.page_write_count_internal;
    break;

  case ETM_EEPROM_DEBUG_DATA_READ_SPI_ERROR:
    error_return = eeprom_debug_data.read_spi_error;
    break;

  case ETM_EEPROM_DEBUG_DATA_READ_SPI_COUNT:
    error_return = eeprom_debug_data.page_read_count_spi;
    break;

  case ETM_EEPROM_DEBUG_DATA_WRITE_SPI_ERROR:
    error_return = eeprom_debug_data.write_spi_error;
    break;

  case ETM_EEPROM_DEBUG_DATA_WRITE_SPI_COUNT:
    error_return = eeprom_debug_data.page_write_count_spi;
    break;

  case ETM_EEPROM_DEBUG_DATA_READ_I2C_ERROR:
    error_return = eeprom_debug_data.read_i2c_error;
    break;

  case ETM_EEPROM_DEBUG_DATA_READ_I2C_COUNT:
    error_return = eeprom_debug_data.page_read_count_i2c;
    break;

  case ETM_EEPROM_DEBUG_DATA_WRITE_I2C_ERROR:
    error_return = eeprom_debug_data.write_i2c_error;
    break;

  case ETM_EEPROM_DEBUG_DATA_WRITE_I2C_COUNT:
    error_return = eeprom_debug_data.page_write_count_i2c;
    break;

  case ETM_EEPROM_DEBUG_DATA_CRC_ERROR:
    error_return = eeprom_debug_data.crc_error;
    break;

  }
  return error_return;

}

