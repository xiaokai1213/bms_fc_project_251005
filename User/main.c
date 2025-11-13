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

// 全局标志位
volatile flag_handle_t flag = {0};  // 运行状态标志位管理,初始化均为0

// 主函数
int main() {
   init();  // 初始化
   while (1) {
      // 检查是否有发送任务
      // 检查是否有周期性任务

      Task_Update();  // 任务更新

      // 电压采集任务
      if (flag.collect_voltage_flag == 1) {
         uint8_t pec_error = 0;                                                             // 校验返回值
         for (uint8_t pec_error_counter = 0; pec_error_counter < 5; pec_error_counter++) {  // 循环5次获取电压并返回校验情况
            pec_error = ltc6804_Get_Voltage();                                              // 获取电压并返回校验情况
            if (pec_error == 0) {                                                           // 检查校验值
               break;                                                                       // 若返回正确则跳出循环
            }
         }
         if (pec_error == 1) {                // 5次获取电压均失败
            flag.pec_fault = 1;               // pec故障标志位置1
            flag.collect_voltage_flag = 1;    // 采集电压标志依旧为1
         } else {                             // 电压获取成功,pec_error=0
            flag.send_voltage_data_flag = 1;  // 发送电压数据位置1
            flag.collect_voltage_flag = 0;    // 采集电压标志位置0
         }
      }

      // 电压数据发送任务
      if (flag.send_voltage_data_flag == 1) {
         // 获取can发送邮箱情况
         uint32_t can_tx_mailbox_num = 0;
         can_tx_mailbox_num = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // 获取空闲发送邮箱数量
         if (can_tx_mailbox_num == 0) {                                 // 空邮箱数量为0
            flag.send_voltage_data_flag == 1;                           // 无法发送，下一轮继续发送
         } else {
         }
         flag.send_voltage_data_flag = 0;
      }
   }
}

/**
 * @brief   任务更新
 */
void Task_Update(void) {
   // 500ms任务更新
   if (task_500ms.flag == 1) {
      flag.collect_voltage_flag = 1;      // 置采集电压检测位为1
      flag.collect_temperature_flag = 1;  // 置采集温度检测位为1
      task_500ms.flag = 0;
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
