#include "Invert_Garbage_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Usart_Task.h"
#include "tim.h"
#include "Drp_Task.h"
#include <stdint.h>
#include "Conveyor_Bj_Task.h"


uint8_t drp[4]= {1};
void Drp_Task(void *argument){
	
	while(singles != 1){
		
		drp[0] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4);
		drp[1] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);
		drp[2] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6);
		drp[3] = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7);
		
	}
	
}
void tr_Full_Task(void *argument){
	
  while(1){
		
		if(drp[0] == 0){
			
			
			tr_st[1]= 2;
		}
		else if(drp[1] == 0){

			tr_st[2]= 3;
		}			
		else if(drp[2] == 0){

			tr_st[2]= 4;
		}			
		else if(drp[3] == 0){

			tr_st[2]= 5;
		}
		else if(drp[0] == 1)
		{
			 HAL_UART_Transmit(&huart1,tr_st,5,100);
			drp[0]= 0;
		}
    		
	}
}
