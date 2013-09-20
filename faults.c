#include "A34760.h"
#include "faults.h"
#include "A34760_PINS.h"
#include <libpic30.h>
#include "Config.h"
#include "MCP23017.h"


TYPE_DEBUG_COUNTER global_debug_counter;

unsigned int CalculatePulseEnergy(unsigned int voltage_adc, unsigned int current_adc);

void StorePulseData(POWERSUPPLY* ptr);

unsigned int CalculatePulseEnergyMilliJouleFromCurrent(unsigned int current_adc);

// The status register used for debug purposes
unsigned int debug_status_register;

// MAGNETRON CONTROL FAULT REGISTERS
unsigned int faults_magnetron_status_reg;
unsigned int faults_magnetron_fault_reg;
unsigned int faults_magnetron_warning_reg;

// HIGH VOLTAGE FAULT REGISTERS
unsigned int faults_high_voltage_status_reg;
unsigned int faults_high_voltage_fault_reg;
unsigned int faults_high_voltage_warning_reg;

// THYRATRON HEATER FAULT REGISTERS
unsigned int faults_thyratron_status_reg;
unsigned int faults_thyratron_fault_reg;
unsigned int faults_thyratron_warning_reg;


// CONTROL BOARD FAULT REGISTERS
unsigned int faults_control_board_status_reg;
unsigned int faults_control_board_fault_reg;
unsigned int faults_control_board_warning_reg;



// Variables to save the active fault/warning masks for this control loop state
unsigned int magnetron_fault_mask;
unsigned int high_voltage_fault_mask;
unsigned int thyratron_fault_mask;
unsigned int control_board_fault_mask;

unsigned int magnetron_warning_mask;
unsigned int high_voltage_warning_mask;
unsigned int thyratron_warning_mask;
unsigned int control_board_warning_mask;


void LoadFaultMaskRegisters(void);
void WriteToEventLog(unsigned char fault_register, unsigned int fault_bit);
unsigned char CheckOverVoltFault(POWERSUPPLY* ptr);
unsigned char CheckUnderVoltFault(POWERSUPPLY* ptr);
unsigned char CheckOverCurrentFault(POWERSUPPLY* ptr);
unsigned char CheckUnderCurrentFault(POWERSUPPLY* ptr);


/*
  FAULT EVALUATION
  
  10_ms_tic_toc
  Once every 10ms, the internal ADC is read and faults are updated

  After Every Pulse

  DPARKER give a big description of each fault here - How the input is measured, how the data is filtered, how the fault is generated (delay if included)

*/


/*
  In each State, Each fault input can do one of the following . . .

  1) It can be totally ignored (This is the default behavior - If the warning mask & the fault mask are NOT set)
  2) It can cause a fault  (This is set by the fault MASK)
     There are no "warm" or "cold" faults.  Any fault in state warm_ready or hv_on will go to state warm fault.
     If the fault is still active in the warm state fault then it will move to cold fault.
  3) It can cause a latched warning (This is set the warning MASK)
     NOTE: the warning register is independent of the fault mask.  To be latched as a warning, the fault input MUST BE IN THE WARNING REGISTER
  4) Special state at startup to handle board level failures . . . ????
*/

/*  
    Fault Log
    
    Each entry in the fault log contains the following information
    4 bits for fault register bit
    2 bits for fault register select
    2 bits for the calling state (HV_ON, WARM_READY, FAULT, STARTUP)
*/

/*
  For each state there are three fault masks for each fault register.
  Warning Ignore Mask - This shows which fault inputs will NOT generate a warning (this is mainly for debugging and and is used to filter out non-events, if a fault is in the warning ignore mask and in the warm or cold fault mask it will still generate a fault)
  Faul Mask - this shows which fault inputs will generate a fault in the current state


*/

/*

  Status_Register -> This register shows the current states of all the Inputs.  It IS NOT LATCHED.
  Fault_Register -> This register latches any fault input that matches the fault_mask for the current state.
  Warning_Latch_Regisiter -> Any fault input that matches the fault_mask or the warning_mask will be latched in this register.

  How faults are checked . . .
  
  Every time through a control loop, 50us->500us depending upon the state, all faults registeres are updated and checked.
  Faults like magnetron_heater_over_current - The limit is compared to the filtered data in RAM every time, even though the filtered data in ram is only update once every 10mS
  The fault status register is then compared to the masks to generate the warnings and faults.

 */

/* ----------------------FAULT MANAGEMENT ----------------------*/

/*
  There are lots of Fault Conditions, see A34335.h for a list of fault registeres and faults.
  For a given state a particular Input may
  1) No Action - But set a non latching status bit indicating the state of the fault input
  2) No Action - But set a latching "warning" bit 
  3) Generate a fault Condition - This will change the state to warm fault (or cold fault if the fault is globably defined as a cold fault)
 

  How are faults handeled . . .
  There a 3 data storage locations for each fault register.
  Status_Register -> This register shows the current states of all the Inputs.  It IS NOT LATCHED.
  Latch_Regisiter -> This is a latched version of the the status register.
  Fault_Register  -> If the Input is a Fault (it matches the warm_fault OR cold_fault mask) it is latched into the Fault Register
  
  How external data and faults are processesed . . . 

  Data is read from the external ADCs and inernal ADCs on a regular 10ms interval or after a pulse (for pulse data types).
  This inerval is scheduled to occur directly proceding a pulse if possible (pulse rate > 100 Hz).
  At PRF less than 100Hz (or in any other state) the readings will take place at opproximate 10ms intervals
  ADC values are filtered with software RC/glitch filters when they are read.
 
 
  Faults are evaluated durring the 10ms_tic_toc the occurs once every (approximatly) 10mS
  The following steps occur
  1) Status registers are reset to zero
  2) The input condition is tested, if it is a "logical fault" then (record_this_xxxxx_fault) is called which does the following
    a) Sets the appropriate bit in the status register
    b) If the bit matches the fault_mask, 
       ^ The appropriate bit in the fault register is set
       ^ The fault is added to the error log - TO BE IMPLEMENTED
    c) If the bit matches the warning_mask, the appropriate bit in the warning register is set 
  
  STEP 2 is repeated for all fault conditions.

  Some faults can not be tested durring the 10ms TicToc.
  These faults will have record_this_xxxxx_fault called when the fault is checked.
  These faults are . . .
  FAULT_HV_LAMBDA_EOC_TIMEOUT - this is evaluated in the TMR1 interrupt and set there.


  After all the faults have been tested and the fault registers updated the following action occurs
  
  + A cold shutdown occurs
  + A warm shutdown occurs
  
  + The fault is added to the Log
  + For *some* important faults, the fault counter is incremented.

  + The fault is added to the Log
  + For *some* important faults, the fault counter is incremented.
  
*/



