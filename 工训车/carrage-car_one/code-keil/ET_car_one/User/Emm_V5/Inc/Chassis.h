#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "main.h"
#include "stdint.h"

void Val_forward(uint16_t vel, uint8_t acc);
void Val_back(uint16_t vel, uint8_t acc);
void Val_left(uint16_t vel, uint8_t acc);
void Val_right(uint16_t vel, uint8_t acc);

void Pos_forward(uint32_t pul, uint8_t acc);
void Pos_back(uint32_t pul, uint8_t acc);
void Pos_left(uint32_t pul, uint8_t acc);
void Pos_right(uint32_t pul, uint8_t acc);

void brake(void);

void Delay_Us(__IO uint32_t delay);

#endif
