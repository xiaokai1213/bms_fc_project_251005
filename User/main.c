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
// Middlewares头文件
#include "base_task.h"

// 全局变量
volatile uint64_t sys_time = 0;                           // 系统时钟，单位：ms
volatile BMS_FC_Flag_t bmsfc_flag = {0};                  // BMS状态标志位管理句柄初始化
BMS_FC_Status_t bmsfc_status = BMS_FC_STATE_BALANCE_OFF;  // BMS状态初始化为均衡关闭状态

/**
 * @brief 主函数入口
 * @return int
 */
int main() {
   sys_init();  // 系统初始化函数

   while (1) {
      Basic_periodic_task_run();  // 基本周期任务运行函数
      bmsfc_state_machine_run();  // BMS状态机运行函数
      delay_ms(2);                // 延时2ms限制执行频率
   }
}

/**
 * @brief BMS状态机运行函数
 */
void bmsfc_state_machine_run(void) {
   switch (bmsfc_status) {
      case BMS_FC_STATE_BALANCE_OFF:  // 均衡关闭状态

         break;

      case BMS_FC_STATE_BALANCE_ON:  // 均衡开启状态

         break;

      case BMS_FC_STATE_FAULT:  // 故障状态

         break;

      default:
         bmsfc_status = BMS_FC_STATE_BALANCE_OFF;  // 默认回到均衡关闭状态
         break;
   }
}

/**
 * @brief 系统初始化函数
 */
void sys_init(void) {
   HAL_Init();          // HAL库初始化
   Stm32_Clock_Init();  // 时钟初始化
   uart_init(115200);   // 串口1初始化，中断收发，支持打印函数

   LED_RELAY_GPIO_Init();  // LED与继电器初始化
   TIM2_Init();            // 定时器2初始化（延时定时器）
   TIM4_Init();            // 定时器4初始化（后台定时器）
   SPI1_Init();            // spi1初始化
   CAN_Init();             // can外设初始化
   NVIC_Init();            // 中断初始化；中断统一管理

   LTC6804_init();  // ltc6804初始化
}