// DPARKER update pulse fault must be called at 10ms Interval or the sections that count "out of range" counts will be arbitrary time lengths

void UpdateFaults(void) {
  // See h file for documentation
  unsigned int temp_u16int;

  // The status registers are not latched so they are reset to zero each time the faults are evaluated
  faults_magnetron_status_reg = 0;  
  faults_high_voltage_status_reg = 0;
  faults_thyratron_status_reg = 0;
  faults_control_board_status_reg = 0;
  
  
  // Load the fault masks for the current state
  LoadFaultMaskRegisters();
  
  //------------------------- START MAGNETRON FAULTS ------------------------------//
  
  // Check External Magnetron Heater Over Voltage Latch
  if (PIN_FILAMENT_OV_LATCH == ILL_FILAMENT_OV_FAULT) {
    RecordThisMagnetronFault(FAULT_HW_MAGNETRON_FILAMENT_OV);
  } 
  
  // Check that the magnetron heater voltage ADC reading has exceed fixed value set Config.h 
  if (ps_filament.v_adc_reading > ps_filament.v_adc_over_abs) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_FILAMENT_OV_HARD_LIMIT);
  }
  
  // Check that the magnetron heater voltage ADC reading is not greater than X% of its program point (set in Config.h)
  // It must remain outside this range for at least v_out_of_range_count before a fault is generated
  if (CheckOverVoltFault(&ps_filament)) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_FILAMENT_OV);
  }
  
  // Check that the magnetron heater voltage ADC reading is not less than X% of its program point (set in Config.h)
  // It must remain outside this range for at least v_out_of_range_count before a fault is generated
  if (CheckUnderVoltFault(&ps_filament)) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_FILAMENT_UV);
  }
  
  // Check that the magnetron heater current ADC reading is not greater than X% of its expected point (set in Config.h)
  // It must remain outside this range for at least v_out_of_range_count before a fault is generated
  if (CheckOverCurrentFault(&ps_filament)) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_FILAMENT_OC);
  }
  
  
  // Check that the magnetron heater current ADC reading is not less than X% of its expected point (set in Config.h)
  // It must remain outside this range for at least v_out_of_range_count before a fault is generated
  if (CheckUnderCurrentFault(&ps_filament)) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_FILAMENT_UC);
  }
  
  
  // Check External Magnetron Magnet Current Out of Range Latch
  if (PIN_MAGNET_CURRENT_OOR_LATCH == ILL_MAGNET_CURRENT_OOR_FAULT) {
    RecordThisMagnetronFault(FAULT_HW_MAGNETRON_MAGNET_COOR);
  }
  
  // Check that the magnetron magnet current ADC reading has no exceeded fixed value set Config.h 
  if (ps_magnet.i_adc_reading > ps_magnet.i_adc_over_abs) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_MAGNET_OC_HARD_LIMIT);
  }
  
  // Check that the magnetron magnet current ADC reading is not greater than X% of its program point (set in Config.h)
  if (CheckOverCurrentFault(&ps_magnet)) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_MAGNET_OC);
  }
  
  // Check that the magnetron magnet current ADC reading is not less than X% of its program point (set in Config.h)
  if (CheckUnderCurrentFault(&ps_magnet)) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_MAGNET_UC);
  }
  
  // Check that the magnetron magnet voltage ADC reading is not greater than X% of its expected value (set in Config.h)
  if (CheckOverVoltFault(&ps_magnet)) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_MAGNET_OV);
  }
  
  // Check that the magnetron magnet voltage ADC reading is not less than X% of its expected value (set in Config.h)
  if (CheckUnderVoltFault(&ps_magnet)) {
    RecordThisMagnetronFault(FAULT_MAGNETRON_MAGNET_UV);
  }
  
  
  //------------------------- START HIGH VOLTAGE FAULTS ------------------------------//
  
  // THE ARC FAULTS ARE CURRENTLY SET IN UpdatePulseData()
  // UpdatesPulseData() is called after every pulse.
  
  // Check the digital fault outputs from from HV LAMBDA -
  // The sum fault line must be set for HV_LAMBDA_SUM_FAULT_COUNTER_TRIP_POINT consective readings before a trip will occur
  
  
  
  if (PIN_HV_LAMBDA_SUM_FAULT == ILL_HV_LAMBDA_SUM_FAULT_FAULTED) {
    // Record the sum fault and check the other fault lines
    RecordThisHighVoltageFault(FAULT_HV_LAMBDA_SUM_FAULT);
    
    temp_u16int = MCP23017ReadSingleByte(&U64_MCP23017, MCP23017_REGISTER_GPIOA);
    if (temp_u16int >= 0xFA00) {
      global_debug_counter.i2c_bus_error++;
    } else {
      U64_MCP23017.input_port_a_in_ram = temp_u16int & 0xFF;
      if (U64_MCP23017.input_port_a_in_ram & BIT_INPUT_HV_LAMBDA_OVER_TEMP) {
	RecordThisHighVoltageFault(FAULT_HV_LAMBDA_OVER_TEMP);
      }
      if (U64_MCP23017.input_port_a_in_ram & BIT_INPUT_HV_LAMBDA_INTERLOCK_FAULT) {
	RecordThisHighVoltageFault(FAULT_HV_LAMBDA_INTERLOCK_FAULT);
      }
      if (U64_MCP23017.input_port_a_in_ram & BIT_INPUT_HV_LAMBDA_LOAD_FLT) {
	RecordThisHighVoltageFault(FAULT_HV_LAMBDA_LOAD_FAULT);
      }
      if (U64_MCP23017.input_port_a_in_ram & BIT_INPUT_HV_LAMBDA_PHASE_LOSS) {
	RecordThisHighVoltageFault(FAULT_HV_LAMBDA_PHASE_LOSS);
      }
    }
  }
  
  /*
    FAULT_LAMBDA_EOC_TIMEOUT is checked/set by the TMR1 Interrupt
  */


  // DPARKER these vpeak readings . . . do we need them?  do they serve any purpose????
  /*

  // Check that the lambda vpeak ADC reading is not greater than X% of its set point (set in Config.h)
  // It must remain outside this range for at least HV_LAMBDA_VPEAK_MAX_OUT_OF_RANGE_COUNT before a fault is generated
  if (hv_lambda_vpeak_adc_reading > hv_lambda_vpeak_adc_over_trip_point) {
    hv_lambda_vpeak_over_voltage_count++;
  } else if (hv_lambda_vpeak_over_voltage_count >= 1) {
    hv_lambda_vpeak_over_voltage_count--;
  }
  if (hv_lambda_vpeak_over_voltage_count > HV_LAMBDA_VPEAK_MAX_OUT_OF_RANGE_COUNT) {
    RecordThisHighVoltageFault(FAULT_HV_LAMBDA_VPEAK_OVER_VOLTAGE);
  }

  // Check that the lambda vpeak ADC reading is not less than X% of its set point (set in Config.h)
  // It must remain outside this range for at least HV_LAMBDA_VPEAK_MAX_OUT_OF_RANGE_COUNT before a fault is generated
  if (hv_lambda_vpeak_adc_reading < hv_lambda_vpeak_adc_under_trip_point) {
    hv_lambda_vpeak_under_voltage_count++;
  } else if (hv_lambda_vpeak_under_voltage_count >= 1) {
    hv_lambda_vpeak_under_voltage_count--;
  }
  if (hv_lambda_vpeak_under_voltage_count > HV_LAMBDA_VPEAK_MAX_OUT_OF_RANGE_COUNT) {
    RecordThisHighVoltageFault(FAULT_HV_LAMBDA_VPEAK_UNDER_VOLTAGE);
  }
  */
  
  //------------------------- START THYRATRON FAULTS ------------------------------//
 
  // Check that the thyratron cathode heater voltage ADC reading has exceed fixed value set Config.h 
  if (ps_thyr_cathode_htr.v_adc_reading > ps_thyr_cathode_htr.v_adc_over_abs) {
    RecordThisThyratronFault(FAULT_THYR_CATHODE_HEATER_OV_HARD_LIMIT);
  }
    
  // Check that the thyratron heater voltage ADC reading is not greater than X% of its program point (set in Config.h)
  // It must remain outside this range for at least THYRATRON_HEATER_MAX_OUT_OF_RANGE_COUNT before a fault is generated  
  if (CheckOverVoltFault(&ps_thyr_cathode_htr)) {
    RecordThisThyratronFault(FAULT_THYR_CATHODE_HEATER_OV);
  }

  // Check that the thyratron heater voltage ADC reading is not less than X% of its program point (set in Config.h)
  // It must remain outside this range for at least THYRATRON_HEATER_MAX_OUT_OF_RANGE_COUNT before a fault is generated
  if (CheckUnderVoltFault(&ps_thyr_cathode_htr)) {
    RecordThisThyratronFault(FAULT_THYR_CATHODE_HEATER_UV);
  }
    
  // Check if the 4-20ma Driver has reported a fault
  if (PIN_4_20_DRVR_FLT == ILL_4_20_DRIVER_FAULT) {
    RecordThisThyratronFault(FAULT_THYR_CATHODE_HEATER_DRVR_FLT);
  }

  // Check to see if the control loop has saturated.
  // If it has that means that the SCR controller is no longer responding and it should be shut down
  temp_u16int = thyratron_cathode_heater_PID.controlOutput;
  if (temp_u16int & 0x8000) {
    temp_u16int = 0x0000;
  }
  temp_u16int = temp_u16int << 1;
  if (temp_u16int >= THYRATRON_DAC_SATURATED) {
    RecordThisThyratronFault(FAULT_THYR_CATHODE_HEATER_CONTROL_SAT);
  }
  


  // Check if the thyratron reservoir ADC reading has exceed fixed value set Config.h 
  if (ps_thyr_reservoir_htr.v_adc_reading > ps_thyr_reservoir_htr.v_adc_over_abs) {
    RecordThisThyratronFault(FAULT_THYR_RESER_HEATER_OV_HARD_LIMIT);
  }


  // Check that the thyratron reservoir voltage ADC reading is not greater than X% of its program point (set in Config.h)
  // It must remain outside this range for at least THYRATRON_HEATER_MAX_OUT_OF_RANGE_COUNT before a fault is generated  
  if (CheckOverVoltFault(&ps_thyr_reservoir_htr)) {
    RecordThisThyratronFault(FAULT_THYR_RESER_HEATER_OV);
  }

  // Check that the thyratron reservoir voltage ADC reading is not less than X% of its program point (set in Config.h)
  // It must remain outside this range for at least THYRATRON_HEATER_MAX_OUT_OF_RANGE_COUNT before a fault is generated
  if (CheckUnderVoltFault(&ps_thyr_reservoir_htr)) {
    RecordThisThyratronFault(FAULT_THYR_RESER_HEATER_UV);
  }

  // Check if the 4-20ma Driver has reported a fault
  if (PIN_4_20_DRVR_FLT == ILL_4_20_DRIVER_FAULT) {
    RecordThisThyratronFault(FAULT_THYR_RESER_HEATER_DRVR_FLT);
  }
  
  // Check to see if the control loop has saturated.
  // If it has that means that the SCR controller is no longer responding and it should be shut down
  temp_u16int = thyratron_reservoir_heater_PID.controlOutput;
  if (temp_u16int & 0x8000) {
    temp_u16int = 0x0000;
  }
  temp_u16int = temp_u16int << 1;
  if (temp_u16int >= THYRATRON_DAC_SATURATED) {
    RecordThisThyratronFault(FAULT_THYR_RESER_HEATER_CONTROL_SAT);
  }


  //------------------------- START CONTROL BOARD FAULTS ------------------------------//


  // Check that the lambda supply powered up
  if (PIN_HV_LAMBDA_POWER_UP == ILL_PIN_HV_LAMBDA_DID_NOT_POWER_UP) {
    RecordThisControlBoardFault(FAULT_LAMBDA_OFF);
  }
  
  // Check to see if digital interlock 1 is open  
  if (PIN_INTERLOCK_1 == ILL_INTERLOCK_OPEN) {
    RecordThisControlBoardFault(FAULT_DIGITAL_INTERLOCK_1);
  }
  
  // Check to see if digital interlock 2 is open  
  if (PIN_INTERLOCK_2 == ILL_INTERLOCK_OPEN) {
    RecordThisControlBoardFault(FAULT_DIGITAL_INTERLOCK_2);
  }
  
  // Check to see if digital interlock 3 is open  
  if (PIN_INTERLOCK_3 == ILL_INTERLOCK_OPEN) {
    RecordThisControlBoardFault(FAULT_DIGITAL_INTERLOCK_3);
  }
  
  // Check to see if digital interlock 4 is open  
  if (PIN_INTERLOCK_4 == ILL_INTERLOCK_OPEN) {
    RecordThisControlBoardFault(FAULT_DIGITAL_INTERLOCK_4);
  }
  
}

