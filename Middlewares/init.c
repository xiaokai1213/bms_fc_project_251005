#include "init.h"
#include "can.h"
#include "led_relay_gpio.h"
#include "ltc6804_1_task.h"
#include "spi.h"
#include "stm32f1xx_it.h"
#include "tim.h"
#include "usart.h"

void init() {
   HAL_Init();             // HAL库初始化
   Stm32_Clock_Init();     // 时钟初始化
   uart_init(115200);      // 串口1初始化，中断收发，支持打印函数
   LED_RELAY_GPIO_Init();  // LED与继电器初始化
   TIM2_Init();            // 定时器2初始化（延时定时器）
   TIM4_Init();            // 定时器4初始化（后台定时器）
   SPI1_Init();            // spi1初始化
   CAN_Init();             // can外设初始化
   NVIC_Init();            // 中断初始化；中断统一管理
   // ltc6804初始化
   LTC6804_init();
}
