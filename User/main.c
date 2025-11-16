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
#include "data_processing.h"

// 全局变量
BMS_State_t state = STATE_INIT;  // 初始化状态
volatile FLAG_t flag = {0};      // 全局标志位

// 主函数
int main() {
   HAL_Init();          // HAL库初始化
   Stm32_Clock_Init();  // 时钟初始化
   uart_init(115200);   // 串口1初始化，中断收发，支持打印函数

   while (1) {
      state_machine_run();
      delay_ms(2);  // 延时2ms限制状态机执行频率
   }
}

// 状态机调度
void state_machine_run(void) {
   switch (state) {
      case STATE_INIT:                // 外设初始化
         LED_RELAY_GPIO_Init();       // LED与继电器初始化
         TIM2_Init();                 // 定时器2初始化（延时定时器）
         TIM4_Init();                 // 定时器4初始化（后台定时器）
         SPI1_Init();                 // spi1初始化
         CAN_Init();                  // can外设初始化
         NVIC_Init();                 // 中断初始化；中断统一管理
         state = STATE_INIT_LTC6804;  // 状态转换：外设初始化->ltc6804初始化
         break;
      case STATE_INIT_LTC6804:  // ltc6804初始化
         LTC6804_init();        // ltc6804初始化
         state = STATE_IDLE;    // 状态转换：ltc6804初始化->空闲状态
         break;
      case STATE_IDLE:                                           // 空闲状态
         if (flag.can_send_busy == 0) {                          // 检查can发送邮箱状态
            if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0) {  // 获取can发送邮箱空闲数量
               flag.can_send_busy = 1;                           // 无空邮箱,标志位置1
               break;                                            // 退出空闲状态
            }
            if (flag.voltage_can_tx_PF_10_ready_flag == 1) {  // 检查电压发送报文PF10状态
               state = STATE_SEND_VOLTAGE;                    // 状态转换:空闲->发送电压
               break;                                         // 退出空闲状态
            }
            if (flag.voltage_can_tx_PF_11_ready_flag == 1) {  // 检查电压发送报文PF11状态
               state = STATE_SEND_VOLTAGE;                    // 状态转换:空闲->发送电压
               break;                                         // 退出空闲状态
            }
            if (flag.voltage_can_tx_PF_12_ready_flag == 1) {  // 检查电压发送报文PF12状态
               state = STATE_SEND_VOLTAGE;                    // 状态转换:空闲->发送电压
               break;                                         // 退出空闲状态
            }
            if (flag.voltage_can_tx_PF_13_ready_flag == 1) {  // 检查电压发送报文PF13状态
               state = STATE_SEND_VOLTAGE;                    // 状态转换:空闲->发送电压
               break;                                         // 退出空闲状态
            }
            if (flag.voltage_can_tx_PF_14_ready_flag == 1) {  // 检查电压发送报文PF14状态
               state = STATE_SEND_VOLTAGE;                    // 状态转换:空闲->发送电压
               break;                                         // 退出空闲状态
            }
         }
         if (task_can_tx_voltage_send.flag == 1) {
            state = STATE_SEND_VOLTAGE;         // 状态转换：空闲->电压发送状态
            task_can_tx_voltage_send.flag = 0;  // 发送电压周期标志位置0
            break;                              // 退出空闲状态
         }
         if (task_collect_voltage.flag == 1) {
            state = STATE_COLLECT_VOLTAGE;  // 状态转换：空闲->电压采集状态
            task_collect_voltage.flag = 0;  // 采集电压周期标志位置0
            break;                          // 退出空闲状态
         }
         if (task_collect_temperature.flag == 1) {
            state = STATE_COLLECT_TEMPERATURE;  // 状态转换：空闲->温度采集状态
            task_collect_temperature.flag = 0;  // 采集温度周期标志位置0
            break;                              // 退出空闲状态
         }
         break;
      case STATE_COLLECT_VOLTAGE:  // 采集电压状态
         if (ltc6804_Get_Voltage() == 0) {
            state = STATE_PROCESS_VOLTAGE_DATA;  // 状态转换：电压采集状态->电压数据处理状态
         } else {
            state = STATE_FAULT_COMMUNICATION_SPI;  // SPI通讯故障,状态转换:电压采集->SPI通讯故障
         }
         break;
      case STATE_COLLECT_TEMPERATURE:  // 采集温度状态
         break;
      case STATE_PROCESS_VOLTAGE_DATA:  // 处理电压数据
         if (process_voltage_data() == 0) {
            flag.voltage_can_tx_PF_10_ready_flag = 1;
            flag.voltage_can_tx_PF_11_ready_flag = 1;
            flag.voltage_can_tx_PF_12_ready_flag = 1;
            flag.voltage_can_tx_PF_13_ready_flag = 1;
            flag.voltage_can_tx_PF_14_ready_flag = 1;  // 电压发送报文挂起
            state = STATE_IDLE;                        // 状态转换:电压数据处理->空闲状态
         } else {
            state = STATE_FAULT_VOLTAGE_ANOMALY;  // 电压异常,状态转换:电压数据处理->电压异常故障状态
         }
         break;
      case STATE_PROCESS_TEMPERATURE_DATA:  // 处理温度数据
         break;
      case STATE_SEND_VOLTAGE:  // 发送电压数据

         break;
      case STATE_SEND_TEMPERATURE:  // 发送温度数据
         break;

      default:
         break;
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
