/*
  -------------------------------------------
  This file contains configuration data specific to the A34760-000
  
  Dan Parker
  2012-06-09

  --------------------------------------------
*/

#ifndef __A34760_H
#define __A34760_H


#include <p30F6014a.h>
#include <dsp.h>
#include <i2c.h>
#include <spi.h>
#include <uart.h>
#include <timer.h>
#include <adc12.h>
#include "Buffer64.h"
#include "MCP23017.h"

// --------- Compile Time Options -----------------

//#define __SELECT_ARI_BENCHTOP_MODE           // This will set the VPROG from the GUI and select the benchtop fault settings


#ifdef __SELECT_ARI_BENCHTOP_MODE
#define __SET_MAGNETRON_OVER_SERIAL_INTERFACE  // If this is set the magnetron voltage/current is set over the serial interface
                                               // If this mode is not selected, the the set point is set over the analog interface
#endif

#define __SERVO_TARGET_CURRENT

#define __RATIO_CONTROL_MODE    
/* 
   -------------- Resource Summary  -------------------
   TMR1 is used to time the Lambda Charge Time and the Lambda Inhibit Time 
   TMR2 is used for 100ms timing.  DO NOT CHANGE PR2 after Initialization
   TMR3 UNUSED
   TMR4 is used for timeout faults on the SPI Bus and I2C BUS
   TMR5 is used for 10mSTicToc.  DO NOT CHANGE PR5 after Initialization



   INT1 - PFN Trigger Signal
   INT2 - Hi/Lo Mode Select (This is not an Interrupt for Company-R)
   INT3 - FP-PFN Modulator Enable (This is not an Interrupt for Company-R)
   INT4 - FP-Modulator On (This is not an Interrupt for Company-R)



   Interupts (From Highest to Lowest Priority)
   * INT1 - (PFN Trigger Signal)
   * TMR1 INT - Times the Lamba Inhibit Time and Charge Time
   * ADC Interrupt
   * U1RX
   * U1TX
   
   
   
   ------------- END Resource Summary ------------------------- 
*/ 



/* ------------------------------ CLOCK AND TIMING CONFIGURATION ------------------------- */
//#define FCY_CLK                    29495000      // 29.495 MHz
//#define FCY_CLK_MHZ                29.495        // 29.495 MHz
#define FCY_CLK                    10000000      // 29.495 MHz
#define FCY_CLK_MHZ                10.000        // 29.495 MHz

#define UART1_BAUDRATE             124000        // U1 Baud Rate
#define I2C_CLK                    100000        // Target I2C Clock frequency of 100KHz



// -------------------------------------------- INTERNAL MODULE CONFIGURATION --------------------------------------------------//

/* 
   --- SPI1 Port ---
   This SPI port is used to connect to the octal DAC
*/
#define A34760_SPI1CON_VALUE  (FRAME_ENABLE_OFF & ENABLE_SDO_PIN & SPI_MODE16_ON & SPI_SMP_OFF & SPI_CKE_ON & SLAVE_ENABLE_OFF & CLK_POL_ACTIVE_HIGH & MASTER_ENABLE_ON)
#define A34760_SPI1STAT_VALUE (SPI_ENABLE & SPI_IDLE_CON & SPI_RX_OVFLOW_CLR)   


/*
  --- SPI2 Port --- 
  This SPI port is used to connect with Isolated 16 Bit DACs
  The prescales of 16:1 and 1:1 will generate a clock = Fcy/16.  In this case 1.843MHz
  This must be slower to compensate for the 2x delay across the optocoupler 200ns with filtering in one direction, 80ns (without filtering) in the other direction
  Minimum clock period is therefore 280ns + holdtime + margins
*/
#define A34760_SPI2CON_VALUE  (FRAME_ENABLE_OFF & DISABLE_SDO_PIN & SPI_MODE16_ON & SPI_SMP_ON & SPI_CKE_ON & SLAVE_ENABLE_OFF & CLK_POL_ACTIVE_HIGH & MASTER_ENABLE_ON)
#define A34760_SPI2STAT_VALUE (SPI_ENABLE & SPI_IDLE_CON & SPI_RX_OVFLOW_CLR)   


