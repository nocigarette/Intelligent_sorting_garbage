#include "AHR_Task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "JY901_Usart.h"

extern User_USART JY901_data;

void AHR_Task(void *argument)
{
	while(1)
	{
		HAL_UART_Receive_IT(&huart1,JY901_data.RxBuffer,RXBUFFER_LEN);
			
		osDelay(10);
	}
}
