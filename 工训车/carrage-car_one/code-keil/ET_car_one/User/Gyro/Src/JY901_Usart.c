#include "JY901_Usart.h"

User_USART JY901_data;
struct SAcc stcAcc;
struct SGyro stcGyro;
struct SAngle stcAngle;

void User_USART_Init(User_USART *Data)
{
	for (uint8_t i = 0; i < RXBUFFER_LEN; i++)
	{
		Data->RxBuffer[i] = 0;
		Data->frame_head = 0x55;
		Data->Rx_flag = 0;
		Data->Rx_len = 0;
	}
}

/*陀螺仪消息处理函数*/
void JY901_process()
{
	if (JY901_data.RxBuffer[0] != 0x55)
		return;
	else if (JY901_data.RxBuffer[0] == 0x55)
	{
		if (JY901_data.RxBuffer[1] == 0x53)
		{
			memcpy(&stcAngle, &JY901_data.RxBuffer[2], 8);
			for (uint8_t i = 0; i < 3; i++)
			{
				JY901_data.angle.angle[i] = (float)stcAngle.Angle[i] / 32768 * 180; // 角度
			}
		}
	}
	memset(JY901_data.RxBuffer, 0, 11);
}
