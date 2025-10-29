#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "main.h"

// 周期任务调度结构体
typedef struct {
   uint16_t counter;  // 计数器
   uint16_t period;   // 周期
   uint8_t flag;     // 标志位
} task_scheduler_t;

extern volatile uint32_t delaytime;                     // 延时器全局变量
extern volatile task_scheduler_t task_5ms;              // 5ms周期任务调度器定义
extern volatile task_scheduler_t task_temp_collect;     // 10ms周期任务调度器定义
extern volatile task_scheduler_t task_voltage_collect;  // 100ms周期任务调度器定义

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);  // hal库定时器统一回调函数
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi);         // spi接收回调函数
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi);         // spi发送回调函数

// 发送完成中断回调：用户可以在这些函数中处理发送完成后的操作，例如释放资源、启动下一次发送等。
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan);  // 当CAN的发送邮箱0（Tx Mailbox 0）完成一次发送时，此回调函数被调用。
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan);  // 当CAN的发送邮箱1（Tx Mailbox 1）完成一次发送时，此回调函数被调用。 用法同邮箱0。
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan);  // 当CAN的发送邮箱2（Tx Mailbox 2）完成一次发送时，此回调函数被调用。 用法同邮箱0。

// 发送中止中断回调：
void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef* hcan);  // 当CAN的发送邮箱0的发送被中止时，此回调函数被调用。
void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef* hcan);  // 当CAN的发送邮箱1的发送被中止时，此回调函数被调用。用法同邮箱0。
void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef* hcan);  // 当CAN的发送邮箱2的发送被中止时，此回调函数被调用。用法同邮箱0。

// FIFO0接收中断回调
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan);  // 当CAN的接收Fifo0中有消息挂起（即收到新消息）时，此回调函数被调用。
void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef* hcan);        // 当CAN的接收Fifo0已满时，此回调函数被调用。

// FIFO1接收中断回调
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan);  // 当CAN的接收Fifo1中有消息挂起（即收到新消息）时，此回调函数被调用。
void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef* hcan);        // 当CAN的接收Fifo1已满时，此回调函数被调用。

// can错误处理中断回调
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan);  // 当CAN发生错误时，此回调函数被调用。

#endif  // _CALLBACK_H
