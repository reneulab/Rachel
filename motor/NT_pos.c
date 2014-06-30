#include "NT_pos.h"
#include "NT_motor.h"
#include "epos.h"
#include "canopen/myCan.h"
#include "canopen/ntcan.h"
#include "canopen/NT_PDO.h"

#include <inttypes.h>
#include <math.h>

extern NTCAN_HANDLE motor_pdo_handle;
extern NTCAN_HANDLE motor_cfg_handle;

int32_t  _pos_goto(int32_t pos_l, int32_t pos_r) {
	int32_t err = 0; 
	CMSG left[2];
	CMSG new_left;
	CMSG right[2];
	CMSG new_right;
    uint32_t target_left[2] = 
		{ pos_l, Switch_On_And_Enable_Operation};

  	
	uint32_t target_right[2] = 
		{ pos_r, Switch_On_And_Enable_Operation};
// Left commands    
   	changeNTCAN(PDO_RX1_ID+MOTOR_EPOS_L_ID,target_left[0],&left[0]);
    changeNTCAN(PDO_RX1_ID+MOTOR_EPOS_L_ID,target_left[1],&left[1]);
	new_left.id = PDO_RX1_ID+MOTOR_EPOS_R_ID; 
	new_left.len = 8;
	new_left.data[0] = left[1].data[0]; 
	new_left.data[1] = left[1].data[1];
	new_left.data[2] = left[1].data[2];
	new_left.data[3] = left[1].data[3];
	new_left.data[4] = left[0].data[0];
	new_left.data[5] = left[0].data[1];
	new_left.data[6] = left[0].data[2];
	new_left.data[7] = right[0].data[3];
	err |= writeNTCAN(motor_pdo_handle, 2, &new_left);
// Right commands        
	changeNTCAN(PDO_RX1_ID+MOTOR_EPOS_R_ID, (uint32_t)target_right[0], &right[0]);
    changeNTCAN(PDO_RX1_ID+MOTOR_EPOS_R_ID, (uint32_t)target_right[1], &right[1]);
	new_right.id = PDO_RX1_ID+MOTOR_EPOS_R_ID; 
	new_right.len = 8;
	new_right.data[0] = right[1].data[0]; 
	new_right.data[1] = right[1].data[1];
	new_right.data[2] = right[1].data[2];
	new_right.data[3] = right[1].data[3];
	new_right.data[4] = right[0].data[0];
	new_right.data[5] = right[0].data[1];
	new_right.data[6] = right[0].data[2];
	new_right.data[7] = right[0].data[3];
	err |= writeNTCAN(motor_pdo_handle, 2,&new_right);
	return err; 
}


int32_t  pos_straight(int32_t length) {
	int32_t err = 0; 
	_pos_goto(length, length);
	return err; 
	
}


int32_t pos_rotate_rad(double rad) {
	int32_t length = (int32_t)rad*295*4000/(2*3.14159);
	int32_t err = 0;
	err = _pos_goto(length, length); 
// left goes backward in posetiv direction
	return err; 
}

int32_t pos_rotate_grad(double grad) {
	int32_t err = 0; 
	err = pos_rotate_rad(2*M_PI*grad/360.0);
	return err; 
}


int32_t ppos_read(int32_t* pos_left, int32_t* vel_left, int32_t* pos_right, int32_t* vel_right) {
	CMSG f;
	int16_t status = 0;
	int32_t ret = PDO_read(motor_pdo_handle, &f);
	switch(f.id) {

		case(PDO_TX2_ID + MOTOR_EPOS_R_ID):
			printf("in 1st case \n");
			*pos_right = ((uint32_t)f.data[0]<<0) | ((uint32_t)f.data[1]<<8) | ((uint32_t)f.data[2]<<16) | ((uint32_t)f.data[3]<<24);
			*vel_right = 0; //((uint32_t)f.data[4]<<0) | ((uint32_t)f.data[5]<<8) | ((uint32_t)f.data[6]<<16) | ((uint32_t)f.data[7]<<24);
			break;

		case(PDO_TX2_ID + MOTOR_EPOS_L_ID):
			printf("in 2nd case \n");
	 		*pos_left = ( ((uint32_t)f.data[0]<<0) | ((uint32_t)f.data[1]<<8) | ((uint32_t)f.data[2]<<16) | ((uint32_t)f.data[3]<<24) );
			*vel_left = 0; //( ((uint32_t)f.data[4]<<0) | ((uint32_t)f.data[5]<<8) | ((uint32_t)f.data[6]<<16) | ((uint32_t)f.data[7]<<24) );
			break;
	
		case(PDO_TX1_ID + MOTOR_EPOS_L_ID):
			printf("in 3nd case \n"); 
			status = ((uint16_t)f.data[0]<<0) | ((uint16_t)f.data[1]<<8);
			break;

		case(PDO_TX1_ID + MOTOR_EPOS_R_ID):
			status = ((uint16_t)f.data[0]<<0) | ((uint16_t)f.data[1]<<8);
			printf("Right status = %x\n",status);
			break;
	}
	return ret;
}
 
