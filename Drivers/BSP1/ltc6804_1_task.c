#include "ltc6804_1_task.h"
#include "delay.h"

LTC6804_ConfigRegsTypeDef cfgr_h_ltc6804[total_ic];  // 配置寄存器全局句柄
LTC6804_StatusRegs stat_h_ltc6804[total_ic];         // 状态寄存器全局句柄
BatteryVoltageRegs cv_h_ltc6804[total_ic];           // 电池电压全局句柄
AuxVoltageRegs av_h_ltc6804[total_ic];               // 辅助电压全局句柄

ltc6804_event_t LTC6804_EVENT = LTC6804_EVENT_INIT;  // ltc6804状态机当前事件变量
ltc6804_state_t LTC6804_STATE = LTC6804_STATE_INIT;  // ltc6804状态机当前状态变量

/**
 * @brief   ltc6804事件状态机
 */
void ltc6804_state_machine(ltc6804_event_t) {}

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
 * @brief   ltc6804电压获取函数，将获取的电压存放在电压结构体句柄中
 * @return  uint8_t error:错误标志位。
 * 0: 读取的数据PEC匹配
 * 1: 读取的数据PEC不匹配
 */
uint8_t ltc6804_Get_Voltage(void) {
   uint8_t error = 1;                              // 定义错误标志位
   LTC6804_adcv(MD_NORMAL, DCP_DISABLED, CH_ALL);  // 电池通道ADC转换命令，以正常转换，禁止放电，所有通道模式发送命令
   delay_ms(10);                                   // 延时10ms等待转换完成
   for (int i = 0; i < 3; i++) {                   // 尝试三次读取电压,若均失败则返回1
      // 读取电池电压到电压存储控制句柄
      if (read_LTC6804_Battery_voltage_registers(total_ic, cv_h_ltc6804) == 0) {
         error = 0;     // PEC正常
         return error;  // 跳出函数,返回标志位
      }
   }
   error = 1;     // pec错误,错误位置1
   return error;  // PEC不匹配,返回标志位
}

/**
 * @brief   ltc6804电芯温度获取函数，获取辅助GPIO测量得到的外部电芯电压
 * @return  uint8_t error:错误标志位
 * 0:读取的数据PEC匹配
 * 1:读取的数据PEC不匹配
 */
uint8_t ltc6804_Get_temperature(void) {
   uint8_t error = 1;                 // 定义错误标志位
   LTC6804_adax(MD_NORMAL, CHG_ALL);  // GPIO通道ADC转换命令，以正常转换，所有通道模式发送命令
   delay_ms(10);                      // 延时10ms等待转换完成
   for (int i = 0; i < 3; i++) {      // 尝试三次读取温度,若均失败则返回1
      // 读取温度电压到温度存储控制句柄
      if (read_LTC6804_Auxiliary_voltage_registers(total_ic, av_h_ltc6804) == 0) {
         error = 0;     // PEC正常
         return error;  // 跳出函数,返回标志位
      }
   }
   error = 1;     // pec错误,错误位置1
   return error;  // PEC不匹配,返回标志位
}

void ltc6804_st(void) {
   LTC6804_adstat(MD_NORMAL, CHST_ALL);
   read_LTC6804_status_registers(total_ic, stat_h_ltc6804);
   printf(",REV=%d\n", (int)stat_h_ltc6804[0].groupA.STAR0.reg_val);
   printf(",REV=%d\n", (int)stat_h_ltc6804[0].groupA.STAR1.reg_val);
}

void ltc6804_cv(void) {
   LTC6804_adcv(MD_NORMAL, DCP_DISABLED, CH_ALL);
   // LTC6804_cvst(MD_NORMAL, ST1);
   delay_ms(20);
   read_LTC6804_Battery_voltage_registers(total_ic, cv_h_ltc6804);
   for (uint8_t ic = 0; ic < total_ic; ic++) {
      printf("[%d]vbat:%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", (int)ic, (float)cv_h_ltc6804[ic].C01V / 10000, (float)cv_h_ltc6804[ic].C02V / 10000, (float)cv_h_ltc6804[ic].C03V / 10000,
             (float)cv_h_ltc6804[ic].C04V / 10000, (float)cv_h_ltc6804[ic].C05V / 10000, (float)cv_h_ltc6804[ic].C06V / 10000, (float)cv_h_ltc6804[ic].C07V / 10000,
             (float)cv_h_ltc6804[ic].C08V / 10000, (float)cv_h_ltc6804[ic].C09V / 10000, (float)cv_h_ltc6804[ic].C10V / 10000, (float)cv_h_ltc6804[ic].C11V / 10000,
             (float)cv_h_ltc6804[ic].C12V / 10000);

      /*
      printf("[%d]v1:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C01V / 10000);
printf("[%d]v2:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C02V / 10000);
printf("[%d]v3:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C03V / 10000);
delay_ms(10);
printf("[%d]v4:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C04V / 10000);
printf("[%d]v5:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C05V / 10000);
// printf("[%d]v6:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C06V / 10000);
delay_ms(10);
printf("[%d]v7:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C07V / 10000);
printf("[%d]v8:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C08V / 10000);
printf("[%d]v9:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C09V / 10000);
delay_ms(10);
printf("[%d]v10:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C10V / 10000);
printf("[%d]v11:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C11V / 10000);
// printf("[%d]v12:%f\n", (int)ic, (float)cv_h_ltc6804[ic].C12V / 10000);
      */

      delay_ms(10);
      printf("\r\n");
   }
}

/**
 * @brief   导线开路检查函数
 * @return
 */
