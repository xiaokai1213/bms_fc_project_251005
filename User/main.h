#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"

// 设置最大电压为4.2V则数值为42000
#define max_cell_voltage 42000  // 最大值不超过65535
#define min_cell_voltage 32000  // 最小值不小于1

// 状态枚举定义-列出bms从控所有可能状态
typedef enum {
   state_init = 0,  // 初始化：外设初始化；ltc6804初始化
   state_standby,   // 待机状态：进行自检和检查任务运行标志位
   state_fault,     // 故障状态
   state_runing,    // 运行状态
   state_idle,      // 空闲状态
   s
} bms_state_t;

// 事件枚举定义-定义触发状态转换的所有事件
typedef enum {
   event_power_on = 0,        // 上电
   event_enter_standby,       // 进入待机
   event_self_test,           // 自检
   event_voltage_collect,     // 电压采集
   event_temp_collect,        // 温度采集
   event_voltage_data_send,   // 电压数据发送
   event_temp_data_send,      // 温度数据发送
   event_data_send_complete,  // 数据发送完成
   e
} bms_event_t;

// 全局枚举声明
extern bms_state_t bms_state;      // bms当前主状态枚举定义
extern bms_event_t event_trigger;  // 事件戳

// 函数声明
void bms_state_machine_dispatch(void);        // 状态机调度函数
void bms_sm_handle_event(bms_event_t event);  // 状态机事件处理函数，状态机核心代码，根据当前事件决定状态转换

#endif  // _MAIN_H