void ResetPulseLatches(void) {
  PIN_PULSE_LATCH_RESET = OLL_PULSE_LATCH_RESET;
  __delay32(DELAY_PULSE_LATCH_RESET);
  PIN_PULSE_LATCH_RESET = !OLL_PULSE_LATCH_RESET;
}


void ResetHWLatches(void) {
  // Reset the latches
  PIN_LATCH_RESET = OLL_RESET_LATCH;
  __delay32(DELAY_LATCH_RESET);
  PIN_LATCH_RESET = !OLL_RESET_LATCH;
}

void ResetAllFaults(void) {
  ResetHWLatches();

  // Clear all fault registers

  // MAGNETRON CONTROL FAULT REGISTERS
  faults_magnetron_fault_reg       = 0;
  faults_magnetron_warning_reg     = 0;
  
  // HIGH VOLTAGE FAULT REGISTERS
  faults_high_voltage_fault_reg    = 0;
  faults_high_voltage_warning_reg  = 0;
  
  // THYRATRON HEATER FAULT REGISTERS
  faults_thyratron_fault_reg       = 0;
  faults_thyratron_warning_reg     = 0;
    
  // CONTROL BOARD FAULT REGISTERS
  faults_control_board_fault_reg   = 0;
  faults_control_board_warning_reg = 0;
}


