#ifndef __SERVO_ACTION_H__
#define __SERVO_ACTION_H__

#include "Servo.h"

extern _Robotic_arm_t robotic_arm;
extern _Servo_t Servo;

void Zero_Servo(_Servo_t *Servo); // ´æ´¢µÄ½Ç¶ÈÇåÁã
void Servo_Countrls(void);

#endif
