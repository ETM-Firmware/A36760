// Magnet Supply Configuration

#ifndef __CONFIG_H
#define __CONFIG_H


/*
  Scaling/Limiting of the Program Voltages, Currents, Ect


  Each of these have a scaled set point that is stored as a 16 bit unsigned integer.  
  Lambda Voltage            - Full Scale = 65535 Volts            - 1 Volt / bit
  Magnetron Heater Voltage  - Full Scale = 65.535 Volts           - 1 mV / bit
  Magnetron Heater Current  - Full Scale = 65.535 Amps            - 1 mA / bit
  Magnetron Magnet Curret   - Full Scale = 65.535 Amps            - 1 mA / bit
  Magnetron Magnet Voltage  - Full Scale = 65.535 Volts           - 1 mV / bit
  Both Thyatron Heaters     - Full Scale = 65.535 Volts           - 1 mV / bit

  Other Readbacks
  Pulse Voltage             - Full Scale = 65535 Volts            - 1 Volt / bit
  Pulse Current             - Full Scale = 655.35 Amps            - 10 mA / bit
  PFN Snubber Curret        - Full Scale = 655.35 Amps            - 10 mA / bit


  Limits are Imposed on the Set Point for each of the settings.
  Each Set point has a hard limit.  If you attempt to program the set point above this limit, it will be capped at the limit (and a saturation warming is set)
  
*/


// ----------------- Magnet Supply Configuration ----------------------- //

#define MAGNET_SUPPLY_WARMUP_RAMP_TIME              100                      // 100ms Units - 10 Seconds total

#define MAX_MAGNET_SUPPLY_VOLTAGE_SET_POINT         32000                    // 32 Volts

#define MAGNET_SUPPLY_VDAC_OUTPUT_AT_0xFFFF         41739                    // 41.739 Volts
#define MAGNET_SUPPLY_VADC_INPUT_AT_0xFFFF          43750                    // 43.750 Volts

#define MAGNET_SUPPLY_VADC_OVER_VOLTAGE_HARD        30000                    // 30 Volts 
#define MAGNET_SUPPLY_VADC_OVER_VOLTAGE_SCALE       1.05
#define MAGNET_SUPPLY_VADC_UNDER_VOLTAGE_SCALE      .50
#define MAGNET_SUPPLY_VADC_MIN_OVER_VOLTAGE         1000
#define MAGNET_SUPPLY_VADC_MAX_OUT_OT_RANGE         50                       // 50 * 10ms = 500mS out of range before a fault trips

#define MAX_MAGNET_SUPPLY_CURRENT_SET_POINT         25000                    // 25 Amps

#define MAGNET_SUPPLY_IDAC_OUTPUT_AT_0xFFFF         41535                    // 41.535 Amps
#define MAGNET_SUPPLY_IADC_INPUT_AT_0xFFFF          43750                    // 43.750 Amps

#define MAGNET_SUPPLY_IADC_OVER_CURRENT_HARD        26000                    // 26 Amps 
#define MAGNET_SUPPLY_IADC_OVER_CURRENT_SCALE       (1.05)
#define MAGNET_SUPPLY_IADC_UNDER_CURRENT_SCALE      (.95)
#define MAGNET_SUPPLY_IADC_MIN_OVER_CURRENT         1000
#define MAGNET_SUPPLY_IADC_MAX_OUT_OT_RANGE         50                       // 50 * 10ms = 500mS out of range before a fault trips


#define PS_MAGNET_DEFAULT_CONFIG {15000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 16000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, 0, 0, 0, 0, EEPROM_DEFAULT_CRC}



// ----------------- Filament Supply Configuration ----------------------- //

#define FILAMENT_SUPPLY_WARMUP_RAMP_TIME              1000                      // 100ms Units - 100 Seconds total

#define MAX_FILAMENT_SUPPLY_VOLTAGE_SET_POINT         13500                    // 13.5 Volts