#if FCY_CLK > 20000000
// FCY between 20 and 30 MHZ
// SPI1 Clock (divide by 4) 5->7.7MHz
// SPI2 Clock (divide by 16) 1.25MHz - 1.875MHz
#define A34760_SPI1CON_CLOCK (SEC_PRESCAL_1_1 & PRI_PRESCAL_4_1)
#define A34760_SPI2CON_CLOCK (SEC_PRESCAL_1_1 & PRI_PRESCAL_16_1)

#elif FCY_CLK > 15000000
// FCY between 15 and 20 MHZ
// SPI1 Clock (divide by 3) 5->6.6MHz
// SPI2 Clock (divide by 12) 1.25MHz - 1.66MHz
#define A34760_SPI1CON_CLOCK (SEC_PRESCAL_3_1 & PRI_PRESCAL_1_1)
#define A34760_SPI2CON_CLOCK (SEC_PRESCAL_3_1 & PRI_PRESCAL_4_1)

#elif FCY_CLK > 8000000
// FCY Between 8 and 15 MHZ
// SPI1 Clock (divide by 2) 4->7.5MHz
// SPI2 Clock (divide by 8) 1 -> 1.875MHz
#define A34760_SPI1CON_CLOCK (SEC_PRESCAL_2_1 & PRI_PRESCAL_1_1)
#define A34760_SPI2CON_CLOCK (SEC_PRESCAL_2_1 & PRI_PRESCAL_4_1)

#else
// FCY Less than 8 MHz.
// SPI1 Clock (divide by 1) 0 -> 8MHz
// SPI2 Clock (divide by 5) 0 -> 1.6 MHz 
#define A34760_SPI1CON_CLOCK (SEC_PRESCAL_1_1 & PRI_PRESCAL_1_1)
#define A34760_SPI2CON_CLOCK (SEC_PRESCAL_5_1 & PRI_PRESCAL_1_1)

#endif





/* 
   --- I2C setup ---
  See i2c.h and Microchip documentation for more information about the condfiguration
*/
#define I2CCON_SETUP   (I2C_ON & I2C_IDLE_STOP &  I2C_CLK_REL & I2C_IPMI_DIS & I2C_7BIT_ADD & I2C_SLW_EN & I2C_SM_DIS & I2C_GCALL_DIS & I2C_STR_DIS & I2C_ACK & I2C_ACK_DIS)
#define I2C_BAUD_RATE_GENERATOR   ((FCY_CLK/I2C_CLK) - (FCY_CLK/4000000) - 1) // This equation assumes PGD of 250nS.  This is not listed in the data sheet.  The supplementat data sheet uses
                                                                              // 250ns in the table and 900ns in the precalcuated values . . . which is it???  does it matter?  no!!


/* 
   --- UART 1 setup ---
   See uart.h and Microchip documentation for more information about the condfiguration

*/
#define A34760_U1MODE_VALUE        (UART_EN & UART_IDLE_STOP & UART_DIS_WAKE & UART_DIS_LOOPBACK & UART_DIS_ABAUD & UART_NO_PAR_8BIT & UART_1STOPBIT)
#define A34760_U1STA_VALUE         (UART_INT_TX & UART_TX_PIN_NORMAL & UART_TX_ENABLE & UART_INT_RX_CHAR & UART_ADR_DETECT_DIS)
#define A34760_U1BRG_VALUE         (((FCY_CLK/UART1_BAUDRATE)/16)-1)

/*
  --- Timer1 Setup ---
  x64 multiplier will yield max period of 142mS, 2.17uS per tick
*/
#define A34760_T1CON_VALUE             (T1_OFF & T1_IDLE_CON & T1_GATE_OFF & T1_PS_1_64 & T1_SOURCE_INT)
#define TMR1_DELAY_HOLDOFF_US          52        // 50uS
#define TMR1_LAMBDA_CHARGE_TIME_US     2400      // 2.400ms
#define TMR1_DELAY_HOLDOFF             (FCY_CLK_MHZ*TMR1_DELAY_HOLDOFF_US/64)    
#define TMR1_LAMBDA_CHARGE_PERIOD      (FCY_CLK_MHZ*TMR1_LAMBDA_CHARGE_TIME_US/64)
//#define TMR1_DELAY_HOLDOFF             24        //52.1uS
//#define TMR1_LAMBDA_CHARGE_PERIOD      1107      // 2.40mS 


