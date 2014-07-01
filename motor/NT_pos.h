#ifndef POS_H
#define POS_H

#include <inttypes.h>
#include "canopen/ntcan.h"

extern void changeNTCAN(uint16_t id,uint32_t badData,CMSG *happyCMSG);
extern int32_t writeNTCAN(NTCAN_HANDLE handle,int32_t len,CMSG *msg);

int32_t pos_straight(int32_t id[],int32_t pos[]);
int32_t pos_rotate_rad(int32_t id[],double rad);
int32_t pos_rotate_grad(int32_t id[],double grad);
int32_t ppos_read(int32_t* pos_left, int32_t* vel_left, int32_t* pos_right, int32_t* vel_right);

#endif
