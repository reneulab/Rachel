#include "ntcan.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "NT_SDO.h"

#define CANOPEN_ERROR	-1
#define CANOPEN_TIMEOUT -2
#define CANOPEN_BROADCAST_ID 0x00

//Canopen Identifier masks
#define COB_MASK	0x7ff
#define COB_ID_MASK	0x07f
#define COB_FUNC_MASK 	0x780


/***************************************************************/
/// Sets up a NTCAN Device and links ID array to that device 
/// IN: Baud rate for device, flag for mode
/// IN: Pointer to ID array for to be linked to NTCAN device	
/// IN: (1st element is the length of array)	
/// IN: number of physical nets	
/// IN: max size of rx buffer, timeout for rx
/// IN: max size of tx buffer, timeout for tx
/// OUT: handle if success, 0xFFFF if error
/***************************************************************/

int32_t initNTCAN(uint32_t baud,  uint32_t flags,
	          int32_t ID[],   int32_t net,			
                  int32_t rxSize, int32_t rxTime,
	          int32_t TxSize, int32_t txTime);

/***************************************************************/
/// Closes a NTCAN Device
/// IN: Handle of device to close 
/// IN: ID array for IDs linked to handle
/// IN: (1st element is the length of array)
/// OUT: 0 if success, 1 if error	
/***************************************************************/

int32_t closeNTCAN(NTCAN_HANDLE handle,int32_t ID[]);



/***************************************************************/
/// Reads data from NTCAN device and prints it to screen
/// IN: Handle of NTCAN device
/// IN: Buffer to store data read
/// IN: Length of data to read in bytes
/// OUT: 0 if success, 1 if error, 2 if Timeout error
/***************************************************************/

int32_t readNTCAN(NTCAN_HANDLE handle, CMSG *msg, int32_t len);


/***************************************************************/
///  Writes data in msg to NTCAN device 
///  IN: Handle of NTCAN device 
///  IN: Length of message to write in bytes
///  IN: MSG struct with ID of device and length preset	
///  OUT: 0 if success, 1 if error
/***************************************************************/
int32_t writeNTCAN(NTCAN_HANDLE handle,int32_t len, CMSG *msg);


///// Timestamp /////
/***************************************************************/
/// Reads data from NTCAN device and prints it to screen
/// IN: Handle of NTCAN device
/// IN: Buffer to store data read
/// IN: Length of data to read in bytes
/// OUT: 0 if success, 1 if error, 2 if Timeout error
/***************************************************************/
int32_t readNTCAN_T(NTCAN_HANDLE handle, CMSG_T *msg, int32_t len);


///// Timestamp /////
/***************************************************************/
///  Writes data in msg to NTCAN device 
///  IN: Handle of NTCAN device 
///  IN: Length of message to write in bytes
///  IN: MSG struct with ID of device and length preset	
///  OUT: 0 if success, 1 if error
/***************************************************************/
int32_t writeNTCAN_T(NTCAN_HANDLE handle,int32_t len, CMSG_T *msg);


/***************************************************************/
/// Converts socket data to NTCAN data
/// IN: id for message
/// IN: pointer to data that needs to be converted
/// IN: pointer to location to store converted data 
/// OUT: socket data changed to correct format         
/*************************************************************/

void changeNTCAN(uint16_t id,uint32_t badData,CMSG *happyData);