void UpdatePulseData(unsigned char mode) {
  /*
    FAULT_PFN_TRIGGER_PERIOD_TOO_SHORT is evaluated by the TMR1 Interrupt
  */
  
  /* 
     First Perform Data itegrity checks on the data from the AD converter
     
     Arc detection is the responsibility of the HW circuits.

     In the event of an ARC the current readback IS NOT INCLUDED in the filtered Values.

     If there was NOT AN ARC, further checking is done on the current readback to insure it is valid.
     
     If the current is inside a specified value (ADC_PULSE_CURRENT_UNDER_GLITCH -> ADC_PULSE_CURRENT_OVER_GLITCH) it is included in filtering math.
     If not the value is ignored from a filtering point of view

     Sowftware over/under current/voltage detection.
        
  */

  /*
    Tracking  Arcs


    Hardware Arc is defined as 
         (1) setting Overcurrent Latch 
      OR (2) failing to set Undercurrent Latch


    Software Glitch is defined as 
         (1) Not setting the ( Hardware ARC OR Software ARC)
     AND (2) Reading outside the Hardware range limits

    There is an Arc Counter that increments with every Hardware Arc (this is stored in EEPROM) (32 bit)
    There is an Arc Counter in RAM that counts number of Hardware arc since the last HV_ON Command (32 bit)


    If (overcurrent latch is set) OR (undercurrent latch is not set)
       Then Hardware Arc
    Else if (ADC reading > overcurrent set point) OR (ADC reading < undercurrent set point)
       Then Software Glitch
    Else 
       The Pulse was good, record the pulse info into the A or B register


    Hardware Arcs can generate faults in the following ways.
    1) Too many consecutive arcs.
    2) Too many arcs per 300 pulses (per second 300 Hz)
    3) Too many arcs per 18000 pulses (per minute at 300 Hz)
    
  */
  
  if ((PIN_PULSE_OVER_CUR_LATCH == ILL_PULSE_OVER_CURRENT_FAULT) || (PIN_PULSE_MIN_CUR_LATCH == ILL_PULSE_MIN_CURRENT_FAULT)) {
    // The high voltage tank current was too high (indicating an arc) or was too low (indicating a short in the modulator)
    // Record and arc and throw away the data from the ADC
    arc_counter_persistent++;
    arc_counter_this_hv_on++;
    
    arc_counter_consecutive++;
    arc_counter_fast++;
    arc_counter_slow++;
    
  } else if ((pulse_magnetron_current_adc_reading > ADC_PULSE_CURRENT_OVER_GLITCH) || (pulse_magnetron_current_adc_reading < ADC_PULSE_CURRENT_UNDER_GLITCH)) {
    // The current readback was outside of the "GLITCH" limits.  Assume it was an error with the ADC and record an ADC error
    global_debug_counter.magnetron_current_adc_glitch++;
  } else if ((pulse_magnetron_voltage_adc_reading > ADC_PULSE_VOLTAGE_OVER_GLITCH) || (pulse_magnetron_voltage_adc_reading < ADC_PULSE_VOLTAGE_UNDER_GLITCH)) {
    global_debug_counter.magnetron_voltage_adc_glitch++;
  } else {
    // Hardware did not detect an arc.  Software appears to have read a valid pulse.  Update all data

    arc_counter_consecutive = 0;
    
    if (mode == PULSE_MODE_A) {
      StorePulseData(&ps_magnetron_mode_A);
    } else {
      StorePulseData(&ps_magnetron_mode_B);
    }
    
  }
  
  ResetPulseLatches();  // This must be called to clear the over/under current latches.
  
  // Decrement the arc_counter_fast if we need to
  pulse_counter_fast++;
  if (pulse_counter_fast > ARC_COUNTER_FAST_DECREMENT_INTERVAL) {
    pulse_counter_fast = 0;
    if (arc_counter_fast) {
      arc_counter_fast--;
    }
  }
  
  // decrement the arc_counter_slow if we need to
  pulse_counter_slow++;
  if (pulse_counter_slow > ARC_COUNTER_SLOW_DECREMENT_INTERVAL) {
    pulse_counter_slow = 0;
    if (arc_counter_slow) {
      arc_counter_slow--;
    }
  }
  

  // CHECK FOR PULSE FAULTS - Too many arcs, voltage/current out of range
  
  // Check for too many consecutive arcs
  if (arc_counter_consecutive > ARC_COUNTER_MAX_CONSECUTIVE) {
    RecordThisHighVoltageFault(FAULT_ARC_OVER_COUNT_CONSECUTIVE);
  }
  
  // Check for too many arcs in a short amount of time
  if (arc_counter_fast > ARC_COUNTER_FAST_MAX_ARCS) {
    RecordThisHighVoltageFault(FAULT_ARC_OVER_COUNT_FAST);
  }
  
  // Check for too many arc is a long amount of time
  if (arc_counter_slow > ARC_COUNTER_SLOW_MAX_ARCS) {
    RecordThisHighVoltageFault(FAULT_ARC_OVER_COUNT_SLOW);
  }
  

  // Check that the magnetron voltage ADC reading is not greater than X% of its program point (set in Config.h)
  // It must remain outside this range for at least PULSE_MAX_OUT_OF_RANGE_COUNT pulses before a fault is generated  
  if (CheckOverVoltFault(&ps_magnetron_mode_A)) {
    RecordThisHighVoltageFault(FAULT_PULSE_VOLTAGE_OOR_MODE_A);
  }
  
  // Check that the magnetron voltage ADC reading is not less than X% of its program point (set in Config.h)
  // It must remain outside this range for at least PULSE_MAX_OUT_OF_RANGE_COUNT pulses before a fault is generated  
  if (CheckUnderVoltFault(&ps_magnetron_mode_A)) {
    RecordThisHighVoltageFault(FAULT_PULSE_VOLTAGE_OOR_MODE_A);
  }

  // Check that the magnetron current ADC reading is not greater than X% of its program point (set in Config.h)
  // It must remain outside this range for at least PULSE_MAX_OUT_OF_RANGE_COUNT pulses before a fault is generated  
  if (CheckOverCurrentFault(&ps_magnetron_mode_A)) {
    RecordThisHighVoltageFault(FAULT_PULSE_CURRENT_OOR_MODE_A);
  }
  
  // Check that the magnetron current ADC reading is not less than X% of its program point (set in Config.h)
  // It must remain outside this range for at least PULSE_MAX_OUT_OF_RANGE_COUNT pulses before a fault is generated  
  if (CheckUnderCurrentFault(&ps_magnetron_mode_A)) {
    RecordThisHighVoltageFault(FAULT_PULSE_CURRENT_OOR_MODE_A);
  }


  // Check that the magnetron voltage ADC reading is not greater than X% of its program point (set in Config.h)
  // It must remain outside this range for at least PULSE_MAX_OUT_OF_RANGE_COUNT pulses before a fault is generated  
  if (CheckOverVoltFault(&ps_magnetron_mode_B)) {
    RecordThisHighVoltageFault(FAULT_PULSE_VOLTAGE_OOR_MODE_B);
  }
  
  // Check that the magnetron voltage ADC reading is not less than X% of its program point (set in Config.h)
  // It must remain outside this range for at least PULSE_MAX_OUT_OF_RANGE_COUNT pulses before a fault is generated  
  if (CheckUnderVoltFault(&ps_magnetron_mode_B)) {
    RecordThisHighVoltageFault(FAULT_PULSE_VOLTAGE_OOR_MODE_B);
  }
  
  // Check that the magnetron current ADC reading is not greater than X% of its program point (set in Config.h)
  // It must remain outside this range for at least PULSE_MAX_OUT_OF_RANGE_COUNT pulses before a fault is generated  
  if (CheckOverCurrentFault(&ps_magnetron_mode_B)) {
    RecordThisHighVoltageFault(FAULT_PULSE_CURRENT_OOR_MODE_B);
  }
  
  // Check that the magnetron current ADC reading is not less than X% of its program point (set in Config.h)
  // It must remain outside this range for at least PULSE_MAX_OUT_OF_RANGE_COUNT pulses before a fault is generated  
  if (CheckUnderCurrentFault(&ps_magnetron_mode_B)) {
    RecordThisHighVoltageFault(FAULT_PULSE_CURRENT_OOR_MODE_B);
  }
  

  // DPARKER NEED TO IMPLEMENT and TEST the PFN fault current input Logic
}

