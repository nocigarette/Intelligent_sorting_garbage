/*
 * @Author       : jkdyh
 * @Date         : 2023-09-19 23:59:24
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-10-05 19:43:22
 * @Description  : 请填写简介
 */
#include "Emm_V5.h"
#include "cmd_TR.h"

#include "can.h"

/**
  * @brief    多机同步运动
  * @param    addr  ：电机地址
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void Emm_V5_Synchronous_motion(uint8_t addr)
{
  uint8_t tx_data[4] = {0};
  
  // 装载命令
  tx_data[0] =  addr;                       // 地址
  tx_data[1] =  0xFF;                       // 功能码
  tx_data[2] =  0x66;                       // 辅助码
  tx_data[3] =  0x6B;                       // 校验字节
  
  // 发送命令
  can_cmd_send(tx_data, 4);
}

/**
  * @brief    读取系统参数
  * @param    addr  ：电机地址
  * @param    s     ：系统参数类型
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void Emm_V5_Read_Sys_Params(uint8_t addr, SysParams_t s)
{
  uint8_t i = 0;
  uint8_t cmd[16] = {0};
  
  // 装载命令
  cmd[i] = addr; ++i;                   // 地址

  switch(s)                             // 功能码
  {
    case S_VER  : cmd[i] = 0x1F; ++i; break;
    case S_RL   : cmd[i] = 0x20; ++i; break;
    case S_PID  : cmd[i] = 0x21; ++i; break;
    case S_VBUS : cmd[i] = 0x24; ++i; break;
    case S_CPHA : cmd[i] = 0x27; ++i; break;
    case S_ENCL : cmd[i] = 0x31; ++i; break;
    case S_TPOS : cmd[i] = 0x33; ++i; break;
    case S_VEL  : cmd[i] = 0x35; ++i; break;
    case S_CPOS : cmd[i] = 0x36; ++i; break;
    case S_PERR : cmd[i] = 0x37; ++i; break;
    case S_FLAG : cmd[i] = 0x3A; ++i; break;
    case S_ORG  : cmd[i] = 0x3B; ++i; break;
    case S_Conf : cmd[i] = 0x42; ++i; cmd[i] = 0x6C; ++i; break;
    case S_State: cmd[i] = 0x43; ++i; cmd[i] = 0x7A; ++i; break;
    default: break;
  }

  cmd[i] = 0x6B; ++i;                   // 校验字节
  
  // 发送命令
  can_cmd_send(cmd, i);
}

/**
 * @brief 步进电机速度式控制
 * @note 速度和加速度最好保持一致
 * @return 无
 */
void Emm_V5_Val_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc)
{
    uint8_t tx_data[8]; // 存储can发送数据

    tx_data[0] = addr; // 地址
    tx_data[1] = 0xF6; // 功能码
    tx_data[2] = dir; // 方向
    tx_data[3] = (uint8_t)(vel >> 8); // 速度(RPM)高8位字节
    tx_data[4] = (uint8_t)(vel >> 0); // 速度(RPM)低8位字节
    tx_data[5] = acc; // 加速度，注意：0是直接启动
    tx_data[6] = 0x00; // 多机同步运动标志
    tx_data[7] = 0x6B; // 校验字节

    can_cmd_send(tx_data, 8);
}

/**
 * @brief 步进电机位置式控制
 * @note 速度和加速度最好保持一致
 * @return 无
 */
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, uint8_t raF, uint8_t snF)
{
    uint8_t tx_data[13]; // 存储can发送数据

	tx_data[0]  =  addr;                      // 地址
	tx_data[1]  =  0xFD;                      // 功能码
	tx_data[2]  =  dir;                       // 方向
	tx_data[3]  =  (uint8_t)(vel >> 8);       // 速度(RPM)高8位字节
	tx_data[4]  =  (uint8_t)(vel >> 0);       // 速度(RPM)低8位字节 
	tx_data[5]  =  acc;                       // 加速度，注意：0是直接启动
	tx_data[6]  =  (uint8_t)(clk >> 24);      // 脉冲数(bit24 - bit31)
	tx_data[7]  =  (uint8_t)(clk >> 16);      // 脉冲数(bit16 - bit23)
	tx_data[8]  =  (uint8_t)(clk >> 8);       // 脉冲数(bit8  - bit15)
	tx_data[9]  =  (uint8_t)(clk >> 0);       // 脉冲数(bit0  - bit7 )
	tx_data[10] =  raF;                       // 多机同步运动标志，false为不启用，true为启用
	tx_data[11] =  snF;                       // 相位/绝对标志，false为相对运动，true为绝对值运动
	tx_data[12] =  0x6B;                      // 校验字节

	can_cmd_send(tx_data, 13);
}

/**
 * @brief 立即停止
 * @param addr 步进电机硬件帧ID
 * @return void
 */
void Emm_V5_Stop_Now(uint8_t addr, uint8_t snF)
{
    uint8_t tx_data[5]; // 存储can发送数据

    tx_data[0] = addr; // 地址
    tx_data[1] = 0xFE; // 功能码
    tx_data[2] = 0x98; // 前一位方向,后一位速度
    tx_data[3] = snF; // 速度
    tx_data[4] = 0x6B; // 加速度
	
	can_cmd_send(tx_data, 5);
}
