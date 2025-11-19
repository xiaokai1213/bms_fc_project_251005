
#ifndef _LTC6804_1_TASK_H
#define _LTC6804_1_TASK_H

#include "ltc6804_1_base_drive.h"  //ltc6804驱动
#include "main.h"

#define total_ic 2

extern LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 6804配置寄存器句柄
extern LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 6804状态寄存器句柄
extern BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 6804电池电压数据寄存器句柄
extern AuxVoltageRegs av_h_ltc6804[total_ic];               // 6804辅助电压寄存器句柄

void LTC6804_init(void);  // ltc6804初始化函数

/* 周期性采集任务（放在此文件以便集中管理所有与 LTC6804 相关的任务） */
void ltc_sample_task(void* arg);
void ltc_sample_read_task(void* arg);

/* 周期性温度采集任务 */
void ltc_temp_sample_task(void* arg);
void ltc_temp_read_task(void* arg);

/* 读取就绪标志，1=有新数据未处理 */
extern volatile uint8_t ltc_cv_updated;
extern volatile uint8_t ltc_av_updated;

/* 原子获取并清除就绪标志（消费者使用） */
uint8_t ltc_get_and_clear_cv_updated(void);
uint8_t ltc_get_and_clear_av_updated(void);

#endif  // _LTC6804_1_TASK_H
