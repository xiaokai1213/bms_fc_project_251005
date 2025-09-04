#ifndef _CAN_H
#define _CAN_H

#include "main.h"

extern CAN_HandleTypeDef hcan;  // can外设初始化句柄

void CAN_Init(void);  // can外设初始化函数

#endif  // _CAN_H
