#ifndef _DELAY_H
#define _DELAY_H

#include "main.h"
#include "sys.h"

extern volatile uint32_t delaytime;  // 延时器全局变量

void delay_ms(uint16_t time);

#endif  // _DELAY_H
