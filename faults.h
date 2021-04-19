#ifndef __FAULTS_H
#define __FAULTS_H

#include "A34760.h"

typedef struct {
  unsigned int magnetron_current_adc_glitch;
  unsigned int magnetron_voltage_adc_glitch;
  unsigned int i2c_bus_error;
  unsigned int spi1_bus_error;
  unsigned int spi2_bus_error;
  unsigned int external_adc_false_trigger;
  unsigned int LTC2656_write_error;
  unsigned int setpoint_not_valid;
  unsigned int scale16bit_saturation;
  unsigned int reversescale16bit_saturation;
  unsigned int pulse_latch_reset_error;
} TYPE_DEBUG_COUNTER;

extern TYPE_DEBUG_COUNTER global_debug_counter;


// Simplified Fault List

/*

Magnetron Heater Faults
* External Analog Comparator Over voltage with Latch
* ADC Voltage reading over hard limit
* ADC Voltage reading over programmed value by X percent
* ADC Voltage reading under programmed value by X percent
* ADC Current reading to high for programmed voltage - A short circuit could cause this
* ADC Current reading to low for porgrammed voltage - An open circuit could cause this

Magnetron Magnet 
* External Analog Comparator Current out of Range Latch (note this will of course be set durring start up)
* ADC Current reading over hard limit
* ADC Current reading over programmed value by X percent
* ADC Current reading under programmed value by X percent
* ADC Voltage reading to high for programmed current - An open circuit could cause this
* ADC Voltage reading to low for porgrammed current - A short circuit could cause this

Arc Faults
* Too many consecutive alts
* Too many arcs over the last 300 pulses (1 second at full rep rate)
* Too many arcs over the last 18000 Pulses (1 minute at full rep rate)


Lambda Faults
* Lambda Sum Fault
* Lambda Over Temp
* Lambda Interlock Open
* Lambda Load Fault (usually a short on the lambda output)
* Lambda Phase Loss
* Lambda EOC Timeout (the lambda did not reach EOC durring the required time)
* ADC Lambda vpeak reading is too high
* ADC Lambda vpeak reading is too low (this needs a delay because the lamabda of course starts at zero Volts so it takes a while for Vpeak to charge up)

Thyratron Faults
* ADC Cathode Heater Voltage over hard limit
* ADC Cathode Heater Voltage over programmed value by X%
* ADC Cathode Heater Voltage under programmed value by X%
* Cathode Heater 4-20mA driver fault (the same line is used to monitor both the cathode heater and reservoir heater driver fault)
* Cathode Heater Control Saturation - The PID loop has saturated indicating that the cathode voltage is no longer under control

* ADC Reservoir Heater Voltage over hard limit
* ADC Reservoir Heater Voltage over programmed value by X%
* ADC Reservoir Heater Voltage under programmed value by X%
* Reservoir Heater 4-20mA driver fault (the same line is used to monitor both the cathode heater and reservoir heater driver fault)
* Reservoir Heater Control Saturation - The PID loop has saturated indicating that the reservoir voltage is no longer under control


Other Faults on control board
* The High Voltage Lambda is Not Powered
* Control Board Interlock #1 is open (this is where the water flow is monitored on the test unit)
* Control Board Interlock #2 is open
* Control Board Interlock #3 is open
* Control Board Interlock #4 is open


*/

// Debug Fault Register

/*
  This register is not handeled like the other fault registers.  The "faults" or "status" in this register can only be latched.  
  This is register is reset at power on.  It is not saved to EEPROM or normally monitored except in the lab.

  EEprom error - the test of the eeprom failed
  IO Expanded Error - the test of the external I/O expander Failed
  BOR Reset - The processor restarted from a Brown out Reset
  TRAPR Reset - The processor restarted from a Trap Reset
  WDT Reset - The processor restarted from a Watch Dog Timer Reset
  IOPUWR Reset - The processor restarted from a Illegal Condition Reset
  
  Math Saturation Errors - Overflow in the 16 bit math - this would be a result of a bug in the code or bad config paramenters
  
*/



