#include "A34760.h"
#include "faults.h"
#include "Serial.h"
#include "A34760_PINS.h"
#include "Buffer64.h"
#include "LTC2656.h"
#include "IOPorts.h"
#include "MCP23017.h"
#include "faults.h"
#include <libpic30.h>
#include "ETMdsp.h"
#include "Config.h"


#define FILAMENT_LOOK_UP_TABLE_VALUES_FOR_MG7095 100,99,99,98,98,97,96,95,94,94,93,92,91,90,89,88,87,86,84,83,82,81,79,78,77,75,74,72,71,69,67,66,64,62,61,59,57,55,53,51,49,47,45,43,41,39,37,35,32,30,28,25,23,21,18,16,13,10,8,5,3,0,0,0

#define FILAMENT_LOOK_UP_TABLE_VALUES_FOR_MG5193 100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,97,94,91,88,84,81,78,75,72,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

#ifdef __MG7095
const unsigned int FilamentLookUpTable[64] = {FILAMENT_LOOK_UP_TABLE_VALUES_FOR_MG7095};
#else
const unsigned int FilamentLookUpTable[64] = {FILAMENT_LOOK_UP_TABLE_VALUES_FOR_MG5193};
#endif

unsigned int arc_detected;

unsigned long low_energy_target_current_set_point_derived; 

signed int look_up_offset;

unsigned int default_pac_2_adc_reading;

unsigned int max_low_energy_target_current_startup_adjust_initital_value = 300;
unsigned int low_energy_target_current_startup_max_cooldown = 12000;
unsigned int low_energy_target_current_startup_adjust_initital_value = 0;
unsigned int low_energy_target_current_startup_adjust_decay_time_pulses = 1200;
unsigned int low_energy_target_current_startup_adjust_direction_positive = 0;
unsigned int low_energy_target_current_startup_adjust;

unsigned int pulse_off_time_10_ms_units;

unsigned int linac_high_energy_target_current_adc_reading;
unsigned int linac_high_energy_target_current_set_point;

unsigned int linac_low_energy_target_current_adc_reading;
unsigned int linac_low_energy_target_current_set_point;
unsigned int linac_low_energy_target_current_set_point_portal_mode;
unsigned int linac_low_energy_target_current_set_point_gantry_mode;



signed int linac_high_energy_program_offset;
signed int linac_low_energy_program_offset;
unsigned char fast_ratio_mode;

unsigned int pulse_counter_this_run;   // This counts the number of pulses in the current "run".  This will be reset to 0 if there are no triggers for 100mS or more.

  

unsigned int false_trigger;

unsigned int last_period = 62501;
unsigned int prf_deciherz = 0;

void ReadADCtoPACArray(void);

volatile unsigned int timing_error_int1_count = 0;

unsigned int software_skip_warmup = 0;

unsigned char ram_config_set_magnetron_magnet_current_from_GUI;

volatile unsigned int _PERSISTENT last_known_action;
volatile unsigned int _PERSISTENT last_osccon;

unsigned int _PERSISTENT processor_crash_count;

unsigned int previous_last_action;

volatile unsigned int lvdinterrupt_counter = 0;

unsigned int MakeScale(unsigned int num, unsigned int den);

unsigned int start_reset_process;

void SavePulseCountersToEEPROM(void);

void UpdateIOExpanderOutputs(void);

unsigned char slow_down_thyratron_pid_counter;


unsigned int average_energy_per_pulse_milli_joules;
unsigned int average_output_power_watts;
unsigned int average_pulse_repetition_frequency_deci_herz;
unsigned int prf_pulse_counter;


unsigned char control_state;

unsigned int pac_1_adc_reading;
unsigned int pac_2_adc_reading;


unsigned int fast_reset_counter_persistent;
unsigned long _PERSISTENT arc_counter_persistent;
unsigned int arc_counter_consecutive;
unsigned int _PERSISTENT arc_counter_this_hv_on;
unsigned long _PERSISTENT pulse_counter_this_hv_on;
unsigned long long _PERSISTENT pulse_counter_persistent;
unsigned int arc_counter_fast;
unsigned int arc_counter_slow;
unsigned int pulse_counter_fast;
unsigned int pulse_counter_slow;

unsigned int led_pulse_count;



unsigned int pulse_magnetron_current_adc_reading;
unsigned int pulse_magnetron_voltage_adc_reading;


// Control structers for the thyratron heater PID loops
tPID thyratron_reservoir_heater_PID;
fractional pid_thyratron_reservoir_heater_controlHistory[3] __attribute__ ((section (".ybss, bss, ymemory")));
fractional pid_thyratron_reservoir_heater_abcCoefficient[3] __attribute__ ((section (".xbss, bss, xmemory")));
fractional pid_thyratron_reservoir_heater_kCoeffs[] = {0,0,0};

tPID thyratron_cathode_heater_PID;
fractional pid_thyratron_cathode_heater_controlHistory[3] __attribute__ ((section (".ybss, bss, ymemory")));
fractional pid_thyratron_cathode_heater_abcCoefficient[3] __attribute__ ((section (".xbss, bss, xmemory")));
fractional pid_thyratron_cathode_heater_kCoeffs[] = {0,0,0};




BUFFER64BYTE uart1_input_buffer;
BUFFER64BYTE uart1_output_buffer;

LTC2656 U44_LTC2656;
MCP23017 U64_MCP23017;

POWERSUPPLY ps_hv_lambda_mode_A;
POWERSUPPLY ps_magnetron_mode_A;
POWERSUPPLY ps_hv_lambda_mode_B;
POWERSUPPLY ps_magnetron_mode_B;
POWERSUPPLY ps_magnet;
POWERSUPPLY ps_filament;
POWERSUPPLY ps_thyr_cathode_htr;
POWERSUPPLY ps_thyr_reservoir_htr;


volatile unsigned char adc_result_index;

unsigned int pfn_rev_current_array[128];

unsigned int pac_1_array[128];
unsigned int pac_2_array[128];

unsigned int thyratron_cathode_heater_voltage_array[128];
unsigned int thyratron_reservoir_heater_voltage_array[128];

unsigned int magnetron_magnet_current_array[128];
unsigned int magnetron_magnet_voltage_array[128];

unsigned int magnetron_filament_current_array[128];
unsigned int magnetron_filament_voltage_array[128];

unsigned int lambda_vpeak_array[128];
unsigned int lambda_vmon_array[128];


volatile unsigned char global_run_post_pulse_process;
volatile unsigned char global_adc_ignore_this_sample;



unsigned char a_b_selected_mode;
volatile unsigned char next_pulse_a_b_selected_mode;




void DoA34760StartUpCommon(void);
void DoA34760StartUpNormalProcess(void);
void DoA34760StartUpFastProcess(void);
void DoA34760StartUpCommonPostProcess(void);



void CalcPowerSupplySettings(POWERSUPPLY* ptr_ps);
void UpdateDacAll(void);
void Do10msTicToc(void);
void DoThyratronPIDs(void);
void DoMagnetronFilamentAdjust(void);
void ReadIsolatedAdcToRam(void);
void FilterADCs(void);
void FastReadAndFilterFeedbacks(void);
void FastReadAndFilterPACInputs(void);
void ExitHvOnState(void);
void DoColdShutDown(void);
void DoWarmShutDown(void);
void StartWarmUp(void);

void ScalePowerSupply(POWERSUPPLY* ptr_ps, unsigned int num, unsigned int den);

void EnableMagnetronFilamentSupply(void);
void DisableMagnetronFilamentSupply(void);
void EnableMagnetronMagnetSupply(void);
void DisableMagnetronMagnetSupply(void);
void DisableHVLambdaSupply(void);
void HVLambdaStartCharging(void);

unsigned int CalculatePoly(unsigned int set_point);

//unsigned int RCFilter256Tau(unsigned int previous_value, unsigned int current_reading); moved to .h
//unsigned int RCFilter64Tau(unsigned int previous_value, unsigned int current_reading); moved to .h
//unsigned int RCFilter16Tau(unsigned int previous_value, unsigned int current_reading); moved to .h


unsigned int mode_A_pulse_magnetron_current_adc_reading_filtered;
unsigned int mode_B_pulse_magnetron_current_adc_reading_filtered;

unsigned int mode_A_pulse_magnetron_voltage_adc_reading_filtered;
unsigned int mode_B_pulse_magnetron_voltage_adc_reading_filtered;

unsigned int mode_A_pulse_magnetron_current_adc_reading_max;
unsigned int mode_A_pulse_magnetron_current_adc_reading_min;

unsigned int mode_B_pulse_magnetron_current_adc_reading_max;
unsigned int mode_B_pulse_magnetron_current_adc_reading_min;

unsigned int mode_A_pulse_magnetron_voltage_adc_reading_filtered;
unsigned int mode_B_pulse_magnetron_voltage_adc_reading_filtered;





void DoStateMachine(void) {
  unsigned int warmup_counter;
  unsigned int lambda_supply_startup_counter;
  unsigned int vtemp;
  
  unsigned long temp_long;
	  


  switch(control_state) {
    
  case STATE_START_UP:
    DoA34760StartUpCommon();
    DoA34760StartUpNormalProcess();
    DoA34760StartUpCommonPostProcess();
    
    if (CheckStartupFailed()) {
      control_state = STATE_FAULT_MCU_CORE_FAULT;
    } else {
      control_state = STATE_SYSTEM_COLD_READY;
    }
    break;
    
  case STATE_FAST_RECOVERY_START_UP:

    // It takes a 360uS to get to here (really just to read flash)

    PIN_UART2_TX = !PIN_UART2_TX;
    DoA34760StartUpCommon();  // This Takes 4.6ms (4.55 ms of this is loading and intialization of Power Supply Structures)
    PIN_UART2_TX = !PIN_UART2_TX;
    DoA34760StartUpFastProcess(); // This takes 4.3mS 
    PIN_UART2_TX = !PIN_UART2_TX;
    DoA34760StartUpCommonPostProcess(); // This takes 60uS
    PIN_UART2_TX = !PIN_UART2_TX;
    
    if (CheckStartupFailed()) {
      control_state = STATE_FAULT_MCU_CORE_FAULT;
    } else {
      control_state = STATE_HV_ON;
    }

    break;

    

  case STATE_SYSTEM_COLD_READY:
    DoColdShutDown();
    while (control_state == STATE_SYSTEM_COLD_READY) {
      Do10msTicToc();  // Execute 10mS timed functions if the 10ms Timer has rolled
      DoSerialCommand();
      if (CheckFaultActive()) {
	control_state = STATE_FAULT_COLD_FAULT;
      } else if (PIN_FP_MODULATOR_ENABLE_INPUT == ILL_MODULATOR_ENABLED) {
	control_state = STATE_WARM_UP;
      }
    }
    break;
    
  case STATE_WARM_UP:
    StartWarmUp();
    warmup_counter = 0;
    software_skip_warmup = 0;
    while (control_state == STATE_WARM_UP) {
      Do10msTicToc();
      DoSerialCommand();
      Nop();
      Nop();
      Nop();
      if (_T2IF) {
	warmup_counter++;
	_T2IF = 0;
	if ((PIN_FP_FAST_RESTART == ILL_FAST_RESTART) || (software_skip_warmup == 1)) {
	  ScalePowerSupply(&ps_filament, warmup_counter, 10);
	  ScalePowerSupply(&ps_magnet, warmup_counter, 10);
	  ScalePowerSupply(&ps_thyr_cathode_htr, warmup_counter, 10);
	  ScalePowerSupply(&ps_thyr_reservoir_htr, warmup_counter, 10);
	} else {
	  ScalePowerSupply(&ps_filament, warmup_counter, ps_filament.warmup_ramp_time);
	  ScalePowerSupply(&ps_magnet, warmup_counter, ps_magnet.warmup_ramp_time);
	  ScalePowerSupply(&ps_thyr_cathode_htr, warmup_counter, ps_thyr_cathode_htr.warmup_ramp_time);
	  ScalePowerSupply(&ps_thyr_reservoir_htr, warmup_counter, ps_thyr_reservoir_htr.warmup_ramp_time);
	}
      }
      if (CheckFaultActive()) {
	control_state = STATE_FAULT_COLD_FAULT;
      } else if (PIN_FP_MODULATOR_ENABLE_INPUT == !ILL_MODULATOR_ENABLED) {
	control_state = STATE_SYSTEM_COLD_READY;
      } else if (warmup_counter > SYSTEM_WARM_UP_TIME) {
	ResetHWLatches();
	control_state = STATE_SYSTEM_WARM_READY;
	software_skip_warmup = 0;
      } else if ((PIN_FP_FAST_RESTART == ILL_FAST_RESTART) && (warmup_counter > 20)) {
	ResetHWLatches();
	control_state = STATE_SYSTEM_WARM_READY;
	software_skip_warmup = 0;
      } else if (software_skip_warmup == 1) {
	ResetHWLatches();
	control_state = STATE_SYSTEM_WARM_READY;
	software_skip_warmup = 0;
      }
    }
    break;
    
  case STATE_SYSTEM_WARM_READY:
    // DPARKER SaveDataToEEPROM(); -- New Commands to deal with EEPROM access and storage
    DoWarmShutDown();
    ScalePowerSupply(&ps_hv_lambda_mode_A,100,100);
    ScalePowerSupply(&ps_hv_lambda_mode_B,100,100);
    ScalePowerSupply(&ps_filament,100,100);
    ScalePowerSupply(&ps_magnet,100,100);
    ScalePowerSupply(&ps_thyr_reservoir_htr,100,100);
    ScalePowerSupply(&ps_thyr_cathode_htr,100,100);
    while (control_state == STATE_SYSTEM_WARM_READY) {
      Do10msTicToc();
      DoSerialCommand();
      if (CheckFaultActive()) {
	control_state = STATE_FAULT_WARM_FAULT;
      } else if (PIN_FP_MODULATOR_ENABLE_INPUT == !ILL_MODULATOR_ENABLED) {
	control_state = STATE_SYSTEM_COLD_READY;
      } else if (PIN_FP_MODULATOR_HV_ON_INPUT == ILL_MODULATOR_HV_ON) {
	control_state = STATE_HV_STARTUP;
      }
    }
    break;
    
  case STATE_HV_STARTUP:
    // THIS STATE uses the same faults as STATE_SYSTEM_WARM_READY

#if !defined(__SET_MAGNETRON_OVER_SERIAL_INTERFACE)
    
    vtemp = Scale16Bit(pac_1_adc_reading, DIRECT_LAMBDA_INPUT_SCALE);
    SetPowerSupplyTarget(&ps_hv_lambda_mode_A, vtemp , 0);
    
    vtemp = Scale16Bit(pac_2_adc_reading, DIRECT_LAMBDA_INPUT_SCALE);
    SetPowerSupplyTarget(&ps_hv_lambda_mode_B, vtemp, 0);
#endif


    lambda_supply_startup_counter = 0;
    PIN_FAST_RESTART_STORAGE_CAP_OUTPUT = OLL_DO_FAST_RESTART;
    while (control_state == STATE_HV_STARTUP) {
      Do10msTicToc();
      DoSerialCommand();
      if (_T2IF) {
	// 100ms Timer over flow 
	_T2IF = 0;
	lambda_supply_startup_counter++;
      }
      if (CheckFaultActive()) {
	control_state = STATE_FAULT_WARM_FAULT;
      } else if (PIN_FP_MODULATOR_ENABLE_INPUT == !ILL_MODULATOR_ENABLED) {
	control_state = STATE_SYSTEM_COLD_READY;
      } else if (PIN_FP_MODULATOR_HV_ON_INPUT == !ILL_MODULATOR_HV_ON) {
	control_state = STATE_SYSTEM_WARM_READY;
      } else if (lambda_supply_startup_counter >= LAMBDA_SUPPLY_STARTUP_DELAY) {
	control_state = STATE_HV_ON;
	HVLambdaStartCharging();
      }
    }
    break;

    
  case STATE_HV_ON:
    arc_counter_consecutive = 0;
    arc_counter_slow = 0;
    arc_counter_fast = 0;
    arc_counter_this_hv_on = 0;
    pulse_counter_this_hv_on = 0;
    global_run_post_pulse_process = 0;
    // PIN_FAST_RESTART_STORAGE_CAP_OUTPUT = OLL_DO_FAST_RESTART; // THIS is a redundent command and should be removed 

    _T1IE = 1; // This is added for the fast restart process.  Normally _T1IE is set in HVLambdaStartCharging(), but the fast restart clears this bit temporarily
        
    while (control_state == STATE_HV_ON) {
      last_known_action = LAST_ACTION_HV_ON_LOOP;
      Do10msTicToc();
      DoSerialCommand();

      if (PIN_GANTRY_PORTAL_SELECT == ILL_GANTRY_MODE) {
	linac_low_energy_target_current_set_point = linac_low_energy_target_current_set_point_gantry_mode;
      } else {
	linac_low_energy_target_current_set_point = linac_low_energy_target_current_set_point_portal_mode;
      }


      if (global_run_post_pulse_process) {
	if (false_trigger) {
	  RecordThisThyratronFault(FAULT_THYR_FALSE_TRIGGER);
	}
	last_known_action = LAST_ACTION_POST_PULSE_PROC;
	// The Pulse Interrupt sets this Flag - And this sequence runs only once
	// Update all the pulse data
	if (pulse_counter_this_run <= 0xFF00) {
	  pulse_counter_this_run++;
	}
	pulse_counter_this_hv_on++;
	pulse_counter_persistent++;
	prf_pulse_counter++;

	ReadIsolatedAdcToRam(); // Durring the pulse interrupt, the magnetron voltage and current was sampled.  Read back that data here
	UpdatePulseData(a_b_selected_mode);      // Run filtering/error detection on pulse data
	a_b_selected_mode = next_pulse_a_b_selected_mode;
	
	// DPARKER impliment and test a current control PID LOOP
	linac_low_energy_target_current_adc_reading = AverageADC128(thyratron_cathode_heater_voltage_array);
	linac_high_energy_target_current_adc_reading = AverageADC128(thyratron_reservoir_heater_voltage_array);
	//linac_low_energy_target_current_adc_reading = RCFilter64Tau(linac_low_energy_target_current_adc_reading, AverageADC128(thyratron_cathode_heater_voltage_array));
	//linac_high_energy_target_current_adc_reading = RCFilter64Tau(linac_high_energy_target_current_adc_reading, AverageADC128(thyratron_reservoir_heater_voltage_array));

	if (pulse_counter_this_run < 30) {
	  linac_low_energy_program_offset = 0;
	  linac_high_energy_program_offset = 0;
	  fast_ratio_mode = 1;
	} else {
	  // There have been enough pulses for the sample and hold to return valid readins.  Start to close the loop around the measured target current
	  // DPARKER - write the algorythim to take linac_target_current_high_energy_mode and linac_high_energy_target_current_set_point
	

	  low_energy_target_current_startup_adjust = 0;
#ifdef __STARTUP_TARGET_CURRENT_ADJUST
	  if (pulse_counter_this_run < low_energy_target_current_startup_adjust_decay_time_pulses) {
	    temp_long = low_energy_target_current_startup_adjust_initital_value;
	    temp_long *= (low_energy_target_current_startup_adjust_decay_time_pulses - pulse_counter_this_run);
	    temp_long /= low_energy_target_current_startup_adjust_decay_time_pulses;
	    low_energy_target_current_startup_adjust = temp_long;
	  }
#endif
	  

  
#ifdef __RATIO_CONTROL_MODE    
	  
	  low_energy_target_current_set_point_derived = linac_low_energy_target_current_set_point;
	  low_energy_target_current_set_point_derived *= linac_high_energy_target_current_adc_reading;
	  low_energy_target_current_set_point_derived /= linac_high_energy_target_current_set_point;
	  

	  if (fast_ratio_mode) {
	    if (linac_low_energy_target_current_adc_reading >= (low_energy_target_current_set_point_derived + LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR)) {
	      linac_low_energy_program_offset -= 9*LINAC_TARGET_CURRENT_LOW_ENERGY_STEP_SIZE;
	    } else if (linac_low_energy_target_current_adc_reading <= (low_energy_target_current_set_point_derived - LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR)) {
	      linac_low_energy_program_offset += 9*LINAC_TARGET_CURRENT_LOW_ENERGY_STEP_SIZE;
	    } else {
	      fast_ratio_mode = 0;
	    }
	  } else {
	    if (linac_low_energy_target_current_adc_reading >= (low_energy_target_current_set_point_derived + LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR)) {
	      linac_low_energy_program_offset -= LINAC_TARGET_CURRENT_LOW_ENERGY_STEP_SIZE;
	    } else if (linac_low_energy_target_current_adc_reading <= (low_energy_target_current_set_point_derived - LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR)) {
	      linac_low_energy_program_offset += LINAC_TARGET_CURRENT_LOW_ENERGY_STEP_SIZE;
	    }
	  }
#else
		  
	  if (PIN_FP_SPARE_2_SAMPLE_VPROG_INPUT == ILL_SAMPLE_VPROG_INPUT) {
	    if (low_energy_target_current_startup_adjust_direction_positive) {
	      low_energy_target_current_set_point_derived = linac_low_energy_target_current_set_point + low_energy_target_current_startup_adjust;
	    } else {
	      low_energy_target_current_set_point_derived = linac_low_energy_target_current_set_point - low_energy_target_current_startup_adjust;
	    }
	  } else {
	    if (low_energy_target_current_startup_adjust_direction_positive) {
	      low_energy_target_current_set_point_derived = linac_low_energy_target_current_set_point + low_energy_target_current_startup_adjust;
	    } else {
	      low_energy_target_current_set_point_derived = linac_low_energy_target_current_set_point - low_energy_target_current_startup_adjust;
	    }	    
	    low_energy_target_current_set_point_derived *= pac_2_adc_reading;
	    low_energy_target_current_set_point_derived /= default_pac_2_adc_reading;
	  }
	  
	  if (linac_low_energy_target_current_adc_reading >= (low_energy_target_current_set_point_derived + LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR)) {
	    linac_low_energy_program_offset -= LINAC_TARGET_CURRENT_LOW_ENERGY_STEP_SIZE;
	  } else if (linac_low_energy_target_current_adc_reading <= (low_energy_target_current_set_point_derived - LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR)) {
	    linac_low_energy_program_offset += LINAC_TARGET_CURRENT_LOW_ENERGY_STEP_SIZE;
	  }

	  if (linac_high_energy_target_current_adc_reading > (linac_high_energy_target_current_set_point + LINAC_TARGET_CURRENT_HIGH_ENERGY_MINIMUM_ERROR)) {
	    linac_high_energy_program_offset -= LINAC_TARGET_CURRENT_HIGH_ENERGY_STEP_SIZE;
	  } else if (linac_high_energy_target_current_adc_reading < (linac_high_energy_target_current_set_point - LINAC_TARGET_CURRENT_HIGH_ENERGY_MINIMUM_ERROR)) {
	    linac_high_energy_program_offset += LINAC_TARGET_CURRENT_HIGH_ENERGY_STEP_SIZE;
	  }
	  
#endif
	  
	  if (linac_low_energy_program_offset > LINAC_TARGET_CURRENT_LOW_ENERGY_PROGRAM_MAX_OFFSET) {
	    linac_low_energy_program_offset = LINAC_TARGET_CURRENT_LOW_ENERGY_PROGRAM_MAX_OFFSET;
	  } else if (linac_low_energy_program_offset < -LINAC_TARGET_CURRENT_LOW_ENERGY_PROGRAM_MAX_OFFSET) {
	    linac_low_energy_program_offset = -LINAC_TARGET_CURRENT_LOW_ENERGY_PROGRAM_MAX_OFFSET;
	  }

	  if (linac_high_energy_program_offset > LINAC_TARGET_CURRENT_HIGH_ENERGY_PROGRAM_MAX_OFFSET) {
	    linac_high_energy_program_offset = LINAC_TARGET_CURRENT_HIGH_ENERGY_PROGRAM_MAX_OFFSET;
	  } else if (linac_high_energy_program_offset < -LINAC_TARGET_CURRENT_HIGH_ENERGY_PROGRAM_MAX_OFFSET) {
	    linac_high_energy_program_offset = -LINAC_TARGET_CURRENT_HIGH_ENERGY_PROGRAM_MAX_OFFSET;
	  }
	  	  
	} 
	
#if !defined(__SET_MAGNETRON_OVER_SERIAL_INTERFACE)
	
	vtemp = Scale16Bit(pac_1_adc_reading, DIRECT_LAMBDA_INPUT_SCALE);
	if (linac_high_energy_target_current_set_point >= 1000) {
	  vtemp += linac_high_energy_program_offset;
	}
	SetPowerSupplyTarget(&ps_hv_lambda_mode_A, vtemp , 0);
	
	vtemp = Scale16Bit(default_pac_2_adc_reading, DIRECT_LAMBDA_INPUT_SCALE);
	if (linac_low_energy_target_current_set_point >= 1000) {
	  vtemp += linac_low_energy_program_offset;
	}
	SetPowerSupplyTarget(&ps_hv_lambda_mode_B, vtemp, 0);
	
#endif



	
	
	UpdateDacAll();                          // We want to Execute DAC update after a pulse so that a pulse does not corrupt the SPI data
	//UpdateIOExpanderOutputs();               // DPAKRER is this needed here?  The io expander outputs should never change in state HV on!!!!
	
	// Set up the 100ms Timer that will roll if there is not another pulse in the next 100mS
	TMR2 = 0;
	_T2IF = 0;          
	global_run_post_pulse_process = 0;	
	SendLoggingDataToUart();
      }
      
      // DPARKER need to write new timing diagram - should be simplier
      
      if (CheckFaultActive()) {
	ExitHvOnState();
	control_state = STATE_FAULT_WARM_FAULT;
      } else if (PIN_FP_MODULATOR_ENABLE_INPUT == !ILL_MODULATOR_ENABLED) {
	ExitHvOnState();
	control_state = STATE_SYSTEM_COLD_READY;
      } else if (PIN_FP_MODULATOR_HV_ON_INPUT == !ILL_MODULATOR_HV_ON) {
	ExitHvOnState();
	control_state = STATE_SYSTEM_WARM_READY;
      }
    }
    break;
    
  case STATE_FAULT_COLD_FAULT:
    DoColdShutDown();
    // SaveDataToEEPROM(); DPARKER use better functions
    while (control_state == STATE_FAULT_COLD_FAULT) {
      Do10msTicToc();
      DoSerialCommand();
      if (CheckFaultActive() == 0) {
	control_state = STATE_SYSTEM_COLD_READY;
      }
    }
    break;
    
  case STATE_FAULT_WARM_FAULT:
    DoWarmShutDown();
    // DPARKER SaveDataToEEPROM();
    while (control_state == STATE_FAULT_WARM_FAULT) {
      Do10msTicToc();
      DoSerialCommand();
      if (CheckColdFaultActive()) {
	control_state = STATE_FAULT_COLD_FAULT;
      } else if (PIN_FP_MODULATOR_ENABLE_INPUT == !ILL_MODULATOR_ENABLED) {
	control_state = STATE_FAULT_COLD_FAULT;
      } else if (CheckFaultActive() == 0) {
	control_state = STATE_SYSTEM_WARM_READY;
      }
    }
    break;
    
  case STATE_FAULT_MCU_CORE_FAULT:
    // DPARKER DO SOMETHING!!!!!!
    // Maybe flash some LEDs, go home
    // your're fracked.  Just hang out here till you get reset
    while (1) {
      Do10msTicToc();
      DoSerialCommand();
    }
    break;

  default:
    // DPARKER throw an ERROR
    control_state = STATE_FAULT_MCU_CORE_FAULT;
    break;
  }
}

