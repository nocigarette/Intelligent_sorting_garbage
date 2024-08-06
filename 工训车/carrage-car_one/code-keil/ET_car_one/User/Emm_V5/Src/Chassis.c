#include "Chassis.h"
#include "Emm_V5.h"
#include "Defines.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

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

void Pos_forward(uint32_t pul, uint8_t acc)
{
	for (uint8_t i = 1; i <= 4; i++)
	{
		if (i % 2 == 1)
			Emm_V5_Pos_Control(i, 0, 1000, acc, pul, 1, 0);
		else
			Emm_V5_Pos_Control(i, 1, 1000, acc, pul, 1, 0);
		
		Delay_Us(10000);
	}
	Emm_V5_Synchronous_motion(0);
}
	
void Pos_back(uint32_t pul, uint8_t acc)
{
	for (uint8_t i = 1; i <= 4; i++)
	{
		if (i % 2 == 1)
			Emm_V5_Pos_Control(i, 1, 1000, acc, pul, 0, 0);
		else
			Emm_V5_Pos_Control(i, 0, 1000, acc, pul, 0, 0);
		
		Delay_Us(10000);
	}
	Emm_V5_Synchronous_motion(0);
}
	
void Pos_left(uint32_t pul, uint8_t acc)
{
	for (uint8_t i = 1; i <= 4; i++)
	{
		if (i == 1 || i == 2)
			Emm_V5_Pos_Control(i, 1, 1000, acc, pul, 0, 0);
		else
			Emm_V5_Pos_Control(i, 0, 1000, acc, pul, 0, 0);
		
		Delay_Us(10000);
	}
	Emm_V5_Synchronous_motion(0);
}
	
void Pos_right(uint32_t pul, uint8_t acc)
{
	for (uint8_t i = 1; i <= 4; i++)
	{
		if (i == 1 || i == 2)
			Emm_V5_Pos_Control(i, 0, 1000, acc, pul, 0, 0);
		else
			Emm_V5_Pos_Control(i, 1, 1000, acc, pul, 0, 0);
		
		Delay_Us(10000);
	}
	Emm_V5_Synchronous_motion(0);
}

void brake(void)
{
	for(uint8_t i = 1; i <= 4; i++)
	{
		Emm_V5_Stop_Now(i, 0);
	}
}

void Delay_Us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}
