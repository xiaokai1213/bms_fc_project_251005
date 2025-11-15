#include "delay.h"
#include "callback.h"
#include "tim.h"

volatile uint32_t delaytime = 0;  // 延时器计数器（全局变量）

// 毫秒延时，最大65535ms
void delay_ms(uint16_t ntime) {
   delaytime = ntime;              // 将延时值赋予定时器全局变量
   HAL_TIM_Base_Start_IT(&htim2);  // 以基础中断模式开启定时器4
   while (delaytime != 0)          // 循环检测delaytime是否归零
      ;
   HAL_TIM_Base_Stop_IT(&htim2);  // 停止基础中断模式定时器4
}
