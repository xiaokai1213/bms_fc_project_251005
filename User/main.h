#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"

// 电池模组参数
#define cell_num             20  // 总共20个电芯
#define temp_num             10  // 总共10个温度检测点

// 设置最大电压为4.2V则数值为42000
#define max_cell_voltage     42000  // 电压最大值不超过65535
#define min_cell_voltage     32000  // 电压最小值不小于1

// 设置最大温度
#define max_cell_temperature 32000  // 最大温度对应的电压值

// 全局变量
extern volatile uint64_t sys_time;  // 系统时钟

#endif  // _MAIN_H
