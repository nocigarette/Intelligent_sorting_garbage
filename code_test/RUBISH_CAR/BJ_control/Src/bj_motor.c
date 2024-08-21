 #include "bj_motor.h"


 void Bj_motor_v(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_STEP){
	
	HAL_GPIO_TogglePin(GPIOx,GPIO_Pin_STEP);

 }
 

 
 
 