void DoA34760StartUpCommon(void) {
  unsigned int *unsigned_int_ptr;  

  // This is debugging info info  If the processor reset, a code that indicates the last major point that the processor entered should be held in RAM at last_known_action
  previous_last_action = last_known_action;
  last_known_action = LAST_ACTION_CLEAR_LAST_ACTION;

  // Figure out why the processor restarted and save to the debug status register
  debug_status_register = 0;
  if (_POR) {
    debug_status_register |= STATUS_POR_RESET;
    // _POR = 0;
  }
  if (_EXTR) {
    debug_status_register |= STATUS_EXTERNAL_RESET;
    //_EXTR = 0;
  }
  if (_SWR) {
    debug_status_register |= STATUS_SOFTARE_RESET;
    //_SWR = 0;
  }
  if (_BOR) {
    debug_status_register |= STATUS_BOR_RESET;
    //_BOR = 0;
  }
  if (_TRAPR) {
    debug_status_register |= STATUS_TRAPR_RESET;
    //_TRAPR = 0;
  }
  if (_WDTO) {
    debug_status_register |= STATUS_WDT_RESET;
    // _WDTO = 0;
  }
  if (_IOPUWR) {
    debug_status_register |= STATUS_IOPUWR_RESET;
    //_IOPUWR = 0;
  }

  
  // Debug Counter Initializations
  global_debug_counter.magnetron_current_adc_glitch = 0;
  global_debug_counter.magnetron_voltage_adc_glitch = 0;
  global_debug_counter.i2c_bus_error = 0;
  global_debug_counter.spi1_bus_error = 0;
  global_debug_counter.spi2_bus_error = 0;
  global_debug_counter.external_adc_false_trigger = 0;
  global_debug_counter.LTC2656_write_error = 0;
  global_debug_counter.setpoint_not_valid = 0;
  global_debug_counter.scale16bit_saturation = 0;
  global_debug_counter.reversescale16bit_saturation = 0;


  linac_high_energy_target_current_set_point = control_loop_cal_data_ram_copy[EEPROM_CNTRL_HIGH_ENERGY_TARGET];

  
  linac_low_energy_target_current_set_point_gantry_mode = control_loop_cal_data_ram_copy[EEPROM_CNTRL_LOW_ENERGY_GANTRY_TARGET];
  linac_low_energy_target_current_set_point_portal_mode = control_loop_cal_data_ram_copy[EEPROM_CNTRL_LOW_ENERGY_PORTAL_TARGET];
  low_energy_target_current_startup_adjust_decay_time_pulses =  control_loop_cal_data_ram_copy[EEPROM_CNTRL_TARGET_STARTUP_PULSES];
  max_low_energy_target_current_startup_adjust_initital_value = control_loop_cal_data_ram_copy[EEPROM_CNTRL_TARGET_MAX_MAGNITUDE];
  low_energy_target_current_startup_max_cooldown = control_loop_cal_data_ram_copy[EEPROM_CNTRL_TARGET_MAX_COOLDOWN];
  

  /*
    Initialize the thyratron heater PID structure
    DPARKER add these values to H file
  */
  thyratron_cathode_heater_PID.abcCoefficients = &pid_thyratron_cathode_heater_abcCoefficient[0];    /*Set up pointer to derived coefficients */
  
  thyratron_cathode_heater_PID.controlHistory = &pid_thyratron_cathode_heater_controlHistory[0];     /*Set up pointer to controller history samples */
  PIDInit(&thyratron_cathode_heater_PID);                                                    /*Clear the controler history and the controller output */
  pid_thyratron_cathode_heater_kCoeffs[0] = Q15(0.02);
  pid_thyratron_cathode_heater_kCoeffs[1] = Q15(0.08);
  pid_thyratron_cathode_heater_kCoeffs[2] = Q15(0.08);
  PIDCoeffCalc(&pid_thyratron_cathode_heater_kCoeffs[0], &thyratron_cathode_heater_PID);             /*Derive the a,b, & c coefficients from the Kp, Ki & Kd */
  
  thyratron_reservoir_heater_PID.abcCoefficients = &pid_thyratron_reservoir_heater_abcCoefficient[0];    /*Set up pointer to derived coefficients */
  thyratron_reservoir_heater_PID.controlHistory = &pid_thyratron_reservoir_heater_controlHistory[0];     /*Set up pointer to controller history samples */
  PIDInit(&thyratron_reservoir_heater_PID);                                          /*Clear the controler history and the controller output */
  pid_thyratron_reservoir_heater_kCoeffs[0] = Q15(0.02);
  pid_thyratron_reservoir_heater_kCoeffs[1] = Q15(0.08);
  pid_thyratron_reservoir_heater_kCoeffs[2] = Q15(0.08);
  PIDCoeffCalc(&pid_thyratron_reservoir_heater_kCoeffs[0], &thyratron_reservoir_heater_PID); /*Derive the a,b, & c coefficients from the Kp, Ki & Kd */
  ClrWdt();

  
    
  // --- ps_magnet initialization ---

  ps_magnet.warmup_ramp_time     = MAGNET_SUPPLY_WARMUP_RAMP_TIME;
  
  ps_magnet.v_max_set_point      = MAX_MAGNET_SUPPLY_VOLTAGE_SET_POINT;
  
  ps_magnet.v_dac_scale          = MakeScale(0xFFFF, MAGNET_SUPPLY_VDAC_OUTPUT_AT_0xFFFF);
  ps_magnet.v_dac_cal_gain       = ps_magnet_config_ram_copy[EEPROM_V_DAC_CAL_GAIN];
  ps_magnet.v_dac_cal_offset     = ps_magnet_config_ram_copy[EEPROM_V_DAC_CAL_OFFSET]; 
  
  ps_magnet.v_adc_scale          = MakeScale(0xFFFF, MAGNET_SUPPLY_VADC_INPUT_AT_0xFFFF);
  ps_magnet.v_adc_cal_gain       = ps_magnet_config_ram_copy[EEPROM_V_ADC_CAL_GAIN];
  ps_magnet.v_adc_cal_offset     = ps_magnet_config_ram_copy[EEPROM_V_ADC_CAL_OFFSET];
  ps_magnet.v_adc_over_abs       = Scale16Bit(MAGNET_SUPPLY_VADC_OVER_VOLTAGE_HARD, ps_magnet.v_adc_scale);
  ps_magnet.v_adc_over_scale     = MakeScale(MAGNET_SUPPLY_VADC_OVER_VOLTAGE_SCALE*8192, 8192);
  ps_magnet.v_adc_under_scale    = MakeScale(MAGNET_SUPPLY_VADC_UNDER_VOLTAGE_SCALE*8192, 8192);
  ps_magnet.v_adc_over_min_value = MAGNET_SUPPLY_VADC_MIN_OVER_VOLTAGE;
  ps_magnet.v_adc_max_oor        = MAGNET_SUPPLY_VADC_MAX_OUT_OT_RANGE;
  
  ps_magnet.i_max_set_point      = MAX_MAGNET_SUPPLY_CURRENT_SET_POINT;
  
  ps_magnet.i_dac_scale          = MakeScale(0xFFFF, MAGNET_SUPPLY_IDAC_OUTPUT_AT_0xFFFF);
  ps_magnet.i_dac_cal_gain       = ps_magnet_config_ram_copy[EEPROM_I_DAC_CAL_GAIN];
  ps_magnet.i_dac_cal_offset     = ps_magnet_config_ram_copy[EEPROM_I_DAC_CAL_OFFSET]; 
  
  ps_magnet.i_adc_scale          = MakeScale(0xFFFF, MAGNET_SUPPLY_IADC_INPUT_AT_0xFFFF);
  ps_magnet.i_adc_cal_gain       = ps_magnet_config_ram_copy[EEPROM_I_ADC_CAL_GAIN];
  ps_magnet.i_adc_cal_offset     = ps_magnet_config_ram_copy[EEPROM_I_ADC_CAL_OFFSET];
  ps_magnet.i_adc_over_abs       = Scale16Bit(MAGNET_SUPPLY_IADC_OVER_CURRENT_HARD, ps_magnet.i_adc_scale);
  ps_magnet.i_adc_over_scale     = MakeScale(MAGNET_SUPPLY_IADC_OVER_CURRENT_SCALE*8192, 8192);
  ps_magnet.i_adc_under_scale    = MakeScale(MAGNET_SUPPLY_IADC_UNDER_CURRENT_SCALE*8192, 8192);
  ps_magnet.i_adc_over_min_value = MAGNET_SUPPLY_IADC_MIN_OVER_CURRENT;
  ps_magnet.i_adc_max_oor        = MAGNET_SUPPLY_IADC_MAX_OUT_OT_RANGE;

  SetPowerSupplyTarget(&ps_magnet, ps_magnet_config_ram_copy[EEPROM_V_SET_POINT], ps_magnet_config_ram_copy[EEPROM_I_SET_POINT]);
  


  // --- ps_filament initialization ---

  ps_filament.warmup_ramp_time     = FILAMENT_SUPPLY_WARMUP_RAMP_TIME;
  
  ps_filament.v_max_set_point      = MAX_FILAMENT_SUPPLY_VOLTAGE_SET_POINT;
  
  ps_filament.v_dac_scale          = MakeScale(0xFFFF, FILAMENT_SUPPLY_VDAC_OUTPUT_AT_0xFFFF);
  ps_filament.v_dac_cal_gain       = ps_filament_config_ram_copy[EEPROM_V_DAC_CAL_GAIN];
  ps_filament.v_dac_cal_offset     = ps_filament_config_ram_copy[EEPROM_V_DAC_CAL_OFFSET]; 
  
  ps_filament.v_adc_scale          = MakeScale(0xFFFF, FILAMENT_SUPPLY_VADC_INPUT_AT_0xFFFF);
  ps_filament.v_adc_cal_gain       = ps_filament_config_ram_copy[EEPROM_V_ADC_CAL_GAIN];
  ps_filament.v_adc_cal_offset     = ps_filament_config_ram_copy[EEPROM_V_ADC_CAL_OFFSET];
  ps_filament.v_adc_over_abs       = Scale16Bit(FILAMENT_SUPPLY_VADC_OVER_VOLTAGE_HARD, ps_filament.v_adc_scale);
  ps_filament.v_adc_over_scale     = MakeScale(FILAMENT_SUPPLY_VADC_OVER_VOLTAGE_SCALE*8192, 8192);
  ps_filament.v_adc_under_scale    = MakeScale(FILAMENT_SUPPLY_VADC_UNDER_VOLTAGE_SCALE*8192, 8192);
  ps_filament.v_adc_over_min_value = FILAMENT_SUPPLY_VADC_MIN_OVER_VOLTAGE;
  ps_filament.v_adc_max_oor        = FILAMENT_SUPPLY_VADC_MAX_OUT_OT_RANGE;
  
  ps_filament.i_max_set_point      = MAX_FILAMENT_SUPPLY_CURRENT_SET_POINT;
  
  ps_filament.i_dac_scale          = MakeScale(0xFFFF, FILAMENT_SUPPLY_IDAC_OUTPUT_AT_0xFFFF);
  ps_filament.i_dac_cal_gain       = ps_filament_config_ram_copy[EEPROM_I_DAC_CAL_GAIN];
  ps_filament.i_dac_cal_offset     = ps_filament_config_ram_copy[EEPROM_I_DAC_CAL_OFFSET]; 
  
  ps_filament.i_adc_scale          = MakeScale(0xFFFF, FILAMENT_SUPPLY_IADC_INPUT_AT_0xFFFF);
  ps_filament.i_adc_cal_gain       = ps_filament_config_ram_copy[EEPROM_I_ADC_CAL_GAIN];
  ps_filament.i_adc_cal_offset     = ps_filament_config_ram_copy[EEPROM_I_ADC_CAL_OFFSET];
  ps_filament.i_adc_over_abs       = Scale16Bit(FILAMENT_SUPPLY_IADC_OVER_CURRENT_HARD, ps_filament.i_adc_scale);
  ps_filament.i_adc_over_scale     = MakeScale(FILAMENT_SUPPLY_IADC_OVER_CURRENT_SCALE*8192, 8192);
  ps_filament.i_adc_under_scale    = MakeScale(FILAMENT_SUPPLY_IADC_UNDER_CURRENT_SCALE*8192, 8192);
  ps_filament.i_adc_over_min_value = FILAMENT_SUPPLY_IADC_MIN_OVER_CURRENT;
  ps_filament.i_adc_max_oor        = FILAMENT_SUPPLY_IADC_MAX_OUT_OT_RANGE;

  SetPowerSupplyTarget(&ps_filament, ps_filament_config_ram_copy[EEPROM_V_SET_POINT], ps_filament_config_ram_copy[EEPROM_I_SET_POINT]);
  


  // --- ps_thyr_cathode_htr initialization ---

  ps_thyr_cathode_htr.warmup_ramp_time     = THYR_CATH_HTR_SUPPLY_WARMUP_RAMP_TIME;
  
  ps_thyr_cathode_htr.v_max_set_point      = MAX_THYR_CATH_HTR_SUPPLY_VOLTAGE_SET_POINT;
  
  ps_thyr_cathode_htr.v_dac_scale          = MakeScale(0xFFFF, THYR_CATH_HTR_SUPPLY_VDAC_OUTPUT_AT_0xFFFF);
  ps_thyr_cathode_htr.v_dac_cal_gain       = ps_thyr_cathode_htr_config_ram_copy[EEPROM_V_DAC_CAL_GAIN];
  ps_thyr_cathode_htr.v_dac_cal_offset     = ps_thyr_cathode_htr_config_ram_copy[EEPROM_V_DAC_CAL_OFFSET]; 
  
  ps_thyr_cathode_htr.v_adc_scale          = MakeScale(0xFFFF, THYR_CATH_HTR_SUPPLY_VADC_INPUT_AT_0xFFFF);
  ps_thyr_cathode_htr.v_adc_cal_gain       = ps_thyr_cathode_htr_config_ram_copy[EEPROM_V_ADC_CAL_GAIN];
  ps_thyr_cathode_htr.v_adc_cal_offset     = ps_thyr_cathode_htr_config_ram_copy[EEPROM_V_ADC_CAL_OFFSET];
  ps_thyr_cathode_htr.v_adc_over_abs       = Scale16Bit(THYR_CATH_HTR_SUPPLY_VADC_OVER_VOLTAGE_HARD, ps_thyr_cathode_htr.v_adc_scale);
  ps_thyr_cathode_htr.v_adc_over_scale     = MakeScale(THYR_CATH_HTR_SUPPLY_VADC_OVER_VOLTAGE_SCALE*8192, 8192);
  ps_thyr_cathode_htr.v_adc_under_scale    = MakeScale(THYR_CATH_HTR_SUPPLY_VADC_UNDER_VOLTAGE_SCALE*8192, 8192);
  ps_thyr_cathode_htr.v_adc_over_min_value = THYR_CATH_HTR_SUPPLY_VADC_MIN_OVER_VOLTAGE;
  ps_thyr_cathode_htr.v_adc_max_oor        = THYR_CATH_HTR_SUPPLY_VADC_MAX_OUT_OT_RANGE;
  
  ps_thyr_cathode_htr.i_max_set_point      = MAX_THYR_CATH_HTR_SUPPLY_CURRENT_SET_POINT;
  
  ps_thyr_cathode_htr.i_dac_scale          = MakeScale(0xFFFF, THYR_CATH_HTR_SUPPLY_IDAC_OUTPUT_AT_0xFFFF);
  ps_thyr_cathode_htr.i_dac_cal_gain       = ps_thyr_cathode_htr_config_ram_copy[EEPROM_I_DAC_CAL_GAIN];
  ps_thyr_cathode_htr.i_dac_cal_offset     = ps_thyr_cathode_htr_config_ram_copy[EEPROM_I_DAC_CAL_OFFSET]; 
  
  ps_thyr_cathode_htr.i_adc_scale          = MakeScale(0xFFFF, THYR_CATH_HTR_SUPPLY_IADC_INPUT_AT_0xFFFF);
  ps_thyr_cathode_htr.i_adc_cal_gain       = ps_thyr_cathode_htr_config_ram_copy[EEPROM_I_ADC_CAL_GAIN];
  ps_thyr_cathode_htr.i_adc_cal_offset     = ps_thyr_cathode_htr_config_ram_copy[EEPROM_I_ADC_CAL_OFFSET];
  ps_thyr_cathode_htr.i_adc_over_abs       = Scale16Bit(THYR_CATH_HTR_SUPPLY_IADC_OVER_CURRENT_HARD, ps_thyr_cathode_htr.i_adc_scale);
  ps_thyr_cathode_htr.i_adc_over_scale     = MakeScale(THYR_CATH_HTR_SUPPLY_IADC_OVER_CURRENT_SCALE*8192, 8192);
  ps_thyr_cathode_htr.i_adc_under_scale    = MakeScale(THYR_CATH_HTR_SUPPLY_IADC_UNDER_CURRENT_SCALE*8192, 8192);
  ps_thyr_cathode_htr.i_adc_over_min_value = THYR_CATH_HTR_SUPPLY_IADC_MIN_OVER_CURRENT;
  ps_thyr_cathode_htr.i_adc_max_oor        = THYR_CATH_HTR_SUPPLY_IADC_MAX_OUT_OT_RANGE;

  SetPowerSupplyTarget(&ps_thyr_cathode_htr, ps_thyr_cathode_htr_config_ram_copy[EEPROM_V_SET_POINT], ps_thyr_cathode_htr_config_ram_copy[EEPROM_I_SET_POINT]);
  


  // --- ps_thyr_reservoir_htr initialization ---

  ps_thyr_reservoir_htr.warmup_ramp_time     = THYR_RESER_HTR_SUPPLY_WARMUP_RAMP_TIME;
  
  ps_thyr_reservoir_htr.v_max_set_point      = MAX_THYR_RESER_HTR_SUPPLY_VOLTAGE_SET_POINT;
  
  ps_thyr_reservoir_htr.v_dac_scale          = MakeScale(0xFFFF, THYR_RESER_HTR_SUPPLY_VDAC_OUTPUT_AT_0xFFFF);
  ps_thyr_reservoir_htr.v_dac_cal_gain       = ps_thyr_reservoir_htr_config_ram_copy[EEPROM_V_DAC_CAL_GAIN];
  ps_thyr_reservoir_htr.v_dac_cal_offset     = ps_thyr_reservoir_htr_config_ram_copy[EEPROM_V_DAC_CAL_OFFSET]; 
  
  ps_thyr_reservoir_htr.v_adc_scale          = MakeScale(0xFFFF, THYR_RESER_HTR_SUPPLY_VADC_INPUT_AT_0xFFFF);
  ps_thyr_reservoir_htr.v_adc_cal_gain       = ps_thyr_reservoir_htr_config_ram_copy[EEPROM_V_ADC_CAL_GAIN];
  ps_thyr_reservoir_htr.v_adc_cal_offset     = ps_thyr_reservoir_htr_config_ram_copy[EEPROM_V_ADC_CAL_OFFSET];
  ps_thyr_reservoir_htr.v_adc_over_abs       = Scale16Bit(THYR_RESER_HTR_SUPPLY_VADC_OVER_VOLTAGE_HARD, ps_thyr_reservoir_htr.v_adc_scale);
  ps_thyr_reservoir_htr.v_adc_over_scale     = MakeScale(THYR_RESER_HTR_SUPPLY_VADC_OVER_VOLTAGE_SCALE*8192, 8192);
  ps_thyr_reservoir_htr.v_adc_under_scale    = MakeScale(THYR_RESER_HTR_SUPPLY_VADC_UNDER_VOLTAGE_SCALE*8192, 8192);
  ps_thyr_reservoir_htr.v_adc_over_min_value = THYR_RESER_HTR_SUPPLY_VADC_MIN_OVER_VOLTAGE;
  ps_thyr_reservoir_htr.v_adc_max_oor        = THYR_RESER_HTR_SUPPLY_VADC_MAX_OUT_OT_RANGE;
  
  ps_thyr_reservoir_htr.i_max_set_point      = MAX_THYR_RESER_HTR_SUPPLY_CURRENT_SET_POINT;
  
  ps_thyr_reservoir_htr.i_dac_scale          = MakeScale(0xFFFF, THYR_RESER_HTR_SUPPLY_IDAC_OUTPUT_AT_0xFFFF);
  ps_thyr_reservoir_htr.i_dac_cal_gain       = ps_thyr_reservoir_htr_config_ram_copy[EEPROM_I_DAC_CAL_GAIN];
  ps_thyr_reservoir_htr.i_dac_cal_offset     = ps_thyr_reservoir_htr_config_ram_copy[EEPROM_I_DAC_CAL_OFFSET]; 
  
  ps_thyr_reservoir_htr.i_adc_scale          = MakeScale(0xFFFF, THYR_RESER_HTR_SUPPLY_IADC_INPUT_AT_0xFFFF);
  ps_thyr_reservoir_htr.i_adc_cal_gain       = ps_thyr_reservoir_htr_config_ram_copy[EEPROM_I_ADC_CAL_GAIN];
  ps_thyr_reservoir_htr.i_adc_cal_offset     = ps_thyr_reservoir_htr_config_ram_copy[EEPROM_I_ADC_CAL_OFFSET];
  ps_thyr_reservoir_htr.i_adc_over_abs       = Scale16Bit(THYR_RESER_HTR_SUPPLY_IADC_OVER_CURRENT_HARD, ps_thyr_reservoir_htr.i_adc_scale);
  ps_thyr_reservoir_htr.i_adc_over_scale     = MakeScale(THYR_RESER_HTR_SUPPLY_IADC_OVER_CURRENT_SCALE*8192, 8192);
  ps_thyr_reservoir_htr.i_adc_under_scale    = MakeScale(THYR_RESER_HTR_SUPPLY_IADC_UNDER_CURRENT_SCALE*8192, 8192);
  ps_thyr_reservoir_htr.i_adc_over_min_value = THYR_RESER_HTR_SUPPLY_IADC_MIN_OVER_CURRENT;
  ps_thyr_reservoir_htr.i_adc_max_oor        = THYR_RESER_HTR_SUPPLY_IADC_MAX_OUT_OT_RANGE;

  SetPowerSupplyTarget(&ps_thyr_reservoir_htr, ps_thyr_reservoir_htr_config_ram_copy[EEPROM_V_SET_POINT], ps_thyr_reservoir_htr_config_ram_copy[EEPROM_I_SET_POINT]);
  


  // --- ps_hv_lambda_mode_A initialization ---

  ps_hv_lambda_mode_A.warmup_ramp_time     = HV_LAMBDA_MODE_A_WARMUP_RAMP_TIME;
  
  ps_hv_lambda_mode_A.v_max_set_point      = MAX_HV_LAMBDA_MODE_A_VOLTAGE_SET_POINT;
  
  ps_hv_lambda_mode_A.v_dac_scale          = MakeScale(0xFFFF, HV_LAMBDA_MODE_A_VDAC_OUTPUT_AT_0xFFFF);
  ps_hv_lambda_mode_A.v_dac_cal_gain       = ps_hv_lambda_mode_A_config_ram_copy[EEPROM_V_DAC_CAL_GAIN];
  ps_hv_lambda_mode_A.v_dac_cal_offset     = ps_hv_lambda_mode_A_config_ram_copy[EEPROM_V_DAC_CAL_OFFSET]; 
  
  ps_hv_lambda_mode_A.v_adc_scale          = MakeScale(0xFFFF, HV_LAMBDA_MODE_A_VADC_INPUT_AT_0xFFFF);
  ps_hv_lambda_mode_A.v_adc_cal_gain       = ps_hv_lambda_mode_A_config_ram_copy[EEPROM_V_ADC_CAL_GAIN];
  ps_hv_lambda_mode_A.v_adc_cal_offset     = ps_hv_lambda_mode_A_config_ram_copy[EEPROM_V_ADC_CAL_OFFSET];
  ps_hv_lambda_mode_A.v_adc_over_abs       = Scale16Bit(HV_LAMBDA_MODE_A_VADC_OVER_VOLTAGE_HARD, ps_hv_lambda_mode_A.v_adc_scale);
  ps_hv_lambda_mode_A.v_adc_over_scale     = MakeScale(HV_LAMBDA_MODE_A_VADC_OVER_VOLTAGE_SCALE*8192, 8192);
  ps_hv_lambda_mode_A.v_adc_under_scale    = MakeScale(HV_LAMBDA_MODE_A_VADC_UNDER_VOLTAGE_SCALE*8192, 8192);
  ps_hv_lambda_mode_A.v_adc_over_min_value = HV_LAMBDA_MODE_A_VADC_MIN_OVER_VOLTAGE;
  ps_hv_lambda_mode_A.v_adc_max_oor        = HV_LAMBDA_MODE_A_VADC_MAX_OUT_OT_RANGE;
  
  ps_hv_lambda_mode_A.i_max_set_point      = MAX_HV_LAMBDA_MODE_A_CURRENT_SET_POINT;
  
  ps_hv_lambda_mode_A.i_dac_scale          = MakeScale(0xFFFF, HV_LAMBDA_MODE_A_IDAC_OUTPUT_AT_0xFFFF);
  ps_hv_lambda_mode_A.i_dac_cal_gain       = ps_hv_lambda_mode_A_config_ram_copy[EEPROM_I_DAC_CAL_GAIN];
  ps_hv_lambda_mode_A.i_dac_cal_offset     = ps_hv_lambda_mode_A_config_ram_copy[EEPROM_I_DAC_CAL_OFFSET]; 
  
  ps_hv_lambda_mode_A.i_adc_scale          = MakeScale(0xFFFF, HV_LAMBDA_MODE_A_IADC_INPUT_AT_0xFFFF);
  ps_hv_lambda_mode_A.i_adc_cal_gain       = ps_hv_lambda_mode_A_config_ram_copy[EEPROM_I_ADC_CAL_GAIN];
  ps_hv_lambda_mode_A.i_adc_cal_offset     = ps_hv_lambda_mode_A_config_ram_copy[EEPROM_I_ADC_CAL_OFFSET];
  ps_hv_lambda_mode_A.i_adc_over_abs       = Scale16Bit(HV_LAMBDA_MODE_A_IADC_OVER_CURRENT_HARD, ps_hv_lambda_mode_A.i_adc_scale);
  ps_hv_lambda_mode_A.i_adc_over_scale     = MakeScale(HV_LAMBDA_MODE_A_IADC_OVER_CURRENT_SCALE*8192, 8192);
  ps_hv_lambda_mode_A.i_adc_under_scale    = MakeScale(HV_LAMBDA_MODE_A_IADC_UNDER_CURRENT_SCALE*8192, 8192);
  ps_hv_lambda_mode_A.i_adc_over_min_value = HV_LAMBDA_MODE_A_IADC_MIN_OVER_CURRENT;
  ps_hv_lambda_mode_A.i_adc_max_oor        = HV_LAMBDA_MODE_A_IADC_MAX_OUT_OT_RANGE;

  SetPowerSupplyTarget(&ps_hv_lambda_mode_A, ps_hv_lambda_mode_A_config_ram_copy[EEPROM_V_SET_POINT], ps_hv_lambda_mode_A_config_ram_copy[EEPROM_I_SET_POINT]);
  


  // --- ps_hv_lambda_mode_B initialization ---

  ps_hv_lambda_mode_B.warmup_ramp_time     = HV_LAMBDA_MODE_B_WARMUP_RAMP_TIME;
  
  ps_hv_lambda_mode_B.v_max_set_point      = MAX_HV_LAMBDA_MODE_B_VOLTAGE_SET_POINT;
  
  ps_hv_lambda_mode_B.v_dac_scale          = MakeScale(0xFFFF, HV_LAMBDA_MODE_B_VDAC_OUTPUT_AT_0xFFFF);
  ps_hv_lambda_mode_B.v_dac_cal_gain       = ps_hv_lambda_mode_B_config_ram_copy[EEPROM_V_DAC_CAL_GAIN];
  ps_hv_lambda_mode_B.v_dac_cal_offset     = ps_hv_lambda_mode_B_config_ram_copy[EEPROM_V_DAC_CAL_OFFSET]; 
  
  ps_hv_lambda_mode_B.v_adc_scale          = MakeScale(0xFFFF, HV_LAMBDA_MODE_B_VADC_INPUT_AT_0xFFFF);
  ps_hv_lambda_mode_B.v_adc_cal_gain       = ps_hv_lambda_mode_B_config_ram_copy[EEPROM_V_ADC_CAL_GAIN];
  ps_hv_lambda_mode_B.v_adc_cal_offset     = ps_hv_lambda_mode_B_config_ram_copy[EEPROM_V_ADC_CAL_OFFSET];
  ps_hv_lambda_mode_B.v_adc_over_abs       = Scale16Bit(HV_LAMBDA_MODE_B_VADC_OVER_VOLTAGE_HARD, ps_hv_lambda_mode_B.v_adc_scale);
  ps_hv_lambda_mode_B.v_adc_over_scale     = MakeScale(HV_LAMBDA_MODE_B_VADC_OVER_VOLTAGE_SCALE*8192, 8192);
  ps_hv_lambda_mode_B.v_adc_under_scale    = MakeScale(HV_LAMBDA_MODE_B_VADC_UNDER_VOLTAGE_SCALE*8192, 8192);
  ps_hv_lambda_mode_B.v_adc_over_min_value = HV_LAMBDA_MODE_B_VADC_MIN_OVER_VOLTAGE;
  ps_hv_lambda_mode_B.v_adc_max_oor        = HV_LAMBDA_MODE_B_VADC_MAX_OUT_OT_RANGE;
  
  ps_hv_lambda_mode_B.i_max_set_point      = MAX_HV_LAMBDA_MODE_B_CURRENT_SET_POINT;
  
  ps_hv_lambda_mode_B.i_dac_scale          = MakeScale(0xFFFF, HV_LAMBDA_MODE_B_IDAC_OUTPUT_AT_0xFFFF);
  ps_hv_lambda_mode_B.i_dac_cal_gain       = ps_hv_lambda_mode_B_config_ram_copy[EEPROM_I_DAC_CAL_GAIN];
  ps_hv_lambda_mode_B.i_dac_cal_offset     = ps_hv_lambda_mode_B_config_ram_copy[EEPROM_I_DAC_CAL_OFFSET]; 
  
  ps_hv_lambda_mode_B.i_adc_scale          = MakeScale(0xFFFF, HV_LAMBDA_MODE_B_IADC_INPUT_AT_0xFFFF);
  ps_hv_lambda_mode_B.i_adc_cal_gain       = ps_hv_lambda_mode_B_config_ram_copy[EEPROM_I_ADC_CAL_GAIN];
  ps_hv_lambda_mode_B.i_adc_cal_offset     = ps_hv_lambda_mode_B_config_ram_copy[EEPROM_I_ADC_CAL_OFFSET];
  ps_hv_lambda_mode_B.i_adc_over_abs       = Scale16Bit(HV_LAMBDA_MODE_B_IADC_OVER_CURRENT_HARD, ps_hv_lambda_mode_B.i_adc_scale);
  ps_hv_lambda_mode_B.i_adc_over_scale     = MakeScale(HV_LAMBDA_MODE_B_IADC_OVER_CURRENT_SCALE*8192, 8192);
  ps_hv_lambda_mode_B.i_adc_under_scale    = MakeScale(HV_LAMBDA_MODE_B_IADC_UNDER_CURRENT_SCALE*8192, 8192);
  ps_hv_lambda_mode_B.i_adc_over_min_value = HV_LAMBDA_MODE_B_IADC_MIN_OVER_CURRENT;
  ps_hv_lambda_mode_B.i_adc_max_oor        = HV_LAMBDA_MODE_B_IADC_MAX_OUT_OT_RANGE;


  SetPowerSupplyTarget(&ps_hv_lambda_mode_B, ps_hv_lambda_mode_B_config_ram_copy[EEPROM_V_SET_POINT], ps_hv_lambda_mode_B_config_ram_copy[EEPROM_I_SET_POINT]);
  


  // --- ps_magnetron_mode_A initialization ---

  ps_magnetron_mode_A.warmup_ramp_time     = MAGNETRON_MODE_A_WARMUP_RAMP_TIME;
  
  ps_magnetron_mode_A.v_max_set_point      = MAX_MAGNETRON_MODE_A_VOLTAGE_SET_POINT;
  
  ps_magnetron_mode_A.v_dac_scale          = MakeScale(0xFFFF, MAGNETRON_MODE_A_VDAC_OUTPUT_AT_0xFFFF);
  ps_magnetron_mode_A.v_dac_cal_gain       = ps_magnetron_mode_A_config_ram_copy[EEPROM_V_DAC_CAL_GAIN];
  ps_magnetron_mode_A.v_dac_cal_offset     = ps_magnetron_mode_A_config_ram_copy[EEPROM_V_DAC_CAL_OFFSET]; 
  
  ps_magnetron_mode_A.v_adc_scale          = MakeScale(0xFFFF, MAGNETRON_MODE_A_VADC_INPUT_AT_0xFFFF);
  ps_magnetron_mode_A.v_adc_cal_gain       = ps_magnetron_mode_A_config_ram_copy[EEPROM_V_ADC_CAL_GAIN];
  ps_magnetron_mode_A.v_adc_cal_offset     = ps_magnetron_mode_A_config_ram_copy[EEPROM_V_ADC_CAL_OFFSET];
  ps_magnetron_mode_A.v_adc_over_abs       = Scale16Bit(MAGNETRON_MODE_A_VADC_OVER_VOLTAGE_HARD, ps_magnetron_mode_A.v_adc_scale);
  ps_magnetron_mode_A.v_adc_over_scale     = MakeScale(MAGNETRON_MODE_A_VADC_OVER_VOLTAGE_SCALE*8192, 8192);
  ps_magnetron_mode_A.v_adc_under_scale    = MakeScale(MAGNETRON_MODE_A_VADC_UNDER_VOLTAGE_SCALE*8192, 8192);
  ps_magnetron_mode_A.v_adc_over_min_value = MAGNETRON_MODE_A_VADC_MIN_OVER_VOLTAGE;
  ps_magnetron_mode_A.v_adc_max_oor        = MAGNETRON_MODE_A_VADC_MAX_OUT_OT_RANGE;
  
  ps_magnetron_mode_A.i_max_set_point      = MAX_MAGNETRON_MODE_A_CURRENT_SET_POINT;
  
  ps_magnetron_mode_A.i_dac_scale          = MakeScale(0xFFFF, MAGNETRON_MODE_A_IDAC_OUTPUT_AT_0xFFFF);
  ps_magnetron_mode_A.i_dac_cal_gain       = ps_magnetron_mode_A_config_ram_copy[EEPROM_I_DAC_CAL_GAIN];
  ps_magnetron_mode_A.i_dac_cal_offset     = ps_magnetron_mode_A_config_ram_copy[EEPROM_I_DAC_CAL_OFFSET]; 
  
  ps_magnetron_mode_A.i_adc_scale          = MakeScale(0xFFFF, MAGNETRON_MODE_A_IADC_INPUT_AT_0xFFFF);
  ps_magnetron_mode_A.i_adc_cal_gain       = ps_magnetron_mode_A_config_ram_copy[EEPROM_I_ADC_CAL_GAIN];
  ps_magnetron_mode_A.i_adc_cal_offset     = ps_magnetron_mode_A_config_ram_copy[EEPROM_I_ADC_CAL_OFFSET];
  ps_magnetron_mode_A.i_adc_over_abs       = Scale16Bit(MAGNETRON_MODE_A_IADC_OVER_CURRENT_HARD, ps_magnetron_mode_A.i_adc_scale);
  ps_magnetron_mode_A.i_adc_over_scale     = MakeScale(MAGNETRON_MODE_A_IADC_OVER_CURRENT_SCALE*8192, 8192);
  ps_magnetron_mode_A.i_adc_under_scale    = MakeScale(MAGNETRON_MODE_A_IADC_UNDER_CURRENT_SCALE*8192, 8192);
  ps_magnetron_mode_A.i_adc_over_min_value = MAGNETRON_MODE_A_IADC_MIN_OVER_CURRENT;
  ps_magnetron_mode_A.i_adc_max_oor        = MAGNETRON_MODE_A_IADC_MAX_OUT_OT_RANGE;

  ps_magnetron_mode_A.i_adc_max_reading    = 0;
  ps_magnetron_mode_A.i_adc_min_reading    = 0xFFFF;
  ps_magnetron_mode_A.v_adc_max_reading    = 0;
  ps_magnetron_mode_A.v_adc_min_reading    = 0xFFFF;

  SetPowerSupplyTarget(&ps_magnetron_mode_A, ps_magnetron_mode_A_config_ram_copy[EEPROM_V_SET_POINT], ps_magnetron_mode_A_config_ram_copy[EEPROM_I_SET_POINT]);
  


  // --- ps_magnetron_mode_B initialization ---

  ps_magnetron_mode_B.warmup_ramp_time     = MAGNETRON_MODE_B_WARMUP_RAMP_TIME;
  
  ps_magnetron_mode_B.v_max_set_point      = MAX_MAGNETRON_MODE_B_VOLTAGE_SET_POINT;
  
  ps_magnetron_mode_B.v_dac_scale          = MakeScale(0xFFFF, MAGNETRON_MODE_B_VDAC_OUTPUT_AT_0xFFFF);
  ps_magnetron_mode_B.v_dac_cal_gain       = ps_magnetron_mode_B_config_ram_copy[EEPROM_V_DAC_CAL_GAIN];
  ps_magnetron_mode_B.v_dac_cal_offset     = ps_magnetron_mode_B_config_ram_copy[EEPROM_V_DAC_CAL_OFFSET]; 
  
  ps_magnetron_mode_B.v_adc_scale          = MakeScale(0xFFFF, MAGNETRON_MODE_B_VADC_INPUT_AT_0xFFFF);
  ps_magnetron_mode_B.v_adc_cal_gain       = ps_magnetron_mode_B_config_ram_copy[EEPROM_V_ADC_CAL_GAIN];
  ps_magnetron_mode_B.v_adc_cal_offset     = ps_magnetron_mode_B_config_ram_copy[EEPROM_V_ADC_CAL_OFFSET];
  ps_magnetron_mode_B.v_adc_over_abs       = Scale16Bit(MAGNETRON_MODE_B_VADC_OVER_VOLTAGE_HARD, ps_magnetron_mode_B.v_adc_scale);
  ps_magnetron_mode_B.v_adc_over_scale     = MakeScale(MAGNETRON_MODE_B_VADC_OVER_VOLTAGE_SCALE*8192, 8192);
  ps_magnetron_mode_B.v_adc_under_scale    = MakeScale(MAGNETRON_MODE_B_VADC_UNDER_VOLTAGE_SCALE*8192, 8192);
  ps_magnetron_mode_B.v_adc_over_min_value = MAGNETRON_MODE_B_VADC_MIN_OVER_VOLTAGE;
  ps_magnetron_mode_B.v_adc_max_oor        = MAGNETRON_MODE_B_VADC_MAX_OUT_OT_RANGE;
  
  ps_magnetron_mode_B.i_max_set_point      = MAX_MAGNETRON_MODE_B_CURRENT_SET_POINT;
  
  ps_magnetron_mode_B.i_dac_scale          = MakeScale(0xFFFF, MAGNETRON_MODE_B_IDAC_OUTPUT_AT_0xFFFF);
  ps_magnetron_mode_B.i_dac_cal_gain       = ps_magnetron_mode_B_config_ram_copy[EEPROM_I_DAC_CAL_GAIN];
  ps_magnetron_mode_B.i_dac_cal_offset     = ps_magnetron_mode_B_config_ram_copy[EEPROM_I_DAC_CAL_OFFSET]; 
  
  ps_magnetron_mode_B.i_adc_scale          = MakeScale(0xFFFF, MAGNETRON_MODE_B_IADC_INPUT_AT_0xFFFF);
  ps_magnetron_mode_B.i_adc_cal_gain       = ps_magnetron_mode_B_config_ram_copy[EEPROM_I_ADC_CAL_GAIN];
  ps_magnetron_mode_B.i_adc_cal_offset     = ps_magnetron_mode_B_config_ram_copy[EEPROM_I_ADC_CAL_OFFSET];
  ps_magnetron_mode_B.i_adc_over_abs       = Scale16Bit(MAGNETRON_MODE_B_IADC_OVER_CURRENT_HARD, ps_magnetron_mode_B.i_adc_scale);
  ps_magnetron_mode_B.i_adc_over_scale     = MakeScale(MAGNETRON_MODE_B_IADC_OVER_CURRENT_SCALE*8192, 8192);
  ps_magnetron_mode_B.i_adc_under_scale    = MakeScale(MAGNETRON_MODE_B_IADC_UNDER_CURRENT_SCALE*8192, 8192);
  ps_magnetron_mode_B.i_adc_over_min_value = MAGNETRON_MODE_B_IADC_MIN_OVER_CURRENT;
  ps_magnetron_mode_B.i_adc_max_oor        = MAGNETRON_MODE_B_IADC_MAX_OUT_OT_RANGE;

  ps_magnetron_mode_B.i_adc_max_reading    = 0;
  ps_magnetron_mode_B.i_adc_min_reading    = 0xFFFF;
  ps_magnetron_mode_B.v_adc_max_reading    = 0;
  ps_magnetron_mode_B.v_adc_min_reading    = 0xFFFF;

  SetPowerSupplyTarget(&ps_magnetron_mode_B, ps_magnetron_mode_B_config_ram_copy[EEPROM_V_SET_POINT], ps_magnetron_mode_B_config_ram_copy[EEPROM_I_SET_POINT]);
  


  PIN_UART2_TX = !PIN_UART2_TX;

  // --------- BEGIN IO PIN CONFIGURATION ------------------
  
  
  // Initialize Ouput Pin Latches BEFORE setting the pins to Output
  PIN_HV_LAMBDA_ENABLE = !OLL_HV_LAMBDA_ENABLED;
  PIN_HV_LAMBDA_INHIBIT = OLL_HV_LAMBDA_INHIBITED;

  //PIN_MAGNETRON_MAGNET_ENABLE = !OLL_MAGNETRON_MAGNET_ENABLED;
  PIN_MAGNETRON_MAGNET_SO = OLL_MAGNETRON_MAGNET_OUTPUT_SHUT_OFF;
  PIN_MAGNETRON_FILAMENT_ENABLE = !OLL_MAGNETRON_FILAMENT_ENABLED;
  
  PIN_SUM_FAULT_FIBER = !OLL_SUM_FAULT_FIBER_FAULT;
  //DPARKER PIN REPURPOSED PIN_UART2_TX = !OLL_SPARE_OPTICAL_OUT_LIGHT_ON;
  PIN_UART2_TX = !OLL_PIN_UART2_TX_LIGHT_ON;
  PIN_FAST_RESTART_STORAGE_CAP_OUTPUT = !OLL_DO_FAST_RESTART;

  PIN_THYRATRON_TRIGGER_ENABLE = !OLL_THYRATRON_TRIGGER_ENABLED;
  PIN_SUM_FAULT_COPPER = !OLL_SUM_FAULT_COPPER_FAULT;
  PIN_LATCH_RESET = !OLL_RESET_LATCH;
  PIN_PULSE_LATCH_RESET = !OLL_PULSE_LATCH_RESET;
  PIN_MAIN_CONTACTOR_CLOSE = !OLL_MAIN_CONTACTOR_CLOSED;
  PIN_SAMPLE_PFN_IREV = !OLL_SAMPLE_PFN_IREV_TRIGGER;
  PIN_RS422_DE = OLL_RS422_DE_ENABLE_RS422_DRIVER;
  // DPARKER PIN_MCU_CLOCK_OUT_TEST_POINT = 0;
  PIN_LAMBDA_VOLTAGE_SELECT = OLL_SELECT_LAMBDA_MODE_A_VOLTAGE;


  // External 16 bit ADC control pins
  PIN_PULSE_IMON_CS = !OLL_SELECT_PULSE_IMON_ADC;
  PIN_PULSE_VMON_CS = !OLL_SELECT_PULSE_VMON_ADC;
  PIN_PULSE_ADC_TRIGGER = !OLL_TRIGGER_PULSE_ADCS;

  
  // HV Lambda IO Pins
  TRIS_PIN_HV_LAMBDA_ENABLE = TRIS_OUTPUT_MODE;
  TRIS_PIN_HV_LAMBDA_INHIBIT = TRIS_OUTPUT_MODE;

  TRIS_PIN_HV_LAMBDA_EOC_INPUT = TRIS_INPUT_MODE;
  TRIS_PIN_HV_LAMBDA_SUM_FAULT = TRIS_INPUT_MODE;
  TRIS_PIN_HV_LAMBDA_HV_ON_READBACK = TRIS_INPUT_MODE;
  TRIS_PIN_HV_LAMBDA_POWER_UP = TRIS_INPUT_MODE;
  

  // Magnetron Filament/Magnet Supply
  TRIS_PIN_MAGNETRON_MAGNET_ENABLE = TRIS_OUTPUT_MODE;
  TRIS_PIN_MAGNETRON_MAGNET_SO = TRIS_OUTPUT_MODE;
  TRIS_PIN_MAGNETRON_FILAMENT_ENABLE = TRIS_OUTPUT_MODE;


  // Optical Input Pins
  TRIS_PIN_MODULATOR_DRIVE_INPUT = TRIS_INPUT_MODE;
  TRIS_PIN_A_B_MODE_SELECT = TRIS_INPUT_MODE;

  // Optical Output PIns
  TRIS_PIN_SUM_FAULT_FIBER = TRIS_OUTPUT_MODE;
  //DPARKER PIN REPURPOSED TRIS_PIN_SPARE_OPTICAL_OUT = TRIS_OUTPUT_MODE;
  TRIS_PIN_UART2_TX = TRIS_OUTPUT_MODE;
  TRIS_PIN_FAST_RESTART_STORAGE_CAP = TRIS_OUTPUT_MODE;


  // Digital Control Input Pins
  TRIS_FP_PIN_MODULATOR_ENABLE_INPUT = TRIS_INPUT_MODE;
  TRIS_FP_PIN_MODULATOR_HV_ON_INPUT = TRIS_INPUT_MODE;
  TRIS_FP_PIN_MODULATOR_RESET = TRIS_INPUT_MODE;
  TRIS_FP_PIN_FAST_RESTART = TRIS_INPUT_MODE;
  TRIS_FP_PIN_SPARE_2_SAMPLE_VPROG_INPUT = TRIS_INPUT_MODE;
  TRIS_PIN_GANTRY_PORTAL_SELECT = TRIS_INPUT_MODE;


  // Analog Compartor/Latch Input Pins
  TRIS_PIN_PULSE_OVER_CUR_LATCH = TRIS_INPUT_MODE;
  TRIS_PIN_PULSE_MIN_CUR_LATCH = TRIS_INPUT_MODE;
  TRIS_PIN_MAGNET_CURRENT_OOR_LATCH = TRIS_INPUT_MODE;
  TRIS_PIN_FILAMENT_OV_LATCH = TRIS_INPUT_MODE;
  
  // Interlock Inputs
  TRIS_PIN_INTERLOCK_1 = TRIS_INPUT_MODE;
  TRIS_PIN_INTERLOCK_2 = TRIS_INPUT_MODE;
  TRIS_PIN_INTERLOCK_3 = TRIS_INPUT_MODE;
  TRIS_PIN_INTERLOCK_4 = TRIS_INPUT_MODE;
  TRIS_PIN_4_20_DRVR_FLT = TRIS_INPUT_MODE;

  // Digital Output Pins
  TRIS_PIN_THYRATRON_TRIGGER_ENABLE = TRIS_OUTPUT_MODE;
  TRIS_PIN_SUM_FAULT_COPPER = TRIS_OUTPUT_MODE;
  TRIS_PIN_LATCH_RESET = TRIS_OUTPUT_MODE;
  TRIS_PIN_PULSE_LATCH_RESET = TRIS_OUTPUT_MODE;
  TRIS_PIN_MAIN_CONTACTOR_CLOSE = TRIS_OUTPUT_MODE;
  TRIS_PIN_SAMPLE_PFN_IREV = TRIS_OUTPUT_MODE;
  TRIS_PIN_RS422_DE = TRIS_OUTPUT_MODE;
  TRIS_PIN_MCU_CLOCK_OUT_TEST_POINT = TRIS_OUTPUT_MODE;
  TRIS_PIN_LAMBDA_VOLTAGE_SELECT = TRIS_OUTPUT_MODE;
  TRIS_PIN_PULSE_IMON_CS = TRIS_OUTPUT_MODE;
  TRIS_PIN_PULSE_VMON_CS = TRIS_OUTPUT_MODE;
  TRIS_PIN_PULSE_ADC_TRIGGER = TRIS_OUTPUT_MODE;



  // ----------- Configure Interupts -------------- //


  // Configure INT1 Interrupt
  _INT1IE = 0; // Disable Interrupt
  _INT1IF = 0; // Clear Interrupt Flag  
  _INT1IP = 6; // Highest Priority
  _INT1EP = 0; // Positive Transition Trigger

  // Configure T1 Interrupt
  _T1IE = 0;  // Disable Interrupt
  _T1IF = 0;  // Clear Interrupt Flag  
  _T1IP = 5;  // Lower Priority than INT1, Higher than everything else  

  // Configure ADC Interrupt
  _ADIE = 0;
  _ADIF = 0;
  _ADIP = 3;


  // Configure Change Notification Interrupt
  _CNIF = 0;
  _CN16IE = 1;  // Allow Change notification on CN16 (PULSE MINIMUM CURRENT LATCH)
  _CNIE = 1;
  _CNIP = 4;
  


  // Configure UART Interrupts
  _U1RXIE = 0;
  _U1RXIP = 5;
  
  _U1TXIE = 0;
  _U1RXIP = 5;


  // Configure LVD Interrupt
  _LVDIF = 0;
  _LVDIE = 0;
  _LVDIP = 7;
  _LVDL = 0b1100;  //DPARKER LVDL should trigger at 4.1 Volts

  

  // --------- CONFIGURATION FOR THE I2C BUS ---------------- //
  I2CCON = I2CCON_SETUP;
  I2CBRG = I2C_BAUD_RATE_GENERATOR;  // Configure I2C bus based on H file parameters



  // --------- CONFIGURATION FOR THE SPI BUSSES ---------------- //
  OpenSPI1((A34760_SPI1CON_VALUE & A34760_SPI1CON_CLOCK), A34760_SPI1STAT_VALUE);  // Configure SPI bus 1 based on H file parameters
  OpenSPI2((A34760_SPI2CON_VALUE & A34760_SPI2CON_CLOCK), A34760_SPI2STAT_VALUE);  // Configure SPI bus 2 based on H file parameters
  

  // ------ CONFIGURE the CAN Modules to be OFF -------------- //
  C1CTRL = 0b0000000100000000;
  C2CTRL = 0b0000000100000000;
  

  // ----------------- UART #1 Setup and Data Buffer -------------------------//
  // Setup the UART input and output buffers
  uart1_input_buffer.write_location = 0;  
  uart1_input_buffer.read_location = 0;
  uart1_output_buffer.write_location = 0;
  uart1_output_buffer.read_location = 0;

  U1MODE = A34760_U1MODE_VALUE;
  U1BRG = A34760_U1BRG_VALUE;
  U1STA = A34760_U1STA_VALUE;
  


 // ---------- Configure Timers ----------------- //


  // Configure TMR1
  T1CON = A34760_T1CON_VALUE;


  // Configure TMR2
  T2CON = A34760_T2CON_VALUE;
  PR2 = A34760_PR2_VALUE;  
  TMR2 = 0;
  _T2IF = 0;

  
  // Configure TMR3
  // Setup Timer 3 to measure interpulse period.
  T3CON = (T3_ON & T3_IDLE_CON & T3_GATE_OFF & T3_PS_1_64 & T3_SOURCE_INT);
  PR3 = 62500;  // 400mS


  // Configure TMR4
  T4CON = A34760_T4CON_VALUE;


  // Configure TMR5
  T5CON = A34760_T5CON_VALUE;
  TMR5 = 0;
  _T5IF = 0;
  PR5 = A34760_PR5_VALUE; 





  // --------------- Initialize U44 - LTC2656 ------------------------- //
  U44_LTC2656.pin_cable_select = _PIN_RD15;
  U44_LTC2656.pin_load_dac = _PIN_RD14;
  U44_LTC2656.pin_por_select = _PIN_NOT_CONNECTED;
  U44_LTC2656.por_select_value = 0;
  U44_LTC2656.spi_port = SPI_PORT_1;
#ifndef __A36760
  U44_LTC2656.pin_dac_clear = _PIN_RB15;
#else
  U44_LTC2656.pin_dac_clear = _PIN_NOT_CONNECTED;
#endif


  SetupLTC2656(&U44_LTC2656);
  

  // ---------------- Initialize U64 - MCP23017 ----------------//

  U64_MCP23017.address = MCP23017_ADDRESS_0;
  U64_MCP23017.i2c_port = I2C_PORT;
  U64_MCP23017.pin_reset = _PIN_NOT_CONNECTED;
  U64_MCP23017.pin_int_a = _PIN_NOT_CONNECTED;
  U64_MCP23017.pin_int_b = _PIN_NOT_CONNECTED;
  U64_MCP23017.output_latch_a_in_ram = MCP23017_U64_LATA_INITIAL;
  U64_MCP23017.output_latch_b_in_ram = MCP23017_U64_LATB_INITIAL;


  ResetAllFaults();


  // DPARKER - At some point want to recover the pulse and arc counter from RAM
  // ------------ Load the pulse and arc counters ---------------- //
  unsigned_int_ptr = &pulse_counter_persistent;              //unsigned_int_ptr now points to the Least Significant word of pulse_counter_persistent
  *unsigned_int_ptr = pulse_counter_repository_ram_copy[3];
  unsigned_int_ptr++;                                        //unsigned_int_ptr now points to the next lest signigicant word of pulse_counter_persistent
  *unsigned_int_ptr = pulse_counter_repository_ram_copy[2];
  unsigned_int_ptr++;                                        //unsigned_int_ptr now points to the next lest signigicant word of pulse_counter_persistent
  *unsigned_int_ptr = pulse_counter_repository_ram_copy[1];
  unsigned_int_ptr++;                                        //unsigned_int_ptr now points to the most signigicant word of pulse_counter_persistent
  *unsigned_int_ptr = pulse_counter_repository_ram_copy[0];


  unsigned_int_ptr = &arc_counter_persistent;                //unsigned_int_ptr now points to the Least Significant word of arc_counter_persistent
  *unsigned_int_ptr = pulse_counter_repository_ram_copy[5];
  unsigned_int_ptr++;                                        //unsigned_int_ptr now points to the most signigicant word of arc_counter_persistent
  *unsigned_int_ptr = pulse_counter_repository_ram_copy[4];
  
  arc_counter_this_hv_on = 0;
  pulse_counter_this_hv_on = 0;
 
  fast_reset_counter_persistent = pulse_counter_repository_ram_copy[6];
  
  /*
    Check to See if this was a faulty processor Reset.
    If it was a faulty processor Reset, the following must occur
    * Durring warmup, if the Fast Restart Pin is set, the warmup will be truncated to 2 seconds
    
    * Read persistent RAM values and save to EEPROM as nessesary
    1) When the processor restarts, copy the persistent counters to a new RAM locations
    2) Load values from EEPROM
    3) The following statement *should* be true
    4) pulse_counter_persistent(from_before_arc) = pulse_counter_persistent(stored_in_EEPROM) + pulse_counter_this_hv_on(from_before_arc)
    4a) If that math works out, save the pulse information.  If that math does not work out, throw away pre-arc data and just reload from EEPROM
  */
  /*
  pulse_counter_persistent_store = pulse_counter_persistent;
  pulse_counter_this_hv_on_store = pulse_counter_this_hv_on;
  arc_counter_persistent_store = arc_counter_persistent;
  arc_counter_this_hv_on_store = arc_counter_this_hv_on;


  if (PIN_FP_FAST_RESTART == ILL_FAST_RESTART) {
    if (pulse_counter_persistent_store > 5) {
      pulse_counter_persistent_store -= 5;
    }
    if ((pulse_counter_persistent + pulse_counter_this_hv_on_store) > pulse_counter_persistent_store) {
      if ((pulse_counter_persistent + pulse_counter_this_hv_on_store) < (pulse_counter_persistent_store + 10)) {
	pulse_counter_persistent = pulse_counter_persistent_store + 5;
      }
    }
    
    if (arc_counter_persistent_store > 5) {
      arc_counter_persistent_store -= 5;
    }
    if ((arc_counter_persistent + arc_counter_this_hv_on_store) > arc_counter_persistent_store) {
      if ((arc_counter_persistent + arc_counter_this_hv_on_store) < (arc_counter_persistent_store + 10)) {
	arc_counter_persistent = arc_counter_persistent_store + 5;
      }
    }
    
    SavePulseCountersToEEPROM();
    }
  */   


 
}



