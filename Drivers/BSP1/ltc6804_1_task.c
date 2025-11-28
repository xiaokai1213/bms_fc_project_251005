#include "ltc6804_1_task.h"
#include "delay.h"

// 全局句柄定义
LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 配置寄存器全局句柄
LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 状态寄存器全局句柄
BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 电池电压全局句柄
AuxVoltageRegs av_h_ltc6804[total_ic];               // 辅助电压全局句柄

static LTC6804_StateEnum_t ltc6804_state = LTC6804_STATE_IDLE;  // 6804状态机初始状态为IDLE

/**
 * @brief   ltc6804初始化函数，外部接口，暴露给其他模块调用
 * @return  uint8_t  返回错误标志位
 * 0:无错误
 * 1:有错误，PEC校验失败
 */
uint8_t LTC6804_init(void) {
   uint8_t error = 0;  // 错误标志位，0表示无错误

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
      error = 1;  // 初始化失败，返回错误标志位
   } else {
      printf("LTC6804_MODULAR INIT OK!\n\r");
      error = 0;  // 初始化成功，返回无错误标志位
   }
   return error;  // 返回错误标志位
}

/**
 * @brief   ltc6804状态机运行函数，外部接口，暴露给其他模块调用
 * @param   event  输入事件
 * @return  LTC6804_ResponseEnum_t  返回响应状态
 */
LTC6804_ResponseEnum_t LTC6804_task_run(LTC6804_EventEnum_t event) {
   static uint64_t conv_start_time = 0;            // 转换开始时间，单位：ms
   LTC6804_ResponseEnum_t resp = LTC6804_RESP_OK;  // 默认响应为正常

   // 输入当前状态和事件，执行状态机转换
   switch (ltc6804_state) {
      case LTC6804_STATE_IDLE:                                     // 空闲状态
         if (event == LTC6804_EVENT_COLLECT_VOLTAGE) {             // 如果是采集电压事件
            LTC6804_adcv(MD_NORMAL, DCP_DISABLED, CH_ALL);         // 电池通道ADC转换命令，以正常转换，禁止放电，所有通道模式发送命令
            conv_start_time = sys_time;                            // 记录转换开始时间
            ltc6804_state = LTC6804_STATE_WAIT_VOLTAGE_CONV;       // 转换到等待电压转换完成状态
            resp = LTC6804_RESP_BUSY;                              // 芯片忙，等待转换完成
         } else if (event == LTC6804_EVENT_COLLECT_TEMPERATURE) {  // 如果是采集温度事件
            LTC6804_adax(MD_NORMAL, CHG_ALL);                      // GPIO通道ADC转换命令，以正常转换，所有通道模式发送命令
            conv_start_time = sys_time;                            // 记录转换开始时间
            ltc6804_state = LTC6804_STATE_WAIT_TEMPERATURE_CONV;   // 转换到等待温度转换完成状态
            resp = LTC6804_RESP_BUSY;                              // 芯片忙，等待转换完成
         } else if (event == LTC6804_EVENT_READ_STATUS) {          // 如果是读取状态寄存器事件
            LTC6804_adstat(MD_NORMAL, CHST_ALL);                   // 状态组ADC转换命令，以正常转换，所有状态组模式发送命令
            conv_start_time = sys_time;                            // 记录读取开始时间
            ltc6804_state = LTC6804_STATE_WAIT_STATUS_READ;        // 转换到等待状态寄存器读取完成状态
            resp = LTC6804_RESP_BUSY;                              // 芯片忙，等待读取完成
         } else {
            resp = LTC6804_RESP_OK;  // 无事件，保持空闲状态，返回正常响应
         }
         break;

      case LTC6804_STATE_WAIT_VOLTAGE_CONV:                                             // 等待电压转换完成状态
         if ((sys_time - conv_start_time) >= LTC6804_VOLTAGE_CONV_MS) {                 // 判断转换时间是否到达
            if (read_LTC6804_Battery_voltage_registers(total_ic, cv_h_ltc6804) == 0) {  // 读取电池电压到存储句柄
               resp = LTC6804_RESP_OK;                                                  // 读取电池电压成功
            } else {
               resp = LTC6804_RESP_ERROR;  // 读取电池电压失败,返回错误状态
            }
            ltc6804_state = LTC6804_STATE_IDLE;  // 转换到空闲状态
         } else {
            resp = LTC6804_RESP_BUSY;  // 芯片忙，等待转换完成
         }
         break;

      case LTC6804_STATE_WAIT_TEMPERATURE_CONV:                                           // 等待温度转换完成状态
         if ((sys_time - conv_start_time) >= LTC6804_TEMPERATURE_CONV_MS) {               // 判断转换时间是否到达
            if (read_LTC6804_Auxiliary_voltage_registers(total_ic, av_h_ltc6804) == 0) {  // 读取辅助寄存器到存储句柄
               resp = LTC6804_RESP_OK;                                                    // 读取辅助寄存器成功
            } else {
               resp = LTC6804_RESP_ERROR;  // 读取辅助寄存器失败,返回错误状态
            }
            ltc6804_state = LTC6804_STATE_IDLE;  // 转换到空闲状态
         } else {
            resp = LTC6804_RESP_BUSY;  // 芯片忙，等待转换完成
         }
         break;

      case LTC6804_STATE_WAIT_STATUS_READ:                                       // 等待状态寄存器读取完成状态
         if ((sys_time - conv_start_time) >= LTC6804_STATUS_READ_MS) {           // 判断读取时间是否到达
            if (read_LTC6804_status_registers(total_ic, stat_h_ltc6804) == 0) {  // 读取状态寄存器到存储句柄
               resp = LTC6804_RESP_OK;                                           // 读取状态寄存器成功
            } else {
               resp = LTC6804_RESP_ERROR;  // 读取状态寄存器失败,返回错误状态
            }
            ltc6804_state = LTC6804_STATE_IDLE;  // 转换到空闲状态
         } else {
            resp = LTC6804_RESP_BUSY;  // 芯片忙，等待读取完成
         }
         break;

      default:
         ltc6804_state = LTC6804_STATE_IDLE;  // 出现未知状态，重置为IDLE
         break;
   }

   return resp;
}
