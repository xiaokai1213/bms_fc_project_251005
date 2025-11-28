#include "base_task.h"
#include "callback.h"
#include "can_task.h"
#include "ltc6804_1_task.h"

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

   // 基于定时器标志触发读取6804状态事件
   if (task_read_6804_status.flag == 1) {
      if (LTC6804_task_run(LTC6804_EVENT_READ_STATUS) == LTC6804_RESP_OK) {  // 读取状态寄存器事件
         task_read_6804_status.flag = 0;                                     // 事件已完成，清标志
      }
   }
}