// ------------------- FAULT REGISTER SET UP --------------------- //

extern unsigned int debug_status_register;

#define STATUS_BOR_RESET                          0x0001 // The processor restarted from a Brown Out Reset 
#define STATUS_TRAPR_RESET                        0x0002 // The processor restarted from a Trap Reset
#define STATUS_WDT_RESET                          0x0004 // The processor restarted from a Watch Dog Time Out 
#define STATUS_IOPUWR_RESET                       0x0008 // The processor restarted from a Illegal Operation Reset
#define STATUS_POR_RESET                          0x0010 // The processor restarted from a Power On Reset
#define STATUS_EXTERNAL_RESET                     0x0020 // The processor restarted from an External Reset
#define STATUS_SOFTARE_RESET                      0x0040 // The processor restarted from a Software Reset
#define STATUS_DIGITAL_IO_EXP_ERROR               0x0080 // The read/write/read test of one of the IO Expanders failed

#define STATUS_UNUSED_3                           0x0100 
#define STATUS_UNUSED_4                           0x0200 
#define STATUS_UNUSED_5                           0x0400 
#define STATUS_UNUSED_6                           0x0800 
#define STATUS_UNUSED_7                           0x1000
#define STATUS_UNUSED_8                           0x2000
#define STATUS_UNUSED_9                           0x4000
#define STATUS_UNUSED_10                          0x8000


// MAGNETRON CONTROL FAULT REGISTER
extern unsigned int faults_magnetron_status_reg;
extern unsigned int faults_magnetron_fault_reg;
extern unsigned int faults_magnetron_warning_reg;


// Magnetron Heater Faults
#define FAULT_HW_MAGNETRON_FILAMENT_OV            0x0001 // The external latched analog comparator detects a heater over voltage. 
#define FAULT_MAGNETRON_FILAMENT_OV_HARD_LIMIT    0x0002 // Voltage ADC reading exceeded it's hard limit
#define FAULT_MAGNETRON_FILAMENT_OV               0x0004 // Voltage ADC reading exceeds programed value by X percent
#define FAULT_MAGNETRON_FILAMENT_UV               0x0008 // Voltage ADC reading is less than programed value by X percent
#define FAULT_MAGNETRON_FILAMENT_OC               0x0010 // Current ADC reading is too high for the programed voltage
#define FAULT_MAGNETRON_FILAMENT_UC               0x0020 // Current ADC reading is too low for the programed voltage
#define FAULT_MAGNETRON_FILAMENT_UNUSED_1         0x0040 // UNUSED FAULT BIT
#define FAULT_MAGNETRON_FILAMENT_UNUSED_2         0x0080 // UNUSED FAULT BIT

// Magnetron Magnet Faults
#define FAULT_HW_MAGNETRON_MAGNET_COOR            0x0100 // The external latched analog comparator detects a magnet current out of range. 
#define FAULT_MAGNETRON_MAGNET_OC_HARD_LIMIT      0x0200 // Current ADC reading exceeded it's hard limit
#define FAULT_MAGNETRON_MAGNET_OC                 0x0400 // Current ADC reading exceeds programed value by X percent
#define FAULT_MAGNETRON_MAGNET_UC                 0x0800 // Current ADC reading is less than programed value by X percent
#define FAULT_MAGNETRON_MAGNET_OV                 0x1000 // Voltage ADC reading is too high for the programed current
#define FAULT_MAGNETRON_MAGNET_UV                 0x2000 // Voltage ADC reading is too low for the programed current
#define FAULT_MAGNETRON_MAGNET_UNUSED_1           0x4000 // UNUSED FAULT BIT
#define FAULT_MAGNETRON_MAGNET_UNUSED_2           0x8000 // UNUSED FAULT BIT


// HIGH VOLTAGE FAULT REGISTERS
extern unsigned int faults_high_voltage_status_reg;
extern unsigned int faults_high_voltage_fault_reg;
extern unsigned int faults_high_voltage_warning_reg;