uint8_t ltc6804_Open_wire_inspection(void) {
   uint8_t wire_error = 0;         // 导线开路错误返回位
   uint8_t wire_error2 = 0;        // 导线开路错误返回位
   float cell_pu[total_ic][12];    // 上拉电流读取的电池电压
   float cell_pd[total_ic][12];    // 下拉电流读取的电池电压
   float cell_diff[total_ic][13];  // 电池2-12的上拉与下拉测量结果之差

   LTC6804_adow(MD_NORMAL, PUP_UP, DCP_DISABLED, CH_ALL);
   delay_ms(3);
   LTC6804_adow(MD_NORMAL, PUP_UP, DCP_DISABLED, CH_ALL);
   delay_ms(3);
   LTC6804_adow(MD_NORMAL, PUP_UP, DCP_DISABLED, CH_ALL);
   delay_ms(3);
   read_LTC6804_Battery_voltage_registers(total_ic, cv_h_ltc6804);
   for (uint8_t ic = 0; ic < total_ic; ic++) {
      cell_pu[ic][0] = (float)cv_h_ltc6804[ic].C01V / 10000;
      cell_pu[ic][1] = (float)cv_h_ltc6804[ic].C02V / 10000;
      cell_pu[ic][2] = (float)cv_h_ltc6804[ic].C03V / 10000;
      cell_pu[ic][3] = (float)cv_h_ltc6804[ic].C04V / 10000;
      cell_pu[ic][4] = (float)cv_h_ltc6804[ic].C05V / 10000;
      cell_pu[ic][5] = (float)cv_h_ltc6804[ic].C06V / 10000;
      cell_pu[ic][6] = (float)cv_h_ltc6804[ic].C07V / 10000;
      cell_pu[ic][7] = (float)cv_h_ltc6804[ic].C08V / 10000;
      cell_pu[ic][8] = (float)cv_h_ltc6804[ic].C09V / 10000;
      cell_pu[ic][9] = (float)cv_h_ltc6804[ic].C10V / 10000;
      cell_pu[ic][10] = (float)cv_h_ltc6804[ic].C11V / 10000;
      cell_pu[ic][11] = (float)cv_h_ltc6804[ic].C12V / 10000;
   }
   LTC6804_adow(MD_NORMAL, PUP_DOWN, DCP_DISABLED, CH_ALL);
   delay_ms(3);
   LTC6804_adow(MD_NORMAL, PUP_DOWN, DCP_DISABLED, CH_ALL);
   delay_ms(3);
   LTC6804_adow(MD_NORMAL, PUP_DOWN, DCP_DISABLED, CH_ALL);
   delay_ms(3);
   read_LTC6804_Battery_voltage_registers(total_ic, cv_h_ltc6804);  // 读取电池电压
   for (uint8_t ic = 0; ic < total_ic; ic++) {
      cell_pd[ic][0] = (float)cv_h_ltc6804[ic].C01V / 10000;
      cell_pd[ic][1] = (float)cv_h_ltc6804[ic].C02V / 10000;
      cell_pd[ic][2] = (float)cv_h_ltc6804[ic].C03V / 10000;
      cell_pd[ic][3] = (float)cv_h_ltc6804[ic].C04V / 10000;
      cell_pd[ic][4] = (float)cv_h_ltc6804[ic].C05V / 10000;
      cell_pd[ic][5] = (float)cv_h_ltc6804[ic].C06V / 10000;
      cell_pd[ic][6] = (float)cv_h_ltc6804[ic].C07V / 10000;
      cell_pd[ic][7] = (float)cv_h_ltc6804[ic].C08V / 10000;
      cell_pd[ic][8] = (float)cv_h_ltc6804[ic].C09V / 10000;
      cell_pd[ic][9] = (float)cv_h_ltc6804[ic].C10V / 10000;
      cell_pd[ic][10] = (float)cv_h_ltc6804[ic].C11V / 10000;
      cell_pd[ic][11] = (float)cv_h_ltc6804[ic].C12V / 10000;
   }

   for (uint8_t ic = 0; ic < total_ic; ic++) {
      cell_diff[ic][0] = cell_pu[ic][0];
      cell_diff[ic][1] = cell_pu[ic][1] - cell_pd[ic][1];
      cell_diff[ic][2] = cell_pu[ic][2] - cell_pd[ic][2];
      cell_diff[ic][3] = cell_pu[ic][3] - cell_pd[ic][3];
      cell_diff[ic][4] = cell_pu[ic][4] - cell_pd[ic][4];
      cell_diff[ic][5] = cell_pu[ic][5] - cell_pd[ic][5];
      cell_diff[ic][6] = cell_pu[ic][6] - cell_pd[ic][6];
      cell_diff[ic][7] = cell_pu[ic][7] - cell_pd[ic][7];
      cell_diff[ic][8] = cell_pu[ic][8] - cell_pd[ic][8];
      cell_diff[ic][9] = cell_pu[ic][9] - cell_pd[ic][9];
      cell_diff[ic][10] = cell_pu[ic][10] - cell_pd[ic][10];
      cell_diff[ic][11] = cell_pu[ic][11] - cell_pd[ic][11];
      cell_diff[ic][12] = cell_pd[ic][11];
   }

   for (uint8_t ic = 0; ic < total_ic; ic++) {
      for (uint8_t i = 0; i < 13; i++) {
         if (i == 0) {
            wire_error = (cell_diff[ic][i] < 0.1) ? 1 : 0;
            if (wire_error == 1) {
               wire_error2 = 1;
            }
            printf("wire_error:%d\n", (int)wire_error);
         } else {
            wire_error = (cell_diff[ic][i] < -0.4) ? 1 : 0;
            if (wire_error == 1) {
               wire_error2 = 1;
            }
            printf("wire_error:%d\n", (int)wire_error);
         }

         // printf("diff[%d]:%f\n", (int)i, cell_diff[ic][i]);
      }
   }
   printf("\r\n");
   return wire_error2;
}
