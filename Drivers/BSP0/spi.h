#ifndef _SPI_H
#define _SPI_H

#include "main.h"

extern SPI_HandleTypeDef hspi1;      // spi1初始化句柄
extern SPI_HandleTypeDef hspi2;      // spi2初始化句柄
extern volatile uint8_t spi1_tx_it_end_sign;  // spi1中断发送结束全局变量标志位
extern volatile uint8_t spi1_rx_it_end_sign;  // spi1中断接收结束全局变量标志位

void SPI1_Init(void);                                 // spi1初始化函数
void SPI2_Init(void);                                 // spi2初始化函数
void spi1_tx_it(uint16_t txsize, uint8_t txdata[]);   // SPI1 以(中断)方式写数据
void spi1_rx_it(uint16_t rx_size, uint8_t* rx_data);  // spi1以中断方式接收数据

#endif  // _SPI_H

/*
序号	函数名	描述
1	HAL_SPI_Init()	初始化SPI外设。
2	HAL_SPI_DeInit()	反初始化SPI外设。
3	HAL_SPI_MspInit()	初始化SPI外设相关的MCU外设引脚或资源。
4	HAL_SPI_MspDeInit()	反初始化SPI外设相关的MCU外设引脚或资源。


5	HAL_SPI_Transmit()	发送数据到SPI外设。
6	HAL_SPI_Receive()	从SPI外设接收数据。
7	HAL_SPI_TransmitReceive()	发送并接收数据到SPI外设。
8	HAL_SPI_Transmit_IT()	使用中断方式发送数据到SPI外设。
9	HAL_SPI_Receive_IT()	使用中断方式从SPI外设接收数据。
10	HAL_SPI_TransmitReceive_IT()	使用中断方式发送并接收数据到SPI外设。


11	HAL_SPI_Transmit_DMA()	使用DMA方式发送数据到SPI外设。
12	HAL_SPI_Receive_DMA()	使用DMA方式从SPI外设接收数据。
13	HAL_SPI_TransmitReceive_DMA()	使用DMA方式发送并接收数据到SPI外设。
14	HAL_SPI_Abort()	终止SPI外设操作。
15	HAL_SPI_Abort_IT()	终止SPI外设操作并启用中断。
16	HAL_SPI_DMAPause()	暂停SPI外设DMA传输。
17	HAL_SPI_DMAResume()	恢复SPI外设DMA传输。
18	HAL_SPI_DMAStop()	停止SPI外设DMA传输。

19	HAL_SPI_IRQHandler()	SPI外设中断处理程序。

20	HAL_SPI_TxCpltCallback()	发送完成回调函数。
21	HAL_SPI_RxCpltCallback()	接收完成回调函数。
22	HAL_SPI_TxRxCpltCallback()	发送接收完成回调函数。

23	HAL_SPI_TxHalfCpltCallback()	发送一半完成回调函数。
24	HAL_SPI_RxHalfCpltCallback()	接收一半完成回调函数。
25	HAL_SPI_TxRxHalfCpltCallback()	发送接收一半完成回调函数。

26	HAL_SPI_ErrorCallback()	SPI外设错误回调函数。
27	HAL_SPI_AbortCpltCallback()	终止完成回调函数。
28	HAL_SPI_GetState()	获取SPI外设状态。
29	HAL_SPI_GetError()	获取SPI外设错误状态。

*/
