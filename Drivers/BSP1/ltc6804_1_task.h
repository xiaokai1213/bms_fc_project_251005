
#ifndef _LTC6804_1_TASK_H
#define _LTC6804_1_TASK_H

#include "ltc6804_1_base_drive.h"  //ltc6804驱动
#include "main.h"

#define total_ic 2

extern LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 6804配置寄存器句柄
extern LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 6804状态寄存器句柄
extern BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 6804电池电压数据寄存器句柄
extern AuxVoltageRegs av_h_ltc6804[total_ic];               // 6804辅助电压寄存器句柄

// ltc6804状态机状态枚举
typedef enum {
   LTC6804_STATE_INIT = 0,             // 初始化
   LTC6804_STATE_CONVERT_VOLTAGE,      // 启动电压ADC转换
   LTC6804_STATE_CONVERT_TEMPERATURE,  // 启动温度ADC转换
   LTC6804_STATE_WAIT_VOLTAGE,         // 等待电压转换完成
   LTC6804_STATE_WAIT_TEMPERATURE,     // 等待温度转换完成
   LTC6804_STATE_READ_VOLTAGE,         // 读取电池电压
   LTC6804_STATE_READ_TEMPERATURE,     // 读取温度
   LTC6804_STATE_IDLE,                 // 空闲
   LTC6804_STATE_ERROR,                // 错误
   LTC6804_STATE
} ltc6804_state_t;  // ltc6804状态机当前状态变量

// ltc6804事件枚举
typedef enum {
   LTC6804_EVENT_INIT = 0,             // 初始化事件
   LTC6804_EVENT_CONVERT_VOLTAGE,      // 启动电压转换事件
   LTC6804_EVENT_CONVERT_TEMPERATURE,  // 启动温度转换事件
   LTC6804_EVENT_READ_VOLTAGE,         // 读取电池电压事件
   LTC6804_EVENT_READ_TEMPERATURE,     // 读取温度事件
   LTC6804_EVENT_IDLE,                 // 空闲事件
   LTC6804_EVENT_ERROR,                // 错误事件
   LTC6804_EVENT
} ltc6804_event_t;  // ltc6804状态机当前事件变量

ltc6804_state_t ltc6804_state_machine(ltc6804_event_t event);  // ltc6804状态机函数
void LTC6804_init(void);                                       // ltc6804初始化函数

// 声明状态变量（在 .c 中定义）
extern volatile ltc6804_state_t LTC6804_STATE;
extern volatile ltc6804_event_t LTC6804_EVENT;

#endif  // _LTC6804_1_TASK_H
