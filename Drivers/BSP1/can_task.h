#ifndef _CAN_TASK_H
#define _CAN_TASK_H

#include "main.h"

// can报文发送标志位管理
typedef struct {
   uint8_t voltage_can_tx_PF_10_flag : 1;  // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_PF_11_flag : 1;  // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_PF_12_flag : 1;  // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_PF_13_flag : 1;  // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_PF_14_flag : 1;  // 电压发送报文10挂起标志位
} CAN_TX_t;
extern CAN_TX_t can_tx_flag = {0};  // 发送相关标志位

void can_tx_extid_8(uint32_t extid, uint8_t data[8]);

#endif  // _CAN_TASK_H
