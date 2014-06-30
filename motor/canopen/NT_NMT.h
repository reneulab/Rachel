#ifndef NMT_H
#define NMT_H
#include <inttypes.h>
#include "ntcan.h"

#define NMT_TX 0x700

enum NMT_transisions {
	NMT_Start_Node = 0x01,
	NMT_Stop_Node = 0x02,
	NMT_Enter_PreOperational = 0x80,
	NMT_Reset_Node = 0x81,
	NMT_Reset_Comunication = 0x82

} NMT_transisions_t;


/**
 * \param fd CAN-bus File descriptor
 * \param nodeid to send NMT message, 0=broadcast
 * \param responses Number of expected responses
 * \return 0 on success, -1 on error
 */
int32_t NMT_change_state(NTCAN_HANDLE handle, uint8_t nodeid, enum NMT_transisions state);

#endif
