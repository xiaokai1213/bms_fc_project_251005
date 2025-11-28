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

// 全局变量
volatile uint64_t sys_time = 0;                           // 系统时钟，单位：ms
BMS_FC_Status_t bmsfc_status = BMS_FC_STATE_BALANCE_OFF;  // BMS状态初始化为均衡关闭状态
BMS_FC_Flag_t bmsfc_flag = {0};                           // BMS状态标志位管理句柄初始化

/**
 * @brief 主函数入口
 * @return int
 */
int main() {
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

   while (1) {
      Basic_periodic_task_run();  // 基本周期任务运行函数
      bmsfc_state_machine_run();  // BMS状态机运行函数
      delay_ms(2);                // 延时2ms限制执行频率
   }
}

/**
 * @brief 基本周期任务运行函数
 */
void Basic_periodic_task_run(void) {
   // 推进 LTC6804 非阻塞任务并基于定时器标志触发采集事件
   if (task_collect_voltage.flag == 1) {
      if (LTC6804_task_run(LTC6804_EVENT_COLLECT_VOLTAGE) == LTC6804_RESP_OK) {  // 采集电压事件
         task_collect_voltage.flag = 0;                                          // 事件已完成，清标志
      }
   }

   if (task_collect_temperature.flag == 1) {
      if (LTC6804_task_run(LTC6804_EVENT_COLLECT_TEMPERATURE) == LTC6804_RESP_OK) {  // 采集温度事件
         task_collect_temperature.flag = 0;                                          // 事件已完成，清标志
      }
   }

   // 推进 CAN 非阻塞任务并基于定时器标志触发发送事件
   if (task_can_tx_voltage_send.flag == 1) {
      if (can_task_run(CAN_EVENT_TX_VOLTAGE_DATA) == CAN_RESP_OK) {  // 发送电压数据事件
         task_can_tx_voltage_send.flag = 0;                          // 事件已完成，清标志
      }
   }

   if (task_can_tx_temperature_send.flag == 1) {
      if (can_task_run(CAN_EVENT_TX_TEMPERATURE_DATA) == CAN_RESP_OK) {  // 发送温度数据事件
         task_can_tx_temperature_send.flag = 0;                          // 事件已完成，清标志
      }
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
