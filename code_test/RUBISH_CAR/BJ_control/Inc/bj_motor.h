#ifndef __BJ_MOTOR_H
#define __BJ_MOTOR_H

#include "stm32f4xx_hal.h"

void Bj_motor_v(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_STEP);

#endif
