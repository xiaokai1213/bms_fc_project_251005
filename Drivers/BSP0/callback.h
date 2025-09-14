#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "main.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);  // hal库定时器统一回调函数

#endif  // _CALLBACK_H