/*
  --- Timer2 Setup ---
  x64 multiplier will yield max period of 142mS, 2.17uS per tick
  Period of 100mS
*/
#define A34760_T2CON_VALUE  (T2_OFF & T2_IDLE_CON & T2_GATE_OFF & T2_PS_1_64 & T2_32BIT_MODE_OFF & T2_SOURCE_INT)
#define A34760_PR2_ROLL_US  100000      // 100mS
#define A34760_PR2_VALUE    FCY_CLK_MHZ*A34760_PR2_ROLL_US/64
//#define A34760_PR2_VALUE    46080


/*
  --- Timer4 Setup ---
  x1 multiplier will yield max period of 2.22mS
*/
#define A34760_T4CON_VALUE  (T4_OFF & T4_IDLE_CON & T4_GATE_OFF & T4_PS_1_1 & T4_32BIT_MODE_OFF & T4_SOURCE_INT)


/*
  --- Timer5 Setup ---
  x8 multiplier will yield max period of 17.7mS, 2.71uS per tick
  Period of 10mS
*/
#define A34760_T5CON_VALUE     (T5_OFF & T5_IDLE_CON & T5_GATE_OFF & T5_PS_1_8 & T5_SOURCE_INT)
#define A34760_PR5_VALUE_US    10000   // 10mS
#define A34760_PR5_VALUE       (FCY_CLK_MHZ*A34760_PR5_VALUE_US/8)
//#define A34760_PR5_VALUE       36869




/* 
   --- 12-BIT ADC Configuration ---
   Goal, when setup, the system will scan through the selected Analog channels and sample
     
   ADC_CONV_CLK_7Tcy2 makes the ADC cloack 350ns at 10 MHz Clock

   AN3 -  PFN Rev Current           - Only sampled after a pulse
   
   AN4 - pac_#1                     - 2.56s tau - Analog Input Bandwidth = 200 Hz
   AN5 - pac_#2                     - 2.56s tau - Analog Input Bandwidth = 200 Hz
   
   AN6 - Thyratron Cathode Heater   - 160mS tau - Analog Input Bandwidth = 10 Hz
   AN7 - Thyratron Reservoir Heater - 160mS tau - Analog Input Bandwidth = 10 Hz
   
   AN8  - magnet_current            - 160mS tau - Analog Input Bandwidth = 200 Hz
   AN9  - magnet_voltage            - 160mS tau - Analog Input Bandwidth = 200 Hz
   
   AN10 - filament_voltage          - 160mS tau - Analog Input Bandwidth = 200 Hz    
   AN11 - filament_current          - 160mS tau - Analog Input Bandwidth = 200 Hz    
   
   AN12 - lambda_vpeak              - 640mS tau - Analog Input Bandwidth = 200 Hz
   AN13 - lambda_vmon               - Only Sampled at EOC
*/



#define A34760_ADCON1_VALUE (ADC_MODULE_OFF & ADC_IDLE_STOP & ADC_FORMAT_INTG & ADC_CLK_AUTO & ADC_AUTO_SAMPLING_ON)
#define A34760_ADCON2_VALUE (ADC_VREF_AVDD_AVSS & ADC_SCAN_ON & ADC_SAMPLES_PER_INT_11 & ADC_ALT_BUF_OFF & ADC_ALT_INPUT_OFF)
#define A34760_ADCON3_VALUE (ADC_SAMPLE_TIME_3 & ADC_CONV_CLK_SYSTEM & ADC_CONV_CLK_7Tcy2)
#define A34760_ADCHS_VALUE  (ADC_CH0_POS_SAMPLEA_AN3 & ADC_CH0_NEG_SAMPLEA_VREFN & ADC_CH0_POS_SAMPLEA_AN3 & ADC_CH0_NEG_SAMPLEB_VREFN)
#define A34760_ADPCFG_VALUE (ENABLE_AN3_ANA & ENABLE_AN4_ANA & ENABLE_AN5_ANA & ENABLE_AN6_ANA & ENABLE_AN7_ANA & ENABLE_AN8_ANA & ENABLE_AN9_ANA & ENABLE_AN10_ANA & ENABLE_AN11_ANA & ENABLE_AN12_ANA & ENABLE_AN13_ANA)
#define A34760_ADCSSL_VALUE (SKIP_SCAN_AN0 & SKIP_SCAN_AN1 & SKIP_SCAN_AN2 & SKIP_SCAN_AN14 & SKIP_SCAN_AN15)

