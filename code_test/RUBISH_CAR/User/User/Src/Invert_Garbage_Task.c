 #include "Invert_Garbage_Task.h"
 #include "FreeRTOS.h"
 #include "task.h"
 #include "main.h"
 #include "cmsis_os.h"
 #include "Usart_Task.h"
 #include "tim.h"
 #include "Conveyor_Bj_Task.h" 
 
 int singles= 0;
 
	void Invert_task(void* argument){
		
		while(1){
			
			if(rv_Camera[0] == '1'){
			
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,200);
				osDelay(500);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,150);
				singles=1;
				osDelay(500);
				tr_st[0]= 1;
				singles=0;
//				rv_Camera[0] = 0;
//				rv_Camera[1] = 0;
//				rv_Camera[2] = 0;
//				rv_Camera[3] = 0;
			}
			else if(rv_Camera[0] == '2'){
			
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,100);
				osDelay(500);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,150);
			  singles=1;
				osDelay(500);
				tr_st[0]= 1;
				singles=0;
//				rv_Camera[0] = 0;
//				rv_Camera[1] = 0;
//				rv_Camera[2] = 0;
//				rv_Camera[3] = 0;
			}
			else if(rv_Camera[0] == '3'){
			
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,100);
				osDelay(500);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,150);
			  singles=1;
				osDelay(500);
				tr_st[0]= 1;
				singles=0;

			}
			else if(rv_Camera[0] == '4'){
			
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,200);
				osDelay(500);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,150);
			  singles=1;
				osDelay(500);
				tr_st[0]= 1;
				singles=0;
//				rv_Camera[0] = 0;
//				rv_Camera[1] = 0;
//				rv_Camera[2] = 0;
//				rv_Camera[3] = 0;
			}
			else{
			
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,150);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,150);
			  tr_st[0] =0;
			}
		}
	}
	
		void Panel_Dj_Task(void* argument){
		
			while(1){
				
			if(rv_Camera[0] != 0){
			
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,200);
				osDelay(500);
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,150);
			
			}
			else{
			
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,200);
				osDelay(500);
						
			}
		}
	}
	