#define FILAMENT_SUPPLY_VDAC_OUTPUT_AT_0xFFFF         17643                    // 17.643 Volts
#define FILAMENT_SUPPLY_VADC_INPUT_AT_0xFFFF          25000                    // 25000 Volts

#define FILAMENT_SUPPLY_VADC_OVER_VOLTAGE_HARD        13750                    // 13.75 Volts 
#define FILAMENT_SUPPLY_VADC_OVER_VOLTAGE_SCALE       1.15
#define FILAMENT_SUPPLY_VADC_UNDER_VOLTAGE_SCALE      .85
#define FILAMENT_SUPPLY_VADC_MIN_OVER_VOLTAGE         2000                     // 2 Volts
#define FILAMENT_SUPPLY_VADC_MAX_OUT_OT_RANGE         200                       // 200 * 10ms = 2000mS out of range before a fault trips

#define MAX_FILAMENT_SUPPLY_CURRENT_SET_POINT         10000                    // 10.0 Amps

#define FILAMENT_SUPPLY_IDAC_OUTPUT_AT_0xFFFF         0xFFFF                   // Bogus Value Because there is no IDAC scale
#define FILAMENT_SUPPLY_IADC_INPUT_AT_0xFFFF          25912                    // 25.912 Amps

#define FILAMENT_SUPPLY_IADC_OVER_CURRENT_HARD        10500                    // 10.5 Amps 
#define FILAMENT_SUPPLY_IADC_OVER_CURRENT_SCALE       1.5
#define FILAMENT_SUPPLY_IADC_UNDER_CURRENT_SCALE      .5
#define FILAMENT_SUPPLY_IADC_MIN_OVER_CURRENT         2000                     // 2 Amps
#define FILAMENT_SUPPLY_IADC_MAX_OUT_OT_RANGE         200                       // 200 * 10ms = 2000mS out of range before a fault trips

#define PS_FILAMENT_DEFAULT_CONFIG {13000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 8500, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, 0, 0, 0, 0, EEPROM_DEFAULT_CRC}


/* 
   The filament power supply will operate at full power when the magentron output power is less than MAGNETRON_FILAMENT_LOW_POWER_EDGE
   The filmanet power supply output will be zero when the magnetron output power is greater than MAGNETRON_FILAMENT_HIGH_POWER_EDGE
   When the magnetron output power is MAGNETRON_FILAMENT_LOW_POWER_EDGE to MAGNETRON_FILAMENT_HIGH_POWER_EDGE, the filament voltage will roll of linearly
*/
#define MAGNETRON_FILAMENT_LOW_POWER_EDGE            1800 // The rolloff starts at 1800W
#define MAGNETRON_FILAMENT_HIGH_POWER_EDGE           3000 // The rolloff finished at 3000W




// ----------------- Thyratron Cathode Heater Supply Configuration ----------------------- //

#define THYR_CATH_HTR_SUPPLY_WARMUP_RAMP_TIME              1800                     // 100ms Units - 180 Seconds total

#define MAX_THYR_CATH_HTR_SUPPLY_VOLTAGE_SET_POINT         6700                     // 6.7 Volts

#define THYR_CATH_HTR_SUPPLY_VDAC_OUTPUT_AT_0xFFFF         0                        // Nil
#define THYR_CATH_HTR_SUPPLY_VADC_INPUT_AT_0xFFFF          38370                    // 38.370 Volts

#define THYR_CATH_HTR_SUPPLY_VADC_OVER_VOLTAGE_HARD        7200                     // 7.2 Volts 
#define THYR_CATH_HTR_SUPPLY_VADC_OVER_VOLTAGE_SCALE       1.05
#define THYR_CATH_HTR_SUPPLY_VADC_UNDER_VOLTAGE_SCALE      .95
#define THYR_CATH_HTR_SUPPLY_VADC_MIN_OVER_VOLTAGE         1000
#define THYR_CATH_HTR_SUPPLY_VADC_MAX_OUT_OT_RANGE         500                      // 500 * 10ms = 5 Sec out of range before a fault trips