#define EOC_RESET_TIME_1p5ms    (1500 * FCY_CLK_MHZ)  /* 1.5ms */
#define EOC_RESET_TIME_3ms      (3000 * FCY_CLK_MHZ)  /* 1.5ms */
#define EOC_RESET_TIME_5ms      (5000 * FCY_CLK_MHZ)  /* 1.5ms */

#define EOC_MAX_COUNT	    10
#define EOC_FAULT_DELAY_TIME   	100	  /* 1s in 10ms step, delay fault report for data logging */
extern unsigned char lambda_eoc_fault;
extern unsigned char eoc_counts_consecutive;
extern int eoc_max_reached_timer;
extern unsigned int eoc_counts_total;


#define FAULT_ARC_OVER_COUNT_CONSECUTIVE          0x0001 // There have been too many consecutive arcs 
#define FAULT_ARC_OVER_COUNT_FAST                 0x0002 // The arc rate exceeded the max arcs per short time period
#define FAULT_ARC_OVER_COUNT_SLOW                 0x0004 // The arc rate exceeded the max arcs per long time period
#define FAULT_ARC_UNUSED                          0x0008 // NOT IMPLIMENTED

#define FAULT_PULSE_CURRENT_OOR_MODE_A            0x0010 
#define FAULT_PULSE_CURRENT_OOR_MODE_B            0x0020
#define FAULT_PULSE_VOLTAGE_OOR_MODE_A            0x0040 
#define FAULT_PULSE_VOLTAGE_OOR_MODE_B            0x0080 

#define FAULT_HV_LAMBDA_SUM_FAULT                 0x0100 // This is the digital sum fault from the Lambda 
#define FAULT_HV_LAMBDA_EOC_TIMEOUT               0x0200 // The lambda did not reach EOC in the time that it should.
#define FAULT_HV_LAMBDA_VPEAK_OVER_VOLTAGE        0x0400 // This vpeak reading from the lamabda is too high (needs a long delay)
#define FAULT_HV_LAMBDA_VPEAK_UNDER_VOLTAGE       0x0800 // This vpeak reading from the lamabda is too low  (needs a long delay)
#define FAULT_HV_LAMBDA_OVER_TEMP                 0x1000 // STATUS ONLY!!! - This is the digital over temp from the Lambda 
#define FAULT_HV_LAMBDA_INTERLOCK_FAULT           0x2000 // STATUS ONLY!!! - This is the digital interlock fault from the Lambda
#define FAULT_HV_LAMBDA_LOAD_FAULT                0x4000 // STATUS ONLY!!! - This is the digital load fault from the Lambda
#define FAULT_HV_LAMBDA_PHASE_LOSS                0x8000 // STATUS ONLY!!! - This is the digital phase loss fault from the Lambda




// THYRATRON HEATER FAULT REGISTERS
extern unsigned int faults_thyratron_status_reg;
extern unsigned int faults_thyratron_fault_reg;
extern unsigned int faults_thyratron_warning_reg;


#define FAULT_THYR_CATHODE_HEATER_OV_HARD_LIMIT   0x0001 // Voltage ADC reading exceeded it's hard limit
#define FAULT_THYR_CATHODE_HEATER_OV              0x0002 // Voltage ADC reading exceeds programed value by X percent
#define FAULT_THYR_CATHODE_HEATER_UV              0x0004 // Voltage ADC reading is less than programed value by X percent
#define FAULT_THYR_CATHODE_HEATER_DRVR_FLT        0x0008 // This fault is set by the a fault condition on the 4-20mA Line
#define FAULT_THYR_CATHODE_HEATER_CONTROL_SAT     0x0010 // This fault indicates that the control loop has saturated and is therfore Out of Regulation

#define FAULT_THYR_RESER_HEATER_OV_HARD_LIMIT     0x0020 // Voltage ADC reading exceeded it's hard limit
#define FAULT_THYR_RESER_HEATER_OV                0x0040 // Voltage ADC reading exceeds programed value by X percent
#define FAULT_THYR_RESER_HEATER_UV                0x0080 // Voltage ADC reading is less than programed value by X percent
#define FAULT_THYR_RESER_HEATER_DRVR_FLT          0x0100 // This fault is set by the a fault condition on the 4-20mA Line
#define FAULT_THYR_RESER_HEATER_CONTROL_SAT       0x0200 // This fault indicates that the control loop has saturated and is therefore Out of Regulation


