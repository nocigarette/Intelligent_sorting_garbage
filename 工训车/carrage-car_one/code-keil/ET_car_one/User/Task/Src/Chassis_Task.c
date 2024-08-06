#include "Chassis_Task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "Chassis.h"
#include "cmd_TR.h"

Emm_param my_param;

void Chassis_Task(void *argument)
{
	while(1)
	{
		/* 进入临界段保护 */
        taskENTER_CRITICAL();
		if(my_param.new_act_cmd == 1)
		{
			switch(my_param.motor_action)
			{
			case 0:
				brake();
				break;
			case 1:
				Pos_forward(my_param.my_pul,my_param.my_acc);
				break;
			case 2:
				Pos_back(my_param.my_pul,my_param.my_acc);
				break;
			case 3:
				Pos_left(my_param.my_pul,my_param.my_acc);
				break;
			case 4:
				Pos_right(my_param.my_pul,my_param.my_acc);
				break;
			}
			my_param.new_act_cmd = 0;
		}
		/* 退出临界段保护 */
        taskEXIT_CRITICAL();

        osDelay(10);
	}
}

void Param_assign(uint32_t pul,uint8_t acc,uint8_t act)
{
	my_param.new_act_cmd = 1;
	my_param.motor_action = act;
	my_param.my_pul = pul;
	my_param.my_acc = acc;
}