#define MAX_THYR_CATH_HTR_SUPPLY_CURRENT_SET_POINT         0                        // Current is not set or measured

#define THYR_CATH_HTR_SUPPLY_IDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define THYR_CATH_HTR_SUPPLY_IADC_INPUT_AT_0xFFFF          0                        // NIL

#define THYR_CATH_HTR_SUPPLY_IADC_OVER_CURRENT_HARD        0                        // NIL
#define THYR_CATH_HTR_SUPPLY_IADC_OVER_CURRENT_SCALE       0
#define THYR_CATH_HTR_SUPPLY_IADC_UNDER_CURRENT_SCALE      0
#define THYR_CATH_HTR_SUPPLY_IADC_MIN_OVER_CURRENT         0
#define THYR_CATH_HTR_SUPPLY_IADC_MAX_OUT_OT_RANGE         0                        // NIL

#define PS_THYR_CATH_HTR_DEFAULT_CONFIG {6300, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, 0, 0, 0, 0, EEPROM_DEFAULT_CRC}



// ----------------- Thyratron reservoir Heater Supply Configuration ----------------------- //

#define THYR_RESER_HTR_SUPPLY_WARMUP_RAMP_TIME              1800                    // 100ms Units - 180 Seconds total

#define MAX_THYR_RESER_HTR_SUPPLY_VOLTAGE_SET_POINT         6000                     // 6 Volts

#define THYR_RESER_HTR_SUPPLY_VDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define THYR_RESER_HTR_SUPPLY_VADC_INPUT_AT_0xFFFF          38370                    // 38.370 Volts

#define THYR_RESER_HTR_SUPPLY_VADC_OVER_VOLTAGE_HARD        6250                     //  6.25 Volts 
#define THYR_RESER_HTR_SUPPLY_VADC_OVER_VOLTAGE_SCALE       1.05
#define THYR_RESER_HTR_SUPPLY_VADC_UNDER_VOLTAGE_SCALE      .95
#define THYR_RESER_HTR_SUPPLY_VADC_MIN_OVER_VOLTAGE         1000
#define THYR_RESER_HTR_SUPPLY_VADC_MAX_OUT_OT_RANGE         500                      // 500 * 10ms = 5 Sec out of range before a fault trips

#define MAX_THYR_RESER_HTR_SUPPLY_CURRENT_SET_POINT         0                        // Current is not set or measured

#define THYR_RESER_HTR_SUPPLY_IDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define THYR_RESER_HTR_SUPPLY_IADC_INPUT_AT_0xFFFF          0                        // NIL

#define THYR_RESER_HTR_SUPPLY_IADC_OVER_CURRENT_HARD        0                        // NIL
#define THYR_RESER_HTR_SUPPLY_IADC_OVER_CURRENT_SCALE       0
#define THYR_RESER_HTR_SUPPLY_IADC_UNDER_CURRENT_SCALE      0
#define THYR_RESER_HTR_SUPPLY_IADC_MIN_OVER_CURRENT         0
#define THYR_RESER_HTR_SUPPLY_IADC_MAX_OUT_OT_RANGE         0                        // NIL

#define PS_THYR_RESER_HTR_DEFAULT_CONFIG {5500, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, 0, 0, 0, 0, EEPROM_DEFAULT_CRC}



// ----------------- HV LAMBDA MODE A Supply Configuration ----------------------- //

#define HV_LAMBDA_MODE_A_WARMUP_RAMP_TIME              0                        // NIL

#define MAX_HV_LAMBDA_MODE_A_VOLTAGE_SET_POINT         20000                    // 20 KV

#define HV_LAMBDA_MODE_A_VDAC_OUTPUT_AT_0xFFFF         22293                    // 22.293 KV
#define HV_LAMBDA_MODE_A_VADC_INPUT_AT_0xFFFF          25000                    // 25 KV