void StorePulseData(POWERSUPPLY* ptr) {
  
  ptr->i_adc_reading = RCFilter16Tau(ptr->i_adc_reading, pulse_magnetron_current_adc_reading);
  ptr->v_adc_reading = RCFilter16Tau(ptr->v_adc_reading, pulse_magnetron_voltage_adc_reading);
  
  // record min/max current
  if (pulse_magnetron_current_adc_reading > ptr->i_adc_max_reading) {
    ptr->i_adc_max_reading = pulse_magnetron_current_adc_reading;
  }
  if (pulse_magnetron_current_adc_reading < ptr->i_adc_min_reading) {
    ptr->i_adc_min_reading = pulse_magnetron_current_adc_reading;
  }

  // record min/max voltage
  if (pulse_magnetron_voltage_adc_reading > ptr->v_adc_max_reading) {
    ptr->v_adc_max_reading = pulse_magnetron_voltage_adc_reading;
  }
  if (pulse_magnetron_voltage_adc_reading < ptr->v_adc_min_reading) {
    ptr->v_adc_min_reading = pulse_magnetron_voltage_adc_reading;
  }

  average_energy_per_pulse_centi_joules = RCFilter16Tau(average_energy_per_pulse_centi_joules, CalculatePulseEnergy(pulse_magnetron_voltage_adc_reading, pulse_magnetron_current_adc_reading));

  average_energy_per_pulse_milli_joules_for_heater_foldback = RCFilter64Tau(average_energy_per_pulse_milli_joules_for_heater_foldback, CalculatePulseEnergyMilliJouleFromCurrent(pulse_magnetron_current_adc_reading));
}


