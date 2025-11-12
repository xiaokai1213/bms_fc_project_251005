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
#include "init.h"
#include "runing.h"

// 全局结构体
volatile bms_t bms;
bms_flag_t bms_flag;

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

   while (1) {
      if (bms_flag.collect_temperature_flag == 1) {
      }
   }
}

/*
伪状态机代码
typedef enum { STATE_A, STATE_B, STATE_C } State_t;

State_t current_state = STATE_A;

void state_machine_run(Event_t event) {
   switch (current_state) {
      case STATE_A:
         if (event == EVENT_X) {
            // 执行动作
            current_state = STATE_B;
         }
         break;
      case STATE_B:
         if (event == EVENT_Y) {
            current_state = STATE_C;
         }
         break;
      case STATE_C:
         // 处理状态C的事件
         break;
   }
}
*/

/**
 * @brief   状态机调度函数，在主函数中循环调用
 */
void bms_state_machine_dispatch(void) {
   switch (bms.stat) {    // 输入当前状态
      case state_init:    // 初始化状态
         init_execute();  // 初始化执行函数
         break;
      case state_runing:    // 运行状态
         runing_execute();  // 运行执行函数
         break;
      case state_fault:  // 故障状态

         break;
      default:  // 默认分支

         break;
   }
}

/**
 * @brief   状态机事件处理函数，状态机核心代码，根据当前事件决定状态转换
 * @param   event：要处理的事件
 */
void bms_sm_handle_event(bms_event_t event) {
   switch (event) {
      case event_power_on:  // 上电事件
         bms.stat = state_init;
         break;
      case event_enter_runing:  // 进入运行状态
         bms.stat = state_runing;
         break;
      case event_self_check:  // 自检事件分支

         break;
      default:  // 默认分支

         break;
   }
}
