#include "ltc6804_1_task.h"
#include "delay.h"
#include "main.h"
#include "task_manager.h"

LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 配置寄存器全局句柄
LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 状态寄存器全局句柄
BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 电池电压全局句柄
AuxVoltageRegs av_h_ltc6804[total_ic];               // 辅助电压全局句柄

/**
 * @brief   ltc6804初始化函数
 * @param   无
 * @return  无
 */
void LTC6804_init(void) {
   GPIO_InitTypeDef GPIO_InitStruct;              // 定义GPIO初始化结构体
   __HAL_RCC_GPIOA_CLK_ENABLE();                  // 6804片选脚时钟使能
   GPIO_InitStruct.Pin = GPIO_PIN_4;              // PA4(CS)
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    // 推挽输出
   GPIO_InitStruct.Pull = GPIO_PULLUP;            // 上拉电阻
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 引脚速度为高速（50MHz）
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        // 初始化GPIOA的上述引脚

   // 配置寄存器设置
   for (uint8_t i = 0; i < total_ic; i++) {
      cfgr_h_ltc6804[i].CFGR0.bits.GPIO1 = 1;   // GPIO1作为温度传感器输入,引脚下拉电路关断
      cfgr_h_ltc6804[i].CFGR0.bits.GPIO2 = 1;   // GPIO2作为温度传感器输入,引脚下拉电路关断
      cfgr_h_ltc6804[i].CFGR0.bits.GPIO3 = 1;   // GPIO3作为温度传感器输入,引脚下拉电路关断
      cfgr_h_ltc6804[i].CFGR0.bits.GPIO4 = 1;   // GPIO4作为温度传感器输入,引脚下拉电路关断
      cfgr_h_ltc6804[i].CFGR0.bits.GPIO5 = 1;   // GPIO5作为温度传感器输入,引脚下拉电路关断
      cfgr_h_ltc6804[i].CFGR0.bits.REFON = 1;   // 基准保持上电状态直到看门狗超时
      cfgr_h_ltc6804[i].CFGR0.bits.SWTRD = 1;   // SETEN引脚状态(只读),硬件设定为1
      cfgr_h_ltc6804[i].CFGR0.bits.ADCOPT = 0;  // 选择27,7,26KHz采样率

      cfgr_h_ltc6804[0].CFGR1.reg_val = 0x00;
      cfgr_h_ltc6804[0].CFGR2.reg_val = 0x00;
      cfgr_h_ltc6804[0].CFGR3.reg_val = 0x00;  // 暂时不使用欠压比较与过压比较门限,所以均设为0

      cfgr_h_ltc6804[i].CFGR4.bits.DCC1 = 0;  // 禁止电池1放电
      cfgr_h_ltc6804[i].CFGR4.bits.DCC2 = 0;  // 禁止电池2放电
      cfgr_h_ltc6804[i].CFGR4.bits.DCC3 = 0;  // 禁止电池3放电
      cfgr_h_ltc6804[i].CFGR4.bits.DCC4 = 0;  // 禁止电池4放电
      cfgr_h_ltc6804[i].CFGR4.bits.DCC5 = 0;  // 禁止电池5放电
      cfgr_h_ltc6804[i].CFGR4.bits.DCC6 = 0;  // 禁止电池6放电
      cfgr_h_ltc6804[i].CFGR4.bits.DCC7 = 0;  // 禁止电池7放电
      cfgr_h_ltc6804[i].CFGR4.bits.DCC8 = 0;  // 禁止电池8放电

      cfgr_h_ltc6804[i].CFGR5.bits.DCC9 = 0;   // 禁止电池9放电
      cfgr_h_ltc6804[i].CFGR5.bits.DCC10 = 0;  // 禁止电池10放电
      cfgr_h_ltc6804[i].CFGR5.bits.DCC11 = 0;  // 禁止电池11放电
      cfgr_h_ltc6804[i].CFGR5.bits.DCC12 = 0;  // 禁止电池12放电

      // 设定放电超时值为1分钟,即设定DCTO=2,0b0010
      cfgr_h_ltc6804[i].CFGR5.bits.DCTO0 = 0;
      cfgr_h_ltc6804[i].CFGR5.bits.DCTO1 = 1;
      cfgr_h_ltc6804[i].CFGR5.bits.DCTO2 = 0;
      cfgr_h_ltc6804[i].CFGR5.bits.DCTO3 = 0;
   }

   wakeup_sleep();                                  // 唤醒芯片
   write_LTC6804_Config(total_ic, cfgr_h_ltc6804);  // 写配置寄存器
   delay_ms(10);
   if (read_LTC6804_Config(total_ic, cfgr_h_ltc6804) == 1) {  // 读取配置寄存器，检查是否写入成功
      printf("LTC6804_MODULAR INIT NG!\n\r");
   } else {
      printf("LTC6804_MODULAR INIT OK!\n\r");
   }
}

/**
 * @brief   配置寄存器写入任务
 * @param   无
 * @return  无
 */
void wr_6804_reg(void) {
   write_LTC6804_Config(total_ic, cfgr_h_ltc6804);                 // 写配置寄存器
   delay_ms(3);                                                    // 等待3ms
   LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804_read[total_ic] = {0};  // 临时配置寄存器读取值句柄
   if (read_LTC6804_Config(total_ic, cfgr_h_ltc6804_read) == 1) {
      printf("LTC6804_MODULAR INIT NG!\n\r");
   } else {
      printf("LTC6804_MODULAR INIT OK!\n\r");
   }
}

/**
 * @brief   电池电压转换与读取任务
 * @param   无
 * @return  无
 */
void re_6804_cell_vot(void) {
   LTC6804_adcv(MD_NORMAL, DCP_DISABLED, CH_ALL);                   // 启动电池电压转换
   delay_ms(4);                                                     // 等待转换完成
   read_LTC6804_Battery_voltage_registers(total_ic, cv_h_ltc6804);  // 读取电池电压数据
}

/**
 * @brief   辅助电压转换与读取任务
 * @param   无
 * @return  无
 */
void re_6804_aux_vot(void) {
   LTC6804_adax(MD_NORMAL, CHG_ALL);                            // 启动辅助电压转换
   delay_ms(4);                                                 // 等待转换完成
   read_LTC6804_Aux_voltage_registers(total_ic, av_h_ltc6804);  // 读取辅助电压数据
}

/**
 * @brief   状态寄存器读取任务
 * @param   无
 * @return  无
 */
void re_6804_stat(void) {
   read_LTC6804_Status_registers(total_ic, stat_h_ltc6804);  // 读取状态寄存器数据
}
