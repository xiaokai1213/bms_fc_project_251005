#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"

// 设置最大电压为4.2V则数值为42000
#define max_cell_voltage 42000  // 最大值不超过65535
#define min_cell_voltage 32000  // 最小值不小于1

extern volatile uint64_t sys_time;  // 系统时间变量，单位：毫秒

// 函数声明

#endif  // _MAIN_H
