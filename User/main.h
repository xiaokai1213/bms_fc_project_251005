#ifndef _MAIN_H
#define _MAIN_H

// 底层hal库头文件和库文件
#include "stdio.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"
// 电池模组参数
#define cell_num         20  // 总共20个电芯

// 设置最大电压为4.2V则数值为42000
#define max_cell_voltage 42000  // 最大值不超过65535
#define min_cell_voltage 32000  // 最小值不小于1

// 状态枚举
typedef enum {
   STATE_INIT = 0,                  // 初始化状态;外设初始化
   STATE_INIT_LTC6804,              // 初始化LTC6804
   STATE_IDLE,                      // 空闲状态
   STATE_COLLECT_VOLTAGE,           // 采集电压
   STATE_COLLECT_TEMPERATURE,       // 采集温度
   STATE_PROCESS_VOLTAGE_DATA,      // 处理电压数据
   STATE_PROCESS_TEMPERATURE_DATA,  // 处理温度数据
   STATE_SEND_VOLTAGE,              // 发送电压
   STATE_SEND_TEMPERATURE,          // 发送温度
   STATE_FAULT_LTC6804,             // LTC6804故障状态
   STATE_FAULT_VOLTAGE_ANOMALY,     // 电压异常故障
   STATE_FAULT_VOLTAGE_OVER,        // 电压过压
   STATE_FAULT_VOLTAGE_UNDER,       // 电压欠压
   STATE_FAULT_VOLTAGE_BALANCE,     // 电压不均衡
   STATE_FAULT_TEMP_OVER,           // 温度过高
   STATE_FAULT_TEMP_UNDER,          // 温度过低
   STATE_FAULT_COMMUNICATION_SPI,   // SPI通信故障
   STATE_FAULT_HARDWARE,            // 硬件故障
   STATE
} BMS_State_t;

// 电压数据管理
typedef struct {
   uint16_t voltage_data;         // 电压值
   uint8_t voltage_anomaly : 1;   // 电压异常位
   uint8_t voltage_can_send : 1;  // 电压发送位
} BATTERY_PACK_DATA_t;
extern BATTERY_PACK_DATA_t bat_pack_data[cell_num];  // 电池组电压数据管理

// 全局标志位管理
typedef struct {
   uint8_t can_send_busy : 1;                    // CAN发送邮箱忙指示,0有空邮箱,1无空邮箱
   uint8_t voltage_can_tx_PF_10_ready_flag : 1;  // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_PF_11_ready_flag : 1;  // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_PF_12_ready_flag : 1;  // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_PF_13_ready_flag : 1;  // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_PF_14_ready_flag : 1;  // 电压发送报文10挂起标志位
} FLAG_t;
extern volatile FLAG_t flag;  // 全局标志位

extern uint8_t bms_address;

// 函数声明
void state_machine_run(void);

#endif  // _MAIN_H