unsigned int CalculatePulseEnergy(unsigned int voltage_adc, unsigned int current_adc) {
  unsigned long long temp64;

  /*
    The Pulse Energy is Calculated for Each Pulse
    The Pulse Energy is Filtered/Averaged with a 64 pulse/tau filter
    The Average Pulse Energy is then multiplied by the PRF to generate the power.
    The filament heater voltage is generated from the power.
    
    
    Step 1 - Calculate pulse power
    
    Current ADC Scalling - 0xFFFF     = 166.7 Amps 
    Voltage ADC Scalling - 0xFFFF     = -63.78 KV
    
    #define PFN_PULSE_WIDTH_NS               3500        
    #define PULSE_VADC_OUTPUT_AT_0xFFFF       63780
    #define PULSE_IADC_OUTPUT_AT_0xFFFF       166.7
    (see config.h for configuration)
    
    
    In Floating Point Math
    energy_per_pulse = v(magnetron) * I(magnetron) * T(PFN)
    energy_per_pulse = I(adc) * V(adc) * (PFN_PULSE_WIDTH_NS/1e9) * (PULSE_VDAC_OUTPUT_AT_0xFFFF/0xFFFF) * (PULSE_IADC_OUTPUT_AT_0xFFFF/0xFFFF)
                     = I(adc) * V(adc) * PFN_PULSE_WIDTH_NS * (PULSE_VDAC_OUTPUT_AT_0xFFFF) * (PULSE_IADC_OUTPUT_AT_0xFFFF) / (2^32 * 1e9)
		     = I(adc) * V(adc) * PFN_PULSE_WIDTH_NS * (PULSE_VDAC_OUTPUT_AT_0xFFFF) * (PULSE_IADC_OUTPUT_AT_0xFFFF) * (2^32/1e9)/ (2^64)

		     
		     
    energy_per_pulse_mj = 1000 * energy_per_pulse
    
    
    energy_per_pulse_mj = I(adc) * V(adc) * PFN_PULSE_WIDTH_NS * 1000 * SCALE / (2^64)
    where PULSE_ENERGY_SCALE = (PULSE_VADC_OUTPUT_AT_0xFFFF * PULSE_IADC_OUTPUT_AT_0xFFFF * 4.294967) 
    
    Because PULSE_ENERGY_SCALE is calculated by the C pre-processor it will be calcualted in floating point then rounded to the nearest integer
    
    
    temp64 = I(adc) * V(adc) * T(ns) // this number is up to 48 bits wide
    temp64 >>= 16                    // this number is up to 32 bits wide
    temp64 *= PULSE_ENERGY_SCALE     // this number is up to 64 bits wide
    temp64 >>= 16                    // this number is up to 48 bits wide
    temp64 *= 1000                   // this number is up to 64 bits wide
    temp64 >>= 32                    // this number is up to 32 bits wide but we will not have a number greater than 16 bits wide
    
    energy_per_pulse_mj = temp
    
  */
  temp64 = 0;
  temp64 = voltage_adc;
  temp64 *= current_adc;
  temp64 *= PFN_PULSE_WIDTH_NS;
  temp64 >>= 16;
  temp64 *= PULSE_ENERGY_SCALE;
  temp64 >>= 16;
  temp64 *= 100;
  temp64 >>= 32;  // this is centi-J per pulse 
  
  // DPARKER consider error checking for overflow???

  return (temp64 & 0xFFFF);

}


unsigned int CalculatePulseEnergyMilliJouleFromCurrent(unsigned int current_adc) {
  // DPARKER WRITE THIS
  return 0;
}


