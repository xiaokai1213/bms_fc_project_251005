#ifndef _USART_H
#define _USART_H

#include "main.h"
#include "sys.h"

#define USART_REC_LEN 200  // 定义最大接收字节数 200
#define EN_USART1_RX  1    // 使能（1）/禁止（0）串口1接收

extern u8 USART_RX_BUF[USART_REC_LEN];    // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;                  // 接收状态标记
extern UART_HandleTypeDef UART1_Handler;  // UART句柄

#define RXBUFFERSIZE 1              // 缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];  // HAL库USART接收Buffer

// 如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

#endif  // _USART_H

/*
HAL_UART_Transmit();                                     // 串口发送数据，使用超时管理机制
HAL_UART_Receive();                                      // 串口接收数据，使用超时管理机制
HAL_UART_Transmit_IT();                                  // 串口中断模式发送
HAL_UART_Receive_IT();                                   // 串口中断模式接收
HAL_UART_Transmit_DMA();                                 // 串口DMA模式发送
HAL_UART_Transmit_DMA();                                 // 串口DMA模式接收
HAL_UART_IRQHandler(UART_HandleTypeDef* huart);          // 串口中断处理函数
HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart);      // 串口发送中断回调函数
HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef* huart);  // 串口发送一半中断回调函数（用的较少）
HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);      // 串口接收中断回调函数
HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef* huart);  // 串口接收一半回调函数（用的较少）
HAL_UART_ErrorCallback();                                // 串口接收错误函数
*/
