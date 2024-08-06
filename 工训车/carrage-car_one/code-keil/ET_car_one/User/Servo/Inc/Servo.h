#ifndef __SERVO_H__
#define __SERVO_H__

#include <math.h>
#include "stdint.h"

#define PI 3.14159
#define TRUE 1
#define FALSE 0

typedef struct
{
	float Vehicle_height;	 // 车身高度
	float Length_Servo;		 // 舵机长度
	float L_Servo_zero_one;	 // 零号舵机与一号舵机的距离
	float L_Servo_one_two;	 // 一号舵机与二号舵机的距离
	float L_Servo_two_three; // 二号舵机与三号舵机的距离
	float L_Servo_three_top; // 三号舵机与爪子的距离
	float MAX_high;			 // 机械臂最高高度
	float MAX_len;			 // 机械臂最长长度
	float Next_max_len;		 // 机械臂次最长长度
} _Robotic_arm_t;

typedef struct
{
	float x;		   // x轴坐标
	float length;	   // 机械臂x轴投影长度
	float hight;	   // 机械臂z轴投影长度
	float angle_zero;  // 零号舵机输出的角度
	float angle_one;   // 一号舵机输出的角度
	float angle_two;   // 二号舵机输出的角度
	float angle_three; // 三号舵机输出的角度
} _Servo_t;

void Robotic_arm_Init(_Robotic_arm_t *r_arm);
/******************************************************4轴串联机械臂正逆解算法***********************************************************/
/*************注意！！！以下提到的偏差角度都不是实际角度，要经过角度转换才能得到实际值**************************/
float _degree_convert(int temp, float angle);					 // 角度转化求值
float out_of_range(_Robotic_arm_t *r_arm, uint16_t length, uint16_t height);			 // 坐标（角度）范围
float _calculatr_j0(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z); // 舵机0的偏差角度
float _calculatr_j2(_Robotic_arm_t *r_arm, float temp, float d2);						 // 舵机2的偏差角度
/******************************************逆解运动公式***************************************************/
void backward_kinematics(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z); // 运动学逆解计算
/******************************************正解运动公式***************************************************/
void forward_kinematics(_Servo_t *Servo, _Robotic_arm_t *r_arm);
void task(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z); // 坐标计算总任务，直接调用此函数
void set_serveo_angle(uint8_t id, _Servo_t *Servo); // 坐标值控制舵机运动
void set_serveo_angles(uint8_t id, float angle); // 手动调节舵机角度运动

#endif
