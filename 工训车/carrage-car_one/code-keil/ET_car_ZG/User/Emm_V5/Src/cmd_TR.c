#include "cmd_TR.h"

#include "can.h"

/**
 * @brief CAN模式配置
 * @return void
 */
void can_Init()
{
    // 初始化CAN过滤器
    CAN_FilterTypeDef can_filter =
        {
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            CAN_FILTER_FIFO0,
            0,
            CAN_FILTERMODE_IDMASK,
            CAN_FILTERSCALE_32BIT,
            CAN_FILTER_ENABLE,
            14};
    HAL_CAN_ConfigFilter(&hcan1, &can_filter); // 设置CAN过滤器
	HAL_CAN_ConfigFilter(&hcan2, &can_filter); // 设置CAN过滤器

    HAL_Delay(1000);

    HAL_CAN_Start(&hcan1);                                             // CAN1开始运行
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); // CAN1开始接受数据
			
	HAL_CAN_Start(&hcan2);                                             // CAN1开始运行
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING); // CAN1开始接受数据
}

void can_cmd_send(uint8_t *cmd, uint8_t len)
{
    __IO uint8_t i = 0, j = 0, k = 0, l = 0, packNum = 0;
    uint8_t Data[8];

    j = len - 2;

    while (i < j)
    {
        k = j - i;

        CAN_TxHeaderTypeDef can_Txheader = {
            0x00,
            (uint32_t)cmd[0] << 8 | (uint32_t)packNum,
            CAN_ID_EXT,
            CAN_RTR_DATA,
            0,
            DISABLE};

        Data[0] = cmd[1];

        if (k < 8)
        {
            for (l = 0; l < k; l++, i++)
            {
                Data[l + 1] = cmd[i + 2];
            }
            can_Txheader.DLC = k + 1;
        }
        else
        {
            for (l = 0; l < k; l++, i++)
            {
                Data[l + 1] = cmd[i + 2];
            }
            can_Txheader.DLC = 8;
        }
		
		if(cmd[0] <= 2)
			HAL_CAN_AddTxMessage(&hcan1, &can_Txheader, Data, NULL);
		else if(cmd[0] > 2)
			HAL_CAN_AddTxMessage(&hcan2, &can_Txheader, Data, NULL);

        // 记录发送的第几包的数据
        ++packNum;
    }
}
