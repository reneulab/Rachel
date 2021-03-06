#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>

#include "NT_test.h"
#include "motor/NT_motor.h"
#include "motor/NT_pos.h"
#include "motor/NT_vel.h"
#include "motor/epos.h"

#define TIMEOUT 1000

#define NODE  	2
#define PDO_TX1_ID  0x180
#define PDO_TX2_ID  0x180
#define NMT_TX 0x700
#define SDO_TX 0x580

int32_t main(void) {
	
	int32_t err;
	double motor_rad = 0;
	int64_t timeB = 0;
	int64_t timeA = 0;
	struct timespec tms; 
	int32_t i = 0; 
	int32_t pos_left = 1234;
	int32_t vel_left =  1234;
	int32_t pos_right = 1234;
	int32_t vel_right = 1234;
	int32_t pdo_filters[3] = 
				{ 0x03,
				  PDO_TX1_ID + NODE, 
				  PDO_TX2_ID + NODE	};  
	int32_t cfg_filters[4] = 
				{ 0x04, 0x00,
				  NMT_TX + NODE,
				  SDO_TX + NODE }; 

	int32_t id[2] = {1,NODE}; 

	printf("                                     Starting!\n");

	err = motor_init(pdo_filters,cfg_filters,id);
	if( err != 0) 
	   { printf("ERROR ON INIT %d \n",err); }
	printf("                                     Init!\n");
	sleep(1);
/**********************************************************************/
	err = motor_enable(id);
	if(err != 0)
	   { printf("ERROR ON ENABLE %d \n",err); }
	sleep(1);
	printf("                                     Enabled! Starting now!!!!\n");
/********************************************************************/
	sleep(2);
	motor_rad =(3.14159/2);
	err = epos_Position_Mode_Setting_Value(NODE,(295*4000/(2*3.14159))*motor_rad);
	if(err != 0)
	   	{ printf("ERROR ON MODE VALUE %d \n",err); }
	sleep(4);  
/*****************************/
/*	clock_gettime(CLOCK_REALTIME,&tms); 
	timeB = tms.tv_sec *1000000; 
	timeB += tms.tv_nsec/1000; 
	printf("time is %ld \n",(long)timeB); 

for(i=0;i<100;i++) {
	usleep(300);
	motor_rad = motor_rad - (3.14159/180);
	err |= epos_Position_Mode_Setting_Value(MOTOR_EPOS_L_ID,(295*4000/(2*3.14159))*motor_rad);
	if(err != 0)
	   	{ printf("ERROR ON MODE VALUE %d \n",err); }
}*/
/*********************/
//	clock_gettime(CLOCK_REALTIME,&tms); 
//	timeA = tms.tv_sec *1000000; 
//	timeA += tms.tv_nsec/1000; 
//	printf("time is %ld \n",(long)timeA); 
	printf("motor rad is %d \n",(int32_t)motor_rad); 
	err |= ppos_read(&pos_left,&vel_left,&pos_right,&vel_right);
	printf("pos left = %d \n",pos_left ); 
	err |= ppos_read(&pos_left,&vel_left,&pos_right,&vel_right);
	printf("2 pos left = %d \n",pos_left ); 
	if(err != 0)
	   	{ printf("ERROR ON MODE VALUE %d \n",err); }
	sleep(2);

/*******************************************************************************/
	printf("                                     Ending! \n");
	printf(" HALT \n  ");
	err = motor_halt(id);
	if(err !=0 )
		{ printf("Error on HALT %d \n",err); }
	sleep(2);

	err = motor_disable(id);
	printf(" DISABLE \n"); 	
	if(err != 0) 
	   	{ printf("error on disable %d \n",err); }
	sleep(4);

	printf(" CLOSE \n"); 
	err = motor_close(pdo_filters,cfg_filters);
	if(err != 0) 
	   	{ printf("ERROR ON CLOSE %d \n",err); }
	sleep(2);
	printf("                                     Goodbye </3 \n");

	return 0;
}

