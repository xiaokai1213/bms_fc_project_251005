#include "runing.h"
#include "callback.h"
#include "ltc6804_1_task.h"

/**
 * @brief   运行执行函数
 */
void runing_execute(void) {
   if (task_100ms.flag == 1) {    // 执行100ms周期性任务
      ltc6804_Get_Voltage();      // 获取电压
      ltc6804_Get_temperature();  // 获取温度
      task_100ms.flag = 0;
   }
}
