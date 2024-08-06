#include "Servo_action.h"

#include "main.h"

extern _Robotic_arm_t robotic_arm;
extern _Servo_t Servo;

void Zero_Servo(_Servo_t *Servo) // 存储的角度清零
{
	Servo->angle_zero = 0;
	Servo->angle_one = 0;
	Servo->angle_two = 0;
	Servo->angle_three = 0;
	Servo->length = 0;
	Servo->x = 0;
}

void Servo_Countrls(void) // 坐标放环动作1
{
	set_serveo_angle(0, &Servo);
	HAL_Delay(1000);
	set_serveo_angle(1, &Servo);
	set_serveo_angle(2, &Servo);
	set_serveo_angle(3, &Servo);

	HAL_Delay(1800);
	set_serveo_angles(4, -40);

	HAL_Delay(500);
//	Zero_Servo(&Servo);
}
