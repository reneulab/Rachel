#include "NT_NMT.h"
#include "canopen.h"
#include "myCan.h"


int32_t NMT_change_state(NTCAN_HANDLE handle, uint8_t nodeid, enum NMT_transisions state) {

	CMSG data;
	data.id = nodeid;
	data.len = 1;
	data.data[0] = state;

	return writeNTCAN(handle,1,&data);
	
}