void LoadFaultMaskRegisters(void) {
  
  switch(control_state) {
    
  case STATE_START_UP:
    magnetron_fault_mask = FAULT_MASK_MAGNETRON_STATE_START_UP;
    thyratron_fault_mask = FAULT_MASK_THYRATRON_STATE_START_UP;
    high_voltage_fault_mask = FAULT_MASK_HIGH_VOLTAGE_STATE_START_UP;
    control_board_fault_mask = FAULT_MASK_CONTROL_BOARD_STATE_START_UP;
 
    magnetron_warning_mask = WARNING_MASK_MAGNETRON_STATE_START_UP;
    thyratron_warning_mask = WARNING_MASK_THYRATRON_STATE_START_UP;
    high_voltage_warning_mask = WARNING_MASK_HIGH_VOLTAGE_STATE_START_UP;
    control_board_warning_mask = WARNING_MASK_CONTROL_BOARD_STATE_START_UP;
    break;  

  case STATE_SYSTEM_COLD_READY:
    magnetron_fault_mask = FAULT_MASK_MAGNETRON_STATE_COLD_READY;
    thyratron_fault_mask = FAULT_MASK_THYRATRON_STATE_COLD_READY;
    high_voltage_fault_mask = FAULT_MASK_HIGH_VOLTAGE_STATE_COLD_READY;
    control_board_fault_mask = FAULT_MASK_CONTROL_BOARD_STATE_COLD_READY;
 
    magnetron_warning_mask = WARNING_MASK_MAGNETRON_STATE_COLD_READY;
    thyratron_warning_mask = WARNING_MASK_THYRATRON_STATE_COLD_READY;
    high_voltage_warning_mask = WARNING_MASK_HIGH_VOLTAGE_STATE_COLD_READY;
    control_board_warning_mask = WARNING_MASK_CONTROL_BOARD_STATE_COLD_READY;
    break;
    
  case STATE_WARM_UP:
    magnetron_fault_mask = FAULT_MASK_MAGNETRON_STATE_WARM_UP;
    thyratron_fault_mask = FAULT_MASK_THYRATRON_STATE_WARM_UP;
    high_voltage_fault_mask = FAULT_MASK_HIGH_VOLTAGE_STATE_WARM_UP;
    control_board_fault_mask = FAULT_MASK_CONTROL_BOARD_STATE_WARM_UP;
 
    magnetron_warning_mask = WARNING_MASK_MAGNETRON_STATE_WARM_UP;
    thyratron_warning_mask = WARNING_MASK_THYRATRON_STATE_WARM_UP;
    high_voltage_warning_mask = WARNING_MASK_HIGH_VOLTAGE_STATE_WARM_UP;
    control_board_warning_mask = WARNING_MASK_CONTROL_BOARD_STATE_WARM_UP;
    break;

  case STATE_SYSTEM_WARM_READY:
    magnetron_fault_mask = FAULT_MASK_MAGNETRON_STATE_WARM_READY;
    thyratron_fault_mask = FAULT_MASK_THYRATRON_STATE_WARM_READY;
    high_voltage_fault_mask = FAULT_MASK_HIGH_VOLTAGE_STATE_WARM_READY;
    control_board_fault_mask = FAULT_MASK_CONTROL_BOARD_STATE_WARM_READY;
 
    magnetron_warning_mask = WARNING_MASK_MAGNETRON_STATE_WARM_READY;
    thyratron_warning_mask = WARNING_MASK_THYRATRON_STATE_WARM_READY;
    high_voltage_warning_mask = WARNING_MASK_HIGH_VOLTAGE_STATE_WARM_READY;
    control_board_warning_mask = WARNING_MASK_CONTROL_BOARD_STATE_WARM_READY;
    break;


  case STATE_HV_STARTUP:
    magnetron_fault_mask = FAULT_MASK_MAGNETRON_STATE_WARM_READY;
    thyratron_fault_mask = FAULT_MASK_THYRATRON_STATE_WARM_READY;
    high_voltage_fault_mask = FAULT_MASK_HIGH_VOLTAGE_STATE_WARM_READY;
    control_board_fault_mask = FAULT_MASK_CONTROL_BOARD_STATE_WARM_READY;
 
    magnetron_warning_mask = WARNING_MASK_MAGNETRON_STATE_WARM_READY;
    thyratron_warning_mask = WARNING_MASK_THYRATRON_STATE_WARM_READY;
    high_voltage_warning_mask = WARNING_MASK_HIGH_VOLTAGE_STATE_WARM_READY;
    control_board_warning_mask = WARNING_MASK_CONTROL_BOARD_STATE_WARM_READY;
    break;


  case STATE_HV_ON:
    magnetron_fault_mask = FAULT_MASK_MAGNETRON_STATE_HV_ON;
    thyratron_fault_mask = FAULT_MASK_THYRATRON_STATE_HV_ON;
    high_voltage_fault_mask = FAULT_MASK_HIGH_VOLTAGE_STATE_HV_ON;
    control_board_fault_mask = FAULT_MASK_CONTROL_BOARD_STATE_HV_ON;
 
    magnetron_warning_mask = WARNING_MASK_MAGNETRON_STATE_HV_ON;
    thyratron_warning_mask = WARNING_MASK_THYRATRON_STATE_HV_ON;
    high_voltage_warning_mask = WARNING_MASK_HIGH_VOLTAGE_STATE_HV_ON;
    control_board_warning_mask = WARNING_MASK_CONTROL_BOARD_STATE_HV_ON;
    break;

  case STATE_FAULT_WARM_FAULT:
    magnetron_fault_mask = FAULT_MASK_MAGNETRON_STATE_WARM_FAULT;
    thyratron_fault_mask = FAULT_MASK_THYRATRON_STATE_WARM_FAULT;
    high_voltage_fault_mask = FAULT_MASK_HIGH_VOLTAGE_STATE_WARM_FAULT;
    control_board_fault_mask = FAULT_MASK_CONTROL_BOARD_STATE_WARM_FAULT;
 
    magnetron_warning_mask = WARNING_MASK_MAGNETRON_STATE_WARM_FAULT;
    thyratron_warning_mask = WARNING_MASK_THYRATRON_STATE_WARM_FAULT;
    high_voltage_warning_mask = WARNING_MASK_HIGH_VOLTAGE_STATE_WARM_FAULT;
    control_board_warning_mask = WARNING_MASK_CONTROL_BOARD_STATE_WARM_FAULT;
    break;
    
  case STATE_FAULT_COLD_FAULT:
    magnetron_fault_mask = FAULT_MASK_MAGNETRON_STATE_COLD_FAULT;
    thyratron_fault_mask = FAULT_MASK_THYRATRON_STATE_COLD_FAULT;
    high_voltage_fault_mask = FAULT_MASK_HIGH_VOLTAGE_STATE_COLD_FAULT;
    control_board_fault_mask = FAULT_MASK_CONTROL_BOARD_STATE_COLD_FAULT;
 
    magnetron_warning_mask = WARNING_MASK_MAGNETRON_STATE_COLD_FAULT;
    thyratron_warning_mask = WARNING_MASK_THYRATRON_STATE_COLD_FAULT;
    high_voltage_warning_mask = WARNING_MASK_HIGH_VOLTAGE_STATE_COLD_FAULT;
    control_board_warning_mask = WARNING_MASK_CONTROL_BOARD_STATE_COLD_FAULT;
    break;
  }
}



