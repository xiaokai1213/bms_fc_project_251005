#include "callback.h"
#include "delay.h"
#include "spi.h"
#include "tim.h"

volatile uint32_t delaytime = 0;                      // 延时器计数器（全局变量）
task_scheduler_t task_5ms = {0, 5, 0};                // 5ms周期任务调度器定义
task_scheduler_t task_temp_collect = {0, 100, 0};     // 100ms周期任务调度器定义
task_scheduler_t task_voltage_collect = {0, 100, 0};  // 100ms周期任务调度器定义

/**
 * @brief 基本定时器周期结束回调函数
 * @param 定时器句柄，传入一个定时器
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
   if (htim->Instance == TIM2) {  // 定时器2，延时定时器,1ms触发一次
      if (delaytime != 0) {       // 判断定时器计数器是否为零
         delaytime--;             // 若不为零则递减
      }
   }

   if (htim->Instance == TIM4) {  // 定时器4，后台定时器,1ms触发一次

      // 更新5ms任务调度器
      task_5ms.counter++;
      if (task_5ms.counter >= task_5ms.period) {
         task_5ms.counter = 0;  // 重置计数器
         task_5ms.flag = 1;     // 标志位置1
      }

      // 更新100ms温度采集任务调度器
      task_temp_collect.counter++;
      if (task_temp_collect.counter >= task_temp_collect.period) {
         task_temp_collect.counter = 0;  // 重置计数器
         task_temp_collect.flag = 1;     // 标志位置1
      }

      // 更新100ms电压采集任务调度器
      task_voltage_collect.counter++;
      if (task_voltage_collect.counter >= task_voltage_collect.period) {
         task_voltage_collect.counter = 0;  // 重置计数器
         task_voltage_collect.flag = 1;     // 标志位置1
      }
   }
}

/**
 * @brief SPI发送完成回调函数
 * @param hspi: SPI句柄指针
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
   if (hspi->Instance == SPI1) {
      spi1_tx_it_end_sign = 1;  // 设置发送完成标志
   }
}

/**
 * @brief spi接收完成回调函数
 * @param hspi：spi句柄
 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi) {
   if (hspi->Instance == SPI1) {
      spi1_rx_it_end_sign = 1;  // 设置接收完成标志
   }
}