// With these settings, the effective sample rate is around 150K samples/sec




// ----------------------- Internal EEprom ---------------------- //



/*
  --- U64 Port MCP23017 Configuration ---

  U64 Port A - 
  bit 0 = Output - HV Fault (DPARKER - routed to sum fault at the moment)
  bit 1 = Output - Pulse Fault
  bit 2 = Output - Thyratron Fault
  bit 3 = Input  - From HV Lambda - Phase Loss
  bit 4 = Input  - From HV Lambda - Load Fault
  bit 5 = Input  - From HV Lambda - Interlock Fault
  bit 6 = Input  - From HV Lambda - Over Temp
  bit 7 = Input  - Front Panel Input Slow Spare
  

  U64 Port B - 

  bit 0 = Output - Board Fault
  bit 1 = Output - Magnetron Fault
  bit 2 = Output - Spare #1
  bit 3 = Output - Spare #2
  bit 4 = Output - Electromagnet Fault
  bit 5 = Output - Spare #3
  bit 6 = Output - Spare #4
  bit 7 = Output - Modulator Warmup Done
*/

#define MCP23017_DEFAULT_IOCON          0b00000000        // Default IOCON settings
#define MCP23017_U64_IODIRA_VALUE       0b11111000        // Port A.0 -> A.2 are Outputs, A.3-A.7 are inputs
#define MCP23017_U64_IODIRB_VALUE       0x00              // Port B is all Outputs
#define MCP23017_U64_LATA_INITIAL       0xFF              // Outputs on Port A all start High (LED/OPTO NO LIGHTS)
#define MCP23017_U64_LATB_INITIAL       0xFF              // Outputs on Port B all start High (LED/OPTO NO LIGHTS)
#define MCP23017_U64_IPOLA_VALUE        0x00              // Do Not Invert the inputs on Port A
#define MCP23017_U64_IPOLB_VALUE        0x00              // Do Not Invert the inputs on Port B



// ---- Hard Coded Delays ---- //
#define DELAY_TCY_10US                          100       // 10us us at 10MHz Clock

#define DELAY_PULSE_CABLE_SELECT_PROP_DELAY_US  20        // 20uS
                                                          // This delay must be longer than the propogation delay on the Isolated DAC Cable Select Line
#define DELAY_PULSE_CABLE_SELECT_PROP_DELAY     (FCY_CLK_MHZ*DELAY_PULSE_CABLE_SELECT_PROP_DELAY_US)



/*
#define DELAY_TCY_TRIGGER_TO_ADC_SAMPLE_US      2.5       // 2uS
                                                          // This is the delay between when a trigger pulse is detected and when the isolated DACs are sampled
#define DELAY_TCY_TRIGGER_TO_ADC_SAMPLE         (FCY_CLK_MHZ*DELAY_TCY_TRIGGER_TO_ADC_SAMPLE_US)


#define DELAY_TCY_ISR_CALL_MODIFIER             5         // 
                                                          // This compensates for the tirgger ISR call delay and instructions before the delay call is made
                                                          // This should be equal to 4 + the number of instructions cycles to reach 
                                                          //   __delay32(DELAY_TCY_TRIGGER_TO_ADC_SAMPLE - DELAY_TCY_ISR_CALL_MODIFIER);
#define DELAY_TCY_TRIGGER_TO_CURRENT_PULSE_NS   100      
*/


