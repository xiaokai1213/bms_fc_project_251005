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

// BMS状态枚举定义
typedef enum {
   BMS_FC_STATE_FAULT = 0,    // 故障状态
   BMS_FC_STATE_BALANCE_OFF,  // 均衡关闭状态
   BMS_FC_STATE_BALANCE_ON,   // 均衡开启状态
   BMS_FC_STATE
} BMS_FC_Status_t;

// BMS状态标志位管理结构体
typedef struct {
   union {
      struct {
         uint8_t over_voltage : 1;      // 过压标志位
         uint8_t under_voltage : 1;     // 欠压标志位
         uint8_t over_temperature : 1;  // 过温标志位
         uint8_t reserved : 5;          // 保留位
      } bits;
      uint8_t all_fault_flags;  // 所有故障标志位
   } Fault;
} BMS_FC_Flag_t;

// 函数声明
void Basic_periodic_task_run(void);  // 基本周期任务运行函数

#endif  // _MAIN_H
