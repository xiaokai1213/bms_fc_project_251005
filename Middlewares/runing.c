#include "runing.h"
#include "ltc6804_1_task.h"

/**
 * @brief   运行执行函数
 */
void runing_execute(void) {
   if (event_trigger == event_voltage_collect) {
      ltc6804_Get_Voltage();  // 获取电池组电压，并存储到电池电压全局句柄
   }

   if (event_trigger == event_temp_collect) {
      ltc6804_Get_temperature();  // 获取电池组极耳温度，并存储到辅助电压全局句柄
   }

   bms_sm_handle_event(event_enter_standby);  // 任务执行结束，进入待机状态
}
