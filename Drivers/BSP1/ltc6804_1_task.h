
#ifndef _LTC6804_1_TASK_H
#define _LTC6804_1_TASK_H

#include "ltc6804_1_base_drive.h"  //ltc6804驱动
#include "main.h"

#define total_ic 2

extern LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 6804配置寄存器句柄
extern LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 6804状态寄存器句柄
extern BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 6804电池电压数据寄存器句柄
extern AuxVoltageRegs av_h_ltc6804[total_ic];               // 6804辅助电压寄存器句柄

void LTC6804_init(void);             // ltc6804初始化函数
uint8_t ltc6804_Get_Voltage(void);   // ltc6804电池电压获取函数
void ltc6804_Get_temperature(void);  // ltc6804温度获取函数
void ltc6804_st(void);
void ltc6804_cv(void);

uint8_t ltc6804_Open_wire_inspection(void);

#endif  // _LTC6804_1_TASK_H
