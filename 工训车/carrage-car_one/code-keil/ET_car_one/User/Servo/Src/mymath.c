#include "mymath.h"

#define PI 3.14159
float rounded(float angle) //四舍五入取两位小数
{
	float temp;
	temp=(int)((angle * 100) + 0.5) / 100.0;
	return temp;
}

float absd(float temp)     //绝对值
{
	if(temp<0)
	{
		return -temp;
	}
	else
	{
		return temp;
	}
		
}

float arctan(uint16_t x,uint16_t y)   //arctan弧度值转化角度值
{ 
	float temp;
	temp=atan2(x,y)*180/PI;
	return temp;
}

float arcsin(float temp)           //arcsin弧度值转化角度值
{
	float angle;
	angle=asin(temp)*180/PI;
	return angle;
	
}

float arccos(float temp)          //arccos弧度值转化角度值
{
	float angle;
	angle=acos(temp)*180/PI;
	return angle;
	
}

float sinx(float temp)            //sin弧度值转化角度值
{
	float angle;
	angle=sin(temp*PI/180);
	return angle;
}
float cosx(float temp)          //cos弧度值转化角度值
{
	float angle;
	angle=cos(temp*PI/180);
	return angle;
}