void DoA34760StartUpNormalProcess(void) {
  unsigned int i2c_test = 0;

  // This was a not a reset from a fast crash so clear the reset data
  debug_status_register = 0;
  _POR = 0;
  _EXTR = 0;
  _SWR = 0;
  _BOR = 0;
  _TRAPR = 0;
  _WDTO = 0;
  _IOPUWR = 0;
  last_known_action = LAST_ACTION_CLEAR_LAST_ACTION;
  processor_crash_count = 0;
  
  ClearOutputsLTC2656(&U44_LTC2656);
  
  // Test U64 - MCP23017
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_IOCON, MCP23017_DEFAULT_IOCON);
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_IOCON, MCP23017_DEFAULT_IOCON);
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_IOCON, MCP23017_DEFAULT_IOCON);
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_OLATA, U64_MCP23017.output_latch_a_in_ram);
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_OLATB, U64_MCP23017.output_latch_b_in_ram);
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_IODIRA, MCP23017_U64_IODIRA_VALUE);
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_IODIRB, MCP23017_U64_IODIRB_VALUE);
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_IPOLA, MCP23017_U64_IPOLA_VALUE);
  i2c_test |= MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_IPOLB, MCP23017_U64_IPOLB_VALUE);
    
  if ((i2c_test & 0xFF00) == 0xFA00) {
    // There was a fault on the i2c bus, the MCP23017 did not initialize properly
    debug_status_register |= STATUS_DIGITAL_IO_EXP_ERROR;
    global_debug_counter.i2c_bus_error++;
  }
  
  i2c_test = MCP23017ReadSingleByte(&U64_MCP23017, MCP23017_REGISTER_IODIRA);
  if ((i2c_test & 0x00FF) != MCP23017_U64_IODIRA_VALUE) {
    // The MCP Write/Read operation failed
    debug_status_register |= STATUS_DIGITAL_IO_EXP_ERROR;
    global_debug_counter.i2c_bus_error++;
  }
 
  DisableMagnetronFilamentSupply();
  DisableMagnetronMagnetSupply();
  DisableHVLambdaSupply();
}






