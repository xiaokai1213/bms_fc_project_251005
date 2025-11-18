#include "can_task.h"
#include "can.h"
#include "led_relay_gpio.h"
#include "ltc6804_1_task.h"

/**
 * @brief   通过can通讯发送电压数据
 * @return  uint8_t:发送状态
 * 0:发送成功
 * 1:发送超时
 * 2:发送异常
 */
uint8_t can_tx_voltage_data(void) {
   uint8_t return_send_state = 0;   // 要返回的发送状态
   uint16_t* voltages_p[cell_num];  // 指针数组,存储20个电芯电压数据地址,方便后续操作
   uint8_t cv01to04[8];             // 建立can报文发送缓冲
   uint8_t cv05to08[8];
   uint8_t cv09to12[8];
   uint8_t cv13to16[8];
   uint8_t cv17to20[8];

   // 初始化电压地址
   voltages_p[0] = &cv_h_ltc6804[0].C01V;
   voltages_p[1] = &cv_h_ltc6804[0].C02V;
   voltages_p[2] = &cv_h_ltc6804[0].C03V;
   voltages_p[3] = &cv_h_ltc6804[0].C04V;
   voltages_p[4] = &cv_h_ltc6804[0].C05V;

   voltages_p[5] = &cv_h_ltc6804[0].C07V;
   voltages_p[6] = &cv_h_ltc6804[0].C08V;
   voltages_p[7] = &cv_h_ltc6804[0].C09V;
   voltages_p[8] = &cv_h_ltc6804[0].C10V;
   voltages_p[9] = &cv_h_ltc6804[0].C11V;

   voltages_p[10] = &cv_h_ltc6804[1].C01V;
   voltages_p[11] = &cv_h_ltc6804[1].C02V;
   voltages_p[12] = &cv_h_ltc6804[1].C03V;
   voltages_p[13] = &cv_h_ltc6804[1].C04V;
   voltages_p[14] = &cv_h_ltc6804[1].C05V;

   voltages_p[15] = &cv_h_ltc6804[1].C07V;
   voltages_p[16] = &cv_h_ltc6804[1].C08V;
   voltages_p[17] = &cv_h_ltc6804[1].C09V;
   voltages_p[18] = &cv_h_ltc6804[1].C10V;
   voltages_p[19] = &cv_h_ltc6804[1].C11V;

   // can报文电压发送缓冲区
   cv01to04[0] = (uint8_t)(*voltages_p[0] >> 8);
   cv01to04[1] = (uint8_t)*voltages_p[0];
   cv01to04[2] = (uint8_t)(*voltages_p[1] >> 8);
   cv01to04[3] = (uint8_t)*voltages_p[1];
   cv01to04[4] = (uint8_t)(*voltages_p[2] >> 8);
   cv01to04[5] = (uint8_t)*voltages_p[2];
   cv01to04[6] = (uint8_t)(*voltages_p[3] >> 8);
   cv01to04[7] = (uint8_t)*voltages_p[3];

   cv05to08[0] = (uint8_t)(*voltages_p[4] >> 8);
   cv05to08[1] = (uint8_t)*voltages_p[4];
   cv05to08[2] = (uint8_t)(*voltages_p[5] >> 8);
   cv05to08[3] = (uint8_t)*voltages_p[5];
   cv05to08[4] = (uint8_t)(*voltages_p[6] >> 8);
   cv05to08[5] = (uint8_t)*voltages_p[6];
   cv05to08[6] = (uint8_t)(*voltages_p[7] >> 8);
   cv05to08[7] = (uint8_t)*voltages_p[7];

   cv09to12[0] = (uint8_t)(*voltages_p[8] >> 8);
   cv09to12[1] = (uint8_t)*voltages_p[8];
   cv09to12[2] = (uint8_t)(*voltages_p[9] >> 8);
   cv09to12[3] = (uint8_t)*voltages_p[9];
   cv09to12[4] = (uint8_t)(*voltages_p[10] >> 8);
   cv09to12[5] = (uint8_t)*voltages_p[10];
   cv09to12[6] = (uint8_t)(*voltages_p[11] >> 8);
   cv09to12[7] = (uint8_t)*voltages_p[11];

   cv13to16[0] = (uint8_t)(*voltages_p[12] >> 8);
   cv13to16[1] = (uint8_t)*voltages_p[12];
   cv13to16[2] = (uint8_t)(*voltages_p[13] >> 8);
   cv13to16[3] = (uint8_t)*voltages_p[13];
   cv13to16[4] = (uint8_t)(*voltages_p[14] >> 8);
   cv13to16[5] = (uint8_t)*voltages_p[14];
   cv13to16[6] = (uint8_t)(*voltages_p[15] >> 8);
   cv13to16[7] = (uint8_t)*voltages_p[15];

   cv17to20[0] = (uint8_t)(*voltages_p[16] >> 8);
   cv17to20[1] = (uint8_t)*voltages_p[16];
   cv17to20[2] = (uint8_t)(*voltages_p[17] >> 8);
   cv17to20[3] = (uint8_t)*voltages_p[17];
   cv17to20[4] = (uint8_t)(*voltages_p[18] >> 8);
   cv17to20[5] = (uint8_t)*voltages_p[18];
   cv17to20[6] = (uint8_t)(*voltages_p[19] >> 8);
   cv17to20[7] = (uint8_t)*voltages_p[19];

   uint64_t current_time = sys_time;  // 记录当前时间
   while (1) {                        // 循环
      // 检查电压报文1发送标志位
      if (flag.voltage_can_tx_01_to_04_ready_flag == 1) {
         if (can_tx_extid_8(can_tx_id_voltage_01_to_04, cv01to04[8]) == 0) {
            flag.voltage_can_tx_01_to_04_ready_flag = 0;  // 发送位置0
         }
      }

      // 检查电压报文2发送标志位
      if (flag.voltage_can_tx_05_to_08_ready_flag == 1) {
         if (can_tx_extid_8(can_tx_id_voltage_05_to_08, cv05to08[8]) == 0) {
            flag.voltage_can_tx_05_to_08_ready_flag = 0;  // 发送位置0
         }
      }

      // 检查电压报文3发送标志位
      if (flag.voltage_can_tx_09_to_12_ready_flag == 1) {
         if (can_tx_extid_8(can_tx_id_voltage_09_to_12, cv09to12[8]) == 0) {
            flag.voltage_can_tx_09_to_12_ready_flag = 0;  // 发送位置0
         }
      }

      // 检查电压报文4发送标志位
      if (flag.voltage_can_tx_13_to_16_ready_flag == 1) {
         if (can_tx_extid_8(can_tx_id_voltage_13_to_16, cv13to16[8]) == 0) {
            flag.voltage_can_tx_13_to_16_ready_flag = 0;  // 发送位置0
         }
      }

      // 检查电压报文4发送标志位
      if (flag.voltage_can_tx_17_to_20_ready_flag == 1) {
         if (can_tx_extid_8(can_tx_id_voltage_17_to_20, cv17to20[8]) == 0) {
            flag.voltage_can_tx_17_to_20_ready_flag = 0;  // 发送位置0
         }
      }

      // 检查每个报文发送状态
      if (flag.voltage_can_tx_01_to_04_ready_flag == 0) {
         if (flag.voltage_can_tx_05_to_08_ready_flag == 0) {
            if (flag.voltage_can_tx_09_to_12_ready_flag == 0) {
               if (flag.voltage_can_tx_13_to_16_ready_flag == 0) {
                  if (flag.voltage_can_tx_17_to_20_ready_flag == 0) {
                     return_send_state = 0;
                     return return_send_state;
                  }
               }
            }
         }
      }

      //  超时退出
      if ((sys_time - current_time) > 1000) {
         return_send_state = 1;  // 发送超时
         return return_send_state;
      }
   }
}

