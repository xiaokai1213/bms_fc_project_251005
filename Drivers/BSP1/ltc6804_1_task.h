
#ifndef _LTC6804_1_TASK_H
#define _LTC6804_1_TASK_H

#include "ltc6804_1_base_drive.h"  //ltc6804驱动
#include "main.h"

#define total_ic 2

extern LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];
extern LTC6804_StatusRegs stat_h_ltc6804[total_ic];
extern BatteryVoltageRegs cv_h_ltc6804[total_ic];
extern AuxVoltageRegs av_h_ltc6804[total_ic];

void LTC6804_init(void);  // ltc6804初始化函数
void ltc6804_st(void);
void ltc6804_cv(void);

uint8_t ltc6804_Open_wire_inspection(void);

#endif  // _LTC6804_1_TASK_H