void DoA34760StartUpFastProcess(void) {
#if !defined(__SET_MAGNETRON_OVER_SERIAL_INTERFACE)
  unsigned int vtemp;
#endif
  
  unsigned int vtemp_2;
  unsigned int itemp_2;

  processor_crash_count++;

  fast_reset_counter_persistent++;
  pulse_counter_repository_ram_copy[6] = fast_reset_counter_persistent;
  PIN_FAST_RESTART_STORAGE_CAP_OUTPUT = OLL_DO_FAST_RESTART;

  // Taken from StartWarmUp(); & the start of State Warm Ready
  PIDInit(&thyratron_reservoir_heater_PID);
  PIDInit(&thyratron_cathode_heater_PID);

  // DPARKER, there are a lot more power supply configuration calls going on here than are needed, that is a lot of wasted CPU cycles as these are complex math operations
  ScalePowerSupply(&ps_hv_lambda_mode_A,100,100);   // DPARKER this may be re-adjusted below
  ScalePowerSupply(&ps_hv_lambda_mode_B,100,100);   // DPARKER this may be re-adjusted below
  ScalePowerSupply(&ps_filament,100,100);           // DPARKER this may be re-adjusted below
  ScalePowerSupply(&ps_magnet,100,100);             // DPARKER this may be re-adjusted below
  ScalePowerSupply(&ps_thyr_reservoir_htr,100,100); // DPARKER this is not actually being used
  ScalePowerSupply(&ps_thyr_cathode_htr,100,100);   // DPARKER this is not actually being used

  EnableMagnetronMagnetSupply();
  EnableMagnetronFilamentSupply();
  HVLambdaStartCharging();  // DPARKER TMR1 and TMR2 must be initialized and ready to go before this call. DPARKER T1 Interrupt must also be ready to go
  _T1IE = 0;  // We don't want to enter the interrupt until we get to STATE_HV_ON

  PIN_UART2_TX = !PIN_UART2_TX;
  // Setup the ADC to read PAC and save to RAM as appropriate
  FastReadAndFilterPACInputs();
  PIN_UART2_TX = !PIN_UART2_TX;

#if !defined(__SET_MAGNETRON_OVER_SERIAL_INTERFACE)
  if (PIN_FP_SPARE_2_SAMPLE_VPROG_INPUT == ILL_SAMPLE_VPROG_INPUT) {
    vtemp = Scale16Bit(pac_1_adc_reading, DIRECT_LAMBDA_INPUT_SCALE);
    SetPowerSupplyTarget(&ps_hv_lambda_mode_A, vtemp, 0);
    
    vtemp = Scale16Bit(pac_2_adc_reading, DIRECT_LAMBDA_INPUT_SCALE);
    SetPowerSupplyTarget(&ps_hv_lambda_mode_B, vtemp, 0);
  }
#endif
  
  if (!ram_config_set_magnetron_magnet_current_from_GUI) {
    // The Magnet Current is calculated from Mode A program voltage
    itemp_2 = CalculatePoly(ps_hv_lambda_mode_A.v_command_set_point);
    vtemp_2 = GenerateMagnetVprog(itemp_2);
    SetPowerSupplyTarget(&ps_magnet, vtemp_2, itemp_2);
  }
    
  // DPARKER Calculate and Setup Magnetron Filament Power
  // DoMagnetronFilamentAdjust();  
  PIN_UART2_TX = !PIN_UART2_TX;
  UpdateDacAll();


  PIN_UART2_TX = !PIN_UART2_TX;
  FastReadAndFilterFeedbacks(); // DPARKER - Move this to as late as possible (want time to feedbacks to get as stable as possible before reading)
  PIN_UART2_TX = !PIN_UART2_TX;

  // DPARKER - convert filament voltage readback to the program that will the exact same value.  Scale the filament to that value.  Do all this before you program the DAC

  control_state = STATE_HV_ON;  //  Want to check the faults based on STATE_HV_ON
  UpdateFaults();
  control_state = STATE_FAST_RECOVERY_START_UP;
  
  // DPARKER - NO CONFIG/CHANGES to the I/O Expander for NOW
}




