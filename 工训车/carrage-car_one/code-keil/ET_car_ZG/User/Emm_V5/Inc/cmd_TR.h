#ifndef __CMD_TR_H__
#define __CMD_TR_H__

#include "stdint.h"

void can_Init(void);
void can_cmd_send(uint8_t *cmd, uint8_t len);

#endif
