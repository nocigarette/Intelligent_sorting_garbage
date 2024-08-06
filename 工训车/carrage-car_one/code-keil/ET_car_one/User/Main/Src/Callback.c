#include "Emm_V5.h"
#include "cmd_TR.h"
#include "Defines.h"
#include "JY901_Usart.h"

#include "usart.h"
#include "can.h"

extern User_USART JY901_data;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//    CAN_RxHeaderTypeDef can_header;
//    uint8_t rx_data[8];
//    if (hcan == &hcan1)
//    {
//        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can_header, rx_data);
//    }
//	else if (hcan == &hcan2)
//    {
//        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can_header, rx_data);
//    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// uart1串口接收到6个字节的字符则进入中断
	if(huart == &huart1)
	{
		JY901_process();
	}
}