void DoA34760StartUpCommonPostProcess(void) {
  
  T2CONbits.TON = 1;
  T5CONbits.TON = 1;
  
  // ---- Configure the dsPIC ADC Module ------------ //
  ADCON1 = A34760_ADCON1_VALUE;             // Configure the high speed ADC module based on H file parameters
  ADCON2 = A34760_ADCON2_VALUE;             // Configure the high speed ADC module based on H file parameters
  ADCON3 = A34760_ADCON3_VALUE;             // Configure the high speed ADC module based on H file parameters
  ADCHS  = A34760_ADCHS_VALUE;              // Configure the high speed ADC module based on H file parameters

  ADPCFG = A34760_ADPCFG_VALUE;             // Set which pins are analog and which are digital I/O
  ADCSSL = A34760_ADCSSL_VALUE;             // Set which analog pins are scanned
  ADCON1bits.ADON = 1;
  _ADIF = 0;
  _ADIE = 1;
 
  
  // Begin UART operation
  _U1TXIF = 0;	// Clear the Transmit Interrupt Flag
  _U1TXIE = 1;	// Enable Transmit Interrupts
  _U1RXIF = 0;	// Clear the Recieve Interrupt Flag
  _U1RXIE = 1;	// Enable Recieve Interrupts
  
  U1MODEbits.UARTEN = 1;	// And turn the peripheral on
  PIN_RS422_DE = OLL_RS422_DE_ENABLE_RS422_DRIVER;  // Enable the U69-RS422 Driver output (The reciever is always enabled)
  command_string.data_state = COMMAND_BUFFER_EMPTY;  // The command buffer is empty
 
  ResetHWLatches();
  ClrWdt();
  // PIN_FAST_RESTART_STORAGE_CAP_OUTPUT = !OLL_DO_FAST_RESTART;  DPARKER THIS DOES NOT BELONG HERE

}





void SetPowerSupplyTarget(POWERSUPPLY* ptr_ps, unsigned int v_command, unsigned int i_command) {
  /*
    ptr_ps - Pointer to the power supply that is having it's set point updated
    v_command - the new voltage command Set point
    i_command - the new current command set point
  */

  
  if (v_command > ptr_ps->v_max_set_point) {
    ptr_ps->v_command_set_point = ptr_ps->v_max_set_point;
    global_debug_counter.setpoint_not_valid++;
  } else {
    ptr_ps->v_command_set_point = v_command;
  }
  
  if (i_command > ptr_ps->i_max_set_point) {
    ptr_ps->i_command_set_point = ptr_ps->i_max_set_point;
    global_debug_counter.setpoint_not_valid++;
  } else {
    ptr_ps->i_command_set_point = i_command;
  }
  
  if (control_state != STATE_WARM_UP) {
    ScalePowerSupply(ptr_ps,100,100);
  }
  CalcPowerSupplySettings(ptr_ps);  // DPARKER is this call redundant since ScalePowerSupply also calls ScalePowerSupply

  ClrWdt();

  // DPARKER need to figure out how to combine ScalePowerSupply & SetPowerSupplyTarget & CalcPowerSupplySettings
  
  
  // DPARKER Record Saturation Errors for each supply???
}



void ScalePowerSupply(POWERSUPPLY* ptr_ps, unsigned int num, unsigned int den) {

  unsigned long temp_32_bit;

  if (num >= den) {
    // Power supply can not be scalled above the set point.
    // Set the scaled_set_point to the command_set_point
    ptr_ps->v_scaled_set_point = ptr_ps->v_command_set_point;
    ptr_ps->i_scaled_set_point = ptr_ps->i_command_set_point;
  } else {
    // Scale v_set_point by num/den
    temp_32_bit = ptr_ps->v_command_set_point;
    temp_32_bit *= num;
    temp_32_bit /= den;
    ptr_ps->v_scaled_set_point = temp_32_bit & 0x0000FFFF;
    
    // Scale i_set_point by num/den
    temp_32_bit = ptr_ps->i_command_set_point;
    temp_32_bit *= num;
    temp_32_bit /= den;
    ptr_ps->i_scaled_set_point = temp_32_bit & 0x0000FFFF;

  }
  // Send these scaled values to the DAC and Under/Over setting registers
  CalcPowerSupplySettings(ptr_ps);
}



void CalcPowerSupplySettings(POWERSUPPLY* ptr_ps) {
  unsigned int v_temp;
  unsigned int i_temp;
  // This function generates DAC settings and ADC trip points based on the passed voltage and current set point
  
  // Generate the DAC Values associated with the set points
  v_temp = Scale16Bit(ptr_ps->v_scaled_set_point, ptr_ps->v_dac_scale);
  v_temp = Scale16Bit(v_temp, ptr_ps->v_dac_cal_gain);
  if (ptr_ps->v_dac_cal_offset > 0) {
    // the offset is positive check for overflow above 0xFFFF
    if ((0xFFFF-ptr_ps->v_dac_cal_offset) < v_temp) {
      v_temp = 0xFFFF;
    } else {
      v_temp = v_temp + ptr_ps->v_dac_cal_offset;
    }
  } else {
    // the offset is negative check for overflow below 0x0000
    if ((0-ptr_ps->v_dac_cal_offset) > v_temp) {
      v_temp = 0x0000;
    } else {
      v_temp = v_temp + ptr_ps->v_dac_cal_offset;
    }
  }
  ptr_ps->v_dac_set_point = v_temp;
  
  i_temp = Scale16Bit(ptr_ps->i_scaled_set_point, ptr_ps->i_dac_scale);
  i_temp = Scale16Bit(i_temp, ptr_ps->i_dac_cal_gain);
  if (ptr_ps->i_dac_cal_offset > 0) {
    // the offset is positive check for overflow above 0xFFFF
    if ((0xFFFF-ptr_ps->i_dac_cal_offset) < i_temp) {
      i_temp = 0xFFFF;
    } else {
      i_temp = i_temp + ptr_ps->i_dac_cal_offset;
    }
  } else {
    // the offset is negative check for overflow below 0x0000
    if ((0-ptr_ps->i_dac_cal_offset) > i_temp) {
      i_temp = 0x0000;
    } else {
      i_temp = i_temp + ptr_ps->i_dac_cal_offset;
    }
  }
  ptr_ps->i_dac_set_point = i_temp;
  
  
  // Set all of the over/under trip points based on the set point
  v_temp = Scale16Bit(ptr_ps->v_scaled_set_point, ptr_ps->v_adc_scale);
  v_temp = Scale16Bit(v_temp, ptr_ps->v_adc_cal_gain);
  if (ptr_ps->v_adc_cal_offset > 0) {
    // the offset is positive check for overflow above 0xFFFF
    if ((0xFFFF-ptr_ps->v_adc_cal_offset) < v_temp) {
      v_temp = 0xFFFF;
    } else {
      v_temp = v_temp + ptr_ps->v_adc_cal_offset;
    }
  } else {
    // the offset is negative check for overflow below 0x0000
    if ((0-ptr_ps->v_adc_cal_offset) > v_temp) {
      v_temp = 0x0000;
    } else {
      v_temp = v_temp + ptr_ps->v_adc_cal_offset;
    }
  }

  ptr_ps->v_adc_over_rel = Scale16Bit(v_temp, ptr_ps->v_adc_over_scale);
  ptr_ps->v_adc_under_rel = Scale16Bit(v_temp,ptr_ps-> v_adc_under_scale);
  if (ptr_ps->v_adc_over_rel < ptr_ps->v_adc_over_min_value) {
    ptr_ps->v_adc_over_rel = ptr_ps->v_adc_over_min_value;
  }
  
  i_temp = Scale16Bit(ptr_ps->i_scaled_set_point, ptr_ps->i_adc_scale);
  i_temp = Scale16Bit(i_temp, ptr_ps->i_adc_cal_gain);
  if (ptr_ps->i_adc_cal_offset > 0) {
    // the offset is positive check for overflow above 0xFFFF
    if ((0xFFFF-ptr_ps->i_adc_cal_offset) < i_temp) {
      i_temp = 0xFFFF;
    } else {
      i_temp = i_temp + ptr_ps->i_dac_cal_offset;
    }
  } else {
    // the offset is negative check for overflow below 0x0000
    if ((0-ptr_ps->i_adc_cal_offset) > i_temp) {
      i_temp = 0x0000;
    } else {
      i_temp = i_temp + ptr_ps->i_adc_cal_offset;
    }
  }

  /*
  //  DPARKER NEED TO ADJUST THE OVER/UNDER SCALE SUCH THAT IT IS SET TO X Percent of MAX VALUE.  NOT PROGRAMED VALUE
  //  This will overwrite the values calculated above
  
  
  #define DPARKER_SCALE_5_PERCENT (410)  // This will be 5%
  dan_temp = Scale16Bit(ptr_ps->v_max_set_point, DPARKER_SCALE_5_PERCENT);
  
  ptr_ps->v_adc_over_rel = v_temp + dan_temp;

  if (v_temp > dan_temp) {
    ptr_ps->v_adc_under_rel = v_temp - dan_temp;
  } else {
    ptr_ps->v_adc_under_rel = 0;
  }
  
					       
  // End New code to adjust the over scaling

  */




  
  ptr_ps->i_adc_over_rel = Scale16Bit(i_temp, ptr_ps->i_adc_over_scale);
  ptr_ps->i_adc_under_rel = Scale16Bit(i_temp, ptr_ps->i_adc_under_scale);
  if (ptr_ps->i_adc_over_rel < ptr_ps->i_adc_over_min_value) {
    ptr_ps->i_adc_over_rel = ptr_ps->i_adc_over_min_value;
  }
  ClrWdt();
}




