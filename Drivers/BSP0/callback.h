#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "main.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);  // hal库定时器统一回调函数
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi);         // spi接收回调函数
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi);         // spi发送回调函数

#endif  // _CALLBACK_H
