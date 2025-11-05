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
#include "standby.h"

// 全局结构体
bms_t bms;

// 主函数
int main() {
   HAL_Init();                           // HAL库初始化
   Stm32_Clock_Init();                   // 时钟初始化
   bms_sm_handle_event(event_power_on);  // 上电事件
   while (1) {
      bms_state_machine_dispatch();  // 执行状态机调度函数
      delay_ms(500);                 // 延时2ms限制状态机执行频率
   }
}

/**
 * @brief   状态机调度函数，在主函数中循环调用
 */
void bms_state_machine_dispatch(void) {
   switch (bms.stat) {    // 输入当前状态
      case state_init:    // 初始化状态
         init_execute();  // 初始化执行函数
         break;

      case state_standby:    // 待机状态
         standby_execute();  // 待机执行函数
         break;

      case state_runing:    // 运行状态
         runing_execute();  // 运行执行函数
         break;

      case state_fault:  // 故障状态
         break;

      case state_idle:  // 空闲状态
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
      case event_power_on:  // 上电事件
         //
         break;

      case event_enter_standby:  // 进入待机事件
         //
         break;

      case event_self_test:  // 自检事件分支
         break;

      case event_voltage_collect:  // 电压采集事件分支
         //
         break;

      case event_temp_collect:  // 温度采集事件分支
                                //
         break;

      case event_voltage_data_send:  // 电压数据发送事件分支
         break;

      case event_temp_data_send:  // 温度数据发送事件分支
         break;

      case event_data_send_complete:  // 数据发送完成事件分支
         break;

      default:  // 默认分支
         break;
   }
}
