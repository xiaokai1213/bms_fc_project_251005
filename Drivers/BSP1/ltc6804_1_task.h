
#ifndef _LTC6804_1_TASK_H
#define _LTC6804_1_TASK_H

#include "ltc6804_1_base_drive.h"  //ltc6804驱动
#include "main.h"

#define total_ic 2

extern LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 6804配置寄存器句柄
extern LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 6804状态寄存器句柄
extern BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 6804电池电压数据寄存器句柄
extern AuxVoltageRegs av_h_ltc6804[total_ic];               // 6804辅助电压寄存器句柄

void LTC6804_init(void);      // ltc6804初始化函数
void wr_6804_reg(void);       // 写ltc6804寄存器
void re_6804_cell_vot(void);  // 读取电池电压
void re_6804_aux_vot(void);   // 读取辅助电压
void re_6804_stat(void);      // 读取状态寄存器

#endif  // _LTC6804_1_TASK_H