// ----------- Data Structures ------------ //


/*
   ---  magnet supply  ---
   This is a current source.
   The voltage set point = current set point * max resistance of the system
   

   --- filament supply ---  
   This is a voltage source.  The RKW is a voltage source so we don't have the option of running it in current mode
   The current set point = voltage set point / min resistance of the system.


   --- thyratron cathode heater supply ---
   This is a voltae source.  Again voltage program is our only option
   The current set point = voltage set point / min resistance of the system


   --- thyratron reservoir heater supply ---
   This is a voltae source.  Again voltage program is our only option
   The current set point = voltage set point / min resistance of the system


   --- HV Lambda Mode A/B ---
   This is a voltage source.
   The voltage is set by the user directly or by the PAC control loop.
   How is the current set point related to the voltage?  I believe it must be independently set
   In voltage control mode, the current is allowed to vary wildly
   In current control mode, the current set point should be set to PID current target.
   
   
   
*/

typedef struct {
  unsigned int warmup_ramp_time;            // CONSTANT - This is the ramp up time of this supply (in 100mS units)


  unsigned int v_command_set_point;         // EEPROM VARIABLE - The programed voltage set point
  unsigned int v_scaled_set_point;          // RAM - The set point that is scaled from 0->v_command_set_point durring the ramp up time
  unsigned int v_max_set_point;             // CONSTANT - that defines the max acceptable set point.  A value higher than this will be truncated to this value
 
  unsigned int v_dac_set_point;             // RAM - The value of the vprog dac associated with this supply
  unsigned int v_dac_scale;                 // CONSTANT - Ratio Between the set point and the value sent to the DAC 
  unsigned int v_dac_cal_gain;              // EEPROM CONSTANT - Used to calibrate the board for gain errors (Resistor tolerances, Gain stage erros, dac gain, ect)
  int v_dac_cal_offset;                     // EEPROM CONSTANT - Used to calibrate the board for DC offsets. (dac offset, op-amp offsets)

  unsigned int v_adc_reading;               // RAM - The most recent averaged/filtered adc reading available.  A fault checks are made against this value.
  unsigned int v_adc_scale;                 // CONSTANT - Ratio Between the set point and the expected adc reading
  unsigned int v_adc_cal_gain;              // EEPROM CONSTANT - Used to calibrate the board for gain errors (Resistor tolerances, Gain stage erros, dac gain, ect)
  int v_adc_cal_offset;                     // EEPROM CONSTANT - Used to calibrate the board for DC offsets. (adc offset, op-amp offsets)
  unsigned int v_adc_over_abs;              // CONSTANT - Max voltage adc reading.  This is absolute
  unsigned int v_adc_over_rel;              // RAM - Max voltage adc reading.  This value = v_set_point*adc_overvoltage_scale
  unsigned int v_adc_under_rel;             // RAM - Min voltage adc reading.  This value = v_set_point*adc_undervoltage_scale
  unsigned int v_adc_over_scale;            // CONSTANT - that defines the ratio between the program point and the trip point
  unsigned int v_adc_under_scale;           // CONSTANT - that defines the ratio between the program point and the trip point
  unsigned int v_adc_over_min_value;        // CONSTANT - that defines the floor of the overvoltage trip poing.  

  unsigned int v_adc_max_oor;               // CONSTANT
  unsigned int v_adc_over_count;            // RAM - Over voltage counter 
  unsigned int v_adc_under_count;           // RAM - Under voltage counter

  unsigned int v_adc_max_reading;           //
  unsigned int v_adc_min_reading;           //


  unsigned int i_command_set_point;         // EEPROM VARIABLE - The programmed current set point
  unsigned int i_scaled_set_point;          // RAM - The set point that is scaled from 0->i_command_set_point durring the ramp up time
  unsigned int i_max_set_point;             // CONSTANT -  that defines the max acceptable set point.  A value higher than this will be truncated to this value
  
  unsigned int i_dac_set_point;             // RAM - The value of the iprog dac associated with this supply
  unsigned int i_dac_scale;                 // CONSTANT - Ratio Between the set point and the value sent to the DAC 
  unsigned int i_dac_cal_gain;              // EEPROM CONSTANT - Used to calibrate the board for gain errors (Resistor tolerances, Gain stage erros, dac gain, ect)
  int i_dac_cal_offset;                     // EEPROM CONSTANT - Used to calibrate the board for DC offsets. (dac offset, op-amp offsets)

  unsigned int i_adc_reading;               // RAM - The most recent averaged/filtered adc reading available.  A fault checks are made against this value.
  unsigned int i_adc_scale;                 // CONSTANT - Ratio Between the set point and the expected adc reading
  unsigned int i_adc_cal_gain;              // EEPROM CONSTANT - Used to calibrate the board for gain errors (Resistor tolerances, Gain stage erros, dac gain, ect)
  int i_adc_cal_offset;                     // EEPROM CONSTANT - Used to calibrate the board for DC offsets. (adc offset, op-amp offsets)
  unsigned int i_adc_over_abs;              // CONSTANT - Max current adc reading.  This is absolute
  unsigned int i_adc_over_rel;              // RAM - Max current adc reading.  This value = i_set_point*adc_overcurrent_scale
  unsigned int i_adc_under_rel;             // RAM - Min current adc reading.  This value = i_set_point*adc_undercurrent_scale
  unsigned int i_adc_over_scale;            // CONSTANT - that defines the ratio between the program point and the trip point
  unsigned int i_adc_under_scale;           // CONSTANT - that defines the ratio between the program point and the trip point    
  unsigned int i_adc_over_min_value;        // CONSTANT - that defines the floor of the overcurrent trip poing.  

  unsigned int i_adc_max_oor;               // CONSTANT
  unsigned int i_adc_over_count;            // RAM - Over current counter
  unsigned int i_adc_under_count;           // RAM - Under Voltage Counter

  unsigned int i_adc_max_reading;           //
  unsigned int i_adc_min_reading;           // 

} POWERSUPPLY;


