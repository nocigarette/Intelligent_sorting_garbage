#include "main.h"

#ifndef _SPI_H_
#define _SPI_H_

//本测试程序使用的是模拟SPI接口驱动
//可自由更改接口IO配置，使用任意最少4 IO即可完成本款液晶驱动显示
/******************************************************************************
接口定义在lcd.h内定义，请根据接线修改并修改相应IO初始化LCD_GPIO_Init()
#define LCD_CTRL   	  	GPIOB		//定义TFT数据端口
#define LCD_LED        	GPIO_Pin_9  //PB9 连接至TFT -LED
#define LCD_RS         	GPIO_Pin_10	//PB10连接至TFT --RS
#define LCD_CS        	GPIO_Pin_11 //PB11 连接至TFT --CS
#define LCD_RST     	GPIO_Pin_12	//PB12连接至TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//PB13连接至TFT -- CLK
#define LCD_SDA        	GPIO_Pin_15	//PB15连接至TFT - SDI
*******************************************************************************/

#define RES_Pin GPIO_PIN_1
#define RES_GPIO_Port GPIOA
#define DC_Pin GPIO_PIN_2
#define DC_GPIO_Port GPIOA
#define CS_Pin GPIO_PIN_3
#define CS_GPIO_Port GPIOA
#define BL_Pin GPIO_PIN_4
#define BL_GPIO_Port GPIOA
#define SCL_Pin GPIO_PIN_5
#define SCL_GPIO_Port GPIOA
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOA

//液晶控制口置1操作语句宏定义

#define	SPI_MOSI_SET  	 SDA_GPIO_Port->BSRR= SDA_Pin  
#define	SPI_SCLK_SET  	 SCL_GPIO_Port->BSRR= SCL_Pin   


//液晶控制口置0操作语句宏定义

#define	SPI_MOSI_CLR  	SDA_GPIO_Port->BSRR=(uint32_t)SDA_Pin << 16U    
#define	SPI_SCLK_CLR  	SCL_GPIO_Port->BSRR=(uint32_t)SCL_Pin << 16U 

void  SPIv_WriteData(uint8_t Data);

#endif
