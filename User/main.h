#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"

// 状态枚举定义-列出bms从控所有可能状态
typedef enum {
   state_init = 0,   // 初始化：外设初始化；ltc6804初始化；等待接收主控关键数据
   state_self_test,  // 自检状态：检查ltc6804关键数据
   state_fault,      // 故障状态
   state_runing,     // 运行状态
   state_idle        // 空闲状态
} bms_state_t;

// 事件枚举定义-定义触发状态转换的所有事件
typedef enum {
   event_power_on = 0,  // 上电事件
} bms_event_t;

// 状态机结构 - 管理状态机运行所需的所有数据
typedef struct {
   bms_state_t current_state;  // 当前状态
   bms_state_t new_state;      // 新状态
   bms_event_t current_event;  // 当前事件
   bms_event_t new_event;      // 新事件
} bms_state_machine_t;

// 全局枚举声明
extern bms_state_machine_t bms_sm;  // bms当前主状态枚举定义

// 函数声明
void bms_sm_dispatch();                       // 状态机调度函数
void bms_sm_handle_event(bms_event_t event);  // 状态机事件处理函数，状态机核心代码，根据当前事件决定状态转换

#endif  // _MAIN_H
