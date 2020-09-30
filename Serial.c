#include "Serial.h"
#include "A34760_PINS.h"
#include "Buffer64.h"
#include "A34760.h"
#include "config.h"
#include "faults.h"
#include "Version.h"
#include <libpic30.h>
#include "eeprom.h"
/*
  Serial Commands

  A single command is stored in command_string
  If there is a valid command stored in command_string, then the command_string.data_state = COMMAND_BUFFER_FULL
  If there is NOT a volid command stored in command_string, then command_string.data_state = COMMAND_BUFFER_EMPTY
  
  
  When a byte is received on the UART it is transfered to the "uart1_input_buffer" by the UART receive interrupt - the input buffer is a circular buffer that is 64 Bytes deep
  (see buffer64.h for more infor on the buffer)
  
  Every time through the command loop (200us to 1ms) DoSerialCommand() will be called
  If the command_string is empty, then the input buffer is searched for a valid command (the oldest valid command will be moved to command_string)

  If a command was found OR the command_string was already full, then the command is executed.

  Assume an average execution cycle of 1mS and 9 bytes per command.  A command rate of 72 K Baund can be sustained. (57.6 K Baud Standard will work)
  
  Assume an average execution cycle of 500uS and 9 bytes per command, A command rate of 144 K Baud can be sustained (115.2 K Baud Standard should be safe) 

*/

void LookForCommand(void);
void ExecuteCommand(void);
unsigned char CheckCRC(unsigned int crc);
unsigned int MakeCRC(unsigned char command_byte, unsigned char register_byte, unsigned int data_word);
unsigned int ReadFromRam(unsigned int ram_location);
void SendCommand(unsigned char command_byte, unsigned char register_byte, unsigned int data_word);

unsigned int GenerateFilamentIprog(unsigned int vprog);
unsigned int GenerateLambdaIprog(unsigned int vprog);

unsigned int GenerateMagnetronVprog(unsigned int iprog);

struct CommandStringStruct command_string;

unsigned char data_logging_to_uart;

unsigned int ReturnPowerSupplyADCScaledVoltage(POWERSUPPLY* ptr, unsigned int value);
unsigned int ReturnPowerSupplyADCScaledCurrent(POWERSUPPLY* ptr, unsigned int value);


unsigned int ReverseScale16Bit(unsigned int value, unsigned int scale);


unsigned int prf_mult = 1;
unsigned int prf_divider = 1;


void CalculatePRFDivider(unsigned int requested_prf);

void DoSerialCommand(void) {
  /* 
     Look for a command and execute it.
  */
  if (_OERR) {
    _OERR = 0;
  }

  if (command_string.data_state != COMMAND_BUFFER_FULL) {
    LookForCommand();
  }
  if (command_string.data_state == COMMAND_BUFFER_FULL) {
    ExecuteCommand();
  }
}



void LookForCommand(void) {
  unsigned char read_byte;
  unsigned int crc;
  /*
    If the state is "waitng for command" then it looks for a command in the buffer, if the state is "executing command" it does nothing
    
    To look for a command in the buffer.
    1) See if there are enough bytes in the buffer to contain a command.
    2) If there are look for command sync
       2b) If there are less bytes in the buffer than it takes to make a command, exit
    3) If command Syncs, check the checksum ^ increment the read_position as each byte is read
       3b) If command does not sync, increment the the read positon and return to step 1    
    4) If the checksum checks out, move the command data into the command data structure
    4b) If the checksum fails, return to step 1     
  */
  
  while ((command_string.data_state == COMMAND_BUFFER_EMPTY) && (Buffer64BytesInBuffer(&uart1_input_buffer) >= COMMAND_LENGTH)) {
    // Look for a command
    read_byte = Buffer64ReadByte(&uart1_input_buffer);
    if (read_byte == SYNC_BYTE_1) {
      read_byte = Buffer64ReadByte(&uart1_input_buffer);
      if (read_byte == SYNC_BYTE_2) {
	read_byte = Buffer64ReadByte(&uart1_input_buffer);
	if (read_byte == SYNC_BYTE_3_RECEIVE) {
	  // All of the sync bytes matched, this should be a valid command
	  command_string.command_byte   = Buffer64ReadByte(&uart1_input_buffer);
	  command_string.data_high_byte = Buffer64ReadByte(&uart1_input_buffer);
	  command_string.data_low_byte  = Buffer64ReadByte(&uart1_input_buffer);
	  command_string.register_byte  = Buffer64ReadByte(&uart1_input_buffer);
	  crc                           = Buffer64ReadByte(&uart1_input_buffer);
	  crc                           = (crc << 8) + Buffer64ReadByte(&uart1_input_buffer);
	  if (CheckCRC(crc)) {
	    command_string.data_state = COMMAND_BUFFER_FULL;
	  }
	}
      }
    }
  }
}


