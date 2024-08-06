#include "Servo.h"
#include "usart.h"
#include "fashion_star_uart_servo.h"
#include "mymath.h"

// #define TASK1
// #define TASK2
/**********************舵机相关参数******************/
// 舵机角度基准值
#define value0 value1 -92   //第二放置区的0号舵机基准值
#define value1 5     //0号舵机基准值（下面那个）
#define value2 -9    //1号舵机基准值
#define value3 2      //2号舵机基准值
#define value4 -10     //3号舵机基准值
#define value5 17     //4号舵机基准值
#define value6 0    //5号舵机基准值
#define intervals 85   //0号舵机转速
#define interval 130 // 舵机角度的旋转周期，ms  150
#define t_acc 100	 // 加速时间 ms
#define t_dec 100	 // 减速时间 ms
#define power 0		 // 舵机执行功率，默认为0
#define wait 0		 // 是否为阻塞式  0--不阻塞

#define True 1
#define False 0
#define A 14.5	// 车身高度
#define P 13.5	// 舵机0到舵机1的距离
#define A0 3.5	// 舵机1自身长度
#define A1 8.00 // 舵机臂1长度（舵机1到舵机2之间的距离）
#define A2 8.00 // 舵机臂2长度（舵机2到舵机3之间的距离）
#define A3 12.5 // 舵机臂3长度（舵机3到爪子中心的距离）
#define LENTHMAX A1 + A2
#define MAX_LEN A1 + A2 + A3
#define MAX_HIGH A0 + A1 + A2 + A3
#define PI 3.14159
SERVO Servo;

/******************************************************4轴串联机械臂正逆解算法***********************************************************/

/*************注意！！！以下提到的偏差角度都不是实际角度，要经过角度转换才能得到实际值************/

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

float out_of_range(uint16_t length, uint16_t height) // 坐标（角度）范围
{
	if (height < MAX_HIGH && length < MAX_LEN)
	{
		return True;
	}
	else
	{
		return False;
	}
}

float _calculatr_j0(SERVO *Servo, float x, float y, float z) // 舵机0的偏差角度
{
	float j0 = 0;
	Servo->length = rounded(sqrt(pow(y, 2) + pow(x, 2)) - P); // 机械臂x轴投影长度
	if (Servo->length == 0)
	{
		j0 = 0;
	}
	else
	{
		if (x > 14.3)
		{
			j0 = arctan(y, x)-3 ;   //
			Servo->hight = A + A0 - z;
		}

		else if (x < 0.5 || x > -0.5)
		{
			j0 = arctan(y, x);
			Servo->hight = A + A0 - z;
		}
	}
	return j0;
}

float _calculatr_j2(float temp, float d2) // 舵机2的偏差角度
{
	float j2 = 0;
	if (temp > d2)
	{
		j2 = rounded(arccos((A1 * A1 + A2 * A2 - pow(d2, 2)) / (2 * A1 * A2)));
	}
	else if (absd(temp - d2) < 1.0f)
	{
		j2 = 180;
	}
	return j2;
}

/******************************************逆解运动公式***************************************************/

void backward_kinematics(SERVO *Servo, float x, float y, float z)
{
	float temp, d1, d2;	  // temp--机械臂投影与舵机臂3长度的差值     d1--角度1  d2--角度2
	float j0, j1, j2 = 0; // j0--舵机0的偏差角   j1--舵机1的偏差角   j2--舵机2的偏差角  j3--舵机3的偏差角

	if (out_of_range(Servo->length, Servo->hight) == 1) // 判断角度是否在设定范围内
	{
		j0 = _calculatr_j0(Servo, x, y, z);
		temp = Servo->length - A3;
		d1 = sqrt(pow(temp, 2) + pow(z, 2));
		d2 = sqrt(pow(A + A0 - z, 2) + pow(temp, 2));
		j1 = rounded(arcsin((d1 / d2) * sinx(arctan(temp, z))) + arccos((A1 * A1 + pow(d2, 2) - A2 * A2) / (2 * A1 * d2)));
		j2 = _calculatr_j2(LENTHMAX, d2);

		if (Servo->x < 0)
		{
			Servo->angle0 = -rounded(_degree_convert(0, j0));
		}
		else
		{
			Servo->angle0 = rounded(_degree_convert(0, j0));
		}

		Servo->angle1 = rounded(_degree_convert(1, j1));
		Servo->angle2 = rounded(_degree_convert(2, j2));
		Servo->angle3 = rounded(_degree_convert(3, Servo->angle1 + Servo->angle2));
	}
	// printf("%f,%f,%f\r\n",j0,j1,j2);
}

