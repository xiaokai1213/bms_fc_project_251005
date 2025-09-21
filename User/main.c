// 主头文件
#include "main.h"
// SYSTEM头文件
#include "delay.h"
#include "sys.h"
#include "usart.h"
// BSP0头文件
#include "can.h"
#include "led_relay_gpio.h"
#include "spi.h"
#include "stm32f1xx_it.h"
#include "tim.h"
// BSP1头文件
#include "can_task.h"
#include "ltc6804_1_task.h"
// Middlewares头文件

int main(void) {
   // 外设初始化
   HAL_Init();             // HAL库初始化
   Stm32_Clock_Init();     // 时钟初始化
   uart_init(115200);      // 串口1初始化，中断收发，支持打印函数
   LED_RELAY_GPIO_Init();  // LED与继电器初始化
   TIM2_Init();            // 定时器2初始化（延时定时器）
   TIM4_Init();            // 定时器4初始化（后台定时器）
   SPI1_Init();            // spi1初始化
   CAN_Init();             // can外设初始化
   NVIC_Init();            // 中断初始化；中断统一管理

   // 外设驱动初始化
   LTC6804_init();

   while (1) {
      ltc6804_cv();
      uint8_t cv[8];
      cv[0] = (uint8_t)(cv_h_ltc6804[0].C01V >> 8);
      cv[1] = (uint8_t)cv_h_ltc6804[0].C01V;
      cv[2] = (uint8_t)(cv_h_ltc6804[0].C02V >> 8);
      cv[3] = (uint8_t)cv_h_ltc6804[0].C02V;
      cv[4] = (uint8_t)(cv_h_ltc6804[0].C03V >> 8);
      cv[5] = (uint8_t)cv_h_ltc6804[0].C03V;
      cv[6] = (uint8_t)(cv_h_ltc6804[0].C04V >> 8);
      cv[7] = (uint8_t)cv_h_ltc6804[0].C04V;
      printf("\r\n");
      RELAY(0);
      can_tx_extid_8(0x123, cv);

      delay_ms(4000);
      RELAY(1);
      delay_ms(4000);
   }
}
