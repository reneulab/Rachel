#include "NT_motor.h"
#include "epos.h"
#include "canopen/NT_NMT.h"
#include "canopen/myCan.h"
#include "canopen/printd.h"
#include "canopen/canopen.h"
#include "canopen/NT_PDO.h"
#include "canopen/ntcan.h"


#include <math.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

#define NET 	0
#define TIMEOUT 1000

int32_t motor_pdo_handle; //!< Process CAN-connection.
int32_t motor_cfg_handle; //!< Configuration CAN-connection.


static int motor_config_node(uint16_t node) {
	int err = 0;
	int num_PDOs;

	// Set Configuration parameters
	err |= epos_Max_Following_Error(node, 200000000);
	err |= epos_Software_Position_Limit(node, -2000000000, 2000000000);
	if( err != 0 ) {
		printd(LOG_FATAL, "Motor: error configuring node %d vel prof, no power?\n", node);
		return err;
	}
	
	if(err != 0) {
		printd(LOG_FATAL, "Motor: error configuring node misc conf%d.\n", node);
		return err;
	}


	// PDO cob id's
	err |= epos_Receive_PDO_n_Parameter(node, 1, PDO_RX1_ID + node);
	err |= epos_Receive_PDO_n_Parameter(node, 2, PDO_RX2_ID + node);
	
	err |= epos_Transmit_PDO_n_Parameter(node, 1, PDO_TX1_ID + node);
	err |= epos_Transmit_PDO_n_Parameter(node, 2, PDO_TX2_ID + node);


	/*** Communication, from pc to epos ***/

	// PDO RX1 target position (used in pos mode)
	num_PDOs = 2;
	Epos_pdo_mapping target_pos[] = {
		{0x2062, 0x00, 32},   // Target Possition
		{0x6040, 0x00, 16}    // Controlword
	};
	err |= epos_Receive_PDO_n_Mapping(node, 1, num_PDOs, target_pos);

	// PDO RX2 target velocity (used in vel mode)
	num_PDOs = 2;
	Epos_pdo_mapping target_vel[] = {
		{0x206B, 0x00, 32},  // Target Velocity
		{0x6040, 0x00, 16}   // Controlword
	};
	err |= epos_Receive_PDO_n_Mapping(node, 2, num_PDOs, target_vel);

	// Disable the rest
	err |= epos_Receive_PDO_n_Mapping(node, 3, 0, NULL);
	err |= epos_Receive_PDO_n_Mapping(node, 4, 0, NULL);


	/*** Communication, from epos to pc ***/

	// PDO TX1 Home
	num_PDOs = 1;
	Epos_pdo_mapping status[] = {
		{0x6041, 0x00, 16}   // Statusword
	};
	err |= epos_Transmit_PDO_n_Mapping(node, 1, num_PDOs, status);

	// PDO TX2 Position and speed
	num_PDOs = 2;
	Epos_pdo_mapping enc[] = {
		{0x6064, 0x00, 32},  // Position Actual value
		{0x606C, 0x00, 32}
	};
	err |= epos_Transmit_PDO_n_Mapping(node, 2, num_PDOs, enc);

	// Disable the rest
	err |= epos_Transmit_PDO_n_Mapping(node, 3, 0, NULL);
	err |= epos_Transmit_PDO_n_Mapping(node, 4, 0, NULL);


	return err;
}


int32_t motor_init(int32_t pdo_filters[],int32_t cfg_filters[]) {
	int err = 0;



// NTCAN STUFF //
	motor_pdo_handle = initNTCAN(NTCAN_BAUD_1000, 
		NTCAN_MODE_NO_INTERACTION,pdo_filters,NET,
		NTCAN_MAX_RX_QUEUESIZE,TIMEOUT,
		NTCAN_MAX_TX_QUEUESIZE,TIMEOUT); 

	motor_cfg_handle = initNTCAN(NTCAN_BAUD_1000, 
		NTCAN_MODE_NO_INTERACTION,cfg_filters,NET,
		NTCAN_MAX_RX_QUEUESIZE,TIMEOUT,
		NTCAN_MAX_TX_QUEUESIZE,TIMEOUT); 


	// Check that we connected OK
	if (motor_pdo_handle == -1 || motor_cfg_handle == -1) {
		printf("Inproper Config CFG \n");
		return MOTOR_ERROR;
	}
// End of NTCAN Stuff //

        // Configure each node
	err |= NMT_change_state(motor_cfg_handle, CANOPEN_BROADCAST_ID, NMT_Enter_PreOperational);
    if (err != 0) {
		printf("Error in NMT change state \n"); 
        return MOTOR_ERROR;
 	}

// Set the default mode
	motor_setmode(Motor_mode_Position);
	if (err != 0) {
		printf("Error in set mode \n`"); 
		return MOTOR_ERROR;        
	}
	
	err |= motor_config_node(MOTOR_EPOS_L_ID);
	if (err != 0) {
		printf("Error in Left config node \n`"); 
		return MOTOR_ERROR;        
	}

  	err |= motor_config_node(MOTOR_EPOS_R_ID);
	if (err != 0) {
		printf("Error in Right config node \n`"); 
		return MOTOR_ERROR;
	}

	return 0;
}