/******************************************正解运动公式*************************************/

void forward_kinematics(SERVO *Servo) // 正解运动公式用来测试逆解算出的角度值是否正确
{
	float length, height; // length--机械臂x轴投影        height--机械臂y轴投影
	float x, y, z;

	length = rounded(A1 * sinx(Servo->angle1) + A2 * sinx(Servo->angle1 + Servo->angle2) + A3);
	height = absd(A1 * cosx(Servo->angle1)) + absd(A2 * cosx(Servo->angle1 + Servo->angle2));
	z = A - round(rounded(height)) + A0;

	if (Servo->x < 0) //   判断设置的x坐标是正值还负值
	{
		x = -round(length * rounded(cosx(Servo->angle0)) + P * rounded(cosx(Servo->angle0)));
		y = -round(length * rounded(sinx(Servo->angle0)) + P * rounded(sinx(Servo->angle0)));
	}
	else
	{
		x = round(length * rounded(cosx(Servo->angle0)) + P * rounded(cosx(Servo->angle0)));
		y = round(length * rounded(sinx(Servo->angle0)) + P * rounded(sinx(Servo->angle0)));
	}
	printf("%f,%f,%f\r\n", y, x, z);
}

void task(SERVO *Servo, float x, float y, float z) // 坐标计算总任务，直接调用此函数
{
	Servo->x = x; // 判断x是正值还是负值
	if (x < 0)
	{
		x = absd(x);
	}

	backward_kinematics(Servo, x, y, z);
	// forward_kinematics(Servo);
}

/**************************************************舵机角度控制*************************************************/

