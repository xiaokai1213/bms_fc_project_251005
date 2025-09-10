#ifndef _TIM_H
#define _TIM_H

#include "main.h"

extern TIM_HandleTypeDef htim2;  // 定时器2句柄
extern TIM_HandleTypeDef htim4;  // 定时器4句柄

void TIM2_Init(void);

#endif  // _TIM_H