int32_t  motor_close(int32_t pdo_filters[],int32_t cfg_filters[]) {
	int32_t err = 0; 

	err |= closeNTCAN(motor_pdo_handle,pdo_filters);
	err |= closeNTCAN(motor_cfg_handle,cfg_filters);
	return err; 
}


int32_t motor_enable(void) {
	int err = 0;

	err |= NMT_change_state(motor_cfg_handle, CANOPEN_BROADCAST_ID, NMT_Enter_PreOperational);
	err |= epos_Controlword(MOTOR_EPOS_L_ID, Shutdown); // switch_on_disabled -> switch_on_enabled
   	err |= epos_Controlword(MOTOR_EPOS_R_ID, Shutdown);
	err |= epos_Controlword(MOTOR_EPOS_L_ID, Switch_On_And_Enable_Operation);
    err |= epos_Controlword(MOTOR_EPOS_R_ID, Switch_On_And_Enable_Operation);

	// Open PDO-communication
	err |= NMT_change_state(motor_cfg_handle, CANOPEN_BROADCAST_ID, NMT_Start_Node);
	return err;
}


int32_t motor_disable(void) {
	int err = 0;

	// Stop PDO-communication
	err |= NMT_change_state(motor_cfg_handle, CANOPEN_BROADCAST_ID, NMT_Enter_PreOperational);
	err |= epos_Controlword(MOTOR_EPOS_L_ID, Disable_Voltage);
  	err |= epos_Controlword(MOTOR_EPOS_R_ID, Disable_Voltage);
	err |= NMT_change_state(motor_cfg_handle, CANOPEN_BROADCAST_ID, NMT_Stop_Node);

	return err;
}


int motor_halt(void) {
	int err = 0;

	// Stop PDO-communication
	err |= NMT_change_state(motor_cfg_handle, CANOPEN_BROADCAST_ID, NMT_Enter_PreOperational);
	err |= epos_Controlword(MOTOR_EPOS_L_ID, Quickstop);
  	err |= epos_Controlword(MOTOR_EPOS_R_ID, Quickstop);
	err |= NMT_change_state(motor_cfg_handle, CANOPEN_BROADCAST_ID, NMT_Stop_Node);

	return err;
}


int motor_setmode(enum Motor_mode mode) {
	int err = 0;
	err |= epos_Modes_of_Operation(MOTOR_EPOS_L_ID, mode);
    err |= epos_Modes_of_Operation(MOTOR_EPOS_R_ID, mode);
	return err;
}


int motor_position(int32_t pos_l, int32_t pos_r)
{
	int err = 0;
	err |=  epos_Position_Mode_Setting_Value(MOTOR_EPOS_L_ID, pos_l);
	err |=  epos_Position_Mode_Setting_Value(MOTOR_EPOS_R_ID, pos_r);
	return err;
}


/********* Utils: *********/
int motor_mmsec_to_rpm(int mm_per_sec) {
	const double wheel_circumference = (2.0*MOTOR_WHEEL_RADIUS)*M_PI;
	const double mm_per_rot = wheel_circumference/MOTOR_GEAR_RATIO;
	int mm_per_min = 60*mm_per_sec;
	int rpm = mm_per_min/mm_per_rot;  // [mm/min]/[mm/1] = [1/min]
	return rpm;
}

int motor_rpm_to_mmsec(int rpm) {
	const double wheel_circumference = (2.0*MOTOR_WHEEL_RADIUS)*M_PI;
	const double mm_per_rot = wheel_circumference/MOTOR_GEAR_RATIO;
	int mm_per_min = rpm*mm_per_rot;
	return mm_per_min/60.0;
}

int motor_enc_to_mm(int enc) {
	const double wheel_circumference = (2.0*MOTOR_WHEEL_RADIUS)*M_PI;
	const double mm_per_rot = wheel_circumference/MOTOR_GEAR_RATIO;
	int mm = enc*mm_per_rot;
	return mm;
}