#define HV_LAMBDA_MODE_A_VADC_OVER_VOLTAGE_HARD        19500                    // 19.5 KV
#define HV_LAMBDA_MODE_A_VADC_OVER_VOLTAGE_SCALE       0                        // NIL
#define HV_LAMBDA_MODE_A_VADC_UNDER_VOLTAGE_SCALE      0                        // NIL
#define HV_LAMBDA_MODE_A_VADC_MIN_OVER_VOLTAGE         0                        // NIL
#define HV_LAMBDA_MODE_A_VADC_MAX_OUT_OT_RANGE         0                        // NIL

#define MAX_HV_LAMBDA_MODE_A_CURRENT_SET_POINT         0                        // NIL

#define HV_LAMBDA_MODE_A_IDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define HV_LAMBDA_MODE_A_IADC_INPUT_AT_0xFFFF          0                        // NIL

#define HV_LAMBDA_MODE_A_IADC_OVER_CURRENT_HARD        0                        // NIL
#define HV_LAMBDA_MODE_A_IADC_OVER_CURRENT_SCALE       0                        // NIL
#define HV_LAMBDA_MODE_A_IADC_UNDER_CURRENT_SCALE      0                        // NIL
#define HV_LAMBDA_MODE_A_IADC_MIN_OVER_CURRENT         0                        // NIL
#define HV_LAMBDA_MODE_A_IADC_MAX_OUT_OT_RANGE         0                        // NIL


#define TEMP_HV_A_VDAC_CAL    0

#define PS_HV_LAMBDA_MODE_A_DEFAULT_CONFIG {10000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, 0, 0, 0, 0, EEPROM_DEFAULT_CRC}




// ----------------- HV LAMBDA MODE B Supply Configuration ----------------------- //

#define HV_LAMBDA_MODE_B_WARMUP_RAMP_TIME              0                        // NIL

#define MAX_HV_LAMBDA_MODE_B_VOLTAGE_SET_POINT         20000                    // 20 KV

#define HV_LAMBDA_MODE_B_VDAC_OUTPUT_AT_0xFFFF         22293                    // 22.293 KV
#define HV_LAMBDA_MODE_B_VADC_INPUT_AT_0xFFFF          25000                    // 25 KV

#define HV_LAMBDA_MODE_B_VADC_OVER_VOLTAGE_HARD        19500                    // 19.5 KV
#define HV_LAMBDA_MODE_B_VADC_OVER_VOLTAGE_SCALE       0                        // NIL
#define HV_LAMBDA_MODE_B_VADC_UNDER_VOLTAGE_SCALE      0                        // NIL
#define HV_LAMBDA_MODE_B_VADC_MIN_OVER_VOLTAGE         0                        // NIL
#define HV_LAMBDA_MODE_B_VADC_MAX_OUT_OT_RANGE         0                        // NIL

#define MAX_HV_LAMBDA_MODE_B_CURRENT_SET_POINT         0                        // NIL

#define HV_LAMBDA_MODE_B_IDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define HV_LAMBDA_MODE_B_IADC_INPUT_AT_0xFFFF          0                        // NIL

#define HV_LAMBDA_MODE_B_IADC_OVER_CURRENT_HARD        0                        // NIL
#define HV_LAMBDA_MODE_B_IADC_OVER_CURRENT_SCALE       0                        // NIL
#define HV_LAMBDA_MODE_B_IADC_UNDER_CURRENT_SCALE      0                        // NIL
#define HV_LAMBDA_MODE_B_IADC_MIN_OVER_CURRENT         0                        // NIL
#define HV_LAMBDA_MODE_B_IADC_MAX_OUT_OT_RANGE         0                        // NIL

#define PS_HV_LAMBDA_MODE_B_DEFAULT_CONFIG {12000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, 0, 0, 0, 0, EEPROM_DEFAULT_CRC}




