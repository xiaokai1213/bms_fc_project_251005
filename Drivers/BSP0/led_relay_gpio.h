#ifndef _LED_RELAY_GPIO_H
#define _LED_RELAY_GPIO_H

#include "main.h"

#define RELAY_Pin       GPIO_PIN_13
#define RELAY_GPIO_Port GPIOC
#define LEDR_Pin        GPIO_PIN_8
#define LEDR_GPIO_Port  GPIOB
#define LEDG_Pin        GPIO_PIN_9
#define LEDG_GPIO_Port  GPIOB

void LED_RELAY_GPIO_Init(void);  // LED与继电器初始化函数

// 继电器端口操作
#define RELAY(x)                                                                                                                       \
   do {                                                                                                                                \
      x ? HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET); \
   } while (0)

// LED端口操作
#define LEDR(x)                                                                                                                    \
   do {                                                                                                                            \
      x ? HAL_GPIO_WritePin(LEDR_GPIO_Port, LEDR_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(LEDR_GPIO_Port, LEDR_Pin, GPIO_PIN_RESET); \
   } while (0)

#define LEDG(x)                                                                                                                    \
   do {                                                                                                                            \
      x ? HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_RESET); \
   } while (0)

#endif  // _LED_RELAY_GPIO_H
