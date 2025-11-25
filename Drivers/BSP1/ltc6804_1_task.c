#include "ltc6804_1_task.h"
#include "delay.h"

LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 配置寄存器全局句柄
LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 状态寄存器全局句柄
BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 电池电压全局句柄
AuxVoltageRegs av_h_ltc6804[total_ic];               // 辅助电压全局句柄

volatile ltc6804_event_t LTC6804_EVENT = LTC6804_EVENT_INIT;  // ltc6804状态机当前事件变量
volatile ltc6804_state_t LTC6804_STATE = LTC6804_STATE_INIT;  // ltc6804状态机当前状态变量

/**
 * @brief   ltc6804事件状态机
 * @param   event: 传入事件
 * @return  ltc6804_state_t: 返回当前状态
 */
ltc6804_state_t ltc6804_state_machine(ltc6804_event_t event) {
   // switch每个状态
   switch (LTC6804_STATE) {
      case LTC6804_STATE_INIT:                              // 初始化状态
         if (event == LTC6804_EVENT_INIT) {                 // 初始化事件
            LTC6804_init();                                 // 初始化
            LTC6804_STATE = LTC6804_STATE_CONVERT_VOLTAGE;  // 转到启动电压ADC转换状态
         }
         break;

      case LTC6804_STATE_IDLE:  // 空闲状态
         if (event == LTC6804_EVENT_IDLE) {
            // 空闲状态处理逻辑（如果有的话）
         }
         break;

      case LTC6804_STATE_CONVERT_VOLTAGE:  // 启动电压ADC转换状态
         if (event == LTC6804_EVENT_CONVERT_VOLTAGE) {
            LTC6804_STATE = LTC6804_STATE_WAIT_VOLTAGE;  // 转到等待电压转换完成状态
         }
         break;

      case LTC6804_STATE_WAIT_VOLTAGE:  // 等待电压转换完成状态
         if (event == LTC6804_EVENT_READ_VOLTAGE) {
            LTC6804_STATE = LTC6804_STATE_READ_VOLTAGE;  // 转到读取电池电压状态
         }
         break;

      case LTC6804_STATE_READ_VOLTAGE:  // 读取电池电压状态
         if (event == LTC6804_EVENT_READ_VOLTAGE) {
            LTC6804_STATE = LTC6804_STATE_IDLE;  // 转到空闲状态
         }
         break;

      case LTC6804_STATE_CONVERT_TEMPERATURE:  // 启动温度ADC转换状态
         if (event == LTC6804_EVENT_CONVERT_TEMPERATURE) {
            LTC6804_STATE = LTC6804_STATE_WAIT_TEMPERATURE;  // 转到等待温度转换完成状态
         }
         break;

      case LTC6804_STATE_WAIT_TEMPERATURE:  // 等待温度转换完成状态
         if (event == LTC6804_EVENT_READ_TEMPERATURE) {
            LTC6804_STATE = LTC6804_STATE_READ_TEMPERATURE;  // 转到读取温度状态
         }
         break;

      case LTC6804_STATE_READ_TEMPERATURE:  // 读取温度状态
         if (event == LTC6804_EVENT_READ_TEMPERATURE) {
            LTC6804_STATE = LTC6804_STATE_IDLE;  // 转到空闲状态
         }
         break;

      case LTC6804_STATE_ERROR:
         if (event == LTC6804_EVENT_ERROR) {
            // 错误处理逻辑（如果有的话）
         }
         break;

      default:
         break;
   }
   return LTC6804_STATE;  // 返回当前状态
}

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
