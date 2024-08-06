#include "Main_Task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "Chassis_Task.h"

void Main_Task(void *argument)
{
	while(1)
	{
		Param_assign(6400, 100, 1);
		osDelay(5000);
		Param_assign(3200, 100, 2);
		osDelay(1000);
	}
}