void set_serveo_angle(uint8_t id, SERVO *Servo) // 坐标值控制舵机运动
{

	if (id == 0)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value1 + Servo->angle0, intervals, t_acc, t_dec, power, wait);
	}
	if (id == 1)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value2 + Servo->angle1, interval, t_acc, t_dec, power, wait);
	}
	if (id == 2)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value3 + Servo->angle2, interval, t_acc, t_dec, power, wait);
	}
	if (id == 3)
	{
		FSUS_SetServoAngleByVelocity(&uart4, id, value4 + Servo->angle3, interval, t_acc, t_dec, power, wait);
	}
	if (id == 6)
	{
		FSUS_SetServoAngleByVelocity(&uart4, 0, value0 + Servo->angle0, intervals, t_acc, t_dec, power, wait);
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
void Servo_zero_countrl(void) // 机械臂归零
{
	set_serveo_angles(0, -92);
	set_serveo_angles(1, 0);
	set_serveo_angles(2, 0);
	set_serveo_angles(3, 0);
	set_serveo_angles(4, 0);
	set_serveo_angles(5, 0);
	HAL_Delay(500);
}

void Servo_wait_countrl(void) //抓取间隔等待
{
	set_serveo_angles(1, 0);
	set_serveo_angles(2, 30);
	set_serveo_angles(3, 0);
	HAL_Delay(500);
}
void Servo_Release_ring(void) // 放环--载货
{
	set_serveo_angles(1, 10);   //10
	set_serveo_angles(2, -84);   //-94
	set_serveo_angles(3, 13);
	HAL_Delay(1650);
	set_serveo_angles(4, -70); // 张开
	//	  HAL_Delay(1500);
	//	  set_serveo_angles(4,2);
	HAL_Delay(500);  //可以不用
}

void Servo_Release_ring2(void) // 放环--载货
{
	// Servo_initialize_2();
	Servo_wait_countrl();
	HAL_Delay(800);
	Servo_initialize_2();
	HAL_Delay(800);
	set_serveo_angles(1, 12);   //10
	set_serveo_angles(2, -84);   //-94
	set_serveo_angles(3, 13);
	HAL_Delay(1300);
	set_serveo_angles(4, -70); // 张开
	//	  HAL_Delay(1500);
	//	  set_serveo_angles(4,2);
	HAL_Delay(500);  //可以不用
}



void Servo_Store(void) // 抓环-放环--放置物料
{
	HAL_Delay(500);
	set_serveo_angles(1, 18);  // 10   调小是向上
	set_serveo_angles(2, -95); //-98  -96
	set_serveo_angles(3, 6);   // 0
	HAL_Delay(1300);
	// set_serveo_angles(4, -41); // 4是控制爪子 -40是张开 -6是合上
	// HAL_Delay(500);
	set_serveo_angles(4, -2);
	HAL_Delay(500);
}

void Servo_Countrl(void) // 坐标抓取动作
{

	set_serveo_angle(0, &Servo);
	set_serveo_angle(1, &Servo);
	set_serveo_angle(2, &Servo);
	set_serveo_angle(3, &Servo);
	// set_serveo_angles(4, -40);
	HAL_Delay(1000);
	set_serveo_angles(4,2);
	// HAL_Delay(500);
	HAL_Delay(500);
	Zero_Servo(&Servo);
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
	Zero_Servo(&Servo);
}

void Servo_Countrls2(void) // 坐标放环动作2
{
	set_serveo_angle(6, &Servo);
	HAL_Delay(1000);
	set_serveo_angle(1, &Servo);
	set_serveo_angle(2, &Servo);
	set_serveo_angle(3, &Servo);

	HAL_Delay(2000);
	set_serveo_angles(4, -40);

	HAL_Delay(500);
	Zero_Servo(&Servo);
}

void Servo_Countrls3(void) // 坐标放环动作1
{
	set_serveo_angle(0, &Servo);
	HAL_Delay(1000);
	set_serveo_angle(1, &Servo);
	set_serveo_angle(2, &Servo);
	set_serveo_angle(3, &Servo);

	HAL_Delay(1800);
	// set_serveo_angles(4, -40);

	// HAL_Delay(500);
	Zero_Servo(&Servo);
}

void Servo_wait_1(void) // 扫描二维码动作
{
	set_serveo_angles(0, 94); // 94  0是下面舵机
	HAL_Delay(500);
	set_serveo_angles(1, 60); // 55
	set_serveo_angles(2, 90);
	set_serveo_angles(3, 60);
	set_serveo_angles(4, -80);
	HAL_Delay(1000);
}

void Servo_wait(void) // 等待摄像头动作准备抓环
{
	set_serveo_angles(3, 55);
    HAL_Delay(500);
	set_serveo_angles(1, 50); // 55 调小是向上
	set_serveo_angles(2, 84);
	set_serveo_angles(4, -4);		//-85
	HAL_Delay(500);   //可以不用
}

void Servo_look(temp) // 看颜色
{
	set_serveo_angles(0, 94);
	HAL_Delay(2500);
	set_serveo_angles(1, 35); // 55 调小是向上
	set_serveo_angles(2, 60);
	set_serveo_angles(3, -25);
	set_serveo_angles(4, -85);
	HAL_Delay(500);
	if (temp == RED)
	{
		set_serveo_angles(5, 119);
	}
	if (temp == GREEN)
	{
		set_serveo_angles(5, 0);
	}
	if (temp == BULE)
	{
		set_serveo_angles(5, -119);
	}
	// HAL_Delay(500);
}

void Servo_grap(void) // 抓环并放环
{
	set_serveo_angles(4, -2);   //爪子合上
	HAL_Delay(500);
	Servo_Release_ring();  //
}

void Servo_Take(int temp) // 第一放置区抓取
{
	if (temp == RED) // 红色
	{
		// set_serveo_angles(4, -70);   //爪子张大
		// HAL_Delay(1000);
		task(&Servo, 14.6,32, 13);
		Servo_Countrls();
		set_serveo_angles(5, 119); //
		// HAL_Delay(500);
		task(&Servo, 14.6,32, 6); //15, 32.5, 5.5
		Servo_Countrl();		   //
		task(&Servo,14.6,32, 13); //
		Servo_Countrl();
		Servo_grap();   //抓取
		HAL_Delay(500);    //可以不用
	}

	if (temp == GREEN) // 绿色
	{
		task(&Servo,-0.8,32.3, 13); //
		Servo_Countrls();
		set_serveo_angles(5, -3); //
		// HAL_Delay(500);
		task(&Servo,-0.8,32.3, 5.5); //
		Servo_Countrl();		   //
		task(&Servo,-0.8,30, 13); //
		Servo_Countrl();
		Servo_grap();
		HAL_Delay(500);
	}
	if (temp == BULE) // 蓝色
	{
		task(&Servo, -15,32.8, 13); //
		Servo_Countrls();
		set_serveo_angles(5, -124); //
		// HAL_Delay(500);
		task(&Servo, -15, 32.8, 5.8); //
		Servo_Countrl();		   //
		task(&Servo, -15, 32.8, 13); //
		Servo_Countrl();	
		Servo_grap();
		HAL_Delay(500);
	}
}

void Servo_Set(int temp) // 第一放置区
{
	if (temp == RED) // 红色
	{
		Servo_Zero();
		set_serveo_angles(5, 119);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, 14.7, 33, 13); // 14.3, 33.4, 5.4)   // 15, 32.5, 6  14.3, 33.4, 6

		Servo_Countrls3(); //
		task(&Servo, 14.7, 33, 5.9); // 14.3, 33.4, 5.4)   // 15, 32.5, 6  14.3, 33.4, 6

		Servo_Countrls(); //

		task(&Servo, 14.7, 33, 12);
		Servo_Countrls();
	}

	if (temp == GREEN) // 绿色
	{
		Servo_Zero();
		set_serveo_angles(5, 0);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, 0, 31, 12.5); // 0.1, 33, 5.2   1.3, 32.7, 6

		Servo_Countrls3();
		task(&Servo, 0, 33, 5.8); // 14.3, 33.4, 5.4)   // 15, 32.5, 6  14.3, 33.4, 6

		Servo_Countrls(); //

		// task(&Servo, 0.3, 32.8, 5.9);
		// Servo_Countrls();
		task(&Servo, 0, 33, 12);
		Servo_Countrls();
		task(&Servo, 0, 32, 14); // 14.3, 33.4, 5.4)   // 15, 32.5, 6  14.3, 33.4, 6

		Servo_Countrls(); //
	}
	if (temp == BULE) // 蓝色
	{
		Servo_Zero();
		set_serveo_angles(5, -119); //
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, -15, 33.3, 6.1); //-14.5, 33.5, 6   -14, 32.5, 6

		Servo_Countrls();

		task(&Servo, -15, 33.3, 12);
		Servo_Countrls();
	}
}

