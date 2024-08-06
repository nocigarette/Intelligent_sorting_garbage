#ifndef __DELAY_H
#define __DELAY_H 			     
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t



void delay_init(int);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_s(u8 s);
#endif
