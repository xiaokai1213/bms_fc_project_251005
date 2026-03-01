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

   cfgr_h_ltc6804[0].CFGR0.reg_val = 0xFE;
   cfgr_h_ltc6804[0].CFGR1.reg_val = 0x00;
   cfgr_h_ltc6804[0].CFGR2.reg_val = 0x00;
   cfgr_h_ltc6804[0].CFGR3.reg_val = 0x00;
   cfgr_h_ltc6804[0].CFGR4.reg_val = 0x00;
   cfgr_h_ltc6804[0].CFGR5.reg_val = 0x00;

   cfgr_h_ltc6804[1].CFGR0.reg_val = 0xFE;
   cfgr_h_ltc6804[1].CFGR1.reg_val = 0x00;
   cfgr_h_ltc6804[1].CFGR2.reg_val = 0x00;
   cfgr_h_ltc6804[1].CFGR3.reg_val = 0x00;
   cfgr_h_ltc6804[1].CFGR4.reg_val = 0x00;
   cfgr_h_ltc6804[1].CFGR5.reg_val = 0x00;

   wakeup_sleep();                                  // 唤醒芯片
   write_LTC6804_Config(total_ic, cfgr_h_ltc6804);  // 写配置寄存器
   delay_ms(3);                                     // 等待3ms
   if (read_LTC6804_Config(total_ic, cfgr_h_ltc6804) == 1) {
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

void re_6804_cell_vot(void) {}
