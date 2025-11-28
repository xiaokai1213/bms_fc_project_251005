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
   // 一级故障标志位联合体
   union {
      uint8_t All_Fault_Level_01;  // 所有故障标志位
      struct {
         uint8_t spi_communication_timeout : 1;  // spi通讯超时标志位
         uint8_t over_voltage : 1;               // 过压标志位
         uint8_t under_voltage : 1;              // 欠压标志位
         uint8_t over_temperature : 1;           // 过温标志位
         uint8_t reserved : 4;                   // 保留位
      } bits;                                    // 故障位域字段
   } Fault_Level_01;                             // 一级故障标志位联合体

   // 二级故障标志位联合体
   union {
      uint8_t All_Fault_Level_02;  // 所有故障标志位
      struct {
         uint8_t can_communication_fault : 1;  // can通讯故障标志位
         uint8_t reserved : 7;                 // 保留位
      } bits;                                  // 故障位域字段
   } Fault_Level_02;                           // 二级故障标志位联合体

   // 均衡使能标志位
   union {
      uint32_t All_Balance_Enable;  // 所有均衡使能标志位
      struct {
         uint32_t Balance_on_or_off : 1;       // 整体均衡使能开关
         uint32_t cell_balance_enable_1 : 1;   // 电芯1均衡使能标志位
         uint32_t cell_balance_enable_2 : 1;   // 电芯2均衡使能标志位
         uint32_t cell_balance_enable_3 : 1;   // 电芯3均衡使能标志位
         uint32_t cell_balance_enable_4 : 1;   // 电芯4均衡使能标志位
         uint32_t cell_balance_enable_5 : 1;   // 电芯5均衡使能标志位
         uint32_t cell_balance_enable_6 : 1;   // 电芯6均衡使能标志位
         uint32_t cell_balance_enable_7 : 1;   // 电芯7均衡使能标志位
         uint32_t cell_balance_enable_8 : 1;   // 电芯8均衡使能标志位
         uint32_t cell_balance_enable_9 : 1;   // 电芯9均衡使能标志位
         uint32_t cell_balance_enable_10 : 1;  // 电芯10均衡使能标志位
         uint32_t cell_balance_enable_11 : 1;  // 电芯11均衡使能标志位
         uint32_t cell_balance_enable_12 : 1;  // 电芯12均衡使能标志位
         uint32_t cell_balance_enable_13 : 1;  // 电芯13均衡使能标志位
         uint32_t cell_balance_enable_14 : 1;  // 电芯14均衡使能标志位
         uint32_t cell_balance_enable_15 : 1;  // 电芯15均衡使能标志位
         uint32_t cell_balance_enable_16 : 1;  // 电芯16均衡使能标志位
         uint32_t cell_balance_enable_17 : 1;  // 电芯17均衡使能标志位
         uint32_t cell_balance_enable_18 : 1;  // 电芯18均衡使能标志位
         uint32_t cell_balance_enable_19 : 1;  // 电芯19均衡使能标志位
         uint32_t cell_balance_enable_20 : 1;  // 电芯20均衡使能标志位
         uint32_t reserved : 11;               // 保留位
      } bits;                                  // 均衡位域字段
   } Balance_Enable;                           // 均衡使能标志位联合体

} BMS_FC_Flag_t;
extern volatile BMS_FC_Flag_t bmsfc_flag;  // BMS状态标志位管理句柄

// 函数声明
void bmsfc_state_machine_run(void);  // BMS状态机运行函数
void sys_init(void);                 // 系统初始化函数

#endif  // _MAIN_H