/*
  --- STATE DEFINITIONS ---
  See flow diagram for more information
  DPARKER add flow diagram doc number
*/
#define STATE_START_UP                       0x04
#define STATE_FAST_RECOVERY_START_UP         0x06
#define STATE_SYSTEM_COLD_READY              0x14
#define STATE_WARM_UP                        0x24
#define STATE_SYSTEM_WARM_READY              0x34
#define STATE_HV_STARTUP                     0x35
#define STATE_HV_ON                          0x44

#define STATE_FAULT_MCU_CORE_FAULT           0xA0
#define STATE_FAULT_COLD_FAULT               0xA4
#define STATE_FAULT_WARM_FAULT               0xA8
#define STATE_FAULT_ILLEGAL_INTERRUPT        0xA1


/*
  --- Public Functions ---
*/

void DoStateMachine(void);
/*
  This is called to update the FSM
  The state is stored as a global variable so this function has no argument and returns no value
*/

unsigned int Scale16Bit(unsigned int value, unsigned int scale);
void SetPowerSupplyTarget(POWERSUPPLY* ptr_ps, unsigned int v_command, unsigned int i_command);


/*
  --- Gobal Variales ---
*/
extern unsigned char control_state;


extern POWERSUPPLY ps_hv_lambda_mode_A;
extern POWERSUPPLY ps_magnetron_mode_A;
extern POWERSUPPLY ps_hv_lambda_mode_B;
extern POWERSUPPLY ps_magnetron_mode_B;
extern POWERSUPPLY ps_magnet;
extern POWERSUPPLY ps_filament;
extern POWERSUPPLY ps_thyr_cathode_htr;
extern POWERSUPPLY ps_thyr_reservoir_htr;

extern volatile unsigned char global_run_post_pulse_process;
extern volatile unsigned char global_adc_ignore_this_sample;

extern unsigned char a_b_selected_mode;








// ----- Pulse Variables


extern unsigned long arc_counter_persistent;
extern unsigned int arc_counter_consecutive;
extern unsigned int arc_counter_this_hv_on;
extern unsigned long pulse_counter_this_hv_on;
extern unsigned long long pulse_counter_persistent;
extern unsigned int arc_counter_fast;
extern unsigned int arc_counter_slow;