#define FAULT_THYR_FALSE_TRIGGER                  0x0400
#define FAULT_THYR_UNUSED_2                       0x0800
#define FAULT_THYR_UNUSED_3                       0x1000
#define FAULT_THYR_UNUSED_4                       0x2000
#define FAULT_THYR_UNUSED_5                       0x4000
#define FAULT_THYR_UNUSED_6                       0x8000


// CONTROL BOARD FAULT REGISTERS
extern unsigned int faults_control_board_status_reg;
extern unsigned int faults_control_board_fault_reg;
extern unsigned int faults_control_board_warning_reg;


#define FAULT_LAMBDA_OFF                          0x0001 // This fault is software determination that the Lambda is not powered up.
#define FAULT_DIGITAL_INTERLOCK_1                 0x0002 // Digital Interlock #1 (Water Flow Switch)is open
#define FAULT_DIGITAL_INTERLOCK_2                 0x0004 // Digital Interlock #2 (The HV tank temp)is open
#define FAULT_DIGITAL_INTERLOCK_3                 0x0008 // Digital Interlock #3 (Thyratron Temp switch) is open
#define FAULT_DIGITAL_INTERLOCK_4                 0x0010 // Digital Interlock #4 (Air Flow) is open

#define FAULT_CB_SPI_UNRECOVERABLE_ERROR          0x0020 
#define FAULT_CB_UNUSED_1                         0x0040 
#define FAULT_CB_UNUSED_2                         0x0080 
#define FAULT_CB_UNUSED_3                         0x0100 
#define FAULT_CB_UNUSED_4                         0x0200
#define FAULT_CB_UNUSED_5                         0x0400
#define FAULT_CB_UNUSED_6                         0x0800
#define FAULT_CB_UNUSED_7                         0x1000
#define FAULT_CB_UNUSED_8                         0x2000
#define FAULT_CB_UNUSED_9                         0x4000
#define FAULT_CB_UNUSED_10                        0x8000


#define FAULT_NO_WATER_FLOW                       FAULT_DIGITAL_INTERLOCK_1
#define FAULT_SF6                                 FAULT_DIGITAL_INTERLOCK_2
#define FAULT_THYRATRON_OVER_TEMP                 FAULT_DIGITAL_INTERLOCK_3

// Fault Interlocks to Real Names
// DPARKER Add real names here


// The fast arc counter will shutdown with more than 25 arcs in 400 pulses
// This is not exactly true.  Every arc increments a counter.  The counter is decremented every 400/25 pulses.
// In order to trip in N pulses you need 25 + N/16 arcs

#define ARC_COUNTER_FAST_PERIOD                   10000   
#define ARC_COUNTER_FAST_MAX_ARCS                 400   
#define ARC_COUNTER_FAST_DECREMENT_INTERVAL       (ARC_COUNTER_FAST_PERIOD / ARC_COUNTER_FAST_MAX_ARCS)


// The slow arc counter will shutdown with more than 100 arcs in 24000 pulses
#define ARC_COUNTER_SLOW_PERIOD                   10000
#define ARC_COUNTER_SLOW_MAX_ARCS                 500 // The system 
#define ARC_COUNTER_SLOW_DECREMENT_INTERVAL       (ARC_COUNTER_SLOW_PERIOD / ARC_COUNTER_SLOW_MAX_ARCS)
    
// The consecutive arc counter will shut down with more than 5 consecutive arcs
#define ARC_COUNTER_MAX_CONSECUTIVE               5


#define NO_FAULTS  0x0000
#define ALL_FAULTS 0x1111    



// ------------ FAULT CONFIGURATION -------------------- //


