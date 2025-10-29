#ifndef _CAN_H
#define _CAN_H

#include "main.h"

extern CAN_HandleTypeDef hcan1;  // can外设初始化句柄

void CAN_Init(void);  // can外设初始化函数

#endif  // _CAN_H

/*
控制函数（Control functions）:********************************************************************************************
HAL_CAN_Start
功能：启动CAN模块。
参数：hcan指向CAN_HandleTypeDef结构体指针。
返回值：HAL_StatusTypeDef，表示操作状态（HAL_OK, HAL_ERROR等）。


HAL_CAN_Stop
功能：停止CAN模块。
参数：hcan指向CAN_HandleTypeDef结构体指针。
返回值：HAL_StatusTypeDef。


HAL_CAN_AddTxMessage
功能：将一个消息添加到发送邮箱，请求发送。
参数：
hcan：CAN_HandleTypeDef结构体指针。
pHeader：指向CAN_TxHeaderTypeDef的指针，包含消息标识符、类型、数据长度等。
aData[]：待发送的数据数组（最多8字节）。
pTxMailbox：指向uint32_t的指针，用于返回分配到的邮箱号（0,1,2之一）。
返回值：HAL_StatusTypeDef。


HAL_CAN_GetTxMailboxesFreeLevel
功能：获取当前空闲的发送邮箱数量（0-3）。
参数：hcan指向CAN_HandleTypeDef结构体指针。
返回值：uint32_t，空闲邮箱数量。


HAL_CAN_IsTxMessagePending
功能：检查指定的发送邮箱中是否有待处理的消息（即尚未发送完成）。
参数：
hcan：CAN_HandleTypeDef结构体指针。
TxMailboxes：要检查的邮箱掩码（可以是多个邮箱，例如CAN_TX_MAILBOX0 | CAN_TX_MAILBOX1）。
返回值：uint32_t，返回有 pending 消息的邮箱掩码。


HAL_CAN_GetRxMessage
功能：从指定的接收FIFO（FIFO0或FIFO1）中读取一个消息。
参数：
hcan：CAN_HandleTypeDef结构体指针。
RxFifo：指定接收FIFO，可以是CAN_RX_FIFO0或CAN_RX_FIFO1。
pHeader：指向CAN_RxHeaderTypeDef的指针，用于存储接收到的消息头（标识符、类型、长度等）。
aData[]：用于存储接收到的数据（最多8字节）。
返回值：HAL_StatusTypeDef。


HAL_CAN_GetRxFifoFillLevel
功能：获取指定接收FIFO中已存储的消息数量。
参数：
hcan：CAN_HandleTypeDef结构体指针。
RxFifo：指定接收FIFO，可以是CAN_RX_FIFO0或CAN_RX_FIFO1。
返回值：uint32_t，消息数量（0-3）。


中断管理函数（Interrupts management）:*************************************************************************
HAL_CAN_ActivateNotification
功能：激活指定的CAN中断。
参数：
hcan：CAN_HandleTypeDef结构体指针。
ActiveITs：要激活的中断掩码，可以是以下一个或多个：
CAN_IT_TX_MAILBOX_EMPTY: 发送邮箱空中断（当发送邮箱为空时，即发送完成）
CAN_IT_RX_FIFO0_MSG_PENDING: FIFO0消息挂起中断（FIFO0中有新消息）
CAN_IT_RX_FIFO0_FULL: FIFO0满中断
CAN_IT_RX_FIFO0_OVERRUN: FIFO0溢出中断
CAN_IT_RX_FIFO1_MSG_PENDING: FIFO1消息挂起中断
CAN_IT_RX_FIFO1_FULL: FIFO1满中断
CAN_IT_RX_FIFO1_OVERRUN: FIFO1溢出中断
CAN_IT_WAKEUP: 唤醒中断
CAN_IT_SLEEP_ACK: 睡眠模式确认中断
CAN_IT_ERROR: 错误中断
CAN_IT_ERROR_WARNING: 错误警告中断
CAN_IT_ERROR_PASSIVE: 错误被动中断
CAN_IT_BUSOFF: 总线关闭中断
CAN_IT_LAST_ERROR_CODE: 最后一次错误代码中断
CAN_IT_TX_MAILBOX_ABORT: 发送邮箱中止中断（当发送请求被中止时）
返回值：HAL_StatusTypeDef。


HAL_CAN_DeactivateNotification
功能：禁用指定的CAN中断。
参数：
hcan：CAN_HandleTypeDef结构体指针。
InactiveITs：要禁用的中断掩码（与ActiveITs相同的中断列表）。
返回值：HAL_StatusTypeDef。


HAL_CAN_IRQHandler
功能：CAN中断请求处理函数。当发生CAN中断时，由用户调用（通常在stm32f4xx_it.c的中断服务函数中调用）。
参数：hcan指向CAN_HandleTypeDef结构体指针。
返回值：无。


注意：使用中断时，需要先使用HAL_CAN_ActivateNotification激活中断，然后在中断服务函数中调用HAL_CAN_IRQHandler，
该函数会根据中断标志位调用相应的回调函数（例如HAL_CAN_TxMailbox0CompleteCallback等）。


CAN回调函数：******************************************************************************************************
*void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef hcan);
当CAN的发送邮箱0（Tx Mailbox 0）完成一次发送时，此回调函数被调用。
用户可以在此函数中处理发送完成后的操作，例如释放资源、启动下一次发送等。


*void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef hcan);
当CAN的发送邮箱1（Tx Mailbox 1）完成一次发送时，此回调函数被调用。
用法同邮箱0。


*void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef hcan);
当CAN的发送邮箱2（Tx Mailbox 2）完成一次发送时，此回调函数被调用。
用法同邮箱0。


*void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef hcan);
当CAN的发送邮箱0的发送被中止时，此回调函数被调用。
用户可以在此函数中处理发送被中止的情况，例如记录错误、重新尝试发送等。


*void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef hcan);
当CAN的发送邮箱1的发送被中止时，此回调函数被调用。
用法同邮箱0。


*void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef hcan);
当CAN的发送邮箱2的发送被中止时，此回调函数被调用。
用法同邮箱0。


*void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef hcan);
当CAN的接收Fifo0中有消息挂起（即收到新消息）时，此回调函数被调用。
用户可以在此函数中读取接收到的消息。


*void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef hcan);
当CAN的接收Fifo0已满时，此回调函数被调用。
用户可以在此函数中处理Fifo0满的情况，例如读取数据以防止数据丢失。


*void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef hcan);
当CAN的接收Fifo1中有消息挂起（即收到新消息）时，此回调函数被调用。
用法同Fifo0。


*void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef hcan);
当CAN的接收Fifo1已满时，此回调函数被调用。
用法同Fifo0。


*void HAL_CAN_SleepCallback(CAN_HandleTypeDef hcan);
当CAN进入睡眠模式时，此回调函数被调用。
用户可以在此函数中处理CAN进入睡眠模式后的相关操作。


*void HAL_CAN_WakeUpFromRxMsgCallback(CAN_HandleTypeDef hcan);
当CAN从睡眠模式被接收到的消息唤醒时，此回调函数被调用。
用户可以在此函数中处理唤醒后的操作。


*void HAL_CAN_ErrorCallback(CAN_HandleTypeDef hcan);
当CAN发生错误时，此回调函数被调用。
用户可以在此函数中处理错误，例如记录错误类型、恢复通信等。


注意：这些回调函数都是弱定义（weak）的，意味着如果用户没有在自己的代码中重新定义它们，
那么将使用HAL库中的默认实现（通常是空函数）。用户可以在自己的代码中重新实现这些函数，以便在特定事件发生时执行自定义操作。

*/