extern unsigned int prf_pulse_counter;
extern unsigned int average_pulse_repetition_frequency_deci_herz;

extern unsigned int led_pulse_count;

extern unsigned int pulse_magnetron_current_adc_reading;
extern unsigned int pulse_magnetron_voltage_adc_reading;



extern unsigned int average_energy_per_pulse_milli_joules;
extern unsigned int average_output_power_watts;
extern unsigned int average_pulse_repetition_frequency_deci_herz;


// adc variables
extern volatile unsigned char adc_result_index;

extern unsigned int pfn_rev_current_array[128];

extern unsigned int pac_1_array[128];
extern unsigned int pac_2_array[128];

extern unsigned int thyratron_cathode_heater_voltage_array[128];
extern unsigned int thyratron_reservoir_heater_voltage_array[128];

extern unsigned int magnetron_magnet_current_array[128];
extern unsigned int magnetron_magnet_voltage_array[128];

extern unsigned int magnetron_filament_current_array[128];
extern unsigned int magnetron_filament_voltage_array[128];

extern unsigned int lambda_vpeak_array[128];
extern unsigned int lambda_vmon_array[128];


extern tPID thyratron_reservoir_heater_PID;

extern tPID thyratron_cathode_heater_PID;


#define PULSE_MODE_A      0
#define PULSE_MODE_B      1


#define SYSTEM_WARM_UP_TIME      1800 //100ms Units  //DPARKER this is way to short
#define LAMBDA_SUPPLY_STARTUP_DELAY  50 // 100ms Units


#define _ISRFASTNOPSV __attribute__((interrupt, shadow, no_auto_psv)) 
#define _ISRNOPSV __attribute__((interrupt, no_auto_psv)) 



unsigned int RCFilter256Tau(unsigned int previous_value, unsigned int current_reading);
unsigned int RCFilter64Tau(unsigned int previous_value, unsigned int current_reading);
unsigned int RCFilter16Tau(unsigned int previous_value, unsigned int current_reading);

extern unsigned int mode_A_pulse_magnetron_current_adc_reading_filtered;
extern unsigned int mode_B_pulse_magnetron_current_adc_reading_filtered;

extern unsigned int mode_A_pulse_magnetron_voltage_adc_reading_filtered;
extern unsigned int mode_B_pulse_magnetron_voltage_adc_reading_filtered;

extern unsigned int pulse_counter_fast;
extern unsigned int pulse_counter_slow;


extern unsigned int mode_A_pulse_magnetron_current_adc_reading_max;
extern unsigned int mode_A_pulse_magnetron_current_adc_reading_min;

extern unsigned int mode_B_pulse_magnetron_current_adc_reading_max;
extern unsigned int mode_B_pulse_magnetron_current_adc_reading_min;

extern BUFFER64BYTE uart1_input_buffer;
extern BUFFER64BYTE uart1_output_buffer;

extern signed int ps_magnet_config_ram_copy[16];
extern signed int ps_filament_config_ram_copy[16];
extern signed int ps_thyr_cathode_htr_config_ram_copy[16];
extern signed int ps_thyr_reservoir_htr_config_ram_copy[16];
extern signed int ps_hv_lambda_mode_A_config_ram_copy[16];
extern signed int ps_hv_lambda_mode_B_config_ram_copy[16];
extern signed int ps_magnetron_mode_A_config_ram_copy[16];
extern signed int ps_magnetron_mode_B_config_ram_copy[16];
extern signed int pulse_counter_repository_ram_copy[16];
extern signed int control_loop_cal_data_ram_copy[16];

extern unsigned long EE_address_ps_magnet_config_in_EEPROM;
extern unsigned long EE_address_ps_filament_config_in_EEPROM;
extern unsigned long EE_address_ps_thyr_cathode_htr_config_in_EEPROM;
extern unsigned long EE_address_ps_thyr_reservoir_htr_config_in_EEPROM;
extern unsigned long EE_address_ps_hv_lambda_mode_A_config_in_EEPROM;
extern unsigned long EE_address_ps_hv_lambda_mode_B_config_in_EEPROM;
extern unsigned long EE_address_ps_magnetron_mode_A_config_in_EEPROM;
extern unsigned long EE_address_ps_magnetron_mode_B_config_in_EEPROM;
extern unsigned long EE_address_pulse_counter_repository_in_EEPROM;
extern unsigned long EE_address_control_loop_cal_data_in_EEPROM;


