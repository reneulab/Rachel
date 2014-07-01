#include "NT_pos.h"
#include "NT_motor.h"
#include "epos.h"
#include "canopen/myCan.h"
#include "canopen/ntcan.h"
#include "canopen/NT_PDO.h"

#include <inttypes.h>
#include <math.h>
extern NTCAN_HANDLE motor_pdo_handle;

int32_t  _pos_goto(int32_t id,int32_t pos) {
	int32_t err = 0; 
	CMSG val[2];
	CMSG msg;
	int32_t tempID; 
    uint32_t target[2] = 
		{ 0x00, Switch_On_And_Enable_Operation};
  	
	tempID = PDO_RX1_ID + id; 
	target[0] = pos; 
	changeNTCAN(tempID,target[0],&val[0]);
    changeNTCAN(tempID,target[1],&val[1]);
	msg.id = tempID; 
	msg.len = 8;
	msg.data[0] = val[0].data[0]; 
	msg.data[1] = val[0].data[1];
	msg.data[2] = val[0].data[2];
	msg.data[3] = val[0].data[3];
	msg.data[4] = val[1].data[0];
	msg.data[5] = val[1].data[1];
	msg.data[6] = val[1].data[2];
	msg.data[7] = val[1].data[3];
	err |= writeNTCAN(motor_pdo_handle, 2, &msg);
	return err; 
}


int32_t  pos_straight(int32_t id[],int32_t length) {
	int32_t err = 0; 
	int8_t i = 0; 
	
	for(i=1;i<id[0];i++) {
		_pos_goto(id[i], length);
	}	
	return err; 
}


int32_t pos_rotate_rad(int32_t id[],double rad) {
	int32_t length = (int32_t)rad*295*4000/(2*3.14159);
	int32_t err = 0;
	int8_t i = 0; 
	
	for(i=1;i<id[0];i++) {
		_pos_goto(id[i], length);
	}	
	return err; 
}

int32_t pos_rotate_grad(int32_t id[],double grad) {
	int32_t err = 0; 
	err = pos_rotate_rad(id,2*M_PI*grad/360.0);
	return err; 
}


int32_t ppos_read(int32_t* pos_left, int32_t* vel_left, int32_t* pos_right, int32_t* vel_right) {
	CMSG f;
	int16_t status = 0;
	int32_t ret = PDO_read(motor_pdo_handle, &f);
	switch(f.id) {

		case(PDO_TX2_ID + 3):
			printf("in 1st case \n");
			*pos_right = ((uint32_t)f.data[0]<<0) | ((uint32_t)f.data[1]<<8) | ((uint32_t)f.data[2]<<16) | ((uint32_t)f.data[3]<<24);
			*vel_right = 0; //((uint32_t)f.data[4]<<0) | ((uint32_t)f.data[5]<<8) | ((uint32_t)f.data[6]<<16) | ((uint32_t)f.data[7]<<24);
			break;

		case(PDO_TX2_ID + 2):
			printf("in 2nd case \n");
	 		*pos_left = ( ((uint32_t)f.data[0]<<0) | ((uint32_t)f.data[1]<<8) | ((uint32_t)f.data[2]<<16) | ((uint32_t)f.data[3]<<24) );
			*vel_left = 0; //( ((uint32_t)f.data[4]<<0) | ((uint32_t)f.data[5]<<8) | ((uint32_t)f.data[6]<<16) | ((uint32_t)f.data[7]<<24) );
			break;
	
		case(PDO_TX1_ID + 2):
			printf("in 3nd case \n"); 
			status = ((uint16_t)f.data[0]<<0) | ((uint16_t)f.data[1]<<8);
			break;

		case(PDO_TX1_ID + 3):
			status = ((uint16_t)f.data[0]<<0) | ((uint16_t)f.data[1]<<8);
			printf("Right status = %x\n",status);
			break;
	}
	return ret;
}
 
