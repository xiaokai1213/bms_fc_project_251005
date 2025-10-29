#include "standby.h"
#include "callback.h"

#include "can_task.h"

#include "can.h"

volatile uint8_t cat = 0;

/**
 * @brief   待机执行函数
 */
void standby_execute(void) {
   if (task_voltage_collect.flag == 1) {           // 检查电压采集周期标志位
      bms_sm_handle_event(event_voltage_collect);  // 电压采集事件
      task_voltage_collect.flag = 0;               // 标志位置0
      // return;                                      // 结束执行函数
   }

   if (task_temp_collect.flag == 1) {
      bms_sm_handle_event(event_temp_collect);  // 温度采集事件
      task_temp_collect.flag = 0;               // 标志位置0
      // return;                                   // 结束执行函数
   }

   printf("dd\n");

   if (cat == 1) {
      printf("main2222");
      cat = 0;
   }

   int n = 0;
   n = (int)HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0);
   printf("%d--", n);

   n = (int)HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO1);
   printf("%d\n", n);

   bms_sm_handle_event(event_enter_standby);  // 无事件触发重新进入待机
}
