#include "motor/NT_motor.h"
#include "motor/NT_pos.h"
#include "motor/NT_vel.h"
#include "motor/epos.h"
#include "motor/canopen/NT_NMT.h"
#include "motor/canopen/NT_SDO.h"
#include "motor/canopen/NT_PDO.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdbool.h>

///// Used for test /////
#define TIMEOUT 1000
#define NODE 	4

/* 	Order of main 
	motor_init	motor_enable
	YOUR_CODE   (motor_halt is in  velocity mode)
	motor_disable	motor_close
*/

int32_t main(void) {
	
	int32_t err;
	int32_t i = 0; 
/// Used to test timing ///
//	int64_t timeB = 0;
//	int64_t timeA = 0;
//	struct timespec tms; 
/// Used for reading posistion
	int32_t pos_left = 1234;
	int32_t vel_left =  1234;
	int32_t pos_right = 1234;
	int32_t vel_right = 1234;
	double motor_rad = 0;
/// used for init ///
	int32_t pdo_filters[3] = {
				0x03,				/// Number of elements in array 	
				PDO_TX1_ID + NODE, 	/// For  each epos ID 
				PDO_TX2_ID + NODE 	/// add 2 elements in filter
			};						/// PDO_TX2 and PDO_TX1
		
	int32_t cfg_filters[4] = {
				0x04,			/// Number of elements in array
				0x00,			/// zero buffer
				NMT_TX + NODE,  /// for each epos ID
				SDO_TX + NODE	/// add 2 elements in filter
			};					/// NMT_TX and SDO_TX
/// Array of epos ID defined by DIP switches on controller
/// DIP_1=1 DIP_2=2 DIP_3=4 DIP_4=8... example DIP_1+DIP_2 = 3
	int32_t id[2] = {0x02,NODE};  /// first number in array in # of elemnts


/********************************************************/
	printf("          Running Program! \n");

	err = motor_init(id,pdo_filters,cfg_filters);
	if( err != 0) 
	   { printf("ERROR ON INIT %d \n",err); }
	printf("          Init!!!!!!!!!!!! \n");
	sleep(4);
/*********************************************************/
	err = motor_enable(id);
	if(err != 0)
	   { printf("ERROR ON ENABLE %d \n",err); }
	sleep(1);
	printf("          Starting now!!!! \n");
/*********************************************************/

	for(i=0;i<2;i++) {
		sleep(2);
		motor_rad =(3.14159/2);
		err = epos_Position_Mode_Setting_Value(NODE,(295*4000/(2*3.14159))*motor_rad);
//		err |= pos_rotate_rad(id,motor_rad);
		printf("motor rad is %d \n",(int32_t)motor_rad); 
		sleep(2); 
		err |= ppos_read(&pos_left,&vel_left,&pos_right,&vel_right);
		printf("pos left = %d \n",pos_right ); 
		sleep(2);

		motor_rad =(3.14159);
		err = epos_Position_Mode_Setting_Value(NODE,(295*4000/(2*3.14159))*motor_rad);
//		err |= pos_rotate_rad(id,motor_rad);
		printf("motor rad is %d \n",(int32_t)motor_rad); 
		sleep(2); 
		err |= ppos_read(&pos_left,&vel_left,&pos_right,&vel_right);
		printf("2 pos left = %d \n",pos_left ); 
		sleep(2); 
	}		
/********************************************************/
	printf("          Ending!!!!!!!!!! \n");
	printf("          HALT!!!!!!!!!!!! \n  ");
	err = motor_halt(id);
	if(err !=0 )
		{ printf("Error on HALT %d \n",err); }
	sleep(2);

	err = motor_disable(id);
	printf("        DISABLE!!!!!!!!! \n"); 	
	if(err != 0) 
	   	{ printf("error on disable %d \n",err); }
	sleep(4);

	printf("          CLOSE!!!!!!!!!!! \n"); 
	err = motor_close(pdo_filters,cfg_filters);
	if(err != 0) 
	   	{ printf("ERROR ON CLOSE %d \n",err); }
	sleep(2);
	printf("Goodbye </3 %15d \n");

	return 0;
}


/////////////////////// used for timing ///////////////
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
	

