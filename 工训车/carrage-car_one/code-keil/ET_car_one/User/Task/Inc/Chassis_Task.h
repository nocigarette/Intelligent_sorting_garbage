#ifndef __CHASSIS_TASK_H__
#define __CHASSIS_TASK_H__

#include "stdint.h"

typedef struct
{
	uint8_t motor_action;
	uint8_t new_act_cmd;
	uint32_t my_pul;
	uint8_t my_acc;
}Emm_param;

extern Emm_param my_param;

void Chassis_Task(void *argument);

void Param_assign(uint32_t pul,uint8_t acc,uint8_t act);

#endif
