// 主头文件
#include "main.h"
// SYSTEM头文件
#include "delay.h"
#include "sys.h"
#include "usart.h"
// BSP0头文件
#include "callback.h"
#include "can.h"
#include "led_relay_gpio.h"
#include "spi.h"
#include "stm32f1xx_it.h"
#include "tim.h"
// BSP1头文件
#include "can_task.h"
#include "ltc6804_1_task.h"
#include "ltc_fsm.h"
// Middlewares头文件
#include "task_manager.h"

volatile uint64_t sys_time = 0;  // 系统时间变量，单位：毫秒

// 主函数
int main() {
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
   // 初始化任务管理器
   TM_Init();
   // 初始化并注册 LTC6804 的 FSM（内部会周期性触发采样）
   ltc_fsm_init();
   while (1) {
      TM_Run();
   }
}
