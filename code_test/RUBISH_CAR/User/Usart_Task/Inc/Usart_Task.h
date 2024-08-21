#ifndef __USART_TASK_H
#define __USART_TASK_H

#include "usart.h"

extern uint8_t rv_Camera[15];
void HAl_UART_RxCpltCallback(UART_HandleTypeDef* huart);

#endif