void SendLoggingDataToUart() {
  /* 
     Data that we need to log with each pulse
     byte 0,1    = 0xFE, 0xF1 - used to sync message
     byte 2,3    = low_energy_target_current_set_point_derived (low 16 bits)
     byte 4,5    = linac_high_energy_target_current_adc_reading
     byte 6,7    = linac_low_energy_target_current_adc_reading
     byte 8,9    = linac_high_energy_program_offset
     byte 10,11    = linac_low_energy_program_offset
     

     byte 12,13    = pulse_counter_this_run
     byte 14,15   = pulse_magnetron_current_adc_reading
     
  */

  if (data_logging_to_uart) {

    Buffer64WriteByte(&uart1_output_buffer, 0xFE);
    Buffer64WriteByte(&uart1_output_buffer, 0xF1);

    Buffer64WriteByte(&uart1_output_buffer, (low_energy_target_current_set_point_derived >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (low_energy_target_current_set_point_derived & 0xFF));

    Buffer64WriteByte(&uart1_output_buffer, (linac_high_energy_target_current_adc_reading >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (linac_high_energy_target_current_adc_reading & 0x00FF));

    Buffer64WriteByte(&uart1_output_buffer, (linac_low_energy_target_current_adc_reading >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (linac_low_energy_target_current_adc_reading & 0x00FF));

    Buffer64WriteByte(&uart1_output_buffer, (linac_high_energy_program_offset >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (linac_high_energy_program_offset & 0x00FF));

    Buffer64WriteByte(&uart1_output_buffer, (linac_low_energy_program_offset >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (linac_low_energy_program_offset & 0x00FF));

    Buffer64WriteByte(&uart1_output_buffer, (pulse_counter_this_run >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (pulse_counter_this_run & 0x00FF));

    Buffer64WriteByte(&uart1_output_buffer, (pulse_magnetron_current_adc_reading >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (pulse_magnetron_current_adc_reading & 0x00FF));


    if ((!U1STAbits.UTXBF) && (Buffer64IsNotEmpty(&uart1_output_buffer))) {
      /*
	There is at least one byte available for writing in the outputbuffer and the transmit buffer is not full.
	Move a byte from the output buffer into the transmit buffer
	All subsequent bytes will be moved from the output buffer to the transmit buffer by the U1 TX Interrupt
      */
      U1TXREG = Buffer64ReadByte(&uart1_output_buffer);
    }
  }
}



void SendCommand(unsigned char command_byte, unsigned char register_byte, unsigned int data_word) {
  unsigned int crc;
  if (!data_logging_to_uart) {
    crc = MakeCRC(command_byte, register_byte, data_word);
    Buffer64WriteByte(&uart1_output_buffer, SYNC_BYTE_1);
    Buffer64WriteByte(&uart1_output_buffer, SYNC_BYTE_2);
    Buffer64WriteByte(&uart1_output_buffer, SYNC_BYTE_3_SEND);
    Buffer64WriteByte(&uart1_output_buffer, command_byte);
    Buffer64WriteByte(&uart1_output_buffer, (data_word >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (data_word & 0x00FF));
    Buffer64WriteByte(&uart1_output_buffer, register_byte);
    Buffer64WriteByte(&uart1_output_buffer, (crc >> 8));
    Buffer64WriteByte(&uart1_output_buffer, (crc & 0x00FF));
    
    if ((!U1STAbits.UTXBF) && (Buffer64IsNotEmpty(&uart1_output_buffer))) {
      /*
	There is at least one byte available for writing in the outputbuffer and the transmit buffer is not full.
	Move a byte from the output buffer into the transmit buffer
	All subsequent bytes will be moved from the output buffer to the transmit buffer by the U1 TX Interrupt
      */
      U1TXREG = Buffer64ReadByte(&uart1_output_buffer);
    }
  }
}

void ExecuteCommand(void) {
  unsigned int itemp;
  unsigned int vtemp;
  unsigned int data_word;
  unsigned int return_data_word;
  unsigned int return_command_byte;
  
  unsigned int eeprom_write_complete;
  unsigned int eeprom_write_attempt;

  
  data_word = command_string.data_high_byte;
  data_word = data_word << 8;
  data_word = data_word + command_string.data_low_byte;
  
  return_data_word = data_word;
  return_command_byte = command_string.command_byte;
  switch (command_string.command_byte) 
    {


      
#if defined(__SET_MAGNETRON_OVER_SERIAL_INTERFACE)
      // We have compiled in the mode that untilizes the serial port to set the magnetron operating parameters (instead of the analong interface)
    case CMD_PAC_SET_MODE_A:
      itemp = data_word;
      vtemp = GenerateMagnetronVprog(itemp);
      SetPowerSupplyTarget(&ps_magnetron_mode_A, vtemp, itemp);
      /*
      ps_magnetron_mode_A_config_ram_copy[EEPROM_V_SET_POINT] = ps_magnetron_mode_A.v_command_set_point;
      ps_magnetron_mode_A_config_ram_copy[EEPROM_I_SET_POINT] = ps_magnetron_mode_A.i_command_set_point;
      _wait_eedata();
      _erase_eedata(EE_address_ps_magnetron_mode_A_config_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_ps_magnetron_mode_A_config_in_EEPROM, ps_magnet_config_ram_copy);
      */
      break;

    case CMD_PAC_SET_MODE_B:
      itemp = data_word;
      vtemp = GenerateMagnetronVprog(itemp);
      SetPowerSupplyTarget(&ps_magnetron_mode_B, vtemp, itemp);
      /*
      ps_magnetron_mode_B_config_ram_copy[EEPROM_V_SET_POINT] = ps_magnetron_mode_B.v_command_set_point;
      ps_magnetron_mode_B_config_ram_copy[EEPROM_I_SET_POINT] = ps_magnetron_mode_B.i_command_set_point;
      _wait_eedata();
      _erase_eedata(EE_address_ps_magnetron_mode_B_config_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_ps_magnetron_mode_B_config_in_EEPROM, ps_magnet_config_ram_copy);
      */
      break;
#endif



#if defined(__SET_MAGNETRON_OVER_SERIAL_INTERFACE)
      // The lambda voltage is set directly over the serial interface.
    case CMD_SET_LAMBDA_VOLTAGE_MODE_A:
      vtemp = data_word;
      itemp = GenerateLambdaIprog(vtemp);
      SetPowerSupplyTarget(&ps_hv_lambda_mode_A, vtemp, itemp);
      /*
      ps_hv_lambda_mode_A_config_ram_copy[EEPROM_V_SET_POINT] = ps_hv_lambda_mode_A.v_command_set_point;
      ps_hv_lambda_mode_A_config_ram_copy[EEPROM_I_SET_POINT] = ps_hv_lambda_mode_A.i_command_set_point;
      _wait_eedata();
      _erase_eedata(EE_address_ps_hv_lambda_mode_A_config_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_ps_hv_lambda_mode_A_config_in_EEPROM, ps_hv_lambda_mode_A_config_ram_copy);
      */
      break;

    case CMD_SET_LAMBDA_VOLTAGE_MODE_B:
      vtemp = data_word;
      itemp = GenerateLambdaIprog(vtemp);
      SetPowerSupplyTarget(&ps_hv_lambda_mode_B, vtemp, itemp);
      /*
      ps_hv_lambda_mode_B_config_ram_copy[EEPROM_V_SET_POINT] = ps_hv_lambda_mode_B.v_command_set_point;
      ps_hv_lambda_mode_B_config_ram_copy[EEPROM_I_SET_POINT] = ps_hv_lambda_mode_B.i_command_set_point;
      _wait_eedata();
      _erase_eedata(EE_address_ps_hv_lambda_mode_B_config_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_ps_hv_lambda_mode_B_config_in_EEPROM, ps_hv_lambda_mode_B_config_ram_copy);
      */
      break;

#endif // #if defined(__SET_MAGNETRON_OVER_SERIAL_INTERFACE)
    case CMD_SET_MAGNETRON_CURRENT_REMOTE_MODE:
      ram_config_set_magnetron_magnet_current_from_GUI = 0;
      break;
      
    case CMD_SET_MAGNETRON_CURRENT_LOCAL_MODE:
      ram_config_set_magnetron_magnet_current_from_GUI = 1;
      break;

    case CMD_SET_PRF_REQUESTED_FREQ:
      CalculatePRFDivider(data_word);
      break;

    case CMD_SET_MAGNETRON_FILAMENT_CONTROL_MODE:
      magnetron_filament_control_mode = data_word;
      break;

    case CMD_SET_MAGNETRON_FILAMENT_OFF_RESISTANCE:
      if (data_word > 10000) {
	magnetron_filament_off_resistance = data_word;
      }
      break;

    case CMD_SET_MAGNETRON_FILAMENT_ON_RESISTANCE:
      if (data_word > 10000) {
	magnetron_filament_on_resistance = data_word;
      }
      break;
	
      
      //#if defined(__SET_MAGNET_CURRENT_OVER_SERIAL_INTERFACE)
    case CMD_SET_MAGNETRON_MAGNET_CURRENT:
      if (ram_config_set_magnetron_magnet_current_from_GUI) {
	itemp = data_word;
	vtemp = GenerateMagnetVprog(itemp);
	SetPowerSupplyTarget(&ps_magnet, vtemp, itemp);
	/*
	ps_magnet_config_ram_copy[EEPROM_V_SET_POINT] = ps_magnet.v_command_set_point;
	ps_magnet_config_ram_copy[EEPROM_I_SET_POINT] = ps_magnet.i_command_set_point;
	_wait_eedata();
	_erase_eedata(EE_address_ps_magnet_config_in_EEPROM, _EE_ROW);
	_wait_eedata();
	_write_eedata_row(EE_address_ps_magnet_config_in_EEPROM, ps_magnet_config_ram_copy);
	*/
      }
      break;

      //#endif //#if defined(__SET_MAGNET_CURRENT_OVER_SERIAL_INTERFACE)

      
    case CMD_READ_RAM_VALUE:
      return_data_word = ReadFromRam(command_string.register_byte);
      break;

      
    case CMD_DATA_LOGGING:
      if (command_string.register_byte == 1) {
	data_logging_to_uart = 1;
      } else {
      	data_logging_to_uart = 0;
      }
      break;

      /*
    case CMD_SET_HIGH_ENERGY_TARGET_CURRENT_SETPOINT:
      linac_high_energy_target_current_set_point = data_word;
      if (linac_high_energy_target_current_set_point < LINAC_TARGET_CURRENT_HIGH_ENERGY_MINIMUM_ERROR) {
	linac_high_energy_target_current_set_point = LINAC_TARGET_CURRENT_HIGH_ENERGY_MINIMUM_ERROR;
      }
      control_loop_cal_data_ram_copy[EEPROM_CNTRL_HIGH_ENERGY_TARGET] = linac_high_energy_target_current_set_point;
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);      
      break;
      

    case CMD_SET_LOW_ENERGY_PORTAL_TARGET_CURRENT_SETPOINT:
      linac_low_energy_target_current_set_point_portal_mode = data_word;
      if (linac_low_energy_target_current_set_point_portal_mode < LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR) {
	linac_low_energy_target_current_set_point_portal_mode = LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR;
      }
      control_loop_cal_data_ram_copy[EEPROM_CNTRL_LOW_ENERGY_PORTAL_TARGET] = linac_low_energy_target_current_set_point_portal_mode;
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);      
      break;

    case CMD_SET_LOW_ENERGY_GANTRY_TARGET_CURRENT_SETPOINT:
      linac_low_energy_target_current_set_point_gantry_mode = data_word;
      if (linac_low_energy_target_current_set_point_gantry_mode < LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR) {
	linac_low_energy_target_current_set_point_gantry_mode = LINAC_TARGET_CURRENT_LOW_ENERGY_MINIMUM_ERROR;
      }
      control_loop_cal_data_ram_copy[EEPROM_CNTRL_LOW_ENERGY_GANTRY_TARGET] = linac_low_energy_target_current_set_point_gantry_mode;
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);      
      break;

    case CMD_SET_TARGET_CURRENT_STARTUP_PULSES:
      low_energy_target_current_startup_adjust_decay_time_pulses = data_word;
      control_loop_cal_data_ram_copy[EEPROM_CNTRL_TARGET_STARTUP_PULSES] = low_energy_target_current_startup_adjust_decay_time_pulses;
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);      
      break;

    case CMD_SET_TARGET_CURRENT_STARTUP_MAGNITUDE:
      max_low_energy_target_current_startup_adjust_initital_value = data_word;
      control_loop_cal_data_ram_copy[EEPROM_CNTRL_TARGET_MAX_MAGNITUDE] = max_low_energy_target_current_startup_adjust_initital_value;
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);      
      break;

    case CMD_SET_TARGET_CURRENT_STARTUP_DIRECTION:
      low_energy_target_current_startup_max_cooldown = data_word;
      control_loop_cal_data_ram_copy[EEPROM_CNTRL_TARGET_MAX_COOLDOWN] = low_energy_target_current_startup_max_cooldown;
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);      
      break;

      */


    case CMD_SET_CNTRL_CAL_DATA:
      if (command_string.register_byte == EEPROM_CNTRL_MAGNET_FACTOR_LINEAR) {
	eeprom_write_complete = 0;
	eeprom_write_attempt = 0;
	while (eeprom_write_complete == 0) {
	  eeprom_write_complete = ETMEEPromWriteWordWithConfirmation(EEPROM_REGISTER_MAGNET_SCALING_LINEAR, data_word);
	  eeprom_write_attempt++;
	  if (eeprom_write_attempt >= 10) {
	    eeprom_write_failure_count++;
	    break;
	  }
	}
	if (eeprom_write_complete) {
	  magnet_scaling_linear_factor = data_word;
	}
      }

      if (command_string.register_byte == EEPROM_CNTRL_MAGNET_FACTOR_CONST) {
	eeprom_write_complete = 0;
	eeprom_write_attempt = 0;
	while (eeprom_write_complete == 0) {
	  eeprom_write_complete = ETMEEPromWriteWordWithConfirmation(EEPROM_REGISTER_MAGNET_SCALING_CONST, data_word);
	  eeprom_write_attempt++;
	  if (eeprom_write_attempt >= 10) {
	    eeprom_write_failure_count++;
	    break;
	  }
	}
	if (eeprom_write_complete) {
	  magnet_scaling_constant_factor = data_word;
	}
      }
      break;

    case CMD_READ_CNTRL_CAL_DATA:
      return_data_word = 0;

      if (command_string.register_byte == EEPROM_CNTRL_MAGNET_FACTOR_LINEAR) {
	return_data_word = magnet_scaling_linear_factor;
      }

      if (command_string.register_byte == EEPROM_CNTRL_MAGNET_FACTOR_CONST) {
	return_data_word = magnet_scaling_constant_factor;
      }
      break;

      /*
    case CMD_SAVE_CNTRL_CAL_DATA_TO_EEPROM:
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);
      break;
      */


      
    case CMD_SET_FILAMENT_OFFSET:
      look_up_offset = (signed int)data_word;
      break;

    case CMD_SET_SCALE_INTERLEAVED:
      eeprom_write_complete = 0;
      eeprom_write_attempt = 0;
      while (eeprom_write_complete == 0) {
	eeprom_write_complete = ETMEEPromWriteWordWithConfirmation(EEPROM_REGISTER_SCALE_INTERLEAVED, data_word);
	eeprom_write_attempt++;
	if (eeprom_write_attempt >= 10) {
	  eeprom_write_failure_count++;
	  break;
	}
      }
      if (eeprom_write_complete) {
	scale_interleaved = data_word;
      }

      
      /*
      control_loop_cal_data_ram_copy[EEPROM_CNTRL_INTERLEAVED_POWER_SCALE] = data_word;
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);
      */ 
      break;
      
    case CMD_SET_SCALE_LOW_ENERGY:
      eeprom_write_complete = 0;
      eeprom_write_attempt = 0;
      while (eeprom_write_complete == 0) {
	eeprom_write_complete = ETMEEPromWriteWordWithConfirmation(EEPROM_REGISTER_SCALE_LOW_ENERGY, data_word);
	eeprom_write_attempt++;
	if (eeprom_write_attempt >= 10) {
	  eeprom_write_failure_count++;
	  break;
	}
      }
      if (eeprom_write_complete) {
	scale_low_energy = data_word;
      }


      /*
      control_loop_cal_data_ram_copy[EEPROM_CNTRL_LOW_ENERGY_POWER_SCALE] = data_word;
      _wait_eedata();
      _erase_eedata(EE_address_control_loop_cal_data_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_control_loop_cal_data_in_EEPROM, control_loop_cal_data_ram_copy);
      */   
      break;

    case CMD_FORCE_SOFTWARE_RESTART:
      /*
	We don't want to be forcing software resets on a production system
      PIN_MCU_CLOCK_OUT_TEST_POINT = 0;
      __delay32(100000); // this should be 10ms
      PIN_MCU_CLOCK_OUT_TEST_POINT = 1;
      __delay32(100);
      PIN_MCU_CLOCK_OUT_TEST_POINT = 0;
     __asm__ ("Reset");
      */
      break;

    case CMD_SOFTWARE_SKIP_WARMUP:
      if (control_state == STATE_WARM_UP) {
	software_skip_warmup = 1;
      }
      break;

    case CMD_SET_MAGNETRON_FILAMENT_CURRENT:
      eeprom_write_complete = 0;
      eeprom_write_attempt = 0;
      while (eeprom_write_complete == 0) {
	eeprom_write_complete = ETMEEPromWriteWordWithConfirmation(EEPROM_REGISTER_MAGNETRON_FILAMENT_CURRENT, data_word);
	eeprom_write_attempt++;
	if (eeprom_write_attempt >= 10) {
	  eeprom_write_failure_count++;
	  break;
	}
      }
      if (eeprom_write_complete) {
	itemp = data_word;
	vtemp = GenerateFilamentVprog(itemp);
	SetPowerSupplyTarget(&ps_filament, vtemp, itemp);
      }

      
      /*
      ps_filament_config_ram_copy[EEPROM_V_SET_POINT] = ps_filament.v_command_set_point;
      ps_filament_config_ram_copy[EEPROM_I_SET_POINT] = ps_filament.i_command_set_point;
      _wait_eedata();
      _erase_eedata(EE_address_ps_filament_config_in_EEPROM, _EE_ROW);
      _wait_eedata();
      _write_eedata_row(EE_address_ps_filament_config_in_EEPROM, ps_filament_config_ram_copy);
      */
      break;

    case CMD_CLEAR_PROCESSOR_RESET_DATA:
      // DPARKER using this command to reset "reset data"
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
      break;


    }
  
  // Echo the command that was recieved back to the controller
  SendCommand(return_command_byte, command_string.register_byte, return_data_word);
  
  command_string.data_state = COMMAND_BUFFER_EMPTY;
}



unsigned int ReadFromRam(unsigned int ram_location) {
  unsigned long temp_long;
  unsigned int data_return;
  unsigned long long int temp_long_long;
  switch (ram_location) 
    {

      // Magnet Supply Data
    case RAM_READ_MAGNETRON_MAGNET_VOLTAGE_ADC:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_magnet, ps_magnet.v_adc_reading);
      break;
      
    case RAM_READ_MAGNETRON_MAGNET_CURRENT_ADC:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_magnet, ps_magnet.i_adc_reading);
      break;
      
    case RAM_READ_MAGNETRON_MAGNET_CURRENT_SET_POINT:
      data_return = ps_magnet.i_command_set_point;
      break;


      // Filament Supply Data
    case RAM_READ_MAGNETRON_FILAMENT_VOLTAGE_ADC:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_filament, ps_filament.v_adc_reading);
      break;
      
    case RAM_READ_MAGNETRON_FILAMENT_CURRENT_ADC:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_filament, ps_filament.i_adc_reading);
      break;
      
    case RAM_READ_MAGNETRON_FILAMENT_VOLTAGE_SET_POINT:
      data_return = ps_filament.v_command_set_point;
      break;

    case RAM_READ_MAGNETRON_FILAMENT_CURRENT_SET_POINT:
      data_return = ps_filament.i_command_set_point;
      break;

    case RAM_READ_MAGNETRON_FILAMENT_CONTROL_MODE:
      data_return = magnetron_filament_control_mode;
      break;

    case RAM_READ_MAGNETRON_FILAMENT_OFF_RESISTANCE:
      data_return = magnetron_filament_off_resistance;
      break;

    case RAM_READ_MAGNETRON_FILAMENT_ON_RESISTANCE:
      data_return = magnetron_filament_on_resistance;
      break;

    case RAM_READ_MAGNETRON_FILAMENT_MEASURED_RESISTANCE:
      data_return = magnetron_filament_resistance_measurement;
      break;

      // Lambda Mode A Data
    case RAM_READ_HV_LAMBDA_SET_POINT_MODE_A:
      data_return = ps_hv_lambda_mode_A.v_command_set_point;
      break;


      // Lambda Mode B Data
    case RAM_READ_HV_LAMBDA_SET_POINT_MODE_B:
      data_return = ps_hv_lambda_mode_B.v_command_set_point;
      break;

      
      // Thyratron Cathode Heater data
    case RAM_READ_THYR_CATH_HTR_VOTAGE_SET_POINT:
      data_return = ps_thyr_cathode_htr.v_command_set_point;
      break;
      
    case RAM_READ_THYR_CATH_HTR_VOTAGE_ADC:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_thyr_cathode_htr, ps_thyr_cathode_htr.v_adc_reading);
      break;


      // Thyratron Reservoir Heater data
    case RAM_READ_THYR_RESER_HTR_VOLTAGE_SET_POINT:
      data_return = ps_thyr_reservoir_htr.v_command_set_point;
      break;
      
    case RAM_READ_THYR_RESER_HTR_VOLTAGE_ADC:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_thyr_reservoir_htr, ps_thyr_reservoir_htr.v_adc_reading);
      break;
      

      // PULSE MODE A DATA
    case RAM_READ_PULSE_MODE_A_FILTERED_CURRENT:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_magnetron_mode_A, ps_magnetron_mode_A.i_adc_reading);
      break;

    case RAM_READ_PULSE_MODE_A_FILTERED_VOLTAGE:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_magnetron_mode_A, ps_magnetron_mode_A.v_adc_reading);
      break;

    case RAM_READ_PULSE_MODE_A_MAX_CURRENT:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_magnetron_mode_A, ps_magnetron_mode_A.i_adc_max_reading);
      break;

    case RAM_READ_PULSE_MODE_A_MIN_CURRENT:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_magnetron_mode_A, ps_magnetron_mode_A.i_adc_min_reading);
      break;

   case RAM_READ_PULSE_MODE_A_MAX_VOLTAGE:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_magnetron_mode_A, ps_magnetron_mode_A.v_adc_max_reading);
      break;

    case RAM_READ_PULSE_MODE_A_MIN_VOLTAGE:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_magnetron_mode_A, ps_magnetron_mode_A.v_adc_min_reading);
      break;


    // PULSE MODE B DATA
    case RAM_READ_PULSE_MODE_B_FILTERED_CURRENT:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_magnetron_mode_B, ps_magnetron_mode_B.i_adc_reading);
      break;

    case RAM_READ_PULSE_MODE_B_FILTERED_VOLTAGE:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_magnetron_mode_B, ps_magnetron_mode_B.v_adc_reading);
      break;

    case RAM_READ_PULSE_MODE_B_MAX_CURRENT:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_magnetron_mode_B, ps_magnetron_mode_B.i_adc_max_reading);
      break;

    case RAM_READ_PULSE_MODE_B_MIN_CURRENT:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_magnetron_mode_B, ps_magnetron_mode_B.i_adc_min_reading);
      break;      

    case RAM_READ_PULSE_MODE_B_MAX_VOLTAGE:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_magnetron_mode_B, ps_magnetron_mode_B.v_adc_max_reading);
      break;     

    case RAM_READ_PULSE_MODE_B_MIN_VOLTAGE:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_magnetron_mode_B, ps_magnetron_mode_B.v_adc_min_reading);
      break;


      // Magnetron Data
    case RAM_READ_PREVIOUS_PULSE_MAGNETRON_VOLTAGE_ADC:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_magnetron_mode_A, pulse_magnetron_voltage_adc_reading);
      break;
      
    case RAM_READ_PREVIOUS_PULSE_MAGNETRON_CURRENT_ADC:
      data_return = ReturnPowerSupplyADCScaledCurrent(&ps_magnetron_mode_A, pulse_magnetron_current_adc_reading);
      break;



      // Counter Data 
    case RAM_READ_ARC_COUNTER_PERSISTENT_HIGH_WORD:
      temp_long = arc_counter_persistent;
      temp_long = temp_long >> 16;
      data_return = temp_long;
      break;
      
    case RAM_READ_ARC_COUNTER_PERSISTENT_LOW_WORD:
      temp_long = arc_counter_persistent;
      data_return = temp_long;
      break;
      
    case RAM_READ_ARC_COUNTER_THIS_HV_ON:
      data_return = arc_counter_this_hv_on;
      break;
      
    case RAM_READ_PULSE_COUNTER_PERSISTENT_WORD_3_MSB:
      temp_long_long = pulse_counter_persistent;
      temp_long_long = temp_long_long >> 48;
      data_return = temp_long_long;
      break;
      
    case RAM_READ_PULSE_COUNTER_PERSISTENT_WORD_2:
      temp_long_long = pulse_counter_persistent;
      temp_long_long = temp_long_long >> 32;
      data_return = temp_long_long;
      break;
      
    case RAM_READ_PULSE_COUNTER_PERSISTENT_WORD_1:
      temp_long_long = pulse_counter_persistent;
      temp_long_long = temp_long_long >> 16;
      data_return = temp_long_long;
      break;
      
    case RAM_READ_PULSE_COUNTER_PERSISTENT_WORD_0_LSB:
      temp_long_long = pulse_counter_persistent;
      data_return = temp_long_long;
      break;
      
    case RAM_READ_PULSE_COUNTER_THIS_HV_ON_HIGH_WORD:
      temp_long = pulse_counter_this_hv_on;
      temp_long = temp_long >> 16;
      data_return = temp_long;
      break;
      
    case RAM_READ_PULSE_COUNTER_THIS_HV_ON_LOW_WORD:
      temp_long = pulse_counter_this_hv_on;
      temp_long &= 0x0000FFFF;
      data_return = temp_long;
      break;
      

      // Power information
    case RAM_READ_AVERAGE_PULSE_ENERGY:
      data_return = average_energy_per_pulse_milli_joules;
      break;

    case RAM_READ_AVERAGE_PULSE_FREQUENCY:
      data_return = prf_deciherz;
      break;

    case RAM_READ_AVERAGE_MAGNETRON_INPUT_POWER:
      data_return = average_output_power_watts;
      break;

    case RAM_READ_HV_LAMBDA_VPEAK_ADC:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_hv_lambda_mode_A, ps_hv_lambda_mode_A.v_adc_reading);
      break;

    case RAM_READ_HV_LAMBDA_VMON_ADC:
      data_return = ReturnPowerSupplyADCScaledVoltage(&ps_hv_lambda_mode_B, ps_hv_lambda_mode_B.v_adc_reading);
      break;
      
    case RAM_READ_FILAMENT_OFFSET:
      data_return = (unsigned int)look_up_offset;
      break;

    case RAM_READ_OPERATION_MODE:
      data_return = operation_mode;
      break;

    case RAM_READ_SCALE_INTERLEAVED:
      data_return = scale_interleaved;
      break;

    case RAM_READ_SCALE_LOW_ENERGY:
      data_return = scale_low_energy;
	break;

      // Fault information
    case RAM_READ_DEBUG_STATUS_REG:
      data_return = debug_status_register;
      break;

    case RAM_READ_FAULT_MAGNETRON_FAULT_REG:
      data_return = faults_magnetron_fault_reg;
      break;

    case RAM_READ_FAULT_MAGNETRON_STATUS_REG:
      data_return = faults_magnetron_status_reg;
      break;

    case RAM_READ_FAULT_MAGNETRON_WARNING_REG:
      data_return = faults_magnetron_warning_reg;
      break;

    case RAM_READ_FAULT_HIGH_VOLTAGE_FAULT_REG:
      data_return = faults_high_voltage_fault_reg;
      break;

    case RAM_READ_FAULT_HIGH_VOLTAGE_STATUS_REG:
      data_return = faults_high_voltage_status_reg;
      break;

    case RAM_READ_FAULT_HIGH_VOLTAGE_WARNING_REG:
      data_return = faults_high_voltage_warning_reg;
      break;

    case RAM_READ_FAULT_THYRATRON_FAULT_REG:
      data_return = faults_thyratron_fault_reg;
      break;

    case RAM_READ_FAULT_THYRATRON_STATUS_REG:
      data_return = faults_thyratron_status_reg;
      break;

    case RAM_READ_FAULT_THYRATRON_WARNING_REG:
      data_return = faults_thyratron_warning_reg;
      break;

    case RAM_READ_FAULT_CONTROL_BOARD_FAULT_REG:
      data_return = faults_control_board_fault_reg;
      break;

    case RAM_READ_FAULT_CONTROL_BOARD_STATUS_REG:
      data_return = faults_control_board_status_reg;
      break;

    case RAM_READ_FAULT_CONTROL_BOARD_WARNING_REG:
      data_return = faults_control_board_warning_reg;
      break;
      
      
      // Read Status
    case RAM_READ_STATE:
      data_return = control_state;
      break;
      
    case RAM_READ_VERSION:
      data_return = VERSION_NUMBER;
      break;

    case RAM_READ_LOCAL_REMOTE_MAGNET_CURRENT_CONTROL:
      data_return = ram_config_set_magnetron_magnet_current_from_GUI;
      break;
      
    case RAM_READ_MAGNETRON_TYPE:
