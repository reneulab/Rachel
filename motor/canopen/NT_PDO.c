#include "NT_PDO.h"
#include "printd.h"
#include "myCan.h"
#include <string.h>
#include "ntcan.h"

int PDO_send(NTCAN_HANDLE handle, uint16_t pdo_id, uint8_t len, CMSG *msg) {

	return writeNTCAN(handle,len,msg);
}


int PDO_read(NTCAN_HANDLE handle,CMSG* pdo) {
	// Don't touch callers struct unless we have to
	int32_t i; 
	int32_t err, func;
	CMSG f; 
	err = readNTCAN(handle, &f,1); 
	if(err != 0) {
		printf("Read error, or no data \n");
		return err;
	}
	func = f.id & (~0x7f); // COB ID mask 
	printf("id = %x \n",func); 

//	printf("func =  %x \n", func); 
//	printf("func == SDO_RX %x \n", func == SDO_RX);
//	printf("func == SDO_TX %x \n", func == SDO_TX);
	printf("len = %d \n", f.len);
//	printf("f data = %x \n ",f.data[0]);
/*	for(i=0;i<f.len;i++) 
		{ printf("Data[%d] = %d \n", i,f.data[i]); }
*/	if(func == 0 || func == SDO_RX || func == SDO_TX) {
		// Not a PDO!
		printd(LOG_WARN, "PDO.c recived non pdo-pkg\n");
		return -1;
	}

	// Copy data to return struct
	memcpy(pdo, &f, sizeof(f));
	return 0;
}
