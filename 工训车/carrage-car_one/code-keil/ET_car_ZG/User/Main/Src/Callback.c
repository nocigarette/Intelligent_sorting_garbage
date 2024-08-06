#include "Emm_V5.h"
#include "cmd_TR.h"
#include "Defines.h"

#include "can.h"

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef can_header;
    uint8_t rx_data[8];
    if (hcan == &hcan1)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can_header, rx_data);
    }
	else if (hcan == &hcan2)
    {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can_header, rx_data);
    }
}