void Servo_Set2(int temp) // 第二放置区的调用
{
	if (temp == RED) // 红色
	{
		Servo_Zero2();
		set_serveo_angles(5, 119);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, 14.3, 33, 6); // 14.3, 33.7, 6   13.9, 33.5, 7偏左一点点  13.8, 33.5, 7

		Servo_Countrls2();

		// task(&Servo, 14.3, 33, 8);
		// Servo_Countrls2();
		task(&Servo, 14.3, 33, 12); // 14.3, 31, 12
		Servo_Countrls2();
	}

	if (temp == GREEN) // 绿色
	{
		Servo_Zero2();
		set_serveo_angles(5, 0);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, -0.8, 32.5, 5.6); // 0.1, 33, 5.2

		Servo_Countrls2();

		// task(&Servo, -0.8, 32.5, 5.8);
		// Servo_Countrls2();
		task(&Servo, -0.8, 32.5, 12);
		Servo_Countrls2();
	}
	if (temp == BULE) // 蓝色
	{
		Servo_Zero2();
		set_serveo_angles(5, -119);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, -14.5, 33, 6.6); //-14.9, 33.2, 65
		Servo_Countrls2();

		// task(&Servo, -14.5, 32.8, 8);
		// Servo_Countrls2();
		task(&Servo, -14.5, 33, 12);
		Servo_Countrls2();
	}
	if (temp == REDMAD) // 红色码垛
	{
		set_serveo_angles(5, 119);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, 13, 32.6, 12);

		Servo_Countrls2();

		task(&Servo, 13, 32, 16);
		Servo_Countrls2();
	}
	if (temp == GREENMAD) // 绿色码垛
	{
		set_serveo_angles(5, 0);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, -2.2, 31.5, 12); // 0.1, 33, 12


		Servo_Countrls2();

		task(&Servo, -2.2, 30, 14);
		Servo_Countrls2();
	}
	if (temp == BULEMAD) // 蓝色码垛
	{
		set_serveo_angles(5, -119);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, -14.4, 32.3, 13); //-14.9, 34.2

		Servo_Countrls2();

		task(&Servo, -14.4, 32, 16); //-14.9, 33.4
		Servo_Countrls2();
	}
}

