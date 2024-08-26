 #include "bj_motor.h"


 void Bj_motor_v(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_STEP){
	
	HAL_GPIO_TogglePin(GPIOx,GPIO_Pin_STEP);

 }
 
 void delay_us(uint16_t time){    //微秒级延时
   
   uint16_t i=0;  
   while(time--){
		 
      i=10;  
      while(i--); 
   }
 }

 void delay_ms(uint16_t time) {   //毫秒级的延时
	
   uint16_t i=0;  
   while(time--){
		 
      i=12000;  
      while(i--);    
   }
 }
 
 void conveyor_upper(void){
	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,GPIO_PIN_RESET); //EN
   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4,GPIO_PIN_RESET);	//DIR
   HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);               //STEP
   delay_us(500);	 
 }
 
 void conveyor_under(void){
	 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2,GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0,GPIO_PIN_RESET);	
   HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_1);
   delay_us(500);	 
 }
 
 void compress(uint32_t dir, float circle, uint32_t speed,  int sd){
	 
	 if(dir==1){
		 
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_SET);		
	}
	
	else if(dir==0){
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_RESET);			
	}
	  
	    for (uint32_t i = 0; i < 200*sd*circle; ++i) {
				
       uint32_t delay = 1000000 / (speed * 2);        
			 HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);   
			 delay_us(delay);
			 HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);
			 delay_us(delay);                 
    }
 }
