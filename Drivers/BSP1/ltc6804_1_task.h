#ifndef _LTC6804_1_TASK_H
#define _LTC6804_1_TASK_H

#include "ltc6804_1_base_drive.h"  //ltc6804驱动
#include "main.h"

// 使用 main.c 中的系统时钟变量（单位：ms）
extern volatile uint64_t sys_time;

#define total_ic                    2   // 6804芯片数量
#define LTC6804_VOLTAGE_CONV_MS     10  // 电池电压转换时间
#define LTC6804_TEMPERATURE_CONV_MS 10  // 温度转换时间

extern LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 6804配置寄存器句柄
extern LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 6804状态寄存器句柄
extern BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 6804电池电压数据寄存器句柄
extern AuxVoltageRegs av_h_ltc6804[total_ic];               // 6804辅助电压寄存器句柄

typedef enum {
   LTC6804_RESP_OK = 0,  // 事件响应完成
   LTC6804_RESP_BUSY,    // 芯片忙
   LTC6804_RESP_ERROR,   // 响应错误
   LTC6804_RESP
} LTC6804_ResponseEnum_t;  // LTC6804响应状态枚举定义

typedef enum {
   LTC6804_EVENT_NONE = 0,             // 无事件
   LTC6804_EVENT_COLLECT_VOLTAGE,      // 采集电压事件
   LTC6804_EVENT_COLLECT_TEMPERATURE,  // 采集温度事件
   LTC6804_EVENT
} LTC6804_EventEnum_t;  // LTC6804事件枚举定义

// LTC6804状态机状态枚举定义
typedef enum {
   LTC6804_STATE_IDLE = 0,               // 空闲状态
   LTC6804_STATE_WAIT_VOLTAGE_CONV,      // 等待电压转换完成状态
   LTC6804_STATE_WAIT_TEMPERATURE_CONV,  // 等待温度转换完成状态
   LTC6804_STATE
} LTC6804_StateEnum_t;  // LTC6804状态机状态枚举定义

uint8_t LTC6804_init(void);                                          // ltc6804初始化函数
LTC6804_ResponseEnum_t LTC6804_task_run(LTC6804_EventEnum_t event);  // ltc6804任务函数

#endif  // _LTC6804_1_TASK_H
