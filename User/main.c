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

// 全局状态机实例
bms_state_machine_t bms_sm;

// 主函数
int main() {
   HAL_Init();          // HAL库初始化
   Stm32_Clock_Init();  // 时钟初始化

   bms_sm.current_state = state_init;      // 初始化状态
   bms_sm.new_state = state_init;          // 初始化状态
   bms_sm.current_event = event_power_on;  // 上电事件
   bms_sm.new_event = event_power_on;      // 上电事件
   while (1) {
      bms_sm_dispatch();  // 执行状态机调度函数
      delay_ms(2);        // 延时2ms限制状态机执行频率
   }
}
/**
 * @brief   状态机调度函数
 */
void bms_sm_dispatch() {
   switch (bms_sm.current_state) {  // 输入当前状态
      case state_init:              // 初始化状态
         init_execute();            // 初始化执行函数
         break;
      case state_self_test:  // 自检状态
         break;
      case state_runing:  // 运行状态
         break;
      case state_fault:  // 故障状态
         break;
      default:
         break;
   }
}

/**
 * @brief   状态机事件处理函数，状态机核心代码，根据当前事件决定状态转换
 * @param   event：要处理的事件
 */
void bms_sm_handle_event(bms_event_t event) {
   switch (event) {
      case event_power_on:
         init_execute();  // 初始化执行函数
         break;

      default:
         break;
   }
}
