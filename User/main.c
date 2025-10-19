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

// 全局状态枚举
bms_state_t bms_current_stat;  // bms当前主状态枚举定义

int main(void) {
   main_stat_init();  // 状态机初始化

   switch (bms_current_stat) {
      case state_init:                // 初始化状态
         uint8_t x = init_execute();  // 初始化执行函数

         break;
      case state_idle:  // 空闲状态
         break;
      case state_fault:  // 故障状态
         break;
      case state_self_test:  // 自检状态
         break;
      case state_receive_cmd:  // 接收主控命令
         break;
      case state_working:  // 工作状态
         break;

      default:
         break;
   }
}

void main_stat_init() {
   bms_current_stat = state_init;  // 上电后进入初始化状态
}

void flag_task_time() {
   if (task_5ms.flag == 1) {  // 5ms周期性任务
      // 清除标志位
      task_5ms.flag = 0;
   }

   if (task_10ms.flag == 1) {  // 10ms周期性任务
      // 清除标志位
      task_10ms.flag = 0;
   }

   if (task_100ms.flag == 1) {    // 100ms周期性任务
      ltc6804_Get_Voltage();      // 获取电池电压
      ltc6804_Get_temperature();  // 获取电池温度
      // 清除标志位
      task_100ms.flag = 0;
   }
}

// 用于暂时存储周期性代码
void stand_xy(void) {
   ltc6804_cv();
   uint8_t cv[8];
   cv[0] = (uint8_t)(cv_h_ltc6804[0].C01V >> 8);
   cv[1] = (uint8_t)cv_h_ltc6804[0].C01V;
   cv[2] = (uint8_t)(cv_h_ltc6804[0].C02V >> 8);
   cv[3] = (uint8_t)cv_h_ltc6804[0].C02V;
   cv[4] = (uint8_t)(cv_h_ltc6804[0].C03V >> 8);
   cv[5] = (uint8_t)cv_h_ltc6804[0].C03V;
   cv[6] = (uint8_t)(cv_h_ltc6804[0].C04V >> 8);
   cv[7] = (uint8_t)cv_h_ltc6804[0].C04V;
   printf("\r\n");
   RELAY(0);
   can_tx_extid_8(0x123, cv);

   delay_ms(4000);
   RELAY(1);
   delay_ms(4000);
}
