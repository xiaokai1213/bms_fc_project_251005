#include "standby.h"
#include "callback.h"

/**
 * @brief   待机执行函数
 */
void standby_execute(void) {
   if (task_500ms.flag == 1) {                     // 检查500ms任务周期标志位
      bms_sm_handle_event(event_voltage_collect);  // 电压采集事件
      task_500ms.flag = 0;                         // 标志位置0
      // return;                                      // 结束执行函数
   }

   if (task_100ms.flag == 1) {                  // 检查100ms任务周期标志位
      bms_sm_handle_event(event_temp_collect);  // 事件
      task_100ms.flag = 0;                      // 标志位置0
      // return;                                   // 结束执行函数
   }

   bms_sm_handle_event(event_enter_standby);  // 无事件触发重新进入待机
}
