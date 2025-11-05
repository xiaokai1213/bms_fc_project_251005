#include "runing.h"
#include "ltc6804_1_task.h"

#include "can_task.h"

/**
 * @brief   运行执行函数
 */
void runing_execute(void) {
      bms_sm_handle_event(event_enter_standby);  // 任务执行结束，进入待机状态
}
