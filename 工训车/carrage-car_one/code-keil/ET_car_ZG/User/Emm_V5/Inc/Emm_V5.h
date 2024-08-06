#ifndef __EMM_V42_H__
#define __EMM_V42_H__

#include "main.h"
#include "can.h"

#include "stdint.h"

void Emm_V5_Val_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc);
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, uint8_t raF, uint8_t snF);
void Emm_V5_Stop_Now(uint8_t addr, uint8_t snF);

#endif
