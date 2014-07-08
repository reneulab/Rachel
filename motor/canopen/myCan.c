/* Library for simple use of can fuctions defined in ntcan.h */
/*************************************************/
#include "myCan.h"
#include "ntcan.h"
#include <stdio.h>
/* Used for error checker */
#define CAN_SET_BAUDRATE		1
#define CAN_GET_BAUDRATE		2
#define CAN_OPEN				3
#define CAN_CLOSE				4
#define CAN_ID_ADD				5
#define CAN_ID_DELETE			6
#define CAN_GET_OVERLAPPED_RESULT 	7
#define CAN_TAKE				8
#define CAN_READ				9	
#define CAN_SEND				10
#define CAN_WRITE				11
#define CAN_SENT_T				12
#define CAN_WRITE_T				13
#define CAN_READ_EVENT			14
#define CAN_SEND_EVENT			15
#define CAN_STATUS				16
#define CAN_IO_CTL				17
#define CAN_TAKE_T 				18
#define CAN_READ_T				19
#define CAN_FORMAT_ERROR		20
#define CAN_FORMAT_EVENT		21
#define RTR_ENABLE				0
#define MAX_TIMEOUT    			50


/****************************************************************/
/*             Function: errorCheck		        	*/
/*    command is one of the funtions in ntcan.h                 */
/*    Notation for command is in #define                        */
/*    Out: 1 if error, system should shutdown                   */
/*         2 if error, wait and retry                           */
/*   (only if command canWrite,canWrite_t,canSend               */
/*     canSendEvent, canIdAdd,canIdDelete)                      */
/*					                  	*/
/*         0 if success :-)                                     */      
/*         0xFF if unknown error                                */
/****************************************************************/
int32_t errorCheck( int32_t command,
                   NTCAN_RESULT result) {

   //printf("command: %d,result: %d\n",command,result);
   switch(result) {
      case NTCAN_SUCCESS: 
         return 0; break;
      case NTCAN_INVALID_PARAMETER:  
         printf( "Error, passed invalid parameter\n");
	 return 1; break; 
      case NTCAN_WRONG_DEVICE_STATE:
	   printf( "Error, system is in sleep mode\n"); 
	   return 1; break;
      default: 
         if(command == CAN_OPEN) {
	    switch(result)
	       case NTCAN_INVALID_DRIVER:
 	          printf( "Error, invalid driver\n");
                  return 1; break; 
	       case NTCAN_INVALID_FIRMWARE:
                  printf( "Error, invalid firmware\n");
	          return 1; break;      	  
               case NTCAN_INVALID_HARDWARE:
                  printf( "Error, incompatiable driver\n");
                   return 1; break;
	       case NTCAN_NET_NOT_FOUND:
		  printf( "Error, no net found\n");
    		  return 5; break;
               case NTCAN_INSUFFICIENT_RESOURCES:
		  printf( "Error, decrease handle queue\n"); 
                  return 1; break;  
	 }

         else if(command == CAN_ID_ADD) {
	    switch(result) {
	       case NTCAN_ID_ALREADY_ENABLED:
		  printf( "Error, ID already enabled\n");
		  return 1; break;             
               case NTCAN_INSUFFICIENT_RESOURCES:
		  printf( "Error, ID active with other handles\n"); 
                  return 1; break; }
         }
  	 else if(command == CAN_ID_DELETE) {
    	    switch(result) {
      	       case NTCAN_ID_NOT_ENABLED:
		  printf( "Error, ID not enabled\n");
		  return 1; break; }
         }

 	 else if(command == CAN_IO_CTL) {
 	    switch(result) {
	       case NTCAN_NOT_IMPLEMENTED:
	          printf( "Error, ulCommand error\n"); 
	          return 1; break; }
	 }

	 else if(command == CAN_WRITE) {
	    switch(result) {
	       case NTCAN_TX_ERROR:
		  printf( "TX Error\n"); 
	          return 1; break; 
	       case NTCAN_TX_TIMEOUT:
		  printf( "Error, TX timeout\n"); 
                  return 1; break; }
	 }
         if((command == CAN_WRITE) || (command == CAN_WRITE_T)) {
	    switch(result) {
	       case NTCAN_CONTR_ERR_PASSIVE:
		  printf( "Error, REC or TEC value > 128\n");
                  return 2; break;
	       case NTCAN_CONTR_OFF_BUS:
		  printf( "Transmission Error\n"); 
		  return 2; break; 
	       case NTCAN_CONTR_WARN:
                  printf( "Reception Error\n"); 
		  return 2; break;
	       case NTCAN_ERROR_NO_BAUDRATE:
		  printf( "Error, baudrate is not set\n");
                  return 1; break; 
	       case NTCAN_ERROR_LOM:
		  printf( "Error, CAN is listen only mode\n");
                  return 1; } 
		  break;
         }
         if( (command == CAN_SEND) || 
             (command == CAN_SEND_EVENT) ||
             (command == CAN_ID_ADD) || 
             (command == CAN_ID_DELETE)) {
            switch(result) {
	       case NTCAN_CONTR_BUSY: 
		  printf( "Error, FIFO is full\n");
		  return 2; break; } 
         }
         if( (command == CAN_WRITE) || (command == CAN_WRITE_T) ||
            (command == CAN_READ) || (command == CAN_READ_T) ) {
            switch(result) {
	       case NTCAN_HANDLE_FORCED_CLOSE:
	   	  printf( "Error, CAN was closed in another thread\n"); 
                  return 1; break;
            /* case NTCAN_IO_PENDING:
		  printf("Error, operation not terminated\n"
                  ret_val = 1; break;    only for windows */
               case NTCAN_OPERATION_ABORTED:
		  printf( "Error, operation aborted\n");
		  return 1; break; }
         }
         if( command != CAN_OPEN) {
            switch(result) {
               case NTCAN_INVALID_HANDLE:
		  printf( "Error, invalid hadnle"); 
		  return 1; break; }
         }
      /* if( (command == CAN_GET_OVERLAPPED_RESULT) || 
             (command == CAN_GET_OVERLAPPED_RESULT_T)) {
            switch(result) {
               case NTCAN_IO_INCOMPLETE:
                 printf("Error, operation not teminated\n")
                 ret_val = 1; break; } 
         } only for windows */
         if( (command == CAN_TAKE) || (command == CAN_TAKE_T) ||
             (command == CAN_READ) || (command == CAN_READ_T) ) {
            switch(result) { 
               case NTCAN_ID_NOT_ENABLED:
		  printf( "Error, ID not enabled\n");
                  return 1; break;   
               case NTCAN_PENDING_READ:
  		  printf( "Error, handle in use in about thread\n");
                  return 1; break; }
         }
         if( (command == CAN_IO_CTL) ||
             (command == CAN_SET_BAUDRATE) ||
             (command == CAN_ID_ADD) ||
             (command == CAN_ID_DELETE)) {
            switch(result) {
	       case NTCAN_NOT_SUPPORTED:
		  printf( "Error CAN not supported\n");
                  return 1; break; }
         }
         if( (command == CAN_SEND) || (command == CAN_SEND_EVENT) ||
             (command == CAN_WRITE)) {
            switch(result) {
	       case NTCAN_PENDING_WRITE:
                  printf( "Error, pending write\n");
                  return 1; break; }
         }
         if( (command == CAN_READ) || (command == CAN_READ_T) ||
             (command == CAN_READ_EVENT)) {
            switch(result) {
	      case NTCAN_RX_TIMEOUT:
//	      		printf( "Error, RX timeout\n");
		  		return 2; break; }
         }
         

   }
   printf( "Unknown Error \n"); 
   return 1; 
}

