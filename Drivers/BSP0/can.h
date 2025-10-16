#ifndef _CAN_H
#define _CAN_H

#include "main.h"

extern CAN_HandleTypeDef hcan;  // can外设初始化句柄

void CAN_Init(void);  // can外设初始化函数

#endif  // _CAN_H

/*
函数名称	功能描述	关键参数说明	返回值与说明
HAL_CAN_Init	初始化CAN控制器	hcan: CAN句柄指针，包含实例（如CAN1）、Init结构体（波特率分频器、模式、时间参数等）	HAL_StatusTypeDef: HAL_OK 成功，HAL_ERROR 失败
HAL_CAN_ConfigFilter	配置CAN滤波器（必需，否则可能无法正常接收）	hcan: CAN句柄指针；sFilterConfig: 滤波器配置结构体（FilterIdHigh/FilterIdLow, FilterMaskIdHigh/FilterMaskIdLow,
模式、尺度、FIFO分配、启用等）	HAL_StatusTypeDef HAL_CAN_Start	启动CAN控制器	hcan: CAN句柄指针	HAL_StatusTypeDef HAL_CAN_Stop	停止CAN控制器	hcan: CAN句柄指针	HAL_StatusTypeDef
HAL_CAN_RequestSleep	请求CAN控制器进入睡眠模式	hcan: CAN句柄指针	HAL_StatusTypeDef
HAL_CAN_WakeUp	将CAN控制器从睡眠模式唤醒	hcan: CAN句柄指针	HAL_StatusTypeDef
HAL_CAN_GetTxMailboxesFreeLevel	获取空闲发送邮箱的数量	hcan: CAN句柄指针	uint8_t: 空闲发送邮箱的数量
HAL_CAN_IsTxMessagePending	检查指定发送邮箱是否有消息等待发送	hcan: CAN句柄指针；TxMailboxes: 要检查的邮箱位掩码	uint32_t: 仍有消息 pending 的邮箱的位掩码
HAL_CAN_GetRxFifoFillLevel	获取指定接收FIFO中填充的消息数	hcan: CAN句柄指针；RxFifo: 接收FIFO编号（CAN_RX_FIFO0或CAN_RX_FIFO1）	uint8_t: FIFO中的消息数量
HAL_CAN_AddTxMessage	将消息添加到发送邮箱，等待发送	hcan: CAN句柄指针；pHeader: 发送消息头结构体；aData: 待发送数据数组；pTxMailbox: 指向变量，用于返回使用的邮箱号	HAL_StatusTypeDef:
HAL_OK成功加入邮箱 HAL_CAN_GetRxMessage	从接收FIFO获取消息	hcan: CAN句柄指针；RxFifo: 接收FIFO编号；pHeader: 存放接收到的消息头；aData: 存放接收数据的数组	HAL_StatusTypeDef: HAL_OK成功获取
HAL_CAN_ActivateNotification	激活指定的CAN中断通知	hcan: CAN句柄指针；ActiveITs: 要使能的中断源位掩码（如CAN_IT_RX_FIFO0_MSG_PENDING）	HAL_StatusTypeDef
HAL_CAN_DeactivateNotification	关闭指定的CAN中断通知	hcan: CAN句柄指针；ActiveITs: 要关闭的中断源位掩码	HAL_StatusTypeDef
HAL_CAN_IRQHandler	CAN全局中断服务例程	hcan: CAN句柄指针	无
HAL_CAN_RxFifo0MsgPendingCallback	FIFO0消息挂起中断回调函数（用户常需实现）	hcan: 触发中断的CAN句柄指针	无
HAL_CAN_RxFifo1MsgPendingCallback	FIFO1消息挂起中断回调函数	hcan: 触发中断的CAN句柄指针	无
HAL_CAN_TxMailboxCompleteCallback	发送邮箱完成中断回调函数	hcan: CAN句柄指针；TxMailbox: 完成发送的邮箱号	无
HAL_CAN_TxMailboxAbortCallback	发送邮箱中止回调函数	hcan: CAN句柄指针；TxMailbox: 中止发送的邮箱号	无
HAL_CAN_ErrorCallback	CAN错误回调函数	hcan: CAN句柄指针	无
*/
