#include "NT_SDO.h"
#include "ntcan.h"
#include "myCan.h"
#include "printd.h"
#include "printf.h"


#define SDO_RESPONSE_ERROR     0x80
#define SDO_RESPONSE_WRITE_OK  0x60
#define SDO_RESPONSE_READ_OK   0x40

/** Automatic calculation of ccd-field for requests **/
static uint8_t SDO_calculate_ccd(char rw, int32_t size) {
	uint8_t base = 0x40;
	if(rw == 'w') {
		base = 0x20;
	}

	switch(size) {
		case 1: return base + 0x0F;
		case 2: return base + 0x0B;
		case 3: return base + 0x07;
		case 4: return base + 0x03;
	}
	return 0;
}

int32_t SDO_read(int32_t handle,SDO_data* d) {
	int32_t err = 0xFFFF; 
	int32_t buffer = 5; 
	int8_t i = 0; 
	uint16_t cob;
	uint8_t ccd;
	CMSG SDO_form; 
	CMSG f;

	cob = SDO_TX + d->nodeid;
	ccd = SDO_calculate_ccd('w',d->size);
	// Send write request 
	SDO_form.id = cob;
	SDO_form.len = 8;
	SDO_form.data[0] = ccd;
	SDO_form.data[1] = d->index & 255;
	SDO_form.data[2] = ((d->index >>8) & 255) ;	
	SDO_form.data[3] = d->subindex;
	SDO_form.data[4] = 0x00;  
	SDO_form.data[5] = 0x00;
	SDO_form.data[6] = 0x00;
	SDO_form.data[7] = 0x00;

	
	err = writeNTCAN(handle,1,&SDO_form);
	if (err != 0) {
		printd(LOG_ERROR,"SDO: Could not write handle=%d.\n", handle);
		return err;
	}	
	for(i=0;i<buffer;i++) {	
		err = readNTCAN(handle,&f,1);
		if(err == 0) {
		printf("len is %x \n",f.len);
			printf("id is %x \n",f.id); 
			printf("data 1!! %x \n", f.data[0]); 
			printf("data 2!! %x \n", f.data[1]); 
			printf("data 3!! %x \n", f.data[2]); 
			printf("data 4!! %x \n", f.data[3]); 
			printf("data 5!! %x \n", f.data[4]); 
			printf("data 6!! %x \n", f.data[5]); 
			printf("data 7!! %x \n", f.data[6]); 
			printf("data 8!! %x \n", f.data[7]); 
			return 0; 
		}
	}
	return 1; 
}

int32_t SDO_write(int32_t handle, const SDO_data* d) {

// Their Varibles
	int32_t err = 0xFFFF;
	const int32_t buffer = 5;
	uint16_t cob, cob_r;
	uint8_t ccd, msb, lsb;
	int32_t i; 
	CMSG f;
 	CMSG SDO_form;
	CMSG temp; 	
// Their Define data
	cob = SDO_RX + d->nodeid;
	ccd = SDO_calculate_ccd('w',d->size);
	// Send write request 
	SDO_form.id = cob;
	SDO_form.len = 8;
	SDO_form.data[0] = ccd;
	SDO_form.data[1] = d->index & 255;
	SDO_form.data[2] = ((d->index >>8) & 255) ;	
	SDO_form.data[3] = d->subindex;
	changeNTCAN(cob, d->data, &temp); 
	SDO_form.data[4] = temp.data[0];  
	SDO_form.data[5] = temp.data[1];
	SDO_form.data[6] = temp.data[2];
	SDO_form.data[7] = temp.data[3];
	printf("id %x %x \n",d->index,d->subindex); 	
	printf("id real %x \n",cob); 	
	printf("Temp data[4] %x \n",SDO_form.data[4]); 	
	printf("Temp data[5] %x \n",SDO_form.data[5]); 	
	printf("Temp data[6] %x \n",SDO_form.data[6]); 	
	printf("Temp data[7] %x \n",SDO_form.data[7]); 	
    err = writeNTCAN(handle,1,&SDO_form);
	if (err != 0) {
		printd(LOG_ERROR,"SDO: Could not write handle=%d.\n", handle);
		return err;
	}
	
	cob_r = SDO_TX + d->nodeid;
	msb = (d->index>>8 & 0xff);
	lsb = (d->index & 0xff);
	for(i=0; i<buffer; i++) {
	   	err = readNTCAN(handle,&f,1);
		if(err == 0 && f.len >= 4 && f.id == cob_r && f.data[1] ==
           lsb && f.data[2] == msb && f.data[3] == d->subindex) {
         	if(f.data[0] == SDO_RESPONSE_WRITE_OK) {
				return 0;
	        } else {
				
 			printd(LOG_ERROR, "Write SDO: response error node=%d index=0x%x\n", 
 			d->nodeid,d->index);
		  return -1;
		 	}	
	     }
	}

	printd(LOG_WARN, "socketcan SDO: timeout node=%d index=0x%x\n", d->nodeid, d->index);
	return -1;
}






int32_t SDO_acknowledge(int32_t handle, const CMSG* f) {
	
	CMSG new_ack[4];
	uint16_t nodeid; 
	
	nodeid= f->id-SDO_RX;

 	new_ack[0].id = nodeid;
	new_ack[0].len = 1;
	new_ack[0].data[0] = SDO_RESPONSE_WRITE_OK;

	
 	new_ack[1].id = nodeid;
	new_ack[1].len = 1;
	new_ack[1].data[0] = f->data[1];


 	new_ack[2].id = nodeid;
	new_ack[2].len = 1;
	new_ack[2].data[0] = f->data[2];


 	new_ack[3].id = nodeid;
	new_ack[3].len = 1;
	new_ack[3].data[0] = f->data[3];

	return writeNTCAN(handle, 4, new_ack);
}



