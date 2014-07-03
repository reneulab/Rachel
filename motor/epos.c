#include "NT_motor.h"
#include "epos.h"
#include "canopen/NT_SDO.h"
#include "canopen/ntcan.h"
#include <stdio.h>
	
extern NTCAN_HANDLE motor_cfg_handle;

//// testing PDO set transmission mode
int32_t epos_test(uint16_t node_id, uint32_t cob) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1400 ;
	d.subindex = 0x02;
	d.size = 1;
	d.data = cob;
	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Receive_PDO_n_Parameter(uint16_t node_id, uint8_t n, uint32_t cob) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1400 + n-1;
	d.subindex = 0x01;
	d.size = 4;
	d.data = cob;
	return SDO_write(motor_cfg_handle, &d);
}

/// Look at documentation Epos2_Firmware_Specification 
int32_t epos_Receive_PDO_n_Mapping(uint16_t node_id,  uint8_t n, uint8_t num_objects, Epos_pdo_mapping* objects) {
	int32_t err = 0;
	int8_t i = 0;
	// Set number of objects to zero
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1600 + n-1;
	d.subindex = 0x00;
	d.size = 1;
	d.data = 0;

	err = SDO_write(motor_cfg_handle, &d);
	if (err != 0) {
		return err;
	}

	// Write objects
	d.size = 4;
	for(i=0; i<num_objects; i++) {
		Epos_pdo_mapping obj = objects[i];

		d.subindex = i+1;
		d.data = ((uint32_t)obj.index << 16) | (obj.subindex<<8) | (obj.length);
		err = SDO_write(motor_cfg_handle, &d);
		if (err != 0) {
			return err;
		}
	}

	// Set Correct number of objects
	d.subindex = 0x00;
	d.size = 1;
	d.data = num_objects;
	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Transmit_PDO_n_Parameter(uint16_t node_id, uint8_t n, uint32_t cob) {
	int8_t err = 0; 
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1800 + n-1;
	d.subindex = 0x01;
	d.size = 4;
	d.data = cob;
	err |= SDO_write(motor_cfg_handle, &d);

	return err; 
}


int32_t epos_Transmit_PDO_n_Mapping(uint16_t node_id, uint8_t n, uint8_t num_objects, Epos_pdo_mapping* objects) {
	int32_t err = 0;
	int8_t i = 0;
	// Set number of objects to zero
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x1A00 + n-1;
	d.subindex = 0x00;
	d.size = 1;
	d.data = 0;

	err = SDO_write(motor_cfg_handle, &d);
	if (err != 0) {
		return err;
	}

	// Write objects
	d.size = 4;
	for(i=0; i<num_objects; i++) {
		Epos_pdo_mapping obj = objects[i];

		d.subindex = i+1;
		d.data = ((uint32_t)obj.index << 16) | (obj.subindex<<8) | (obj.length);
		err = SDO_write(motor_cfg_handle, &d);
		if (err != 0) {
			return err;
		}
	}

	// Set Correct number of objects
	d.subindex = 0x00;
	d.size = 1;
	d.data = num_objects;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Miscellaneous_Configuration(uint16_t node_id, uint16_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x2008;
	d.subindex = 0x00;
	d.size = 2;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Controlword(uint16_t node_id, enum Epos_ctrl ctrl) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6040;
	d.subindex = 0x00;
	d.size = 2;
	d.data = ctrl;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Max_Following_Error(uint16_t node_id, int32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6065;
	d.subindex = 0x00;
	d.size= 4;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}

int32_t epos_Position_Mode_Setting_Value(uint16_t node_id, int32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x2062;
	d.subindex = 0x00;
	d.size= 4;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Modes_of_Operation(uint16_t node_id, enum Epos_mode mode) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6060;
	d.subindex = 0x00;
	d.size = 1;
	d.data = mode;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Position_Window(uint16_t node_id, uint32_t enc) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6067;
	d.subindex = 0x00;
	d.size = 4;
	d.data = enc;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Position_Window_Time(uint16_t node_id, uint32_t ms) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6068;
	d.subindex = 0x00;
	d.size = 4;
	d.data = ms;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Target_Position(uint16_t node_id, int32_t enc) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x607A;
	d.subindex = 0x00;
	d.size = 4;
	d.data = enc;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Software_Position_Limit(uint16_t node_id, int32_t min, int32_t max) {
	int32_t err = 0;

	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x607D;
	d.subindex = 0x01;
	d.size = 4;
	d.data = min;

	err = SDO_write(motor_cfg_handle, &d);
	if (err != 0) {
		return err;
	}

	d.subindex = 0x02;
	d.data = max;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Maximal_Profile_Velocity(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x607F;
	d.subindex = 0x00;
	d.size = 4;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Profile_Velocity(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6081;
	d.subindex = 0x00;
	d.size = 4;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Profile_Acceleration(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6083;
	d.subindex = 0x00;
	d.size = 4;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Profile_Deceleration(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6084;
	d.subindex = 0x00;
	d.size = 4;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Quickstop_Deceleration(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6085;
	d.subindex = 0x00;
	d.size = 4;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Motion_Profile_Type(uint16_t node_id, enum Epos_Profile_Type mode) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x6086;
	d.subindex = 0x00;
	d.size = 2;
	d.data = mode;

	return SDO_write(motor_cfg_handle, &d);
}


int32_t epos_Max_Acceleration(uint16_t node_id, uint32_t value) {
	SDO_data d;
	d.nodeid = node_id;
	d.index = 0x60C5;
	d.subindex = 0x00;
	d.size = 4;
	d.data = value;

	return SDO_write(motor_cfg_handle, &d);
}

//uint32_t epos_Read_Position(uint16_t node_id) {
//	SDO_data d;
//	d.nodeid = node_id;
//	d.index = 0x6064;
//	d.subindex = 0x00;
//	d.size = 4;//SDO_type_uint8;
//	SDO_read(motor_cfg_handle, &d);
//	return *(d.data.uint32_t);
//return 0;
//}