/****************************************************************/
int32_t initNTCAN(uint32_t baud,  uint32_t flags,
		int32_t ID[], 	 int32_t net,
	       	int32_t rxSize, int32_t rxTime,
		int32_t txSize, int32_t txTime)
{
   NTCAN_HANDLE		handle;	// 
   NTCAN_RESULT		result;	// Used for checking for error
   int32_t		i;    	// Index for multiple objects
   int32_t		timeout=0; // Current number of timeouts
/* Open can */ 
   result = canOpen(net,flags,txSize,rxSize,   // Opens device
                    txTime,rxTime,&handle);
   	if(errorCheck(CAN_OPEN,result) != 0)     // Error check
      { return 0xFFFF; }		// returns 1 if error
/* Setting baudrate */   
	result = canSetBaudrate(handle,baud); // sets baudrate
   	if(errorCheck(CAN_SET_BAUDRATE,result) != 0) 
      { return 0xFFFF; }		// returns 1 if error
   result = canGetBaudrate(handle,&baud);  	// Reads buadrate
/* Adding an ID */
	for(i=1; i<ID[0]; i++) {
      do  { result = canIdAdd(handle,ID[i]); 	// Adds ID to handle
            timeout++; 									// reapeat if Timeout	
            if(timeout>MAX_TIMEOUT) 
				{ return 0xFFFF;}  // return if repeated error
          } while( errorCheck(CAN_ID_ADD,result) == 2); 
      if(errorCheck(CAN_ID_ADD,result) != 0) 	// error check
         { return 0xFFFF; }
	 }
/* Flushing FIFO buffer */
   result = canIoctl(handle,NTCAN_IOCTL_FLUSH_RX_FIFO,NULL);   
   if(errorCheck(CAN_IO_CTL,result) != 0) 		// flushing FIFO
      { return 0xFFFF; } 								// error check
   return handle; // returns handle for NTCAN device 
}