// ------- ADC glitch fault detection -------------
#define ADC_PULSE_CURRENT_OVER_GLITCH  0xFA00  // ~128.87 Amps
#define ADC_PULSE_CURRENT_UNDER_GLITCH 0x3000  // ~24.75 Amps

#define ADC_PULSE_VOLTAGE_OVER_GLITCH  0xF000  // DPARKER set real values
#define ADC_PULSE_VOLTAGE_UNDER_GLITCH 0x0800  // DPARKER set real values




// The following Faults are Considered "Cold Faults" and will cause the system to move to the "Cold Fault" state if their bit is set in the fault register
// This is Independent of the control loop state
#ifndef __SELECT_ARI_BENCHTOP_MODE
#define MASK_CONTROL_BOARD_COLD_FAULTS              (NO_FAULTS)
#else
#define MASK_CONTROL_BOARD_COLD_FAULTS              (FAULT_NO_WATER_FLOW)
#endif

#define MASK_THYRATRON_COLD_FAULTS                  (NO_FAULTS)
#define MASK_MAGNETRON_COLD_FAULTS                  (FAULT_HW_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_OV_HARD_LIMIT | FAULT_MAGNETRON_MAGNET_OC_HARD_LIMIT)
#define MASK_HIGH_VOLTAGE_COLD_FAULTS               (NO_FAULTS)



// STATE START UP FAULT DEFINITIONS
#define FAULT_MASK_MAGNETRON_STATE_START_UP         (NO_FAULTS)
#define FAULT_MASK_THYRATRON_STATE_START_UP         (NO_FAULTS)
#define FAULT_MASK_HIGH_VOLTAGE_STATE_START_UP      (NO_FAULTS)
#define FAULT_MASK_CONTROL_BOARD_STATE_START_UP     (NO_FAULTS)

#define WARNING_MASK_MAGNETRON_STATE_START_UP       (NO_FAULTS)
#define WARNING_MASK_THYRATRON_STATE_START_UP       (NO_FAULTS)
#define WARNING_MASK_HIGH_VOLTAGE_STATE_START_UP    (NO_FAULTS)
#define WARNING_MASK_CONTROL_BOARD_STATE_START_UP   (NO_FAULTS)


// STATE COLD READY FAULT DEFINITIONS
#define FAULT_MASK_MAGNETRON_STATE_COLD_READY       (NO_FAULTS)
#define FAULT_MASK_THYRATRON_STATE_COLD_READY       (NO_FAULTS)
#define FAULT_MASK_HIGH_VOLTAGE_STATE_COLD_READY    (NO_FAULTS)
#define FAULT_MASK_CONTROL_BOARD_STATE_COLD_READY   (NO_FAULTS)

#define WARNING_MASK_MAGNETRON_STATE_COLD_READY     (NO_FAULTS)
#define WARNING_MASK_THYRATRON_STATE_COLD_READY     (NO_FAULTS)
#define WARNING_MASK_HIGH_VOLTAGE_STATE_COLD_READY  (NO_FAULTS)
#define WARNING_MASK_CONTROL_BOARD_STATE_COLD_READY (NO_FAULTS)


// STATE WARM UP FAULT DEFINITIONS
#define FAULT_MASK_MAGNETRON_STATE_WARM_UP          (FAULT_HW_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_OV_HARD_LIMIT | FAULT_MAGNETRON_MAGNET_OC_HARD_LIMIT)
#define FAULT_MASK_THYRATRON_STATE_WARM_UP          (NO_FAULTS)
#define FAULT_MASK_HIGH_VOLTAGE_STATE_WARM_UP       (NO_FAULTS)
#define FAULT_MASK_CONTROL_BOARD_STATE_WARM_UP      (NO_FAULTS)

#define WARNING_MASK_MAGNETRON_STATE_WARM_UP        (NO_FAULTS)
#define WARNING_MASK_THYRATRON_STATE_WARM_UP        (NO_FAULTS)
#define WARNING_MASK_HIGH_VOLTAGE_STATE_WARM_UP     (NO_FAULTS)
#define WARNING_MASK_CONTROL_BOARD_STATE_WARM_UP    (NO_FAULTS)