// void Servo_Set3(int temp) // 第三放置区的调用   码垛
// {
// 	if (temp == RED) // 红色
// 	{
// 		Servo_Zero2();
// 		set_serveo_angles(5, 119);
// 		Servo_Store();
// 		task(&Servo, 13.8, 33.0, 13); // 14.4, 35.4, 9.5

// 		Servo_Countrls2();

// 		task(&Servo, 13.8, 33.0, 16); // 29,16  //14.4, 32 ,13
// 		Servo_Countrls2();
// 	}

// 	if (temp == GREEN) // 绿色
// 	{
// 		Servo_Zero2();
// 		set_serveo_angles(5, 0);
// 		HAL_Delay(500);
// 		Servo_Store();
// 		task(&Servo, -0.9, 32.5, 13); // 0.1, 35,8.7

// 		Servo_Countrls2();

// 		task(&Servo, -0.9, 32.5, 16); // 0.1, 32,
// 		Servo_Countrls2();
// 	}
// 	if (temp == BULE) // 蓝色
// 	{
// 		Servo_Zero2();
// 		set_serveo_angles(5, -119);
// 		HAL_Delay(500);
// 		Servo_Store();
// 		task(&Servo, -13.9, 32.5, 13); //-14.7, 35.2, 9.5原  -14.0, 34, 10.5太远了

// 		Servo_Countrls2();

// 		task(&Servo, -13.9, 32.5, 16); // 高度13
// 		Servo_Countrls2();
// 	}
// }
void Servo_Set3(int temp) // 第三放置区的调用   码垛
{
	if (temp == RED) // 红色
	{
		Servo_Zero2();
		set_serveo_angles(5, 119);
		Servo_Store();
		task(&Servo, 14.3, 33.3, 13.3); 

		Servo_Countrls2();

		task(&Servo, 14.3, 32, 16); 
		Servo_Countrls2();
	}

	if (temp == GREEN) // 绿色
	{
		Servo_Zero2();
		set_serveo_angles(5, 0);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, -0.7, 33.2, 13.3); 

		Servo_Countrls2();

		task(&Servo, -0.7, 30, 16); 
		Servo_Countrls2();
	}
	if (temp == BULE) // 蓝色
	{
		Servo_Zero2();
		set_serveo_angles(5, -119);
		HAL_Delay(500);
		Servo_Store();
		task(&Servo, -14.2, 33.5, 13.3); 

		Servo_Countrls2();

		task(&Servo, -14.3, 30, 16); 
		Servo_Countrls2();
	}
}

void Servo_Zero(void) // 舵机转盘归正--放置1区
{
	//HAL_Delay(500);
	set_serveo_angles(1, 14);
	set_serveo_angles(2, -60);
	set_serveo_angles(3, 1);
	set_serveo_angles(4, -60);
	HAL_Delay(500);
	task(&Servo, 0, 34.3, 5.4);
	HAL_Delay(500);
	set_serveo_angle(0, &Servo);
	HAL_Delay(500);
	Zero_Servo(&Servo);
}

void Servo_Zero2(void) // 舵机转盘归正--放置2区
{
	HAL_Delay(500);
	set_serveo_angles(1, 14);
	set_serveo_angles(2, -60);
	set_serveo_angles(3, 1);
	set_serveo_angles(4, -40);
	HAL_Delay(500);
	task(&Servo, -0.2, 32.8, 5.6);
	HAL_Delay(500);
	set_serveo_angle(6, &Servo);
	HAL_Delay(500);
	Zero_Servo(&Servo);
}

