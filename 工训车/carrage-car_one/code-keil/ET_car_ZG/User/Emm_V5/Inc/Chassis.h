#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "stdint.h"

void Val_forward(uint16_t vel, uint8_t acc);
void Val_back(uint16_t vel, uint8_t acc);
void Val_left(uint16_t vel, uint8_t acc);
void Val_right(uint16_t vel, uint8_t acc);

void brake(void);

#endif
