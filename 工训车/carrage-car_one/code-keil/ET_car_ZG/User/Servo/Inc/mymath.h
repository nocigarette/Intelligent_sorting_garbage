#ifndef __MYMATH_H
#define __MYMATH_H

#include "math.h"
#include "main.h"

#define RED 1
#define GREEN 2
#define BULE 3
#define REDMAD 4
#define GREENMAD 5
#define BULEMAD 6

float absd(float temp);
float rounded(float angle);
float cosx(float temp);
float sinx(float temp);
float arcsin(float temp);
float arccos(float temp);
float arctan(uint16_t x,uint16_t y);

#endif