void UpdateDacAll(void) {
  unsigned int dac_value_array[8];
  unsigned char dac_write_failed;
  unsigned char number_tries;

  last_known_action = LAST_ACTION_UPDATE_DAC_ALL;

  dac_value_array[0] = ps_thyr_reservoir_htr.v_dac_set_point;
  dac_value_array[1] = ps_thyr_cathode_htr.v_dac_set_point;
  dac_value_array[2] = ps_magnet.v_dac_set_point;
  dac_value_array[3] = ps_magnet.i_dac_set_point;
  dac_value_array[4] = ps_filament.v_dac_set_point;
  dac_value_array[5] = ps_filament.i_dac_set_point;
  dac_value_array[6] = ps_hv_lambda_mode_A.v_dac_set_point;
  dac_value_array[7] = ps_hv_lambda_mode_B.v_dac_set_point;

  dac_write_failed = 1;
  number_tries = 0;
  while ((dac_write_failed != 0) & (number_tries < 20)) {
    dac_write_failed = WriteLTC2656AllDacChannels(&U44_LTC2656, dac_value_array);
    number_tries++;
    if (dac_write_failed) {
      global_debug_counter.LTC2656_write_error++;
    }
    ClrWdt();
  }
  if (number_tries >= 20) {
    RecordThisControlBoardFault(FAULT_CB_SPI_UNRECOVERABLE_ERROR);
  }
}




void Do10msTicToc(void) {
  /*
    Certain functions need to happen at regular interval for the system to work

    Thyratron PIDs - The gain and phase of the PID control loop is a function of it's execution frequency therefor it must be updated at a regular interval
    Analog Filters - The filter response is function of the execution frequency so they must be executed at a regular interval

    DAC updates - The DAC must be regularly.  Durring HV ON this should happen AFTER a pulse so that the SPI bus is not corrupted by EMI
    If the state is not in HV_ON or the system is pulsing at a very low freqeuncy, DAC updates must be handeled by this function.
    
    Calculating the PRF

    Other timing functions like flashing LEDs
  */

  unsigned int vtemp;
  unsigned int itemp;
  unsigned long long_math_value;
  unsigned long temp32;

  last_known_action = LAST_ACTION_DO_10MS;
  
  if (_POR) {
    debug_status_register |= STATUS_POR_RESET;
    // _POR = 0;
  }
  if (_EXTR) {
    debug_status_register |= STATUS_EXTERNAL_RESET;
    //_EXTR = 0;
  }
  if (_SWR) {
    debug_status_register |= STATUS_SOFTARE_RESET;
    //_SWR = 0;
  }
  if (_BOR) {
    debug_status_register |= STATUS_BOR_RESET;
    //_BOR = 0;
  }
  if (_TRAPR) {
    debug_status_register |= STATUS_TRAPR_RESET;
    //_TRAPR = 0;
  }
  if (_WDTO) {
    debug_status_register |= STATUS_WDT_RESET;
    // _WDTO = 0;
  }
  if (_IOPUWR) {
    debug_status_register |= STATUS_IOPUWR_RESET;
    //_IOPUWR = 0;
  }


  ClrWdt();
  if (_T5IF) {
    _T5IF = 0;
    //10ms roll has occured
    
    led_pulse_count = ((led_pulse_count + 1) & 0b00001111);
    if (led_pulse_count == 0) {
      // 10ms * 16 counter has ocurred
      // Flash the LED - NOTE "PIN_MAIN_CONTACTOR_CLOSE = !PIN_MAIN_CONTACTOR_CLOSE" was causing any changes made in Port F durring interrupt to be overwritten
      if (PIN_MAIN_CONTACTOR_CLOSE) {
	PIN_MAIN_CONTACTOR_CLOSE = 0;
      } else {
	PIN_MAIN_CONTACTOR_CLOSE = 1;
      }  
      
      average_pulse_repetition_frequency_deci_herz = RCFilter16Tau(average_pulse_repetition_frequency_deci_herz, ((prf_pulse_counter*125)>>1));
      prf_pulse_counter = 0;  
    }
    
    temp32 = 1562500;
    if (last_period < 345) {
      last_period = 345;
    }
    temp32 /= last_period;
    prf_deciherz = temp32;
    if (_T3IF) {
      // We are pulsing at less than 2.5Hz
      // Set the rep rate to zero
      prf_deciherz = 0;
    }
    
    
    if ((PIN_FP_MODULATOR_RESET == !ILL_MODULATOR_RESET) && (start_reset_process)) {
      ResetAllFaults();
      ResetHWLatches(); // Clear the hardware Latches
      ResetPulseLatches();
    }
    
    if (PIN_FP_MODULATOR_RESET == ILL_MODULATOR_RESET) {
      // Start the Reset Process
      start_reset_process = 1;
    } else {
      start_reset_process = 0;
    }
        
    // ResetSPI1(); // ResetSPI(2) // DPARKER - This may be requried to fix bit errors in the SPI bus
    
    
    FilterADCs();  // Read Data from the DAC arrays, average, and filter
    
    // If the magnet current is based on mode A program voltage (instead of from GUI), set the magnet current based on mode A program voltage
    if (!ram_config_set_magnetron_magnet_current_from_GUI) {
      // The Magnet Current is calculated from Mode A program voltage
      itemp = CalculatePoly(ps_hv_lambda_mode_A.v_command_set_point);
      vtemp = GenerateMagnetVprog(itemp);
      SetPowerSupplyTarget(&ps_magnet, vtemp, itemp);
    }
  
    if ((control_state == STATE_SYSTEM_COLD_READY) || (control_state == STATE_FAULT_COLD_FAULT)) {
      ScalePowerSupply(&ps_hv_lambda_mode_A,0,100);
      ScalePowerSupply(&ps_hv_lambda_mode_A,0,100);
      ScalePowerSupply(&ps_filament,0,100);
      ScalePowerSupply(&ps_magnet,0,100);
      ScalePowerSupply(&ps_thyr_reservoir_htr,0,100);
      ScalePowerSupply(&ps_thyr_cathode_htr,0,100);
    }




    UpdateFaults();  // Update all the fault registers.  Note this must only happen once every 10ms because some faults are timed
    //  DPARKER figure out some better way to timer certain faults
  
    // Update the sum fault outputs (copper and fiber)
    if (CheckFaultActive()) {
      PIN_SUM_FAULT_FIBER = OLL_SUM_FAULT_FIBER_FAULT;
      PIN_SUM_FAULT_COPPER = OLL_SUM_FAULT_COPPER_FAULT;
    } else {
      PIN_SUM_FAULT_FIBER = !OLL_SUM_FAULT_FIBER_FAULT;
      PIN_SUM_FAULT_COPPER = !OLL_SUM_FAULT_COPPER_FAULT;
    }
    
    slow_down_thyratron_pid_counter++;
    slow_down_thyratron_pid_counter &= 0b00001111;
    if (slow_down_thyratron_pid_counter == 0) {
      //execute PID loop once every 160ms
      DoThyratronPIDs();
    }
    

    // The system is configured to adjust the filament power based on magnetron power.  Otherwise the filament power will be maxed at all times
    DoMagnetronFilamentAdjust();



    if ((control_state != STATE_HV_ON) || (_T2IF)) {
      // Do10msTicToc needs to be responsible for updating the DAC
      UpdateDacAll();
      UpdateIOExpanderOutputs();
      pulse_counter_this_run = 0;
    } 
    if (pulse_counter_this_run == 0) {
      if (pulse_off_time_10_ms_units < low_energy_target_current_startup_max_cooldown) {
	pulse_off_time_10_ms_units++;
      }
      long_math_value = max_low_energy_target_current_startup_adjust_initital_value;
      long_math_value *= pulse_off_time_10_ms_units;
      long_math_value /= low_energy_target_current_startup_max_cooldown;
      low_energy_target_current_startup_adjust_initital_value = long_math_value;
    } else {
      pulse_off_time_10_ms_units = 0;
    }
  } 
}





void DoThyratronPIDs(void) {
  /*
    The pid functions provided by dsp.h use Q15 fractional data.
    This represents fractional inputs/outputs from -1 to 1.
    Our inputs to the PID come from an unipolar ADC.
    The control output of the PID goes directly to a DAC.
    In order for the PID control to work with our unsigned 16 bit data two changed must be made.
    (1) Measured output must be converted from 16 bit data to Q(15).  This is easily done by shifting one bit to the right.
        From to PIDs point of view we are just decreasing our resolution by 1 bit, not a problem here.
    (2) Control output must be convereted so that it can iterface to our DAC.
        The control output ranges from -1 to 1.  so perform the following logic.
	If (control_output < 0) then (control_output = 0).
	Multiply control_output by 2 (shift left 1 bit) to use the full range of the DAC.
	NOTE: You must perform the math like this we because the PID will set the output to Zero at start.
	If you shift the entire 16 bits to unsinged interger, 0 will become 0x7FFF and your control_output will start at 50%, not zero!!!!
    (3) In order to set the DAC the control output "is moved" to v_dac_set_point register by setting v_dac_scale to zero, v_dac_cal_gain to zero
        and setting v_dac_cal_offset to 0x1000 "zero level" + the output of the PID loop (which ranges from zero to 0xFFFF)
    (4) If the DAC setting exceeds an output program of 2V (32000 or 0x7D00) , then we have saturated the output.  Throw some faults
  */

  unsigned int temp;

  last_known_action = LAST_ACTION_DO_THYRATRON_PID;

  // Run the Thyratron Heaters PID loops on the latest data
  // bit shift the data 1 bit to the right because the PID loop is expecting Q15
  temp = Scale16Bit(ps_thyr_reservoir_htr.v_scaled_set_point, ps_thyr_reservoir_htr.v_adc_scale);
  temp = Scale16Bit(temp, ps_thyr_reservoir_htr.v_adc_cal_gain);
  temp = temp + ps_thyr_reservoir_htr.v_adc_cal_offset;
  thyratron_reservoir_heater_PID.controlReference = (temp >> 1);
  thyratron_reservoir_heater_PID.measuredOutput = (ps_thyr_reservoir_htr.v_adc_reading >> 1);
  PID(&thyratron_reservoir_heater_PID);


  temp = Scale16Bit(ps_thyr_cathode_htr.v_scaled_set_point, ps_thyr_cathode_htr.v_adc_scale);
  temp = Scale16Bit(temp, ps_thyr_cathode_htr.v_adc_cal_gain);
  temp = temp + ps_thyr_cathode_htr.v_adc_cal_offset;
  thyratron_cathode_heater_PID.controlReference = (temp >> 1);
  thyratron_cathode_heater_PID.measuredOutput = (ps_thyr_cathode_htr.v_adc_reading >> 1);
  PID(&thyratron_cathode_heater_PID);
  

  // Update the DAC setting for the Reservoir Heater
  // The output of the PID is Q15, convert to 16 bit unsigned for the DAC
  temp = thyratron_reservoir_heater_PID.controlOutput;
  if (temp & 0x8000) {
    temp = 0x0000;
  }
  temp = temp << 1;
  temp += 0;
  if (temp > THYRATRON_DAC_SATURATED) {
    // The dac output will exceed 2V (20mA from the 4-20mA driver).  Limit the output to 0x7D00
    temp = THYRATRON_DAC_SATURATED;
  }
  ps_thyr_reservoir_htr.v_dac_cal_gain = 0;
  ps_thyr_reservoir_htr.v_dac_scale = 0;
  ps_thyr_reservoir_htr.v_dac_cal_offset = temp;
  
  
  // Update the DAC setting for the Cathode Heater
  // The output of the PID is Q15, convert to 16 bit unsigned for the DAC
  temp = thyratron_cathode_heater_PID.controlOutput;
  if (temp & 0x8000) {
    temp = 0x0000;
  }
  temp = temp << 1;
  temp += 0; 
  if (temp > THYRATRON_DAC_SATURATED) {
    // The dac output will exceed 2V (20mA from the 4-20mA driver).  Limit the output to 0x7D00
    // DPARKER how are thyratron dac saturations detected in faults.c - confirm this works
    temp = THYRATRON_DAC_SATURATED;
  }
  ps_thyr_cathode_htr.v_dac_cal_gain = 0;
  ps_thyr_cathode_htr.v_dac_scale = 0;
  ps_thyr_cathode_htr.v_dac_cal_offset = temp;

  CalcPowerSupplySettings(&ps_thyr_cathode_htr);
  CalcPowerSupplySettings(&ps_thyr_reservoir_htr);
}


void DoMagnetronFilamentAdjust(void) {
  unsigned long temp32;
  unsigned int look_up_position;
  unsigned int filament_scale;
  signed int new_position;
  
  temp32 = average_energy_per_pulse_milli_joules;
  //temp32 *= average_pulse_repetition_frequency_deci_herz;
  temp32 *= prf_deciherz;
  temp32 >>= 6;
  temp32 *= 13;
  temp32 >>= 11;

  // DPARKER this will not work at powers greater than 6.5KW because the apparent power will roll around.  This is very bad
  // Need to truncate for (average output power watts) and properly handel look up position.
  if (temp32 >= 0xFFFF) {
    average_output_power_watts = 0xFFFF;
  } else {
    average_output_power_watts = (temp32 & 0xFFFF);
  }
  temp32 >>= 7;
  look_up_position = (temp32 & 0b00111111);
  new_position = (signed int)look_up_position;
  new_position += look_up_offset;
  if (new_position <= 0) {
    new_position = 0;
  }
  if (new_position >= 63) {
    new_position = 63;
  }
  look_up_position = new_position;
  look_up_position &= 0b00111111;

  if ((control_state == STATE_HV_ON) || (control_state == STATE_SYSTEM_WARM_READY)  || (control_state == STATE_HV_STARTUP) || (control_state == STATE_FAULT_WARM_FAULT)) {
    filament_scale = FilamentLookUpTable[look_up_position];
    ScalePowerSupply(&ps_filament,filament_scale,100);
  }  
}



void ReadIsolatedAdcToRam(void) {
  /*
    This function reads back data from the external AD7686.
    When this function is called, the data has already been sampled, and the conversion is complete.
    The convert pin to the two AD7686 should be active

    DO NOT COPY THIS FUNCTION TO OTHER APPLICATIONS - IT WILL NOT WORK BECAUSE THIS DOES NOT INCLUDE TO CODE TO SAMPLE THE INPUTS
  */

  // reads data into global variable 
  unsigned long adc_read;


  last_known_action = LAST_ACTION_READ_ISOLATED_ADC;

  if (PIN_PULSE_ADC_TRIGGER == OLL_TRIGGER_PULSE_ADCS) {
    // The convert signal is asserted like it should be


    // ----------  READ BACK THE CURRENT ADC CONVERSION ------------------------- //
    PIN_PULSE_IMON_CS = OLL_SELECT_PULSE_IMON_ADC;   // Select the Current Monitor ADC
    __delay32(DELAY_PULSE_CABLE_SELECT_PROP_DELAY);        // Wait for the cable select signal to propagate
    adc_read = SendAndReceiveSPI(0, SPI_PORT_2);     // ReadData 16 bit value off of the Current Monitor ADC
    if (adc_read == 0x11110000) {
      // THERE WAS an SPI ERROR
      pulse_magnetron_current_adc_reading = 0;
      global_debug_counter.spi2_bus_error++;
    } else {
      pulse_magnetron_current_adc_reading = adc_read & 0xFFFF;
    }
    PIN_PULSE_IMON_CS = !OLL_SELECT_PULSE_IMON_ADC;  // De - Select the Current Monitor ADC  
    __delay32(DELAY_PULSE_CABLE_SELECT_PROP_DELAY);        // Wait for the cable select signal to propagate
    
    

    // ----------  READ BACK THE VOLTAGE ADC CONVERSION ------------------------- //
    PIN_PULSE_VMON_CS = OLL_SELECT_PULSE_VMON_ADC;   // Select the Voltage Monitor ADC
    __delay32(DELAY_PULSE_CABLE_SELECT_PROP_DELAY);      // Wait for the cable select signal to propagate
    adc_read = SendAndReceiveSPI(0, SPI_PORT_2);     // ReadData 16 bit value off of the Voltage Monitor ADC
    if (adc_read == 0x11110000) {
      // THERE WAS an SPI ERROR
      pulse_magnetron_voltage_adc_reading = 0;
      global_debug_counter.spi2_bus_error++;
    } else {
      pulse_magnetron_voltage_adc_reading = adc_read & 0xFFFF;
    }
    PIN_PULSE_VMON_CS = !OLL_SELECT_PULSE_VMON_ADC;  // De - Select the Current Monitor ADC  
    __delay32(DELAY_PULSE_CABLE_SELECT_PROP_DELAY);        // Wait for the cable select signal to propagate
  
  } else {
    // This function was called when the ADC convert signal was not asserted.  Something very bad happened 
    pulse_magnetron_voltage_adc_reading = 0;
    pulse_magnetron_current_adc_reading = 0;
    global_debug_counter.external_adc_false_trigger++;
  }
  PIN_PULSE_ADC_TRIGGER = !OLL_TRIGGER_PULSE_ADCS;   // Clear the convert trigger signal
} 




