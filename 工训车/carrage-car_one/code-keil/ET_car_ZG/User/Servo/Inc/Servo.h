#ifndef __SERVO_H
#define __SERVO_H
#include "math.h"
#include "stm32f4xx_hal.h"

typedef struct
{
	int x;		  // 锟芥储锟斤拷x值
	float length; // 锟斤拷械锟斤拷锟斤拷x锟斤拷锟酵队帮拷锟斤拷锟�
	float hight;  // 锟斤拷械锟斤拷锟斤拷y锟斤拷锟酵队帮拷锟斤拷锟�
	float angle0; // 0号舵机角度
	float angle1; // 1号舵机角度
	float angle2; // 2号舵机角度
	float angle3; // 3号舵机角度
} SERVO;

extern SERVO Servo;
void set_serveo_angles(uint8_t id, float angle);
float _calculatr_j2(float temp, float d2);
void forward_kinematics(SERVO *Servo);
float out_of_range(uint16_t lengh, uint16_t height);
float _calculatr_j0(SERVO *Servo, float x, float y, float z);
void backward_kinematics(SERVO *Servo, float x, float y, float z);
void task(SERVO *Servo, float x, float y, float z);
void Servo_zero_countrl(void);
float _degree_convert(int temp, float angle);
void set_serveo_angle(uint8_t id, SERVO *Servo);

void Servo_zero_countrl(void);
void Servo_Countrl(void);
void Servo_Release_ring(void);
void Servo_Store(void);
void Servo_grap(void);
void Servo_Set(int temp);
void Servo_Take(int temp);
void Servo_wait(void);
void Servo_wait_1(void);
void Servo_Countrls(void);
void Servo_Countrls2(void);
void Servo_Countrls3(void);
void Servo_Zero(void);
void Zero_Servo(SERVO *Servo);
void Servo_Set2(int temp);
void Servo_Zero2(void);
void Servo_Set3(int temp);
void Servo_initialize(void);
void Servo_initialize_2(void);
void Servo_initialize_3(void);
void Servo_position(void);
void Servo_position_2(void);
void Servo_position_3(void);
void Servo_position_4(void);
void Servo_position_5(void);
void Servo_Release_ring2(void); // 放环--载货
void Servo_look(int temp);
void Servo_wait_control(void);
void Servo_grap1(void);
void Servo_Countrls3(void); 
void Servo_Countrls4(void); 

#endif
