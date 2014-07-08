#ifndef PDO_H
#define PDO_H
#include "ntcan.h"
#include <inttypes.h>

// TODO config.h?
enum PDO_n_ID {
	PDO_TX1_ID = 0x180,
	PDO_TX2_ID = 0x280,

	PDO_RX1_ID = 0x200, /* +node id */
	PDO_RX2_ID = 0x300  /* +node id */
};

/**
 * \return    0 on sucsess, -1 on error
 */
int32_t PDO_send(NTCAN_HANDLE handle, uint16_t pdo_id, int32_t pos);


/**
 * Recives PDO packages.
 * Filters, and throws away! broadcast and SDO (0x600 0x580) packages.
 * \param pdo       where to write received data
 * \param timeout   same as poll (typically milliseconds), 0 for no timeout,
 *                  -1 makes the call blocking.
 * \return          0 on success, -1 on error, -2 on timeout
 */
int PDO_read(NTCAN_HANDLE, CMSG* pdo);

#endif
