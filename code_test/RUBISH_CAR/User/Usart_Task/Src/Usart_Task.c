#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Usart_Task.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

	uint8_t rv_Camera[4]= {0,0,0,0};
	uint8_t tr_st[5]= {0,0,0,0};
	
	void dfgd(void)
	{
		
	}
	
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{

		if(huart == &huart1){
			
			HAL_UART_Receive_IT(&huart1, rv_Camera, 4);
			
	}
}

	void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
	{

		if(huart == &huart1){
		
		
			
	}
}