void RecordThisMagnetronFault(unsigned int fault_bit) {  

  faults_magnetron_status_reg |= fault_bit;

  if (fault_bit & magnetron_warning_mask) {
    faults_magnetron_warning_reg |= fault_bit;
  }

  if (fault_bit & magnetron_fault_mask) {
    if (!(faults_magnetron_fault_reg & fault_bit)) {
      // This fault has NOT YET BEEN SET in the fault register so it is a NEW Fault
      // Set the coresponding bit and save the fault
      faults_magnetron_fault_reg |= fault_bit;
      WriteToEventLog(MAGNETRON_FAULT,fault_bit);
    }
  }
}



void RecordThisHighVoltageFault(unsigned int fault_bit) {

  faults_high_voltage_status_reg |= fault_bit;

  if (fault_bit & high_voltage_warning_mask) {
    faults_high_voltage_warning_reg |= fault_bit;
  }

  if (fault_bit & high_voltage_fault_mask) {
    if (!(faults_high_voltage_fault_reg & fault_bit)) {
      // This fault has NOT YET BEEN SET in the fault register so it is a NEW Fault
      // Set the coresponding bit and save the fault
      faults_high_voltage_fault_reg |= fault_bit;
      WriteToEventLog(HIGH_VOLTAGE_FAULT,fault_bit);
    }
  }
}

void RecordThisThyratronFault(unsigned int fault_bit) {
  
  faults_thyratron_status_reg |= fault_bit;
  
  if (fault_bit & thyratron_warning_mask) {
    faults_thyratron_warning_reg |= fault_bit;
  }

  if (fault_bit & thyratron_fault_mask) {
    if (!(faults_thyratron_fault_reg & fault_bit)) {
      // This fault has NOT YET BEEN SET in the fault register so it is a NEW Fault
      // Set the coresponding bit and save the fault
      faults_thyratron_fault_reg |= fault_bit;
      WriteToEventLog(THYRATRON_FAULT,fault_bit);
    }
  }
}


void RecordThisControlBoardFault(unsigned int fault_bit) { 

  faults_control_board_status_reg |= fault_bit;

  if (fault_bit & control_board_warning_mask) {
    faults_control_board_warning_reg |= fault_bit;
  }

  if (fault_bit & control_board_fault_mask) {
    if (!(faults_control_board_fault_reg & fault_bit)) {
      // This fault has NOT YET BEEN SET in the fault register so it is a NEW Fault
      // Set the coresponding bit and save the fault
      faults_control_board_fault_reg |= fault_bit;
      WriteToEventLog(CONTROL_BOARD_FAULT,fault_bit);
    }
  }
}


void WriteToEventLog(unsigned char fault_register, unsigned int fault_bit) {
  // DPARKER this function should write to the event log in ram and update the fault counter in RAM
  // These values are moved to EEPROM later on  . . . like programmed later on
}


unsigned int CheckStartupFailed(void) {
  return 0;
}

unsigned int CheckFaultActive(void) {
  return (faults_control_board_fault_reg | faults_thyratron_fault_reg | faults_magnetron_fault_reg | faults_high_voltage_fault_reg);
}


unsigned int CheckColdFaultActive(void) {
  unsigned int temp;
  temp = (faults_control_board_fault_reg & MASK_CONTROL_BOARD_COLD_FAULTS);
  temp |= (faults_thyratron_fault_reg & MASK_THYRATRON_COLD_FAULTS);
  temp |= (faults_magnetron_fault_reg & MASK_MAGNETRON_COLD_FAULTS);
  temp |= (faults_high_voltage_fault_reg & MASK_HIGH_VOLTAGE_COLD_FAULTS);
  return temp;
}



unsigned char CheckOverVoltFault(POWERSUPPLY* ptr) {
  if (ptr->v_adc_reading > ptr->v_adc_over_rel) {
    ptr->v_adc_over_count++;
    if (ptr->v_adc_over_count > ptr->v_adc_max_oor) {
      ptr->v_adc_over_count = ptr->v_adc_max_oor + 10;
      return 1;
    }
  } else if (ptr->v_adc_over_count >= 1) {
    ptr->v_adc_over_count--;
  }
  return 0;
}

unsigned char CheckUnderVoltFault(POWERSUPPLY* ptr) {
  unsigned int mod_v_adc_reading;

  mod_v_adc_reading = ptr->v_adc_reading;
  if (mod_v_adc_reading < 0x0100) {
    mod_v_adc_reading = 0x0100; // DPARKER add parameter
  }
  if (mod_v_adc_reading < ptr->v_adc_under_rel) {
    ptr->v_adc_under_count++;
    if (ptr->v_adc_under_count > ptr->v_adc_max_oor) {
      ptr->v_adc_under_count = ptr->v_adc_max_oor + 10;
      return 1;
    }
  } else if (ptr->v_adc_under_count >= 1) {
    ptr->v_adc_under_count--;
  }
  return 0;
}


unsigned char CheckOverCurrentFault(POWERSUPPLY* ptr) {
  if (ptr->i_adc_reading > ptr->i_adc_over_rel) {
    ptr->i_adc_over_count++;
    if (ptr->i_adc_over_count > ptr->i_adc_max_oor) {
      ptr->i_adc_over_count = ptr->i_adc_max_oor + 10;
      return 1; 
    }
  } else if (ptr->i_adc_over_count >= 1) {
    ptr->i_adc_over_count--;
  }
  return 0;
}

unsigned char CheckUnderCurrentFault(POWERSUPPLY* ptr) {
  unsigned int mod_i_adc_reading;

  mod_i_adc_reading = ptr->i_adc_reading;
  if (mod_i_adc_reading < 0x0100) {
    mod_i_adc_reading = 0x0100;  // DPARKER add parameter
  }
  if (mod_i_adc_reading < ptr->i_adc_under_rel) {
    ptr->i_adc_under_count++;
    if (ptr->i_adc_under_count > ptr->i_adc_max_oor) {
      ptr->i_adc_under_count = ptr->i_adc_max_oor + 10;
      return 1;
    }
  } else if (ptr->i_adc_under_count >= 1) {
    ptr->i_adc_under_count--;
  }
  return 0;
}



