#include "runing.h"
#include "callback.h"
#include "can.h"
#include "ltc6804_1_task.h"

runing_management_flag_handle_t runing_flag = {0};  // 运行状态标志位管理,初始化均为0

/**
 * @brief   运行执行函数
 */
void runing_execute(void) {
   // 检查是否有发送任务
   // 检查是否有周期性任务

   Task_Update();  // 任务更新

   // 电压采集任务
   if (runing_flag.collect_voltage_flag == 1) {
      uint8_t pec_error = 0;                                                             // 校验返回值
      for (uint8_t pec_error_counter = 0; pec_error_counter < 5; pec_error_counter++) {  // 循环5次获取电压并返回校验情况
         pec_error = ltc6804_Get_Voltage();                                              // 获取电压并返回校验情况
         if (pec_error == 0) {                                                           // 检查校验值
            break;                                                                       // 若返回正确则跳出循环
         }
      }
      if (pec_error == 1) {                       // 5次获取电压均失败
         runing_flag.pec_fault = 1;               // pec故障标志位置1
         runing_flag.collect_voltage_flag = 1;    // 采集电压标志依旧为1
      } else {                                    // 电压获取成功,pec_error=0
         runing_flag.send_voltage_data_flag = 1;  // 发送电压数据位置1
         runing_flag.collect_voltage_flag = 0;    // 采集电压标志位置0
      }
   }

   // 电压数据发送任务
   if (runing_flag.send_voltage_data_flag == 1) {
      // 获取can发送邮箱情况
      uint32_t can_tx_mailbox_num = 0;
      can_tx_mailbox_num = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // 获取空闲发送邮箱数量
      if (can_tx_mailbox_num == CAN_TX_MAILBOX0) {
      }
      runing_flag.send_voltage_data_flag = 0;
   }
}

/**
 * @brief   任务更新
 */
void Task_Update() {
   // 500ms任务更新
   if (task_500ms.flag == 1) {
      runing_flag.collect_voltage_flag = 1;      // 置采集电压检测位为1
      runing_flag.collect_temperature_flag = 1;  // 置采集温度检测位为1
      task_500ms.flag = 0;
   }
}