// ----------------- Power Supply Magnetron mode A Configuration ----------------------- //

#define MAGNETRON_MODE_A_WARMUP_RAMP_TIME              0                        // NIL

#define MAX_MAGNETRON_MODE_A_VOLTAGE_SET_POINT         55000                    // 55 KV

#define MAGNETRON_MODE_A_VDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define MAGNETRON_MODE_A_VADC_INPUT_AT_0xFFFF          55457                    // 55.46 KV // DPARKER this is based on measured voltage divider ration

#define MAGNETRON_MODE_A_VADC_OVER_VOLTAGE_HARD        52000                    // 52 KV
#define MAGNETRON_MODE_A_VADC_OVER_VOLTAGE_SCALE       1.25                     // 
#define MAGNETRON_MODE_A_VADC_UNDER_VOLTAGE_SCALE      .8                       // 
#define MAGNETRON_MODE_A_VADC_MIN_OVER_VOLTAGE         1000                     // 
#define MAGNETRON_MODE_A_VADC_MAX_OUT_OT_RANGE         20                       // 

#define MAX_MAGNETRON_MODE_A_CURRENT_SET_POINT         11500                    // 115.00 Amps

#define MAGNETRON_MODE_A_IDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define MAGNETRON_MODE_A_IADC_INPUT_AT_0xFFFF          16667                   // 166.67 Amps

#define MAGNETRON_MODE_A_IADC_OVER_CURRENT_HARD        12500                    // 125.00 Amps
#define MAGNETRON_MODE_A_IADC_OVER_CURRENT_SCALE       1.05                     // 
#define MAGNETRON_MODE_A_IADC_UNDER_CURRENT_SCALE      .95                      // 
#define MAGNETRON_MODE_A_IADC_MIN_OVER_CURRENT         1000                     // 10 Amps
#define MAGNETRON_MODE_A_IADC_MAX_OUT_OT_RANGE         20                       // 

#define PS_MAGNETRON_MODE_A_DEFAULT_CONFIG {40000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 10000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, 0, 0, 0, 0, EEPROM_DEFAULT_CRC}


// ----------------- Power Supply Magnetron mode B Configuration ----------------------- //

#define MAGNETRON_MODE_B_WARMUP_RAMP_TIME              0                        // NIL

#define MAX_MAGNETRON_MODE_B_VOLTAGE_SET_POINT         55000                    // 55 KV

#define MAGNETRON_MODE_B_VDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define MAGNETRON_MODE_B_VADC_INPUT_AT_0xFFFF          55457                    // 55.46 KV  // DPARKER this is based on measured voltage divider ration

#define MAGNETRON_MODE_B_VADC_OVER_VOLTAGE_HARD        52000                    // 52 KV
#define MAGNETRON_MODE_B_VADC_OVER_VOLTAGE_SCALE       1.25                     // 
#define MAGNETRON_MODE_B_VADC_UNDER_VOLTAGE_SCALE      .8                       // 
#define MAGNETRON_MODE_B_VADC_MIN_OVER_VOLTAGE         1000                     // 
#define MAGNETRON_MODE_B_VADC_MAX_OUT_OT_RANGE         20                       // 

#define MAX_MAGNETRON_MODE_B_CURRENT_SET_POINT         11500                    // 115.00 Amps

#define MAGNETRON_MODE_B_IDAC_OUTPUT_AT_0xFFFF         0                        // NIL
#define MAGNETRON_MODE_B_IADC_INPUT_AT_0xFFFF          16667                   // 166.67 Amps

#define MAGNETRON_MODE_B_IADC_OVER_CURRENT_HARD        12500                    // 125.00 Amps
#define MAGNETRON_MODE_B_IADC_OVER_CURRENT_SCALE       1.05                     // 
#define MAGNETRON_MODE_B_IADC_UNDER_CURRENT_SCALE      .95                      // 
#define MAGNETRON_MODE_B_IADC_MIN_OVER_CURRENT         1000                     // 10 Amps
#define MAGNETRON_MODE_B_IADC_MAX_OUT_OT_RANGE         20                       // 

