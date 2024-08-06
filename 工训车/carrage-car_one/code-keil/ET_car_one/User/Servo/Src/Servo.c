#include "Servo.h"
#include "myusart.h"
#include "fashion_star_uart_servo.h"
#include "mymath.h"

#define value0 value1 - 89 // 第二放置区的0号舵机基准值
#define value1 0		   // 0号舵机基准值（下面那个）
#define value2 0		   // 1号舵机基准值
#define value3 0		   // 2号舵机基准值
#define value4 0		   // 3号舵机基准值
#define value5 0		   // 4号舵机基准值
#define value6 0		   // 5号舵机基准值
#define intervals 85	   // 0号舵机转速
#define interval 130	   // 舵机角度的旋转周期，ms  150
#define t_acc 100		   // 加速时间 ms
#define t_dec 100		   // 减速时间 ms
#define power 0			   // 舵机执行功率，默认为0
#define wait 0			   // 是否为阻塞式  0--不阻塞

_Robotic_arm_t robotic_arm;
_Servo_t Servo;

void Robotic_arm_Init(_Robotic_arm_t *r_arm)
{
    r_arm->Vehicle_height = 0.00;
    r_arm->Length_Servo = 3.5;
    r_arm->L_Servo_zero_one = 0.00;
    r_arm->L_Servo_one_two = 13.70;
    r_arm->L_Servo_two_three = 14.00;
    r_arm->L_Servo_three_top = 6.51;
    r_arm->MAX_high = r_arm->Length_Servo + r_arm->L_Servo_one_two + r_arm->L_Servo_two_three + r_arm->L_Servo_three_top;
    r_arm->MAX_len = r_arm->L_Servo_one_two + r_arm->L_Servo_two_three + r_arm->L_Servo_three_top;
    r_arm->Next_max_len = r_arm->L_Servo_one_two + r_arm->L_Servo_two_three;
}

/******************************************************4轴串联机械臂正逆解算法***********************************************************/
/*************注意！！！以下提到的偏差角度都不是实际角度，要经过角度转换才能得到实际值**************************/
float _degree_convert(int temp, float angle) // 角度转化求值
{
    float res = 0;
	if (temp == 0)
	{
		res = 90 - angle;
	}
	if (temp == 1 || temp == 2)
	{
		res = 180 - angle;
	}
	if (temp == 3)
	{
		res = angle - 90;
	}
	return res;
}

