#ifndef __LTC2656_H
#define __LTC2656_H


#include "ETMSPI.h"
#include "IOPorts.h"




// ---------------- SPI Configuration for LTC2656 -----------------------//
/* 

   The SPI port needs to be configured before any calls to functions in this module are made
   Multiple devices may be connected to the SPI port but the SPI port only needs to be configured once
   The SPI port should be configured the same for all devices on the same SPI port
   To use the following deffenitions, spi.h must be included where this setup is done
   
   Set up the configuration parameters for the SPI port used to connect to the LTC2656
   Set spi.h for more documentation

*/


/* -------------- DAC Command Definitions ------------------- */
#define LTC2656_CMD_WRITE_TO_INPUT_REGISTER_N             0b00000000 // writes to input register N
#define LTC2656_CMD_UPDATE_DAC_REGISTER_N                 0b00010000 // updates output N
#define LTC2656_CMD_WRITE_TO_INPUT_REGISTER_N_UPDATE_ALL  0b00100000 // Writes to input register N and updates ALL outputs
#define LTC2656_CMD_WRITE_AND_UPDATE_N                    0b00110000 // Write to input register N and updates output N
#define LTC2656_CMD_POWER_DOWN_N                          0b01000000 // Powers down channel N
#define LTC2656_CMD_POWER_DOWN_CHIP                       0b01010000 // Powers down the chip and reference
#define LTC2656_CMD_SELECT_INTERNAL_REFERENCE             0b01100000 // Selects and Powers up the internal Reference
#define LTC2656_CMD_SELECT_EXTERNAL_REFERENCE             0b01110000 // Selects External Reference and powers down internal Reference
#define LTC2656_CMD_NO_OPERATION                          0b11110000 // No operation

#define LTC2656_ADR_A                                     0b00000000 // DAC A
#define LTC2656_ADR_B                                     0b00000001 // DAC B
#define LTC2656_ADR_C                                     0b00000010 // DAC C
#define LTC2656_ADR_D                                     0b00000011 // DAC D
#define LTC2656_ADR_E                                     0b00000100 // DAC E
#define LTC2656_ADR_F                                     0b00000101 // DAC F
#define LTC2656_ADR_G                                     0b00000110 // DAC G
#define LTC2656_ADR_H                                     0b00000111 // DAC H
#define LTC2656_ADR_ALL_DACS                              0b00001111 // ALL DACs

#define LTC2656_WRITE_AND_UPDATE_DAC_A                    (LTC2656_CMD_WRITE_AND_UPDATE_N | LTC2656_ADR_A)
#define LTC2656_WRITE_AND_UPDATE_DAC_B                    (LTC2656_CMD_WRITE_AND_UPDATE_N | LTC2656_ADR_B)
#define LTC2656_WRITE_AND_UPDATE_DAC_C                    (LTC2656_CMD_WRITE_AND_UPDATE_N | LTC2656_ADR_C)
#define LTC2656_WRITE_AND_UPDATE_DAC_D                    (LTC2656_CMD_WRITE_AND_UPDATE_N | LTC2656_ADR_D)
#define LTC2656_WRITE_AND_UPDATE_DAC_E                    (LTC2656_CMD_WRITE_AND_UPDATE_N | LTC2656_ADR_E)
#define LTC2656_WRITE_AND_UPDATE_DAC_F                    (LTC2656_CMD_WRITE_AND_UPDATE_N | LTC2656_ADR_F)
#define LTC2656_WRITE_AND_UPDATE_DAC_G                    (LTC2656_CMD_WRITE_AND_UPDATE_N | LTC2656_ADR_G)
#define LTC2656_WRITE_AND_UPDATE_DAC_H                    (LTC2656_CMD_WRITE_AND_UPDATE_N | LTC2656_ADR_H)

/* -------------- END DAC Command Definitions ------------------- */


typedef struct {
  unsigned int pin_cable_select;
  unsigned int pin_dac_clear;
  unsigned int pin_load_dac;
  unsigned int pin_por_select;
  unsigned int por_select_value;
  unsigned char spi_port;
} LTC2656;


#define LTC2656_SELECT_CHIP                      0
#define LTC2656_CLEAR_OUTPUTS                    0
#define LTC2656_LOAD_DAC                         0
#define LTC2656_ONLY_SPI_PORT                    0
#define LTC2656_SPI_PORT_1                       1
#define LTC2656_SPI_PORT_2                       2

#define LTC2656_DELAY_DAC_CLEAR                  100


//void SetupLTC2656(volatile struct LTC2656Struct *ptr_LTC2656);
void SetupLTC2656(LTC2656* ptr_LTC2656);
/*  
   Function Arguments:
   *ptr_LTC2656 : This is pointer to the structure that defines a particular LTC2656 chip
              
   This function should be called when somewhere durring the startup of the processor.
   This will configure the SPI port (selected by spi_port) and the CS pin.
   This will also set all the outputs to Zero.
   
*/


unsigned char WriteLTC2656(LTC2656* ptr_LTC2656, unsigned int command_word, unsigned int data_word);

/*  
   Function Arguments:
   *ptr_LTC2656     : This is pointer to the structure that defines a particular LTC2656 chip

   command_word     : This is the command word as defined above.  See the LTC2656 spec sheet for more information
                      You may wish to #define more commands using the "N commands and a Channel"
		      For Example if you wanted a command that powered down only CHN B
		      #define LTC2656_CMD_POWER_DOWN_CHIP             (LTC2656_CMD_POWER_DOWN_N | LTC2656_ADR_B)
   data_word        : This is the data (Voltage) to be sent along with the command
                      Voltage is scaled to the chip reference - Internal Reference is selected in this SetupLTC2656()
		      If using 12 bit version of the chip, only the 12 MSB are used. The 4 LSB are don't care.

   This will send a 32 bit command to to the LTC2656.  
   If will read back the data sent to the LTC2656
   If there is an error with the data transmission (error on SPI bus, or the data check failed), then the function will return the error code
   If the data transmission is successful, the function will return 0
*/


unsigned char WriteLTC2656AllDacChannels(LTC2656* ptr_LTC2656, unsigned int *dac_array);
/*  
   Function Arguments:
   *ptr_LTC2656     : This is pointer to the structure that defines a particular LTC2656 chip

   *ptr_data        : This is a pointer to the array (8 words) that contains the data for the 8 dac channels

   This will attempt to send out 8 commands (32 bits per command) and the SPI BUS
   The return data will be checked to ensure that the data was properly recieved
   If the entire data transfer is successful without errors, the function will return 0.
   If there are any errors on the spi bus or a data error is detected the function will exit immediatly and return the error code
*/


#endif