/****************************************************************/
/****************************************************************/
int32_t closeNTCAN(NTCAN_HANDLE handle, int32_t ID[])
{
   NTCAN_RESULT	result;
   int32_t 			timeout = 0;
   int32_t			i=0;  
/* Flushing system */  
   result = canIoctl(handle, NTCAN_IOCTL_FLUSH_RX_FIFO, NULL);   
   if(errorCheck(CAN_IO_CTL,result) != 0) 		// error check 
   	{ return 1; }
/* Deleting ID */     
	for(i=1; i<ID[0]; i++) {
// Deletes all ids and retrys if timeout error       
   	do  { 
			result = canIdDelete(handle, ID[i]); 
            timeout++;
            if(timeout>MAX_TIMEOUT) {   // checks for max timeout
               printf( "Max timeout reached, aborting\n");
               return 2; }
      } while( errorCheck(CAN_ID_DELETE,result) == 2);
      if(errorCheck(CAN_ID_DELETE,result) != 0)  // error check
         { return 1; }
   }
/* Closing NTCAN device */
   result = canClose(handle);  // closes device 
   if(errorCheck(CAN_CLOSE,result) != 0) // error check
      { return 1; }
   return 0;
}

/******************************************************************/
int32_t readNTCAN(NTCAN_HANDLE handle, CMSG *msg, int32_t len)
 {
   NTCAN_RESULT 	result;
   int32_t 			timeout=0; 
/* Reading Object of NTCAN device */  
	do {
		result = canRead(handle,msg, &len, 0); 
		timeout++;
/* If timeout error is recieved repeatly then read is aborted */
		if(timeout > MAX_TIMEOUT) {
      		result = canIoctl(handle, NTCAN_IOCTL_ABORT_RX, NULL);
        	if(errorCheck(CAN_IO_CTL,result != 0))
        		{ return 1; } 									// error check
        	printf( "Repeated Timeout, read aborted\n");
        	return 2; 
		}
      } while(errorCheck(CAN_READ,result) == 2); 
	if(errorCheck(CAN_READ,result) != 0)
		{ return 1; }  										// error check
	return 0; 
}

/*****************************************************************/
int32_t writeNTCAN(NTCAN_HANDLE handle, int32_t len, CMSG *msg) 
{
   NTCAN_RESULT	result;
/* Writes to NTCAN device */
result = canWrite(handle, msg, &len,NULL); 
   if(errorCheck(CAN_WRITE,result) != 0)
      { return 1; }
   return 0;
}

/******************************************************************/
int32_t readNTCAN_T(NTCAN_HANDLE handle, CMSG_T *msg, int32_t len)
 {
   NTCAN_RESULT 	result;
   int32_t 			timeout=0; 
/* Reading Object of NTCAN device */  
	do {
		result = canReadT(handle,msg, &len, NULL); 
		timeout++;
/* If timeout error is recieved repeatly then read is aborted */
		if(timeout > MAX_TIMEOUT) {
      		result = canIoctl(handle, NTCAN_IOCTL_ABORT_RX, NULL);
        	if(errorCheck(CAN_IO_CTL,result != 0))
        		{ return 1; } 	// error check
        	printf( "Repeated Timeout, read aborted\n");
        	return 2; 
		}
      } while(errorCheck(CAN_READ,result) == 2); 
	if(errorCheck(CAN_READ,result) != 0)
		{ return 1; }  										// error check
	return 0;	 
}

/*****************************************************************/
int32_t writeNTCAN_T(NTCAN_HANDLE handle, int32_t len, CMSG_T *msg) 
{
   NTCAN_RESULT	result;

result = canWriteT(handle, msg, &len,NULL); 
   if(errorCheck(CAN_WRITE,result) != 0)
      { return 1; }
   return 0;
}

/****************************************************************/
void changeNTCAN( uint16_t id, uint32_t badData, CMSG *happyData) 
{ 
	//changing a uint32_t
	happyData->data[3]= (uint8_t)((badData >> 24) & 255);
	happyData->data[2] = (uint8_t)((badData >> 16) & 255);  
	// into a uint8_t array of 8
	happyData->data[1] = (uint8_t)((badData >> 8) & 255);
	// *** if problem change endians-ness
	happyData->data[0] = (uint8_t)(badData & 255); 
	happyData->len = 4;
	happyData->id = id;
	return; 
	
} 