#define PS_MAGNETRON_MODE_B_DEFAULT_CONFIG {40000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 10000, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, EEPROM_CAL_GAIN_1, EEPROM_CAL_OFFSET_0, 0, 0, 0, 0, 0, EEPROM_DEFAULT_CRC}



#define EEPROM_V_SET_POINT                  0
#define EEPROM_V_DAC_CAL_GAIN               1
#define EEPROM_V_DAC_CAL_OFFSET             2
#define EEPROM_V_ADC_CAL_GAIN               3
#define EEPROM_V_ADC_CAL_OFFSET             4

#define EEPROM_I_SET_POINT                  5
#define EEPROM_I_DAC_CAL_GAIN               6
#define EEPROM_I_DAC_CAL_OFFSET             7
#define EEPROM_I_ADC_CAL_GAIN               8
#define EEPROM_I_ADC_CAL_OFFSET             9

#define EEPROM_UNUSED_1                     10
#define EEPROM_UNUSED_2                     11
#define EEPROM_UNUSED_3                     12
#define EEPROM_UNUSED_4                     13
#define EEPROM_UNUSED_5                     14

#define EEPROM_CRC                          15



#define EEPROM_CAL_GAIN_1                   8192
#define EEPROM_CAL_OFFSET_0                 0
#define EEPROM_DEFAULT_CRC                  0



// Pulse Readback

#define PFN_PULSE_WIDTH_NS                3000        
#define PULSE_VADC_OUTPUT_AT_0xFFFF       63780
#define PULSE_IADC_OUTPUT_AT_0xFFFF       166.7

#define PULSE_ENERGY_SCALE               (PULSE_VADC_OUTPUT_AT_0xFFFF * PULSE_IADC_OUTPUT_AT_0xFFFF * 4.294967) 



// --- CONFIGURATION for Analong Control Inputs
/*
  Scale factors
  Analog Inputs 0-10 Volts
  10V = 20KV (Voltage program mode)   
  10V = 125 Amps (Current Program mode)
  
  PAC_INPUT_CURRENT_SCALE = 156.250/655.35*2^13 = 1953
  PAC_INPUT_VOLTAGE_SCALE = does not matter yet because this is not a linear function
  DIRECT_LAMBDA_INPUT_SCALE = 25000/65535*2^13 = 2500
*/

#define PAC_INPUT_CURRENT_SCALE                1953
#define PAC_INPUT_VOLTAGE_SCALE                8192  // DPARKER USE 1 for now
#define DIRECT_LAMBDA_INPUT_SCALE              3125



// Configuration for Poly Conversions for Lambda Voltage to Lambda Current
#define CONTROL_LOOP_CAL_DATA_DEFAULT_CONFIG        {0,870,750,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}

#define EEPROM_CNTRL_MAGNET_FACTOR_SQUARE   0
#define EEPROM_CNTRL_MAGNET_FACTOR_LINEAR   1
#define EEPROM_CNTRL_MAGNET_FACTOR_CONST    2
#define EEPROM_CNTRL_UNUSED_1               3
#define EEPROM_CNTRL_UNUSED_2               4
#define EEPROM_CNTRL_UNUSED_3               5
#define EEPROM_CNTRL_UNUSED_4               6
#define EEPROM_CNTRL_UNUSED_5               7
#define EEPROM_CNTRL_UNUSED_6               8
#define EEPROM_CNTRL_UNUSED_7               9
#define EEPROM_CNTRL_UNUSED_8               10
#define EEPROM_CNTRL_UNUSED_9               11
#define EEPROM_CNTRL_UNUSED_10              12
#define EEPROM_CNTRL_UNUSED_11              13
#define EEPROM_CNTRL_UNUSED_12              14
#define EEPROM_CNTRL_CRC                    15



#endif








