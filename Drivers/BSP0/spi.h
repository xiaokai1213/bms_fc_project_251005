#ifndef _SPI_H
#define _SPI_H

#include "main.h"

extern SPI_HandleTypeDef hspi1;  // spi1初始化句柄
extern SPI_HandleTypeDef hspi2;  // spi2初始化句柄

void SPI1_Init(void);  // spi1初始化函数
void SPI2_Init(void);  // spi2初始化函数

uint8_t spi1_read_write_byte(uint8_t txdata);  // SPI1读写一个字节

#endif  // _SPI_H
