#include "standby.h"
#include "callback.h"

#include "can.h"

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
   uint32_t CAN_NOX = HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0);
   printf("can_xx=%d\n", (int)CAN_NOX);
   CAN_RxHeaderTypeDef rx_header;
   uint8_t rx_data[8];

   if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data) == HAL_OK) {
      if ((rx_header.IDE == CAN_ID_EXT) && (rx_header.ExtId == 0x12345678)) {
         printf("main2222");
      }
   }

   bms_sm_handle_event(event_enter_standby);  // 无事件触发重新进入待机
}