void FilterADCs(void) {
  /*
    This function should be called once every 10mS from Do10msTicToc()
    The ADC value array is averaged and the the result of that average is low pass filtered.

    From Internal DAC
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
  
  unsigned int adc_reading;
#if !defined(__SET_MAGNETRON_OVER_SERIAL_INTERFACE)
  unsigned int vtemp;
#endif

  last_known_action = LAST_ACTION_FILTER_ADC;

  // AN3 -  PFN Rev Current           - Only Sampled after a pulse
  // PFN Rev Current is read after a pulse

  //AN4 - pac_#1                     - 256 samples/tau - Analog Input Bandwidth = 200 Hz  
  adc_reading = AverageADC128(pac_1_array);
  pac_1_adc_reading = RCFilter16Tau(pac_1_adc_reading, adc_reading);


  //AN5 - pac_#2                     - 256 samples/tau - Analog Input Bandwidth = 200 Hz
  adc_reading = AverageADC128(pac_2_array);
  pac_2_adc_reading = RCFilter16Tau(pac_2_adc_reading, adc_reading);
  

  if (PIN_FP_SPARE_2_SAMPLE_VPROG_INPUT == ILL_SAMPLE_VPROG_INPUT) {
    default_pac_2_adc_reading = pac_2_adc_reading;
  }


  //AN6 - Thyratron Cathode Heater   - 16 samples/tau - Analog Input Bandwidth = 10 Hz
  //adc_reading = AverageADC128(thyratron_cathode_heater_voltage_array);
  //ps_thyr_cathode_htr.v_adc_reading = RCFilter16Tau(ps_thyr_cathode_htr.v_adc_reading, adc_reading);


  //AN7 - Thyratron Reservoir Heater - 16 samples/tau - Analog Input Bandwidth = 10 Hz
  //adc_reading = AverageADC128(thyratron_reservoir_heater_voltage_array);
  //ps_thyr_reservoir_htr.v_adc_reading = RCFilter16Tau(ps_thyr_reservoir_htr.v_adc_reading, adc_reading);


  //AN8  - magnet_current            - 16 samples/tau - Analog Input Bandwidth = 200 Hz  
  adc_reading = AverageADC128(magnetron_magnet_current_array);
  ps_magnet.i_adc_reading = RCFilter16Tau(ps_magnet.i_adc_reading, adc_reading);

  //AN9  - magnet_voltage            - 16 samples/tau - Analog Input Bandwidth = 200 Hz
  adc_reading = AverageADC128(magnetron_magnet_voltage_array);
  ps_magnet.v_adc_reading = RCFilter16Tau(ps_magnet.v_adc_reading, adc_reading);
 
  //AN10 - filament_voltage          - 16 samples/tau - Analog Input Bandwidth = 200 Hz    
  adc_reading = AverageADC128(magnetron_filament_voltage_array);
  ps_filament.v_adc_reading = RCFilter16Tau(ps_filament.v_adc_reading, adc_reading);

  //AN11 - filament_current          - 16 samples/tau - Analog Input Bandwidth = 200 Hz    
  adc_reading = AverageADC128(magnetron_filament_current_array);
  ps_filament.i_adc_reading = RCFilter16Tau(ps_filament.i_adc_reading, adc_reading);

  //AN12 - lambda_vpeak              - 64 samples/tau - Analog Input Bandwidth = 200 Hz
  adc_reading = AverageADC128(lambda_vpeak_array);
  ps_hv_lambda_mode_A.v_adc_reading = RCFilter16Tau(ps_hv_lambda_mode_A.v_adc_reading, adc_reading);

  //AN13 - lambda_vmon               - not averaged / filtered  - Analog Input Bandwidth = 200 Hz
  adc_reading = AverageADC128(lambda_vmon_array);
  ps_hv_lambda_mode_B.v_adc_reading = RCFilter16Tau(ps_hv_lambda_mode_B.v_adc_reading, adc_reading);
  // lambda_vmon is read at EOC
  
}





void FastReadAndFilterFeedbacks(void) {
  /*
    This function should be called once durring fast restart.
    The ADC value array is populated, averaged and the the result of that average used to set initial condition for RC Filter.

    From Internal DAC
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


  unsigned int index;
  unsigned int n;
  unsigned int adc_reading;
  
  // Configure ADC for this particular operation
  ADCON1 = A34760_ADCON1_VALUE;
  ADCON2 = (ADC_VREF_AVDD_AVSS & ADC_SCAN_ON & ADC_SAMPLES_PER_INT_8 & ADC_ALT_BUF_ON & ADC_ALT_INPUT_OFF);
  ADCON3 = A34760_ADCON3_VALUE;
  ADCHS  = A34760_ADCHS_VALUE;
  ADPCFG = A34760_ADPCFG_VALUE;
  ADCSSL = (SKIP_SCAN_AN0 & SKIP_SCAN_AN1 & SKIP_SCAN_AN2 & SKIP_SCAN_AN3 & SKIP_SCAN_AN4 & SKIP_SCAN_AN5 & SKIP_SCAN_AN14 & SKIP_SCAN_AN15);
  ADCON1bits.ADON = 1;

  index = 0;
  while (index < 128) {
    while(!_BUFS);
    // Copy each reading into 8 data readings 
    for (n = 0; n < 8; n++) {
      thyratron_cathode_heater_voltage_array[index] = ADCBUF0;
      thyratron_reservoir_heater_voltage_array[index] = ADCBUF1;
      magnetron_magnet_current_array[index] = ADCBUF2;
      magnetron_magnet_voltage_array[index] = ADCBUF3;
      magnetron_filament_current_array[index] = ADCBUF4;
      magnetron_filament_voltage_array[index] = ADCBUF5;
      lambda_vpeak_array[index] = ADCBUF6;
      lambda_vmon_array[index++] = ADCBUF7;
    }
    while(_BUFS);
    // Copy each reading into 8 data readings 
    for (n = 0; n < 8; n++) {
      thyratron_cathode_heater_voltage_array[index] = ADCBUF8;
      thyratron_reservoir_heater_voltage_array[index] = ADCBUF9;
      magnetron_magnet_current_array[index] = ADCBUFA;
      magnetron_magnet_voltage_array[index] = ADCBUFB;
      magnetron_filament_current_array[index] = ADCBUFC;
      magnetron_filament_voltage_array[index] = ADCBUFD;
      lambda_vpeak_array[index] = ADCBUFE;
      lambda_vmon_array[index++] = ADCBUFF;
    }
  }
  ADCON1bits.ADON = 0;

  //AN6 - Thyratron Cathode Heater   - 16 samples/tau - Analog Input Bandwidth = 10 Hz
  adc_reading = AverageADC128(thyratron_cathode_heater_voltage_array);
  ps_thyr_cathode_htr.v_adc_reading = adc_reading;

  //AN7 - Thyratron Reservoir Heater - 16 samples/tau - Analog Input Bandwidth = 10 Hz
  adc_reading = AverageADC128(thyratron_reservoir_heater_voltage_array);
  ps_thyr_reservoir_htr.v_adc_reading = adc_reading;

  //AN8  - magnet_current            - 16 samples/tau - Analog Input Bandwidth = 200 Hz  
  adc_reading = AverageADC128(magnetron_magnet_current_array);
  ps_magnet.i_adc_reading = adc_reading;

  //AN9  - magnet_voltage            - 16 samples/tau - Analog Input Bandwidth = 200 Hz
  adc_reading = AverageADC128(magnetron_magnet_voltage_array);
  ps_magnet.v_adc_reading = adc_reading;
 
  //AN10 - filament_voltage          - 16 samples/tau - Analog Input Bandwidth = 200 Hz    
  adc_reading = AverageADC128(magnetron_filament_voltage_array);
  ps_filament.v_adc_reading = adc_reading;

  //AN11 - filament_current          - 16 samples/tau - Analog Input Bandwidth = 200 Hz    
  adc_reading = AverageADC128(magnetron_filament_current_array);
  ps_filament.i_adc_reading = adc_reading;

  //AN12 - lambda_vpeak              - 64 samples/tau - Analog Input Bandwidth = 200 Hz
  adc_reading = AverageADC128(lambda_vpeak_array);
  ps_hv_lambda_mode_A.v_adc_reading = adc_reading;

  //AN13 - lambda_vmon               - not averaged / filtered  - Analog Input Bandwidth = 200 Hz
  adc_reading = AverageADC128(lambda_vmon_array);
  ps_hv_lambda_mode_B.v_adc_reading = adc_reading;

}




void FastReadAndFilterPACInputs(void) {
  unsigned int index;
  unsigned int n;
  unsigned int adc_reading;
  
  // Configure ADC for this particular operation
  ADCON1 = A34760_ADCON1_VALUE;
  ADCON2 = (ADC_VREF_AVDD_AVSS & ADC_SCAN_ON & ADC_SAMPLES_PER_INT_8 & ADC_ALT_BUF_ON & ADC_ALT_INPUT_OFF);
  ADCON3 = A34760_ADCON3_VALUE;
  ADCHS  = A34760_ADCHS_VALUE;
  ADPCFG = A34760_ADPCFG_VALUE;
  ADCSSL = (SKIP_SCAN_AN0 & SKIP_SCAN_AN1 & SKIP_SCAN_AN2 & SKIP_SCAN_AN3 & SKIP_SCAN_AN6 & SKIP_SCAN_AN7 & SKIP_SCAN_AN8 & SKIP_SCAN_AN9 & SKIP_SCAN_AN10 & SKIP_SCAN_AN11 & SKIP_SCAN_AN12 & SKIP_SCAN_AN13 & SKIP_SCAN_AN14 & SKIP_SCAN_AN15);
  ADCON1bits.ADON = 1;

  
  // Repeate the sample sequence until we have filled up the pac arrays
  index = 0;
  while (index < 128) {
    // wait for buffer 0-8 to fill up
    while(!_BUFS);
    for (n = 0; n < 4; n++) {
      pac_1_array[index] = ADCBUF0;
      pac_2_array[index++] = ADCBUF1;
      pac_1_array[index] = ADCBUF2;
      pac_2_array[index++] = ADCBUF3;
      pac_1_array[index] = ADCBUF4;
      pac_2_array[index++] = ADCBUF5;
      pac_1_array[index] = ADCBUF6;
      pac_2_array[index++] = ADCBUF7;  
    }
    
    // wait for buffers 9-F to fill up
    while(_BUFS);
    for (n = 0; n < 4; n++) {
      pac_1_array[index] = ADCBUF8;
      pac_2_array[index++] = ADCBUF9;
      pac_1_array[index] = ADCBUFA;
      pac_2_array[index++] = ADCBUFB;
      pac_1_array[index] = ADCBUFC;
      pac_2_array[index++] = ADCBUFD;
      pac_1_array[index] = ADCBUFE;
      pac_2_array[index++] = ADCBUFF;  
    }    
  }
  ADCON1bits.ADON = 0;
  

  // Average Pac Array and Populte the RC Filter
  // This is just copied from Filter ADCs
  adc_reading = AverageADC128(pac_1_array);
  pac_1_adc_reading = adc_reading;
  
  adc_reading = AverageADC128(pac_2_array);
  pac_2_adc_reading = adc_reading;
}






void ExitHvOnState(void) {
  // this subroutine is called to properly shut down the HV on state
  // This required interrupts to be disabled/cleared/ect
  
  SET_CPU_IPL(7); // Disable all interrupts
  Nop();
  Nop();
  Nop();
  Nop();
  _T1IE = 0;      // Disable the T1 interrupt.  The T1 interrupt enables INT1 Interrupt
  _INT1IE = 0;    // Disable the INT1 Interrupt.  INT1 enables T1 Interrupt.
  SET_CPU_IPL(0); // Enable all interrupts
  SavePulseCountersToEEPROM();
  PIN_FAST_RESTART_STORAGE_CAP_OUTPUT = !OLL_DO_FAST_RESTART;
}

void SavePulseCountersToEEPROM(void) {
  unsigned int *unsigned_int_ptr;
  unsigned_int_ptr = &pulse_counter_persistent;              //unsigned_int_ptr now points to the Least Significant word of pulse_counter_persistent
  pulse_counter_repository_ram_copy[3] = *unsigned_int_ptr; 
  unsigned_int_ptr++;                                        //unsigned_int_ptr now points to the next lest signigicant word of pulse_counter_persistent
  pulse_counter_repository_ram_copy[2] = *unsigned_int_ptr;
  unsigned_int_ptr++;                                        //unsigned_int_ptr now points to the next lest signigicant word of pulse_counter_persistent
  pulse_counter_repository_ram_copy[1] = *unsigned_int_ptr;
  unsigned_int_ptr++;                                        //unsigned_int_ptr now points to the most signigicant word of pulse_counter_persistent
  pulse_counter_repository_ram_copy[0] = *unsigned_int_ptr;
  
  
  unsigned_int_ptr = &arc_counter_persistent;                //unsigned_int_ptr now points to the Least Significant word of arc_counter_persistent
  pulse_counter_repository_ram_copy[5] = *unsigned_int_ptr;
  unsigned_int_ptr++;                                        //unsigned_int_ptr now points to the most signigicant word of arc_counter_persistent
  pulse_counter_repository_ram_copy[4] = *unsigned_int_ptr;

  // The values have now been saved to the array in RAM.  Save the RAM data to EEPROM
  _wait_eedata();
  _erase_eedata(EE_address_pulse_counter_repository_in_EEPROM, _EE_ROW);
  _wait_eedata();
  _write_eedata_row(EE_address_pulse_counter_repository_in_EEPROM, pulse_counter_repository_ram_copy);
}


void DoColdShutDown(void) {
  /*
    This is a function to safely move from any state to cold state

    The following actions occur
    * The HV Lambda is disabled
    * The Filament supply is disabled
    * The magnet supply is disabled
    * The thyratron DACs are set to Zero //DPARKER how to do with without PID taking over
    
    // DPARKER when do we write values from EEPROM to RAM
    */
  DisableHVLambdaSupply();
  DisableMagnetronFilamentSupply();
  DisableMagnetronMagnetSupply();
  ScalePowerSupply(&ps_hv_lambda_mode_A,0,100);
  ScalePowerSupply(&ps_hv_lambda_mode_A,0,100);
  ScalePowerSupply(&ps_filament,0,100);
  ScalePowerSupply(&ps_magnet,0,100);
  ScalePowerSupply(&ps_thyr_reservoir_htr,0,100);
  ScalePowerSupply(&ps_thyr_cathode_htr,0,100);

  //DPARKER need to do more to shut down the PID loops
  PIDInit(&thyratron_reservoir_heater_PID);
  PIDInit(&thyratron_cathode_heater_PID);

  T1CONbits.TON = 0;
  _INT1IE = 0;
  _T1IE = 0;
  _INT1IF = 0;
  _T1IF = 0;
  PIN_THYRATRON_TRIGGER_ENABLE = !OLL_THYRATRON_TRIGGER_ENABLED;
}


void DoWarmShutDown(void) {
  /*
    This function is called to safely move from any warm/HV on state to, warm state

    The Following Actions occur
    * The HV Lambda is disabled
    * Clean up Interupts associated with pulsing
    
    // DPARKER when do we write values from EEPROM TO RAM???
    */
  DisableHVLambdaSupply();
  T1CONbits.TON = 0;
  _INT1IE = 0;
  _T1IE = 0;
  _INT1IF = 0;
  _T1IF = 0;
  PIN_THYRATRON_TRIGGER_ENABLE = !OLL_THYRATRON_TRIGGER_ENABLED;
}




void StartWarmUp(void) {
  /*
    This subroutine is called at the start of the warmup process.
    It enables the heater and magnet supplies at zero and starts the ramp process
    
    + Set all outputs to Zero
    + Setup TMR2 for 100ms Intervals
  */  

  PIDInit(&thyratron_reservoir_heater_PID);
  PIDInit(&thyratron_cathode_heater_PID);

  ScalePowerSupply(&ps_magnet, 0, 100);
  ScalePowerSupply(&ps_filament, 0, 100);
  ScalePowerSupply(&ps_thyr_cathode_htr, 0, 100);
  ScalePowerSupply(&ps_thyr_reservoir_htr, 0, 100);

  EnableMagnetronMagnetSupply();
  EnableMagnetronFilamentSupply();

  TMR2 = 0;
  _T2IF = 0;
}




void EnableMagnetronFilamentSupply(void) {
  PIN_MAGNETRON_FILAMENT_ENABLE = OLL_MAGNETRON_FILAMENT_ENABLED;
}

void DisableMagnetronFilamentSupply(void) {
  PIN_MAGNETRON_FILAMENT_ENABLE = !OLL_MAGNETRON_FILAMENT_ENABLED;
}

void EnableMagnetronMagnetSupply(void) {
  PIN_MAGNETRON_MAGNET_ENABLE = OLL_MAGNETRON_MAGNET_ENABLED;
}

void DisableMagnetronMagnetSupply(void) {
  PIN_MAGNETRON_MAGNET_ENABLE = !OLL_MAGNETRON_MAGNET_ENABLED;
}


void DisableHVLambdaSupply(void) {
  PIN_HV_LAMBDA_ENABLE = !OLL_HV_LAMBDA_ENABLED;
  PIN_HV_LAMBDA_INHIBIT = OLL_HV_LAMBDA_INHIBITED;
}


void UpdateIOExpanderOutputs(void) {
  // This updates all of the LED/Digital Outputs From the Control Board
  // See 34-34357 for the configuration of the output polarities

  // All of the outputs are "Active Low"
  // For Faults - The Fault state is "NOT ACTIVE" or a "high output"
  // FOr HV ON - HV On is the "Not Active" state or "high otuput"

  unsigned int temp_lat_a;
  unsigned int temp_lat_b;
  
  temp_lat_a = 0x00;
  temp_lat_b = 0x00;

  last_known_action = LAST_ACTION_UPDATE_IO_EXPANDER;
  
  if ((control_state == STATE_SYSTEM_WARM_READY) || (control_state == STATE_HV_ON) || (control_state == STATE_FAULT_WARM_FAULT) || (control_state == STATE_HV_STARTUP)) {
    // warmup_done bit is active.  Clear this bit
    temp_lat_b &= ~BIT_OUTPUT_MODULATOR_WARMUP_DONE;
  } else {
    temp_lat_b |= BIT_OUTPUT_MODULATOR_WARMUP_DONE;
  }

  if (faults_control_board_fault_reg) {
    temp_lat_b |= BIT_OUTPUT_CONTROL_BOARD_FAULT;   
  } else {
    temp_lat_b &= ~BIT_OUTPUT_CONTROL_BOARD_FAULT;
  }
  
  if (faults_thyratron_fault_reg) {
    // a thyratron fault is active - Set this bit
    temp_lat_a |= BIT_OUTPUT_THYRATRON_FAULT;
  } else {
    temp_lat_a &= ~BIT_OUTPUT_THYRATRON_FAULT;
  }
  
  if (faults_magnetron_fault_reg & 0xFF00) {
    // an electromagnet fault is active - Set this bit
    temp_lat_b |= BIT_OUTPUT_ELECTROMAGNET_FAULT;
  } else {
    temp_lat_b &= ~BIT_OUTPUT_ELECTROMAGNET_FAULT;
  }

  if (faults_magnetron_fault_reg & 0x00FF) {
    // an magnetron filament fault is active - Set this bit
    temp_lat_b |= BIT_OUTPUT_FILAMENT_FAULT;
  } else {
    temp_lat_b &= ~BIT_OUTPUT_FILAMENT_FAULT;
  }

  if (faults_high_voltage_fault_reg & 0xFF00) {
    // Lambda Faultis active - Set this bit
    temp_lat_a |= BIT_OUTPUT_HV_FAULT;
  } else {
    temp_lat_a &= ~BIT_OUTPUT_HV_FAULT;
  }

  if (faults_high_voltage_fault_reg & 0x00FF) {
    // Pulse Fault is active - Set this bit
    temp_lat_a |= BIT_OUTPUT_PULSE_FAULT;
  } else {
    temp_lat_a &= ~BIT_OUTPUT_PULSE_FAULT;
  }
  
  if (control_state == STATE_HV_ON) {
    // HV is ON - Set this bit
    temp_lat_b |= BIT_OUTPUT_HV_ON_READBACK;
  } else {
    temp_lat_b &= ~BIT_OUTPUT_HV_ON_READBACK;
  }


  // We only want to update if there is a change in state.  Otherwise we have a lot of unnessary I2C traffic
  if (U64_MCP23017.output_latch_a_in_ram != temp_lat_a) {
    U64_MCP23017.output_latch_a_in_ram = temp_lat_a;
    if (MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_OLATA, U64_MCP23017.output_latch_a_in_ram) != 0) {
      global_debug_counter.i2c_bus_error++;
    }
  }

  // We only want to update if there is a change in state.  Otherwise we have a lot of unnessary I2C traffic
  if (U64_MCP23017.output_latch_b_in_ram != temp_lat_b) {
    U64_MCP23017.output_latch_b_in_ram = temp_lat_b;
    if (MCP23017WriteSingleByte(&U64_MCP23017, MCP23017_REGISTER_OLATB, U64_MCP23017.output_latch_b_in_ram) != 0) { 
      global_debug_counter.i2c_bus_error++;
    }
  }
}


