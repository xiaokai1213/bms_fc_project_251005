#include "task_manager.h"
#include "ltc6804_1_task.h"

/*
总流程：
配置ltc6804寄存器->读取电压寄存器->读取温度寄存器
->读取状态寄存器->发送can报文->更新数据->循环
can报文中断接收
*/

/**
 * @brief 任务循环
 */
void task_run(void) {
   while (1) {
      wr_6804_reg();        // 写入6804配置
      re_6804_cell_vot();   // 读取电池电压
      re_6804_aux_vot();    // 读取辅助电压
      re_6804_stat();       // 读取状态寄存器
      send_cell_vot_can();  // 发送电池电压can报文
      send_aux_vot_can();   // 发送辅助电压can报文
   }
}
