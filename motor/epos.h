#ifndef EPOS_H
#define EPOS_H

#include <inttypes.h>

/**
 * All functions return 0 on success and error codes as defined by SDO_write in
 * libsocketcan on error.
 * */

//uint8_t epos_Error_Register(uint16_t node_id);

int32_t epos_test(uint16_t node_id,uint32_t cob);


int32_t epos_Receive_PDO_n_Parameter(uint16_t node_id, uint8_t n, uint32_t cob);

typedef struct {
	uint16_t index;
	uint8_t subindex;
	uint8_t length;
} Epos_pdo_mapping;

int32_t epos_Receive_PDO_n_Mapping(uint16_t node_id, uint8_t n, uint8_t num_objects, Epos_pdo_mapping* objects);

int32_t epos_Transmit_PDO_n_Parameter(uint16_t node_id, uint8_t n, uint32_t cob);

int32_t epos_Transmit_PDO_n_Mapping(uint16_t node_id, uint8_t n, uint8_t num_objects, Epos_pdo_mapping* objects);

enum Epos_misc_conf {
	Disable_sensor_supervised_by_software = (1<<0),
	Disable_sensor_supervised_by_hardware = (1<<1),
	Meassure_DC_motor_resistance_at_every_change_to_enable = (1<<2),
	Meassure_main_position_sensors_motor_speed_exacting_by_detecting_encoder_pulse_time = (1<<3),
	Meassure_auxiliary_position_sensors_motor_speed_exacting_by_detecting_encoder_pulse_time = (1<<4),
	Polarity_bit = (1<<8)
};

int32_t epos_Miscellaneous_Configuration(uint16_t node_id, uint16_t value);

enum Epos_ctrl {
	Shutdown = 0x06,
	Switch_On = 0x07,
	Switch_On_And_Enable_Operation = 0x0f,
	Disable_Voltage = 0x00,
	Quickstop = 0x02,
	Disable_Operation = 0x07,
	Enable_Operation = 0x0f
};

// Operatin Mode-specific
/*Profile_Position_New_Setpoint32_t = (1<<4),
Profile_Position_Change_Set_Immediately = (1<<5),
Profile_Position_Relative = (1<<6),
Profile_Position_Halt = (1<<8)*/

int32_t epos_Controlword(uint16_t node_id, enum Epos_ctrl ctrl);

int32_t epos_Position_Mode_Setting_Value(uint16_t node_id, int32_t value);

enum Epos_mode {
	Interpolated_Position_Mode = 7,
	Homing_Mode = 6,
	Profile_Velocity_Mode = 3,
	Profile_Position_Mode = 1,
	Position_Mode = 0xFF,
	Velocity_Mode = 0xFE,
	Current_Mode = 0xFD,
	Diagnostic_Mode = 0xFC,
	Master_Encoder_Mode = 0xFB,
	Step_Direction_Mode = 0xFA
};

int32_t epos_Modes_of_Operation(uint16_t node_id, enum Epos_mode mode);

/** 2^32 - 1 = off */
int32_t epos_Position_Window(uint16_t node_id, uint32_t enc);

int32_t epos_Position_Window_Time(uint16_t node_id, uint32_t ms);

int32_t epos_Target_Position(uint16_t node_id, int32_t enc);

/** Default -2^31 to 2^31-1 */
int32_t epos_Software_Position_Limit(uint16_t node_id, int32_t min, int32_t max);

/** Motor [rpm] Default 25000 */
int32_t epos_Maximal_Profile_Velocity(uint16_t node_id, uint32_t value);

/** Motor [rpm] Default 1000 */
int32_t epos_Profile_Velocity(uint16_t node_id, uint32_t value);

/** Motor [rpm/s] Default 10000 */
int32_t epos_Profile_Acceleration(uint16_t node_id, uint32_t value);

/** Motor [rpm/s] Default 10000 */
int32_t epos_Profile_Deceleration(uint16_t node_id, uint32_t value);

/** Motor [rpm/s] Default 10000 */
int32_t epos_Quickstop_Deceleration(uint16_t node_id, uint32_t value);

int32_t epos_Max_Following_Error(uint16_t node_id, int32_t value);

enum Epos_Profile_Type {
	trapezodial_profile = 0,
	sinusoidal_profile = 1
};

int32_t epos_Motion_Profile_Type(uint16_t node_id, enum Epos_Profile_Type mode);

int32_t epos_Max_Acceleration(uint16_t node_id, uint32_t value);

int32_t epos_Read_Position(uint16_t node_id);

#endif
