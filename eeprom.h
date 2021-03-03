#ifndef ___EEPROM_H
#define ___EEPROM_H







/*
  EEPROM is broken into logical Pages
  Each page is 32 bytes, 16 words
  This is 14 data words and 2 words of checking
       1 word of status (provided by the user) 
       1 word of CRC (generated by this module)
  
  Register locations are 0->(EEPROM_SIZE_WORDS/2)  - We divide by two because each page is mirrored 
  note that all xXXF registers can not be directly written because they are CRC and managed by this module
  
  EEPROM is set up with adjacent pages and primary and mirror pages
  for example 
  logical page 0 uses eeprom page 0 with page 1 as the mirror
  logical page 3 uses eeprom page 6 with page 7 as the mirror

  Register location 32 is logical page 2 word zero
  Therefore it is found at absolute location Page 4 word zero and mirrored at Page 5 word zero
*/

#define EEPROM_REGISTER_MAGNET_SCALING_LINEAR       0+32
#define EEPROM_REGISTER_MAGNET_SCALING_CONST        1+32
#define EEPROM_REGISTER_SCALE_INTERLEAVED           2+32
#define EEPROM_REGISTER_SCALE_LOW_ENERGY            3+32
#define EEPROM_REGISTER_MAGNETRON_FILAMENT_CURRENT  4+32


#define PAGE_0_SETTINGS                             0+2
#define PAGE_1_PULSE_COUNTERS                       1







unsigned int ETMEEPromReadPage(unsigned int page_number, unsigned int *page_data);
/*
  Data will be returned in *page_data
  If the function return is 0xFFFF the data is valid
  If the function return is 0, the data is NOT valid
  If the data in "A" mirror is valid, it will be returned without checking the data in "B" mirror

*/


unsigned int ETMEEPromWritePage(unsigned int page_number, unsigned int *page_data);
/* 
   Write data to "A" and "B" mirror, the data is not confirmed to be written correctly.
   Will return 0xFFFF if there were no write errors
   Will return 0 if there was a write error to one (or both) registers
*/


unsigned int ETMEEPromWritePageFast(unsigned int page_number, unsigned int *page_data);
/*
  Write data to "A" or "B" mirror.  Therefore this is at least twice as fast as the normal page write
  For Pages 0 -> 63 (0x3F), this will alternate between writing to the "A" mirror and "B" mirror
  For Pages >= 64 (0x3F), this will always write to the "A" mirror ("B" mirror will never get written)
  Because the ReadPage functions returns page "A" if its CRC checks out then you could miss more recent results written to "B" mirror
  Therefore this function should only be used to write things where you don't care if the most recent write was missed.
  Things like pulse counter, system on time, warm up timers make sense here.
  Will return 0xFFFF if there were no write errors
  Will return 0 if there was a write error
*/


unsigned int ETMEEPromWritePageWithConfirmation(unsigned int page_number, unsigned int *page_data);
/*
  Writes to the A and B pages and confirms the values after writing
  This could take a long time to execute depending upon the EEProm type and bus speed
  Will return 0xFFFF if the page data is confirmed in both registers
  Will return 0 if there were any errors (write/read/crc/confirmation)
*/


unsigned int ETMEEPromWriteWordWithConfirmation(unsigned int register_location, unsigned int data);
/*
  Updates A and B pages where register_location resides and confirms the values after writing
  This could take a long time to execute depending upon the EEProm type and bus speed
  Will return 0xFFFF if the page data is confirmed in both registers
  Will return 0 if there were any errors (write/read/crc/confirmation)
*/


unsigned int ETMEEPromReturnDebugData(unsigned int debug_data_index);
/*
  Returns a single word of debug data.
  Types of debug data defined below
*/

#define ETM_EEPROM_DEBUG_DATA_READ_INTERNAL_COUNT           0
#define ETM_EEPROM_DEBUG_DATA_READ_INTERNAL_ERROR           1
#define ETM_EEPROM_DEBUG_DATA_WRITE_INTERNAL_COUNT          2
#define ETM_EEPROM_DEBUG_DATA_WRITE_INTERNAL_ERROR          3
#define ETM_EEPROM_DEBUG_DATA_READ_I2C_COUNT                4
#define ETM_EEPROM_DEBUG_DATA_READ_I2C_ERROR                5
#define ETM_EEPROM_DEBUG_DATA_WRITE_I2C_COUNT               6
#define ETM_EEPROM_DEBUG_DATA_WRITE_I2C_ERROR               7
#define ETM_EEPROM_DEBUG_DATA_READ_SPI_COUNT                8
#define ETM_EEPROM_DEBUG_DATA_READ_SPI_ERROR                9
#define ETM_EEPROM_DEBUG_DATA_WRITE_SPI_COUNT               10
#define ETM_EEPROM_DEBUG_DATA_WRITE_SPI_ERROR               11
#define ETM_EEPROM_DEBUG_DATA_CRC_ERROR                     12

#endif