#ifdef __MG7095
      data_return = 7095;
#else
      data_return = 5193;
#endif
      break;
   
    case RAM_READ_CONTROL_BOARD_TYPE:
#ifndef __A36760
      data_return = 34760;
#else
      data_return = 36760;
#endif
      break;
      
      
      // Read Bedug Counters
      
    case RAM_READ_COUNT_MAGNETRON_CURRENT_ADC_GLITCH:
      data_return = eeprom_write_failure_count;
      break;
      
    case RAM_READ_COUNT_MAGNETRON_VOLTAGE_ADC_GLITCH:
      data_return = eeprom_read_failure_count;
      break;
      
    case RAM_READ_COUNT_I2C_BUS_ERROR:
      data_return = pac_2_adc_reading;
      break;
      
    case RAM_READ_COUNT_SPI1_BUS_ERROR:
      data_return = default_pac_2_adc_reading;
      break;
      
    case RAM_READ_COUNT_SPI2_BUS_ERROR:
      data_return = pac_1_adc_reading;
      break;
      
    case RAM_READ_COUNT_EXTERNAL_ADC_FALSE_TRIGGER:
      data_return = global_debug_counter.external_adc_false_trigger;
      break;

    case RAM_READ_COUNT_LTC2656_WRITE_ERROR:
      data_return = global_debug_counter.LTC2656_write_error;
      break;
      
    case RAM_READ_COUNT_SETPOINT_NOT_VALID:
      data_return = global_debug_counter.setpoint_not_valid;
      break;
      
    case RAM_READ_COUNT_SCALE16BIT_SATURATION:
      data_return = global_debug_counter.scale16bit_saturation;
      break;

    case RAM_READ_LAST_OSCCON_BEFORE_CRASH:
      //data_return = last_osccon;
      data_return = timing_error_int1_count;
      break;

    case RAM_READ_PULSE_LATCH_RESET_ERROR:
      data_return = global_debug_counter.pulse_latch_reset_error;
      break;

    case RAM_READ_COUNT_REVERSESCALE16BIT_SATURATION:
      data_return = global_debug_counter.reversescale16bit_saturation;
      break;

    case RAM_READ_COUNT_LVD_INTERRUPT:
      data_return = lvdinterrupt_counter;
      break;

    case RAM_READ_LAST_ACTION_BEFORE_CRASH:
      data_return = previous_last_action;
      break;

    case RAM_READ_COUNT_PROCESSOR_CRASH:
      data_return = processor_crash_count;
      break;
      
    case RAM_READ_COUNT_TIMING_ERROR_INT1:
      //data_return = timing_error_int1_count;
      if (PIN_GANTRY_PORTAL_SELECT == ILL_GANTRY_MODE) {
	data_return = 1;
      } else {
	data_return = 0;
      }

      break;


    case RAM_READ_HIGH_TARGET_CURRENT_SET_POINT:
      data_return = linac_high_energy_target_current_set_point;
      break;

    case RAM_READ_LOW_TARGET_CURRENT_SET_POINT:
      data_return = linac_low_energy_target_current_set_point;
      break;

    case RAM_READ_HIGH_TARGET_CURRENT_READING:
      data_return = linac_high_energy_target_current_adc_reading;
      break;

    case RAM_READ_LOW_TARGET_CURRENT_READING:
      data_return = linac_low_energy_target_current_adc_reading;
      break;

    case RAM_READ_TARGET_ADJUST_MAX_PULSES:
      data_return = low_energy_target_current_startup_adjust_decay_time_pulses;
      break;
      
    case RAM_READ_TARGET_ADJUST_MAX_MAGNITUDE:
      data_return = max_low_energy_target_current_startup_adjust_initital_value;
      break;

    case RAM_READ_TARGET_ADJUST_MAX_COOLDOWN:
      data_return = low_energy_target_current_startup_max_cooldown;
      break;

    case RAM_READ_TARGET_ADJUST_INITIAL_MAGNITUDE:
      data_return = low_energy_target_current_startup_adjust_initital_value;
      break;

    default:
      data_return = 0;
      break;
      
    }
  
  return data_return;
}