/**
 * @brief   通过can通讯发送温度数据
 * @return  uint8_t:发送状态
 * 0:发送成功
 * 1:发送超时
 * 2:发送异常
 */
uint8_t can_tx_temperature_data(void) {
   uint8_t return_send_state = 0;       // 要返回的发送状态
   uint16_t* temperatures_p[temp_num];  // 指针数组,存储温度传感器数据地址,方便后续操作
   uint8_t ct01to04[8];                 // 建立can报文发送缓冲
   uint8_t ct05to08[8];
   uint8_t ct09to10[8];

   // 初始化温度地址
   temperatures_p[0] = &av_h_ltc6804[0].G1V;
   temperatures_p[1] = &av_h_ltc6804[0].G2V;
   temperatures_p[2] = &av_h_ltc6804[0].G3V;
   temperatures_p[3] = &av_h_ltc6804[0].G4V;
   temperatures_p[4] = &av_h_ltc6804[0].G5V;

   temperatures_p[5] = &av_h_ltc6804[1].G1V;
   temperatures_p[6] = &av_h_ltc6804[1].G2V;
   temperatures_p[7] = &av_h_ltc6804[1].G3V;
   temperatures_p[8] = &av_h_ltc6804[1].G4V;
   temperatures_p[9] = &av_h_ltc6804[1].G5V;

   // can报文温度数据填充
   ct01to04[0] = (uint8_t)(*temperatures_p[0] >> 8);
   ct01to04[1] = (uint8_t)*temperatures_p[0];
   ct01to04[2] = (uint8_t)(*temperatures_p[1] >> 8);
   ct01to04[3] = (uint8_t)*temperatures_p[1];
   ct01to04[4] = (uint8_t)(*temperatures_p[2] >> 8);
   ct01to04[5] = (uint8_t)*temperatures_p[2];
   ct01to04[6] = (uint8_t)(*temperatures_p[3] >> 8);
   ct01to04[7] = (uint8_t)*temperatures_p[3];

   ct05to08[0] = (uint8_t)(*temperatures_p[4] >> 8);
   ct05to08[1] = (uint8_t)*temperatures_p[4];
   ct05to08[2] = (uint8_t)(*temperatures_p[5] >> 8);
   ct05to08[3] = (uint8_t)*temperatures_p[5];
   ct05to08[4] = (uint8_t)(*temperatures_p[6] >> 8);
   ct05to08[5] = (uint8_t)*temperatures_p[6];
   ct05to08[6] = (uint8_t)(*temperatures_p[7] >> 8);
   ct05to08[7] = (uint8_t)*temperatures_p[7];

   ct09to10[0] = (uint8_t)(*temperatures_p[8] >> 8);
   ct09to10[1] = (uint8_t)*temperatures_p[8];
   ct09to10[2] = (uint8_t)(*temperatures_p[9] >> 8);
   ct09to10[3] = (uint8_t)*temperatures_p[9];
   ct09to10[4] = 0;
   ct09to10[5] = 0;
   ct09to10[6] = 0;
   ct09to10[7] = 0;

   uint16_t current_time = sys_time;  // 记录当前时间
   while (1) {                        // 循环
      // 检查温度报文1发送标志位
      if (flag.temperature_can_tx_01_to_04_ready_flag == 1) {
         if (can_tx_extid_8(can_tx_id_temperature_01_to_04, ct01to04[8]) == 0) {
            flag.temperature_can_tx_01_to_04_ready_flag = 0;  // 发送位置0
         }
      }
      // 检查温度报文2发送标志位
      if (flag.temperature_can_tx_05_to_08_ready_flag == 1) {
         if (can_tx_extid_8(can_tx_id_temperature_05_to_08, ct05to08[8]) == 0) {
            flag.temperature_can_tx_05_to_08_ready_flag = 0;  // 发送位置0
         }
      }
      // 检查温度报文3发送标志位
      if (flag.temperature_can_tx_09_to_10_ready_flag == 1) {
         if (can_tx_extid_8(can_tx_id_temperature_09_to_10, ct09to10[8]) == 0) {
            flag.temperature_can_tx_09_to_10_ready_flag = 0;  // 发送位置0
         }
      }

      // 检查每个报文发送状态
      if (flag.temperature_can_tx_01_to_04_ready_flag == 0) {
         if (flag.temperature_can_tx_05_to_08_ready_flag == 0) {
            if (flag.temperature_can_tx_09_to_10_ready_flag == 0) {
               return_send_state = 0;
               return return_send_state;
            }
         }
      }

      //  超时退出
      if ((sys_time - current_time) > 1000) {
         return_send_state = 1;  // 发送超时
         return return_send_state;
      }
   }
}

