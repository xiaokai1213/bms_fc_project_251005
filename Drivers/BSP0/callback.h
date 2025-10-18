#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "main.h"

// 周期任务调度结构体
typedef struct {
   volatile uint8_t counter;  // 计数器
   volatile uint8_t period;   // 周期
   volatile uint8_t flag;     // 标志位
} task_scheduler_t;

extern volatile uint32_t delaytime;  // 延时器全局变量
extern task_scheduler_t task_5ms;    // 5ms周期任务调度器定义
extern task_scheduler_t task_10ms;   // 10ms周期任务调度器定义
extern task_scheduler_t task_100ms;  // 100ms周期任务调度器定义

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);  // hal库定时器统一回调函数
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi);         // spi接收回调函数
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi);         // spi发送回调函数

#endif  // _CALLBACK_H
