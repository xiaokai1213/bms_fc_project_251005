#include "callback.h"
#include "can.h"
#include "delay.h"
#include "main.h"
#include "spi.h"
#include "tim.h"

volatile task_scheduler_t task_collect_voltage = {0, 200, 0};      // 采集电压周期任务调度器定义
volatile task_scheduler_t task_collect_temperature = {0, 210, 0};  // 采集温度周期任务调度器定义
volatile task_scheduler_t task_can_tx_voltage_send = {0, 500, 0};  // 电压发送周期任务调度器定义
volatile task_scheduler_t task_500ms = {0, 500, 0};                // 500ms周期任务调度器定义

/********************************************************TIM定时器中断****************************************************/
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

      // 更新采集电压任务调度器
      task_collect_voltage.counter++;
      if (task_collect_voltage.counter >= task_collect_voltage.period) {
         task_collect_voltage.counter = 0;  // 重置计数器
         task_collect_voltage.flag = 1;     // 标志位置1
      }

      // 更新采集温度任务调度器
      task_collect_temperature.counter++;
      if (task_collect_temperature.counter >= task_collect_temperature.period) {
         task_collect_temperature.counter = 0;  // 重置计数器
         task_collect_temperature.flag = 1;     // 标志位置1
      }

      // 电压发送周期任务调度器
      task_can_tx_voltage_send.counter++;
      if (task_can_tx_voltage_send.counter >= task_can_tx_voltage_send.period) {
         task_can_tx_voltage_send.counter = 0;  // 重置计数器
         task_can_tx_voltage_send.flag = 1;     // 标志位置1
      }

      // 更新500ms任务调度器
      task_500ms.counter++;
      if (task_500ms.counter >= task_500ms.period) {
         task_500ms.counter = 0;  // 重置计数器
         task_500ms.flag = 1;     // 标志位置1
      }
   }
}

/********************************************************SPI中断****************************************************/
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

/********************************************************CAN中断发送完成****************************************************/
/**
 * @brief   当CAN的发送邮箱0（Tx Mailbox 0）完成一次发送时，此回调函数被调用。
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan) {
   flag.can_send_busy = 0;  // can发送邮箱状态置0
}

/**
 * @brief   当CAN的发送邮箱1（Tx Mailbox 1）完成一次发送时，此回调函数被调用。
 */
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan) {
   flag.can_send_busy = 0;  // can发送邮箱状态置0
}

/**
 * @brief   当CAN的发送邮箱2（Tx Mailbox 2）完成一次发送时，此回调函数被调用。
 */
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan) {
   flag.can_send_busy = 0;  // can发送邮箱状态置0
}

/********************************************************CAN中断发送被中止****************************************************/
/**
 * @brief   当CAN的发送邮箱0的发送被中止时，此回调函数被调用。
 */
void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef* hcan) {}

/**
 * @brief   当CAN的发送邮箱1的发送被中止时，此回调函数被调用。用法同邮箱0。
 */
void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef* hcan) {}

/**
 * @brief   当CAN的发送邮箱2的发送被中止时，此回调函数被调用。用法同邮箱0。
 */
void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef* hcan) {}

/********************************************************CAN邮箱0接收中断****************************************************/
/**
 * @brief   当CAN的接收Fifo0中有消息挂起（即收到新消息）时，此回调函数被调用。
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {}

/**
 * @brief   当CAN的接收Fifo0已满时，此回调函数被调用。
 */
void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef* hcan) {}

/********************************************************CAN邮箱1接收中断****************************************************/
/**
 * @brief   当CAN的接收Fifo1中有消息挂起（即收到新消息）时，此回调函数被调用。
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan) {}

/**
 * @brief   当CAN的接收Fifo1已满时，此回调函数被调用。
 */
void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef* hcan) {}

/********************************************************CAN错误处理****************************************************/
/**
 * @brief   当CAN发生错误时，此回调函数被调用。
 */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan) {}
