#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"

// 状态枚举定义-列出bms从控所有可能状态
typedef enum {
   state_init = 0,    // 初始化：外设初始化；ltc6804初始化；等待接收主控关键数据
   state_self_test,   // 自检状态：检查ltc6804关键数据
   state_idle,        // 空闲状态
   state_fault,       // 故障状态
   state_working,     // 工作状态
   state_receive_cmd  // 接收主控命令
} bms_state_t;

// 事件枚举定义-定义触发状态转换的所有事件
typedef enum {
   event_power_on = 0,  // 上电事件
} bms_event_t;

// 全局枚举声明
extern bms_state_t bms_current_stat;  // bms当前主状态枚举定义

// 函数声明
void stand_xy(void);    // 用于暂时存储周期性代码
void flag_task_time();  // 重构的周期性任务代码

void main_stat_init();  // 主函数状态初始化

#endif  // _MAIN_H
