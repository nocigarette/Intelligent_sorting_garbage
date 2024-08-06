#ifndef _MY_USART_H
#define _MY_USART_H

#include "JY901.h"
#include "Defines.h"

#include "usart.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"

/* 重定义存储陀螺仪发送的3种数据的结构体 */
// 欧拉角/角度
typedef struct
{
	float angle[3];
} Angle;

// 加速度
typedef struct
{
	float a[3];
} Acc;

// 角速度
typedef struct
{
	float w[3];
} SGyro;

typedef struct
{
	uint8_t Rx_flag;				// 接收完成标志
	uint8_t Rx_len;					// 接收长度
	uint8_t frame_head;				// 帧头
	uint8_t RxBuffer[RXBUFFER_LEN]; // 数据存储
	Angle angle;
	Acc acc;
	SGyro w;
} User_USART;

extern User_USART JY901_data;

void User_USART_Init(User_USART *Data);
void JY901_process(void);

#endif
