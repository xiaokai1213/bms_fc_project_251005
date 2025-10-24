#include "standby.h"
#include "callback.h"

/**
 * @brief   待机执行函数
 */
void standby_execute(void) {
   if (task_voltage_collect.flag == 1) {           // 检查电压采集周期标志位
      bms_sm_handle_event(event_voltage_collect);  // 电压采集事件
      task_voltage_collect.flag = 0;               // 标志位置0
      return;                                      // 结束执行函数
   }

   if (task_temp_collect.flag == 1) {
      bms_sm_handle_event(event_temp_collect);
      task_temp_collect.flag = 0;
      return;
   }

   bms_sm_handle_event(event_enter_standby);  // 无事件触发重新进入待机
}