unsigned int MakeCRC(unsigned char command_byte, unsigned char register_byte, unsigned int data_word) {
  unsigned int crc;
  crc = SYNC_BYTE_1 + SYNC_BYTE_2 + SYNC_BYTE_3_SEND;
  crc += command_byte + register_byte;
  crc += (data_word >> 8);
  crc += (data_word & 0x00FF);
  
  return crc;
  // DPAKRER Make real CRC
}


unsigned char CheckCRC(unsigned int crc) {
  unsigned int crcCheck;
  // At the moment the CRC is just a checksum
  crcCheck = SYNC_BYTE_1 + SYNC_BYTE_2 + SYNC_BYTE_3_RECEIVE; 
  crcCheck += command_string.command_byte + command_string.register_byte;
  crcCheck += command_string.data_high_byte + command_string.data_low_byte;
  if (crcCheck == crc) {
    return 1;
  } else {
    return 0;
  }
  // DPARKER make Real CRC

}



void _ISRNOPSV _U1RXInterrupt(void) {
  _U1RXIF = 0;
  while (U1STAbits.URXDA) {
    Buffer64WriteByte(&uart1_input_buffer, U1RXREG);
  }
}



void _ISRNOPSV _U1TXInterrupt(void) {
  _U1TXIF = 0;
  while ((!U1STAbits.UTXBF) && (Buffer64BytesInBuffer(&uart1_output_buffer))) {
    /*
      There is at least one byte available for writing in the outputbuffer and the transmit buffer is not full.
      Move a byte from the output buffer into the transmit buffer
    */
    U1TXREG = Buffer64ReadByte(&uart1_output_buffer);
  }
}