void Zero_Servo(SERVO *Servo) // 存储的角度清零
{
	Servo->angle0 = 0;
	Servo->angle1 = 0;
	Servo->angle2 = 0;
	Servo->angle3 = 0;
	Servo->length = 0;
	Servo->x = 0;
}

void Servo_initialize(void)
{
	set_serveo_angles(0, 94); //
	set_serveo_angles(1, 10);
	set_serveo_angles(2, -54);
	set_serveo_angles(3, 0);
	set_serveo_angles(4, -70);
	HAL_Delay(800);
}

void Servo_initialize_2(void)
{
	set_serveo_angles(1, 10);
	set_serveo_angles(2, -54);
	set_serveo_angles(3, 0);
	HAL_Delay(800);
}

void Servo_initialize_3(void)    // 正面机械臂初始化动作
{
	set_serveo_angles(0, 0); //
	set_serveo_angles(1, 10);
	set_serveo_angles(2, -54);
	set_serveo_angles(3, 0);
	set_serveo_angles(4, -70);
	set_serveo_angles(5,0);
	HAL_Delay(800);
}

void Servo_position(void)
{
	// set_serveo_angles(0, 1); //
	// HAL_Delay(2000);
	// set_serveo_angles(1, 75);
	// set_serveo_angles(2, 5);
	// set_serveo_angles(3, -89);
	// set_serveo_angles(4, -75);
	// HAL_Delay(800);
	set_serveo_angles(0, 1); 
	// set_serveo_angles(1, 75);   // 低
	// set_serveo_angles(2, 5);
	// set_serveo_angles(3, -89);
	// set_serveo_angles(4, -75);
	// HAL_Delay(800);
	set_serveo_angles(1, 30);    //高
	set_serveo_angles(2, 40);
	set_serveo_angles(3, -97);  //调小是向上
	set_serveo_angles(4, -85);
	HAL_Delay(800);
}

void Servo_position_2(void)
{
	// set_serveo_angles(0, -90); //
	// HAL_Delay(2000);
	// set_serveo_angles(1, 75);
	// set_serveo_angles(2, 5);
	// set_serveo_angles(3, -89);
	// set_serveo_angles(4, -75);
	// HAL_Delay(800);
	set_serveo_angles(0, -94); 
	HAL_Delay(2000);
	// set_serveo_angles(1, 75);    // 低
	// set_serveo_angles(2, 5);
	// set_serveo_angles(3, -89);
	// set_serveo_angles(4, -75);
	// HAL_Delay(800);
	set_serveo_angles(1, 30);   // 高
	set_serveo_angles(2, 40);
	set_serveo_angles(3, -87);  //调小是向上
	set_serveo_angles(4, -110);
	HAL_Delay(800);
}

void Servo_position_3(void)
{
	// set_serveo_angles(0, -90); //
	// HAL_Delay(2000);
	// set_serveo_angles(1, 5);
	// set_serveo_angles(2, 110);
	// set_serveo_angles(3, -35);
	// set_serveo_angles(4, -75);
	// HAL_Delay(800);
	set_serveo_angles(0, -94); 
	HAL_Delay(2000);
	set_serveo_angles(1, 5);
	set_serveo_angles(2, 70);
	set_serveo_angles(3, -75);
	set_serveo_angles(4, -120);
	HAL_Delay(800);
// 	set_serveo_angles(1, 75);    // 低
// 	set_serveo_angles(2, 5);
// 	set_serveo_angles(3, -89);
// 	set_serveo_angles(4, -75);
// 	HAL_Delay(800);
}

void Servo_grap1(void)
{
    set_serveo_angles(4, -2);   //爪子合上
	HAL_Delay(500);
	Servo_Release_ring2();  //
}

void Servo_wait_control()  //
{
	set_serveo_angles(1, 0);
	set_serveo_angles(2, 30);
	set_serveo_angles(3, 0);
	HAL_Delay(500);
}