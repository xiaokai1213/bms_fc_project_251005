#ifndef _STM32F1XX_IT_H
#define _STM32F1XX_IT_H

#include "main.h"

void NVIC_Init(void);

void SysTick_Handler(void);             // 系统滴答中断
void USB_LP_CAN1_RX0_IRQHandler(void);  // can邮箱RX0中断
void CAN1_RX1_IRQHandler(void);         // can邮箱RX1中断
void TIM2_IRQHandler(void);             // 定时器2中断
void TIM4_IRQHandler(void);             // 定时器4中断

#endif  // _STM32F1XX_IT_H
