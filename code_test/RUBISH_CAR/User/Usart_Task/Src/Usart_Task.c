#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Usart_Task.h"
#include "tim.h"
#include "gpio.h"


	uint8_t rv_Camera[4]= {0};
	uint8_t tr_st[5]= {0};
	
	void HAl_UART_RxCpltCallback(UART_HandleTypeDef* huart){

		if(huart == &huart1){
		
			HAL_UART_Receive_IT(&huart1, rv_Camera, 15);
			
	}
}