unsigned int GenerateMagnetronVprog(unsigned int iprog) {
  return 42000;
  // DPARKER write this function
}



unsigned int GenerateLambdaIprog(unsigned int vprog) {
  return 0;
}

 
unsigned int GenerateMagnetVprog(unsigned int iprog) {
  unsigned int temp;
  temp = Scale16Bit(iprog,10240); // temp = iprog*1.25
  return temp;
}


unsigned int GenerateFilamentIprog(unsigned int vprog) {
  unsigned int temp;
  temp = Scale16Bit(vprog, 5425);
  return temp;
}

unsigned int GenerateFilamentVprog(unsigned int iprog) {
  unsigned int temp;
  temp = Scale16Bit(iprog, 19363);
  return temp;
}


unsigned int ReturnPowerSupplyADCScaledVoltage(POWERSUPPLY* ptr, unsigned int value) {
  //value = value - ptr->v_adc_cal_offset;
  if (ptr->v_adc_cal_offset > 0) {
    // the offset is positive
    // Check to make sure we do not overflow below 0x0000
    if (ptr->v_adc_cal_offset >= value) {
      value = 0;
    } else {
      value = value - ptr->v_adc_cal_offset;
    }
  } else {
    // The offset is negative
    // Check to make sure we do not overflow above 0xFFFF
    if ((0xFFFF + ptr->v_adc_cal_offset) <= value) {
      value = 0xFFFF;
    } else {
      value = value - ptr->v_adc_cal_offset;
    }
  }
  value = ReverseScale16Bit(value, ptr->v_adc_cal_gain);
  value = ReverseScale16Bit(value, ptr->v_adc_scale);
  return value;
}

