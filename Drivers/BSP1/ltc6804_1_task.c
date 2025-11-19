#include "ltc6804_1_task.h"
#include "delay.h"
#include "main.h"
#include "task_manager.h"

LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 配置寄存器全局句柄
LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 状态寄存器全局句柄
BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 电池电压全局句柄
AuxVoltageRegs av_h_ltc6804[total_ic];               // 辅助电压全局句柄
// 采集就绪标志
volatile uint8_t ltc_cv_updated = 0;
volatile uint8_t ltc_av_updated = 0;

/**
 * @brief   ltc6804初始化函数
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
   delay_ms(10);
   if (read_LTC6804_Config(total_ic, cfgr_h_ltc6804) == 1) {
      printf("LTC6804_MODULAR INIT NG!\n\r");
   } else {
      printf("LTC6804_MODULAR INIT OK!\n\r");
   }
}

/**
 * @brief 周期任务：触发 ADC 转换（非阻塞），并注册一次性延迟读取任务
 * @param arg 未使用
 */
void ltc_sample_task(void* arg) {
   /* 触发电压 ADC 转换（不读取），尽量短时间内完成 */
   LTC6804_adcv(MD_NORMAL, DCP_DISABLED, CH_ALL);

   /* 注册一次性读取任务：延迟 10ms 后读取转换结果并打印（一次性任务） */
   extern int TM_Register(TaskFunc func, void* arg, uint32_t period_ms, uint8_t repeat, uint32_t delay_ms);
   TM_Register(ltc_sample_read_task, NULL, 0, 0, 10);
}

/**
 * @brief 一次性读取任务：读取电压寄存器并打印第一块第一个电芯的电压
 * @param arg 未使用
 */
void ltc_sample_read_task(void* arg) {
   uint8_t pec = read_LTC6804_Battery_voltage_registers(total_ic, cv_h_ltc6804);
   if (pec != 0) {
      printf("ltc6804 read voltage PEC error=%d\r\n", pec);
      return;
   }
   /* 标记电压数据已更新，供其它模块读取或上报 */
   ltc_cv_updated = 1;
}

/**
 * @brief 周期任务：触发温度（辅助）ADC转换并注册延迟读取任务
 * @param arg 未使用
 */
void ltc_temp_sample_task(void* arg) {
   /* 触发辅助 ADC 转换（GPIO/温度） */
   LTC6804_adax(MD_NORMAL, CHG_ALL);

   /* 注册一次性读取任务：延迟 10ms 后读取转换结果并打印 */
   extern int TM_Register(TaskFunc func, void* arg, uint32_t period_ms, uint8_t repeat, uint32_t delay_ms);
   TM_Register(ltc_temp_read_task, NULL, 0, 0, 10);
}

/**
 * @brief 一次性读取任务：读取辅助寄存器并打印部分温度/辅助电压
 * @param arg 未使用
 */
void ltc_temp_read_task(void* arg) {
   uint8_t pec = read_LTC6804_Auxiliary_voltage_registers(total_ic, av_h_ltc6804);
   if (pec != 0) {
      printf("ltc6804 read aux PEC error=%d\r\n", pec);
      return;
   }
   /* 标记辅助(温度)数据已更新，供其它模块读取或上报 */
   ltc_av_updated = 1;
}

/**
 * @brief 原子读取并清除电压更新标志
 * @return 原来的标志值（1 = 有新数据）
 */
uint8_t ltc_get_and_clear_cv_updated(void) {
   uint8_t v;
   __disable_irq();
   v = ltc_cv_updated;
   ltc_cv_updated = 0;
   __enable_irq();
   return v;
}

/**
 * @brief 原子读取并清除辅助/温度更新标志
 * @return 原来的标志值（1 = 有新数据）
 */
uint8_t ltc_get_and_clear_av_updated(void) {
   uint8_t v;
   __disable_irq();
   v = ltc_av_updated;
   ltc_av_updated = 0;
   __enable_irq();
   return v;
}
