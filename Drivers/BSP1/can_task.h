#ifndef _CAN_TASK_H
#define _CAN_TASK_H

#include "main.h"

uint8_t can_tx_voltage_data(void);                        // 通过can通讯发送电压数据
uint8_t can_tx_temperature_data(void);                    // 通过can通讯发送温度数据
uint8_t can_tx_extid_8(uint32_t extid, uint8_t data[8]);  // 扩展can发送八位数据帧

#endif  // _CAN_TASK_H
