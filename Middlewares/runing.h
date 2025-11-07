#ifndef _RUNING_H
#define _RUNING_H

#include "main.h"

typedef struct {
   uint8_t collect_voltage_flag;      // 采集电压
   uint8_t collect_temperature_flag;  // 采集温度
   uint8_t send_voltage_data_flag;    // 发送电压数据标志位
   uint8_t pec_fault;                 // PEC校验故障
} runing_management_flag_handle_t;

void runing_execute(void);  // 运行执行函数
void Task_Update();         // 任务更新

#endif  // _RUNING_H
