#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"

typedef struct {
   uint8_t voltage_sampling_flag;         // 电压采集标志位
   uint8_t temperature_sampling_flag;     // 温度采集标志位
   uint8_t under_voltage_alarm_flag;      // 电压过低标志位
   uint8_t over_voltage_alarm_flag;       // 电压过高标志位
   uint8_t under_temperature_alarm_flag;  // 温度过低标志位
   uint8_t over_temperature_alarm_flag;   // 温度过高标志位
} system_flags_compact_t;

extern system_flags_compact_t bms_flags;

void stand_xy(void);  // 用于暂时存储周期性代码

void flag_task_time();  // 重构的周期性任务代码

#endif  // _MAIN_H
