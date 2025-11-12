#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"

// 设置最大电压为4.2V则数值为42000
#define max_cell_voltage 42000  // 最大值不超过65535
#define min_cell_voltage 32000  // 最小值不小于1

typedef struct {
   uint8_t collect_voltage_flag;      // 采集电压
   uint8_t collect_temperature_flag;  // 采集温度
   uint8_t send_voltage_data_flag;    // 发送电压数据标志位
   uint8_t pec_fault;                 // PEC校验故障
} bms_flag_t;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 状态枚举定义-列出bms从控所有可能状态
typedef enum {
   state_init = 0,    // 初始化：外设初始化；ltc6804初始化
   state_fault,       // 故障状态
   state_runing,      // 运行状态
   state_self_check,  // 自检
   s
} bms_state_t;

// 事件枚举定义-定义触发状态转换的所有事件
typedef enum {
   event_power_on = 0,  // 上电
   event_enter_runing,  // 进入运行状态
   event_self_check,    // 自检
   e
} bms_event_t;

// 全局标志位结构体，标志位统一管理
typedef struct {
   uint8_t flag_fault;  // 故障标志位
} bms_flag_tt;

// 全局时钟，1ms计时一次，挂载在系统滴答定时器上
typedef struct {
   uint64_t time_now;
   uint64_t time_new;
   uint64_t time_old;
} bms_time_t;

// 全局统一管理结构体，包含状态、事件、标志、时间
typedef struct {
   bms_state_t stat;
   bms_event_t event;
   bms_flag_t flag;
   bms_time_t time;
} bms_t;

extern volatile bms_t bms;  // 全局结构体

// 函数声明
void bms_state_machine_dispatch(void);        // 状态机调度函数
void bms_sm_handle_event(bms_event_t event);  // 状态机事件处理函数，状态机核心代码，根据当前事件决定状态转换

#endif  // _MAIN_H
