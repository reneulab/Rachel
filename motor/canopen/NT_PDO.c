#include "NT_PDO.h"
#include "printd.h"
#include "myCan.h"
#include <string.h>
#include "ntcan.h"

int PDO_send(NTCAN_HANDLE handle, uint16_t pdo_id, int32_t pos) {
	int32_t len;
	CMSG msg; 

	len = 1;  
	msg.id = pdo_id; 
	msg.len = 8;
// endiness correct canopne alway LSB first 	
	msg.data[0] = pos & 255;
	msg.data[1] = (pos >> 8) & 255; 
	msg.data[2] = (pos >> 16) & 255; 
	msg.data[3] = (pos >> 24) & 255; 
	
	msg.data[4] = 0x00;                 /// 00000000 00010000
	msg.data[5] = 0x00;
	
	msg.data[6] = 0x00;
	msg.data[7] = 0x00;
	printf("PDO_send id %x \n",msg.id);  
	
	printf("PDO_send data[0] %x \n",msg.data[0]); 
	printf("PDO_send data[1] %x \n",msg.data[1]); 
	printf("PDO_send data[2] %x \n",msg.data[2]); 
	printf("PDO_send data[3] %x \n",msg.data[3]); 
	
	printf("PDO_send data[4] %x \n",msg.data[4]); 
	printf("PDO_send data[5] %x \n",msg.data[5]); 
	
	printf("PDO_send data[6] %x \n",msg.data[6]); 
	printf("PDO_send data[7] %x \n",msg.data[7]); 

	return writeNTCAN(handle,len,&msg);
}


int PDO_read(NTCAN_HANDLE handle,CMSG* pdo) {
	// Don't touch callers struct unless we have to
	int32_t i; 
	int32_t err, func;
	int32_t size = 1;
	CMSG f; 

//	f.id = 0x283; 
//	f.len = 8; 

	err = readNTCAN(handle, &f,size); 
	if(err != 0) {
		printf("Read error, or no data \n");
		return err;
	}
	func = f.id & (~0x7f); // COB ID mask 
	printf("len = %d \n",f.len); 
	printf("id = %x \n",f.id); 
	printf("func = %x \n",func); 
//	printf("func =  %x \n", func); 
//	printf("func == SDO_RX %x \n", func == SDO_RX);
//	printf("func == SDO_TX %x \n", func == SDO_TX);
	for(i=0;i<f.len;i++) 
		{ printf("Data[%d] = %x \n", i,f.data[i]); }
	if(func == 0 || func == SDO_RX || func == SDO_TX) {
		// Not a PDO!
		printd(LOG_WARN, "PDO.c recived non pdo-pkg\n");
		return -1;
	}

	// Copy data to return struct
	memcpy(pdo, &f, sizeof(f));
	return 0;
}
