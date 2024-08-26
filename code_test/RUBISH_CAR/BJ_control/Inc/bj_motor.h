 #ifndef __BJ_MOTOR_H
 #define __BJ_MOTOR_H

 #include "stm32f4xx_hal.h"

 void Bj_motor_v(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_STEP);
 void delay_us(uint16_t time);
 void delay_ms(uint16_t time);
 void conveyor_upper(void);
 void conveyor_under(void);
 void compress(uint32_t dir, float circle, uint32_t speed,  int sd);
 
 #endif