// STATE WARM READY FAULT DEFINITIONS
#ifndef __MK_POWER_SUPPLIES
#define FAULT_MASK_MAGNETRON_STATE_WARM_READY       (FAULT_HW_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_OV_HARD_LIMIT | FAULT_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_UV | FAULT_MAGNETRON_FILAMENT_OC | FAULT_MAGNETRON_FILAMENT_UC | FAULT_HW_MAGNETRON_MAGNET_COOR)
#else
#define FAULT_MASK_MAGNETRON_STATE_WARM_READY       (FAULT_HW_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_OV_HARD_LIMIT | FAULT_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_OC | FAULT_MAGNETRON_FILAMENT_UC | FAULT_HW_MAGNETRON_MAGNET_COOR)
#endif
#define FAULT_MASK_THYRATRON_STATE_WARM_READY       (NO_FAULTS)
#define FAULT_MASK_HIGH_VOLTAGE_STATE_WARM_READY    (NO_FAULTS)
#ifndef __SELECT_ARI_BENCHTOP_MODE
#define FAULT_MASK_CONTROL_BOARD_STATE_WARM_READY   (NO_FAULTS)
#else
#define FAULT_MASK_CONTROL_BOARD_STATE_WARM_READY   (FAULT_THYRATRON_OVER_TEMP | FAULT_NO_WATER_FLOW | FAULT_SF6)
#endif
 
#define WARNING_MASK_MAGNETRON_STATE_WARM_READY     (NO_FAULTS)
#define WARNING_MASK_THYRATRON_STATE_WARM_READY     (NO_FAULTS)
#define WARNING_MASK_HIGH_VOLTAGE_STATE_WARM_READY  (NO_FAULTS)
#define WARNING_MASK_CONTROL_BOARD_STATE_WARM_READY (NO_FAULTS)


// STATE HV ON FAULT DEFINITIONS
#ifndef __MK_POWER_SUPPLIES
#define FAULT_MASK_MAGNETRON_STATE_HV_ON            (FAULT_HW_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_OV_HARD_LIMIT | FAULT_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_UV | FAULT_MAGNETRON_FILAMENT_OC | FAULT_MAGNETRON_FILAMENT_UC | FAULT_HW_MAGNETRON_MAGNET_COOR)
#else
#define FAULT_MASK_MAGNETRON_STATE_HV_ON            (FAULT_HW_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_OV_HARD_LIMIT | FAULT_MAGNETRON_FILAMENT_OV | FAULT_MAGNETRON_FILAMENT_OC | FAULT_MAGNETRON_FILAMENT_UC | FAULT_HW_MAGNETRON_MAGNET_COOR)
#endif


#define FAULT_MASK_THYRATRON_STATE_HV_ON            (NO_FAULTS)
#define FAULT_MASK_HIGH_VOLTAGE_STATE_HV_ON         (FAULT_ARC_OVER_COUNT_FAST | FAULT_ARC_OVER_COUNT_SLOW | FAULT_HV_LAMBDA_SUM_FAULT | FAULT_HV_LAMBDA_EOC_TIMEOUT | FAULT_HV_LAMBDA_OVER_TEMP | FAULT_HV_LAMBDA_INTERLOCK_FAULT | FAULT_HV_LAMBDA_LOAD_FAULT | FAULT_HV_LAMBDA_PHASE_LOSS)
#ifndef __SELECT_ARI_BENCHTOP_MODE
#define FAULT_MASK_CONTROL_BOARD_STATE_HV_ON        (FAULT_LAMBDA_OFF)
#else
#define FAULT_MASK_CONTROL_BOARD_STATE_HV_ON        (FAULT_LAMBDA_OFF | FAULT_THYRATRON_OVER_TEMP | FAULT_NO_WATER_FLOW | FAULT_SF6)
#endif