/**
 * @brief 扩展can发送八位数据帧
 * @param   extid:扩展标识符(29位)，0x1FFFFFFF是最大的29位数
 * @param   data[8]: 要发送的数据指针
 * @return  uint8_t :发送状态
 * 0：发送成功
 * 1：邮箱满，发送失败
 * 2：邮箱未满，发送失败
 */
uint8_t can_tx_extid_8(uint32_t extid, uint8_t data[8]) {
   CAN_TxHeaderTypeDef TxHeader;  // can发送消息句柄
   uint32_t TxMailbox;            // 用于返回使用的邮箱号
   uint8_t txdata[8] = {0};       // 发送数据缓冲区
   uint8_t send_state = 0;        // 发送状态

   // 复制数据到缓冲区
   for (uint8_t i = 0; i < 8; i++) {
      txdata[i] = data[i];
   }

   // 配置发送消息句柄
   TxHeader.StdId = 0;
   TxHeader.ExtId = extid;
   TxHeader.IDE = CAN_ID_EXT;
   TxHeader.RTR = CAN_RTR_DATA;
   TxHeader.DLC = 8;
   TxHeader.TransmitGlobalTime = DISABLE;

   // 发送消息
   if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) != 0) {
      if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, txdata, &TxMailbox) != HAL_OK) {
         printf("can_tx_error\n");
         send_state = 2;
      } else {
         send_state = 0;
      }
   } else {
      send_state = 1;
   }
   return send_state;
}

/*
CAN_TxHeaderTypeDef：定义发送消息的帧格式，包括：
StdId：标准标识符（11位）
ExtId：扩展标识符（29位）
IDE：标识符扩展（CAN_ID_STD 或 CAN_ID_EXT）
RTR：远程传输请求（CAN_RTR_DATA 或 CAN_RTR_REMOTE）
DLC：数据长度代码（0-8）
CAN_RxHeaderTypeDef：定义接收消息的帧格式，字段与 CAN_TxHeaderTypeDef 类似，用于存放接收到的消息头信息。
CAN_FilterTypeDef：定义CAN滤波器的配置参数。
*/
