 #include "Conveyor_Bj_Task.h"
 #include "Invert_Garbage_Task.h"
 #include "FreeRTOS.h"
 #include "task.h"
 #include "main.h"
 #include "cmsis_os.h"
 #include "Usart_Task.h"
 #include "tim.h"
 #include "bj_motor.h"
 #include "stm32f4xx_hal.h"
 
 void Conveyor_Task(void *argument){
	 
	 while(1){
		
		if(rv_Camera[0] == 0){
			
			while(1){
				
      conveyor_upper();
      conveyor_under();
				
			}
		}
    else{
			
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);
		
		} 
	 }
  }