#define WARNING_MASK_MAGNETRON_STATE_HV_ON          (NO_FAULTS)
#define WARNING_MASK_THYRATRON_STATE_HV_ON          (NO_FAULTS)
#define WARNING_MASK_HIGH_VOLTAGE_STATE_HV_ON       (NO_FAULTS)
#define WARNING_MASK_CONTROL_BOARD_STATE_HV_ON      (NO_FAULTS)


// STATE WARM FAULT FAULT DEFINITIONS
#define FAULT_MASK_MAGNETRON_STATE_WARM_FAULT       (FAULT_MAGNETRON_MAGNET_OC_HARD_LIMIT)
#define FAULT_MASK_THYRATRON_STATE_WARM_FAULT       (NO_FAULTS)
#define FAULT_MASK_HIGH_VOLTAGE_STATE_WARM_FAULT    (NO_FAULTS)
#ifndef __SELECT_ARI_BENCHTOP_MODE
#define FAULT_MASK_CONTROL_BOARD_STATE_WARM_FAULT   (NO_FAULTS)
#else
#define FAULT_MASK_CONTROL_BOARD_STATE_WARM_FAULT   (FAULT_NO_WATER_FLOW)
#endif

#define WARNING_MASK_MAGNETRON_STATE_WARM_FAULT     (NO_FAULTS)
#define WARNING_MASK_THYRATRON_STATE_WARM_FAULT     (NO_FAULTS)
#define WARNING_MASK_HIGH_VOLTAGE_STATE_WARM_FAULT  (NO_FAULTS)
#define WARNING_MASK_CONTROL_BOARD_STATE_WARM_FAULT (NO_FAULTS)


// STATE COLD FAULT FAULT DEFINITIONS
#define FAULT_MASK_MAGNETRON_STATE_COLD_FAULT       (NO_FAULTS)
#define FAULT_MASK_THYRATRON_STATE_COLD_FAULT       (NO_FAULTS)
#define FAULT_MASK_HIGH_VOLTAGE_STATE_COLD_FAULT    (NO_FAULTS)
#define FAULT_MASK_CONTROL_BOARD_STATE_COLD_FAULT   (NO_FAULTS)

#define WARNING_MASK_MAGNETRON_STATE_COLD_FAULT     (NO_FAULTS)
#define WARNING_MASK_THYRATRON_STATE_COLD_FAULT     (NO_FAULTS)
#define WARNING_MASK_HIGH_VOLTAGE_STATE_COLD_FAULT  (NO_FAULTS)
#define WARNING_MASK_CONTROL_BOARD_STATE_COLD_FAULT (NO_FAULTS)





void UpdateFaults(void);
/*
  This function updates all faults that are checked on a periodic basis.
  This is all faults EXCEPT for the pulse faults (these are checked after each pulse)
  It is called by Do10msTicToc() once every 10 ms
  What this function does
  * Loads the fault/warning masks for the current state
  * Checks all the Magnetron Faults Inputs and sets Status/Warning/Fault registers
  * Checks all the HV Lambda Fault Inputs and sets Status/Warning/Fault registers
  * Checks all the Thyratron Fault Inputs and sets Status/Warning/Fault registers
  * Checks all the Control Board Faults and sets Status/Warning/Fault registers

*/


void UpdatePulseData(unsigned char mode);
/*
  This function updates all the pulse faults and is called after each pulse.
  * Looks for an arc. 
*/

void ResetHWLatches(void);

void ResetAllFaults();
// DPARKER need to write function

unsigned int CheckStartupFailed(void);

unsigned int CheckFaultActive(void);

unsigned int CheckColdFaultActive(void);


#define MAGNETRON_FAULT                 0x00
#define HIGH_VOLTAGE_FAULT              0x01
#define THYRATRON_FAULT                 0x02
#define CONTROL_BOARD_FAULT             0x03


#define THYRATRON_DAC_SATURATED         0x7D00



void RecordThisMagnetronFault(unsigned int fault_bit);
void RecordThisHighVoltageFault(unsigned int fault_bit);
void RecordThisThyratronFault(unsigned int fault_bit);
void RecordThisControlBoardFault(unsigned int fault_bit);


void ResetPulseLatches(void);


#endif



