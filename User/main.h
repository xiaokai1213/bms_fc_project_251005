#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"

// 设置最大电压为4.2V则数值为42000
#define max_cell_voltage 42000  // 最大值不超过65535
#define min_cell_voltage 32000  // 最小值不小于1

// 全局标志位句柄
typedef struct {
   uint8_t collect_voltage_flag;      // 采集电压
   uint8_t collect_temperature_flag;  // 采集温度
   uint8_t send_voltage_data_flag;    // 发送电压数据标志位
   uint8_t pec_fault;                 // PEC校验故障
} flag_handle_t;
extern volatile flag_handle_t flag;

// 函数声明
void Task_Update(void);  // 任务更新

#endif  // _MAIN_H