void HVLambdaStartCharging(void) {
  /*
    This starts the charging process . . .
    The following ocurrs . . .
    * The Lamabda is enabled via the ENABLE Pin
    * The Lamabda Inhibit input is set to NOT INHIBIT
    * TMR1 is set up to time the charging process and ensure the lamabda reaches EOC before the first pulse
    * TMR2 is set up to time the 100ms Timeout for the next pulse
    * HouseKeeping - global_run_post_pulse_process is cleared
    
  */

  // --------  Select A or B mode based on the input ----------- //
  if (PIN_A_B_MODE_SELECT == ILL_A_MODE_SELECTED) {
    a_b_selected_mode = PULSE_MODE_A;
    PIN_LAMBDA_VOLTAGE_SELECT = OLL_SELECT_LAMBDA_MODE_A_VOLTAGE;
  } else {
    a_b_selected_mode = PULSE_MODE_B;
    PIN_LAMBDA_VOLTAGE_SELECT = !OLL_SELECT_LAMBDA_MODE_A_VOLTAGE;
  }

  // Actually enable the Lambda
  PIN_HV_LAMBDA_ENABLE = OLL_HV_LAMBDA_ENABLED;
  PIN_HV_LAMBDA_INHIBIT = !OLL_HV_LAMBDA_INHIBITED;
  

  // Setup T1 to time to first charge
  T1CONbits.TON = 0;
  TMR1 = 0;
  _T1IF = 0;
  _T1IE = 1;
  PR1 = (TMR1_LAMBDA_CHARGE_PERIOD);
  T1CONbits.TON = 1;

  // Setup T2 to roll in 100ms if there are no pulses
  TMR2 = 0;  
  _T2IF = 0;
}



/*
  Scaling
  
  Scaling is up as Q13.
  0x0000 = 0
  0xFFFF = 7.99988
  
  Scaling = (num/den) * (2^13))
*/

unsigned int MakeScale(unsigned int num, unsigned int den) {
  unsigned long temp;
  unsigned int return_value;
  
  temp = num;
  temp = temp << 16;
  temp /= den;
  temp = temp >> 3;
  
  if (temp & 0xFFFF0000) {
    // DPARKER saturation error
    return_value = 0xFFFF;
  } else {
    return_value = temp & 0xFFFF;
  }
  return return_value;
}


unsigned int Scale16Bit(unsigned int value, unsigned int scale) {
  unsigned long temp;
  temp = value;
  temp *= scale;
  temp = temp >> 13;
  if (temp > 0x0000FFFF) {
    temp = 0x0000FFFF;
    global_debug_counter.scale16bit_saturation++;
  }
  value = temp & 0xFFFF;
  return value;
}



unsigned int RCFilter256Tau(unsigned int previous_value, unsigned int current_reading) {
  unsigned long temp32;
  unsigned int temp16;
  if (previous_value == 0) {
    previous_value = current_reading;
  }
  temp32 = previous_value;
  temp32 *= 255;
  temp32 += current_reading;
  temp32 = temp32 >> 8;
  temp16 = temp32;
  if (current_reading > temp16) {
    temp16++;
  } else if (current_reading < temp16) {
    temp16--;
  }
  return temp16;
}


unsigned int RCFilter64Tau(unsigned int previous_value, unsigned int current_reading) {
  unsigned long temp32;
  unsigned int temp16;
  if (previous_value == 0) {
    previous_value = current_reading;
  }
  temp32 = previous_value;
  temp32 *= 63;
  temp32 += current_reading;
  temp32 = temp32 >> 6;
  temp16 = temp32;
  if (current_reading > temp16) {
    temp16++;
  } else if (current_reading < temp16) {
    temp16--;
  }
  return temp16;
}


unsigned int RCFilter16Tau(unsigned int previous_value, unsigned int current_reading) {
  unsigned long temp32;
  unsigned int temp16;
  if (previous_value == 0) {
    previous_value = current_reading;
  }
  temp32 = previous_value;
  temp32 *= 15;
  temp32 += current_reading;
  temp32 = temp32 >> 4;
  temp16 = temp32;
  if (current_reading > temp16) {
    temp16++;
  } else if (current_reading < temp16) {
    temp16--;
  }
  return temp16;
}










void _ISRFASTNOPSV _INT1Interrupt(void) {
  /*
    This interrupt does NOTHING TO CONTROL THE TYHRATRON TRIGGER
    By the time this interrupt is called, the trigger has already been routed to the thyratron
    This interrupt just schedules data acquisition and prepares for the NEXT pulse.
    
    This interrupt is enabled when the PIN_THYRATRON_TRIGGER_ENABLE is set (and disabled when that output is cleared).
    Therefore, if this interrupt has been entered then a trigger pulse was probably sent to the thyratron.
    It is possible that a trigger pulse was masked by a hardware fault BEFORE the software could detect it.
    If this were to happen then, the hardware faults will be detected in the next flow through the control loop
    and the system will detect Magnetron Undervoltage Undercurrent faults (because it is expecting a pulse that does not happen)
    
    Assuming a sucessful trigger, this interupt will do the following conecptual steps
    (1) Wait a set amount of time to sample the Magnetron Voltage, Magnetron Current, and PFN Fault Current at the approriate time
    (2) Wait for the pulse EMI to pass
    (2) Read the Magnetron Voltage and Magnetron Current into RAM.
    (3) Inhibit the HV supply a set amount of time.
    (4) After the inhibt time has passed, !Inhibit the power supply, and start the EOC timer
    (5) Set the status bit that indicates a pulse occured
  */ 

  PIN_HV_LAMBDA_INHIBIT = OLL_HV_LAMBDA_INHIBITED;  

  // Trigger Delay is 800ns
  // ADC Trigger Delay Across barrier is 950ns (this should be made faster)
  // ISR Call Delay from trigger to output going high is 10 TCy
  // Repeat stament will exexute N+1 Times
  // Repeate statement itself is 1 Cycle
  // There fore to calculate the trigger delay (from the start of the pulse)
  // ((PFN_Sample_Point_US + 800ns (trigger delay) - 950ns (ADC Trigger Delay)) = Total Delay
  // Total Delay/Tcy = Total Clock Delay
  // Repeat Call = Total Clock Delay - 10 - 2

  Nop();
  Nop();
  Nop();
  Nop();


  PIN_PULSE_ADC_TRIGGER = OLL_TRIGGER_PULSE_ADCS;                  // Trigger the conversion process on both ADCs
  _INT1IF = 0;
  _INT1IE = 0;

  last_known_action = LAST_ACTION_INT1_INT;

  // Setup timer1 to time the time it takes the PFN EMI to dissipate
  T1CONbits.TON = 0;
  TMR1 = 0;
  PR1 = TMR1_DELAY_HOLDOFF;
  _T1IE = 0;
  _T1IF = 0;
  T1CONbits.TON = 1;
  
  false_trigger = 0;

  while (TMR1 < 2);  // 128 clock cycles, 12.8uS
  if (PIN_MODULATOR_DRIVE_INPUT != ILL_MODULATOR_DRIVE_ACTIVE) {
    false_trigger = 1;
  }

  while (TMR1 < 3);  // 192 clock cycles, 19.2uS
  if (PIN_MODULATOR_DRIVE_INPUT != ILL_MODULATOR_DRIVE_ACTIVE) {
    false_trigger = 1;
  }

  while (TMR1 < 3);  // 256 clock cycles, 25.6uS
  if (PIN_MODULATOR_DRIVE_INPUT != ILL_MODULATOR_DRIVE_ACTIVE) {
    false_trigger = 1;
  }

  arc_detected = 0;
  if ((PIN_PULSE_OVER_CUR_LATCH == ILL_PULSE_OVER_CURRENT_FAULT) || (PIN_PULSE_MIN_CUR_LATCH == ILL_PULSE_MIN_CURRENT_FAULT)) {
    arc_detected = 1;
  }

  PIN_PULSE_LATCH_RESET = OLL_PULSE_LATCH_RESET;  // Clear the pulse latches so we can detect a false trigger

  // Calculate the PRF
  last_period = TMR3;
  TMR3 = 0;
  if (_T3IF) {
    // The timer exceed it's period of 400ms - Will happen if the PRF is less than 2.5 Hz
    last_period = 62501;  // This will indicate that the PRF is less than 2,5 Hz
  }
  _T3IF = 0;

  while(!_T1IF);                                                   // what for the holdoff time to pass


  // Read the state of the A_B select Optical input and adjust the system as nesseasry
  if (PIN_A_B_MODE_SELECT == ILL_A_MODE_SELECTED) {
    next_pulse_a_b_selected_mode = PULSE_MODE_A;
    PIN_LAMBDA_VOLTAGE_SELECT = OLL_SELECT_LAMBDA_MODE_A_VOLTAGE;
  } else {
    next_pulse_a_b_selected_mode = PULSE_MODE_B;
    PIN_LAMBDA_VOLTAGE_SELECT = !OLL_SELECT_LAMBDA_MODE_A_VOLTAGE;
  }


  PIN_THYRATRON_TRIGGER_ENABLE = !OLL_THYRATRON_TRIGGER_ENABLED;   // Disable the Pic trigger signal gate
  PIN_HV_LAMBDA_INHIBIT = !OLL_HV_LAMBDA_INHIBITED;                // Start the lambda charge process

  // Set up Timer1 to produce interupt at end of charge period
  T1CONbits.TON = 0;
  TMR1 = 0;
  _T1IF = 0;
  _T1IE = 1;
  PR1 = (TMR1_LAMBDA_CHARGE_PERIOD - TMR1_DELAY_HOLDOFF);
  T1CONbits.TON = 1;

  // Wait for the pulse latches to clear
  while ((PIN_PULSE_OVER_CUR_LATCH == ILL_PULSE_OVER_CURRENT_FAULT) && (TMR1 < 20));
  while ((PIN_PULSE_MIN_CUR_LATCH == ILL_PULSE_MIN_CURRENT_FAULT) && (TMR1 < 20));
  
  if (TMR1 >= 20) {
    // there was an error with the pulse latch reset 
    global_debug_counter.pulse_latch_reset_error++; 
  }
	 
	 

  PIN_PULSE_LATCH_RESET = !OLL_PULSE_LATCH_RESET;
  
  // Clear the Change Notification data that is used to detect a false trigger
  if (PIN_PULSE_MIN_CUR_LATCH); // We need to read this port in order to clear CN data
  _CNIF = 0; // Clear the interrupt flag that gets set when we have a valid pulse
  
  
  
  if (global_run_post_pulse_process) {
    timing_error_int1_count++;
  }

  global_run_post_pulse_process = 1; // This tells the main control loop that a pulse has occured and that it should run the post pulse process once (and only once) 
  global_adc_ignore_this_sample = 1;  // This allows the internal ADC ISR to know that there was a pulse and to discard all the data from the sequence where the pulse occured
}



void _ISRNOPSV _T1Interrupt(void) {
  /*
    This interrupt indicates that the cap charger should have finished charging and it is time to enable the trigger pulse.

    This interrupt is called X us after charging starts
    If the lambda is not at EOC, it does not enable the trigger and sets the Lambda EOC Timeout Fault bit
    If the lambda is at EOC, It enables the trigger & sets status bits to show that the lambda is not charging and that the system is ready to fire.    
  */
  unsigned char lambda_eoc_fault;

  last_known_action = LAST_ACTION_T1_INT;
  
  _T1IF = 0;         // Clear the interrupt flag
  _T1IE = 0;         // Disable the interrupt (This will be enabled the next time that a capacitor charging sequence starts)
  T1CONbits.TON = 0;   // Stop the timer from incrementing (Again this will be restarted with the next time the capacitor charge sequence starts)
  
  // DPARKER - Consider adding more checks - Magnet Current, Actual Lambda Voltage, Check all the fault registers to confirm good to go!!!!

  lambda_eoc_fault = 0;
  if (PIN_HV_LAMBDA_EOC_INPUT != ILL_HV_LAMBDA_AT_EOC) {
    __delay32(DELAY_TCY_10US);
    if (PIN_HV_LAMBDA_EOC_INPUT != ILL_HV_LAMBDA_AT_EOC) {
      __delay32(DELAY_TCY_10US);
      if (PIN_HV_LAMBDA_EOC_INPUT != ILL_HV_LAMBDA_AT_EOC) {
	__delay32(DELAY_TCY_10US);
	if (PIN_HV_LAMBDA_EOC_INPUT != ILL_HV_LAMBDA_AT_EOC) {
	  __delay32(DELAY_TCY_10US);
	  if (PIN_HV_LAMBDA_EOC_INPUT != ILL_HV_LAMBDA_AT_EOC) {
	    RecordThisHighVoltageFault(FAULT_HV_LAMBDA_EOC_TIMEOUT);
	  }
	} 
      }
    }
  }

  // Enable the the thyratron trigger and Enable the Trigger Interrupt 
  PIN_THYRATRON_TRIGGER_ENABLE = OLL_THYRATRON_TRIGGER_ENABLED; // Enable the thyratron trigger pass through.
  _INT1IF = 0;                                                  // Enable INT1 (thyratron trigger) interrupt
  _INT1IE = 1;

}  


void _ISRNOPSV _ADCInterrupt(void) {
  _ASAM = 0; // Stop Auto Sampling
  _ADIF = 0;
  
  // Switch the sate of PIN_UART2_TX (debugging info)
  if (PIN_UART2_TX) {
    PIN_UART2_TX = 0;
  } else {
    PIN_UART2_TX = 1;
  }

  last_known_action = LAST_ACTION_ADC_INTERRUPT;
  // DPARKER what for the conversion to complete???
  
  if (global_adc_ignore_this_sample) {
    // There was a pulse durring the sample sequence.  Throw the data away!!!
    global_adc_ignore_this_sample = 0;
  } else {
    // Copy Data From Buffer to RAM
    pfn_rev_current_array[adc_result_index] = ADCBUF0;
    
    pac_1_array[adc_result_index] = ADCBUF1;
    pac_2_array[adc_result_index] = ADCBUF2;
    
    thyratron_cathode_heater_voltage_array[adc_result_index] = ADCBUF3;
    thyratron_reservoir_heater_voltage_array[adc_result_index] = ADCBUF4;
    
    magnetron_magnet_current_array[adc_result_index]= ADCBUF5;
    magnetron_magnet_voltage_array[adc_result_index]= ADCBUF6;
    
    magnetron_filament_voltage_array[adc_result_index] = ADCBUF7;
    magnetron_filament_current_array[adc_result_index] = ADCBUF8;

    
    lambda_vpeak_array[adc_result_index] = ADCBUF9;
    lambda_vmon_array[adc_result_index] = ADCBUFA;
    
    adc_result_index++;
    adc_result_index &= 0b01111111;
  }
  _ASAM = 1; // Start Auto Sampling
}


void _ISRNOPSV _CNInterrupt(void) {
  if (PIN_PULSE_MIN_CUR_LATCH == !ILL_PULSE_MIN_CURRENT_FAULT) {
    __delay32(100); // 10uS
    if (PIN_PULSE_MIN_CUR_LATCH == !ILL_PULSE_MIN_CURRENT_FAULT) {
      __delay32(100); // 10uS
      if (PIN_PULSE_MIN_CUR_LATCH == !ILL_PULSE_MIN_CURRENT_FAULT) {
	RecordThisThyratronFault(FAULT_THYR_FALSE_TRIGGER);
      }
    }
  }
  _CNIF = 0;
}


void _ISRNOPSV _LVDInterrupt(void) {
  last_known_action = LAST_ACTION_LVD_INT;
  _LVDIF = 0;
  lvdinterrupt_counter++;
}


void _ISRNOPSV _OscillatorFail(void) {
 last_known_action = LAST_ACTION_OSC_FAIL;
 last_osccon = OSCCON;
  Nop();
  Nop();
  __asm__ ("Reset");
}

void _ISRNOPSV _AddressError(void) {
 last_known_action = LAST_ACTION_ADDRESS_ERROR;
  Nop();
  Nop();
  __asm__ ("Reset");
}

void _ISRNOPSV _StackError(void) {
 last_known_action = LAST_ACTION_STACK_ERROR;
  Nop();
  Nop();
  __asm__ ("Reset");
}

void _ISRNOPSV _MathError(void) {
 last_known_action = LAST_ACTION_MATH_ERROR;
  Nop();
  Nop();
  __asm__ ("Reset");
}



// DPARKER THIS FUNCTION FOR DEBUGGING ONLY
void _ISRNOPSV _DefaultInterrupt(void) {
  /*
  control_state = STATE_FAULT_ILLEGAL_INTERRUPT;
  global_debug_counter.magnetron_current_adc_glitch = IFS0;
  global_debug_counter.magnetron_voltage_adc_glitch = IFS1;
  global_debug_counter.i2c_bus_error = IFS2;

  global_debug_counter.spi1_bus_error = IEC0;
  global_debug_counter.spi2_bus_error = IEC1;
  global_debug_counter.external_adc_false_trigger = IEC2;

  while (1) {
    ClrWdt();
    DoSerialCommand();
  }
  */
  last_known_action = LAST_ACTION_DEFAULT_INT;
  Nop();
  Nop();
  __asm__ ("Reset");
}




unsigned int CalculatePoly(unsigned int set_point) {

  signed long sum;
  signed long temp;
  unsigned int value;

  temp = set_point;
  temp *= set_point;
  temp = temp >> 14;
  temp *= control_loop_cal_data_ram_copy[EEPROM_CNTRL_MAGNET_FACTOR_SQUARE];
  temp = temp >> 10;
  sum = temp;

  temp = set_point;
  temp *= control_loop_cal_data_ram_copy[EEPROM_CNTRL_MAGNET_FACTOR_LINEAR];
  temp = temp >> 10;

  sum += temp;

  temp = control_loop_cal_data_ram_copy[EEPROM_CNTRL_MAGNET_FACTOR_CONST];
  
  temp = temp << 2;

  sum += temp;

  value = (sum & 0xFFFF);
  
  return value; 
}