float out_of_range(_Robotic_arm_t *r_arm, uint16_t length, uint16_t height) // 坐标（角度）范围
{
    if (height < r_arm->MAX_high && length < r_arm->MAX_len)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

float _calculatr_j0(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z) // 舵机0的偏差角度
{
    float j0 = 0;
    Servo->length = rounded(sqrt(pow(y, 2) + pow(x, 2)) - r_arm->L_Servo_zero_one); // 机械臂x轴投影长度
    if (Servo->length == 0)
    {
        j0 = 0;
    }
    else
    {
        j0 = arctan(y, x);
        Servo->hight = r_arm->Vehicle_height + r_arm->Length_Servo - z;
    }
    return j0;
}

float _calculatr_j2(_Robotic_arm_t *r_arm, float temp, float d2) // 舵机2的偏差角度
{
    float j2 = 0;
    if (temp > d2)
    {
        j2 = rounded(arccos((pow(r_arm->L_Servo_one_two, 2) + pow(r_arm->L_Servo_two_three, 2) - pow(d2, 2)) / (2 * r_arm->L_Servo_one_two * r_arm->L_Servo_two_three)));
    }
    else if (absd(temp - d2) < 1.0f)
    {
        j2 = 180;
    }
    return j2;
}
/******************************************逆解运动公式***************************************************/
void backward_kinematics(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z) // 运动学逆解计算
{
    float temp, d1, d2;
    float j0, j1, j2 = 0;

    if (out_of_range(&robotic_arm, Servo->length, Servo->hight) == 1) // 判断角度是否在设定范围内
    {
        j0 = _calculatr_j0(Servo, r_arm, x, y, z);
        temp = Servo->length - r_arm->L_Servo_three_top;                                                                                                                                        // 机械臂在给定坐标下在x-y坐标系上的投影长度 - 三号舵机与爪端的长度
        d1 = sqrt(pow(temp, 2) + pow(z, 2));                                                                                                                                                    // 三号舵机到零号舵机在地面投影点之间的垂直距离
        d2 = sqrt(pow(r_arm->Vehicle_height + r_arm->Length_Servo - z, 2) + pow(temp, 2));                                                                                                      // 三号舵机到一号舵机之间的垂直距离
        j1 = rounded(arcsin((d1 / d2) * sinx(arctan(temp, z))) + arccos((pow(r_arm->L_Servo_one_two, 2) + pow(d2, 2) - pow(r_arm->L_Servo_two_three, 2)) / (2 * r_arm->L_Servo_one_two * d2))); // 计算1,2轴与y轴之间的夹角
        j2 = _calculatr_j2(r_arm, r_arm->Next_max_len, d2);

        if (Servo->x < 0)
		{
			Servo->angle_zero = -rounded(_degree_convert(0, j0));
		}
		else
		{
			Servo->angle_zero = rounded(_degree_convert(0, j0));
		}

		Servo->angle_one = rounded(_degree_convert(1, j1));
		Servo->angle_two = rounded(_degree_convert(2, j2));
		Servo->angle_three = rounded(_degree_convert(3, Servo->angle_one + Servo->angle_two));
    }
}
/******************************************正解运动公式***************************************************/
void forward_kinematics(_Servo_t *Servo, _Robotic_arm_t *r_arm) // 正解运动公式用来测试逆解算出的角度值是否正确
{
	float length, height; // length--机械臂x轴投影        height--机械臂y轴投影
	float x = 0.0f, y = 0.0f, z = 0.0f;

	length = rounded(r_arm->L_Servo_one_two * sinx(Servo->angle_one) + r_arm->L_Servo_two_three * sinx(Servo->angle_one + Servo->angle_two) + r_arm->L_Servo_three_top);
	height = absd(absd(r_arm->L_Servo_one_two * cosx(Servo->angle_one)) - absd(r_arm->L_Servo_two_three * cosx(Servo->angle_one + Servo->angle_two)));
	z = r_arm->Vehicle_height + rounded(height) + r_arm->Length_Servo;

	if (Servo->x < 0) //   判断设置的x坐标是正值还负值
	{
		x = -round(length * rounded(cosx(Servo->angle_zero)) + r_arm->L_Servo_zero_one * rounded(cosx(Servo->angle_zero)));
		y = -round(length * rounded(sinx(Servo->angle_zero)) + r_arm->L_Servo_zero_one * rounded(sinx(Servo->angle_zero)));
	}
	else
	{
		x = round(length * rounded(cosx(Servo->angle_zero)) + r_arm->L_Servo_zero_one * rounded(cosx(Servo->angle_zero)));
		y = round(length * rounded(sinx(Servo->angle_zero)) + r_arm->L_Servo_zero_one * rounded(sinx(Servo->angle_zero)));
	}
}

void task(_Servo_t *Servo, _Robotic_arm_t *r_arm, float x, float y, float z) // 坐标计算总任务，直接调用此函数
{
    Servo->x = x;
    if (x < 0)
        x = absd(x);

    backward_kinematics(Servo, r_arm, x, y, z);
}

/******************************************写死运动公式***************************************************/
void set_serveo_angle(uint8_t id, _Servo_t *Servo) // 坐标值控制舵机运动
{

	if (id == 0)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value1 + Servo->angle_zero, intervals, t_acc, t_dec, power, wait);
	}
	if (id == 1)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value2 + Servo->angle_one, interval, t_acc, t_dec, power, wait);
	}
	if (id == 2)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value3 + Servo->angle_two, interval, t_acc, t_dec, power, wait);
	}
	if (id == 3)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value4 + Servo->angle_three, interval, t_acc, t_dec, power, wait);
	}
	if (id == 6)
	{
		FSUS_SetServoAngleByVelocity(&uart4, 0, value0 + Servo->angle_zero, intervals, t_acc, t_dec, power, wait);
	}
}

void set_serveo_angles(uint8_t id, float angle) // 手动调节舵机角度运动
{
	if (id == 0)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value1 + angle, intervals, t_acc, t_dec, power, wait);
	}
	if (id == 1)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value2 + angle, interval, t_acc, t_dec, power, wait);
	}
	if (id == 2)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value3 + angle, interval, t_acc, t_dec, power, wait);
	}
	if (id == 3)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value4 + angle, interval, t_acc, t_dec, power, wait);
	}
	if (id == 4)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value5 + angle, interval, t_acc, t_dec, power, wait);
	}
	if (id == 5)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value6 + angle, interval + 30, t_acc, t_dec, power, wait);
	}
}


