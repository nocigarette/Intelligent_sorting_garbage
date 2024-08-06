/*
 * @Author: jkydh
 * @Date: 2023-10-29 22:27:35
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-30 00:36:59
 * @Description: 请填写简介
 */
#include "Chassis.h"
#include "Emm_V5.h"

void Val_forward(uint16_t vel, uint8_t acc)
{
	for (uint8_t i = 1; i <= 4; i++)
	{
		if (i % 2 == 1)
			Emm_V5_Val_Control(i, 0, vel, acc);
		else
			Emm_V5_Val_Control(i, 1, vel, acc);
	}
}

void Val_back(uint16_t vel, uint8_t acc)
{
	for (uint8_t i = 1; i <= 4; i++)
	{
		if (i % 2 == 1)
			Emm_V5_Val_Control(i, 1, vel, acc);
		else
			Emm_V5_Val_Control(i, 0, vel, acc);
	}
}

void Val_left(uint16_t vel, uint8_t acc)
{
	for (uint8_t i = 1; i <= 4; i++)
	{
		if (i == 1 || i == 2)
			Emm_V5_Val_Control(i, 1, vel, acc);
		else
			Emm_V5_Val_Control(i, 0, vel, acc);
	}
}

void Val_right(uint16_t vel, uint8_t acc)
{
	for (uint8_t i = 1; i <= 4; i++)
	{
		if (i == 1 || i == 2)
			Emm_V5_Val_Control(i, 0, vel, acc);
		else
			Emm_V5_Val_Control(i, 1, vel, acc);
	}
}

void brake(void)
{
	for(uint8_t i = 1; i <= 4; i++)
	{
		Emm_V5_Stop_Now(i, 0);
	}
}