unsigned int ReturnPowerSupplyADCScaledCurrent(POWERSUPPLY* ptr, unsigned int value) {
  if (ptr->i_adc_cal_offset > 0) {
    // Check to make sure we do not overflow below 0x0000
    // Our offset is 
    if (ptr->i_adc_cal_offset > value) {
      value = 0;
    } else {
      value = value - ptr->i_adc_cal_offset;
    }
  } else {
    // Check to make sure we do not overflow above 0xFFFF
    if ((0xFFFF + ptr->i_adc_cal_offset) < value) {
      value = 0xFFFF;
    } else {
      value = value - ptr->i_adc_cal_offset;
    }
  }
  value = ReverseScale16Bit(value, ptr->i_adc_cal_gain);
  value = ReverseScale16Bit(value, ptr->i_adc_scale);
  return value;
}
 

unsigned int ReverseScale16Bit(unsigned int value, unsigned int scale) {
  unsigned long temp;
  temp = value;
  temp = temp << 13;
  temp /= scale;
  if (temp > 0x0000FFFF) {
    temp = 0x0000FFFF;
    global_debug_counter.reversescale16bit_saturation++;
  }
  temp = temp & 0xFFFF;
  return temp;
}

void CalculatePRFDivider(unsigned int requested_prf) {
  if (requested_prf == 0) {
    prf_mult = 1;
    prf_divider = 1;
  } else if (requested_prf >= 400) {
    prf_mult = 1;
    prf_divider = 1;
  } else if (requested_prf >= 350) {
    prf_mult = 7;
    prf_divider = 8;
  } else if (requested_prf >= 320) {
    prf_mult = 4;
    prf_divider = 5;
  } else if (requested_prf >= 300) {
    prf_mult = 3;
    prf_divider = 4;
  } else if (requested_prf >= 266) {
    prf_mult = 2;
    prf_divider = 3;
  } else if (requested_prf >= 200) {
    prf_mult = 1;
    prf_divider = 2;
  } else if (requested_prf >= 160) {
    prf_mult = 2;
    prf_divider = 5;
  } else if (requested_prf >= 133) {
    prf_mult = 1;
    prf_divider = 3;
  } else if (requested_prf >= 100) {
    prf_mult = 1;
    prf_divider = 4;
  } else if (requested_prf >= 80) {
    prf_mult = 1;
    prf_divider = 5;
  } else if (requested_prf >= 66) {
    prf_mult = 1;
    prf_divider = 6;
  } else if (requested_prf >= 50) {
    prf_mult = 1;
    prf_divider = 8;
  } else if (requested_prf >= 40) {
    prf_mult = 1;
    prf_divider = 10;
  } else if (requested_prf >= 33) {
    prf_mult = 1;
    prf_divider = 12;
  } else if (requested_prf >= 25) {
    prf_mult = 1;
    prf_divider = 16;
  } else if (requested_prf >= 20) {
    prf_mult = 1;
    prf_divider = 20;
  } else if (requested_prf >= 16) {
    prf_mult = 1;
    prf_divider = 25;
  } else if (requested_prf >= 10) {
    prf_mult = 1;
    prf_divider = 40;    
  } else if (requested_prf >= 5) {
    prf_mult = 1;
    prf_divider = 80;    
  } else if (requested_prf >= 4) {
    prf_mult = 1;
    prf_divider = 100;    
  } else if (requested_prf >= 2) {
    prf_mult = 1;
    prf_divider = 200;    
  } else {
    prf_mult = 1;
    prf_divider = 400;    
  }
}