// U64 MCP23017 Deffenitions                    

// Port A
#define BIT_INPUT_SPARE_SLOW                    0b10000000 
#define BIT_INPUT_HV_LAMBDA_OVER_TEMP           0b01000000 
#define BIT_INPUT_HV_LAMBDA_INTERLOCK_FAULT     0b00100000 
#define BIT_INPUT_HV_LAMBDA_LOAD_FLT            0b00010000 
#define BIT_INPUT_HV_LAMBDA_PHASE_LOSS          0b00001000 
#define BIT_OUTPUT_THYRATRON_FAULT              0b00000100 
#define BIT_OUTPUT_PULSE_FAULT                  0b00000010
#define BIT_OUTPUT_HV_FAULT                     0b00000001 


// Port B
#define BIT_OUTPUT_MODULATOR_WARMUP_DONE        0b10000000 
#define BIT_OUTPUT_SPARE_4                      0b01000000 
#define BIT_OUTPUT_SPARE_3                      0b00100000 
#define BIT_OUTPUT_ELECTROMAGNET_FAULT          0b00010000 
#define BIT_OUTPUT_SPARE_2                      0b00001000 
#define BIT_OUTPUT_HV_ON_READBACK               0b00000100 
#define BIT_OUTPUT_FILAMENT_FAULT               0b00000010 
#define BIT_OUTPUT_CONTROL_BOARD_FAULT          0b00000001 


extern MCP23017 U64_MCP23017;


extern unsigned char ram_config_set_magnetron_magnet_current_from_GUI;

extern volatile unsigned int lvdinterrupt_counter;

extern volatile unsigned int _PERSISTENT last_known_action;
extern volatile unsigned int _PERSISTENT last_osccon;

extern unsigned int previous_last_action;

extern volatile unsigned int timing_error_int1_count;

extern unsigned int _PERSISTENT processor_crash_count;

extern unsigned int software_skip_warmup;

#define LAST_ACTION_DEFAULT_INT                        0xFABC
#define LAST_ACTION_CLEAR_LAST_ACTION                  0x0000
#define LAST_ACTION_ADC_INTERRUPT                      0x0001
#define LAST_ACTION_LVD_INT                            0x0002
#define LAST_ACTION_T1_INT                             0x0003
#define LAST_ACTION_INT1_INT                           0x0004
#define LAST_ACTION_UPDATE_IO_EXPANDER                 0x0005
#define LAST_ACTION_FILTER_ADC                         0x0006
#define LAST_ACTION_READ_ISOLATED_ADC                  0x0007
#define LAST_ACTION_DO_THYRATRON_PID                   0x0008
#define LAST_ACTION_DO_10MS                            0x0009
#define LAST_ACTION_UPDATE_DAC_ALL                     0x000A
#define LAST_ACTION_POST_PULSE_PROC                    0x000B
#define LAST_ACTION_HV_ON_LOOP                         0x000C
#define LAST_ACTION_OSC_FAIL                           0x000D
#define LAST_ACTION_ADDRESS_ERROR                      0x000E
#define LAST_ACTION_STACK_ERROR                        0x000F
#define LAST_ACTION_MATH_ERROR                         0x0010

extern unsigned int linac_high_energy_target_current_adc_reading;
extern unsigned int linac_high_energy_target_current_set_point;

extern unsigned int linac_low_energy_target_current_adc_reading;
extern unsigned int linac_low_energy_target_current_set_point;

extern signed int linac_high_energy_program_offset;
extern signed int linac_low_energy_program_offset;

extern unsigned int pulse_counter_this_run;   // This counts the number of pulses in the current "run".  This will be reset to 0 if there are no triggers for 100mS or more.

#endif
