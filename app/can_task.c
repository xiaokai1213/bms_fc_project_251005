#include "can_task.h"
#include "can.h"
#include "ltc6804_1_task.h"

CAN_ID_HandleTypeDef CAN_ID_Handle;  // CAN ID句柄

/**
 * @brief   CAN地址初始化函数
 * @param   无
 * @return  无
 */
void CAN_Address_init(void) {
   // 初始化canID句柄中的发送ID和接收ID
   CAN_ID_Handle.TX_ID.cell_vot_1_4_id = 0x1000FF11;    // 电池电压1-4的CAN ID
   CAN_ID_Handle.TX_ID.cell_vot_5_8_id = 0x1000FF12;    // 电池电压5-8的CAN ID
   CAN_ID_Handle.TX_ID.cell_vot_9_12_id = 0x1000FF13;   // 电池电压9-12的CAN ID
   CAN_ID_Handle.TX_ID.cell_vot_13_16_id = 0x1000FF14;  // 电池电压13-16的CAN ID
   CAN_ID_Handle.TX_ID.cell_vot_17_20_id = 0x1000FF15;  // 电池电压17-20的CAN ID
   CAN_ID_Handle.TX_ID.aux_vot_1_4_id = 0x1000FF16;     // 辅助电压1-4的CAN ID
   CAN_ID_Handle.TX_ID.aux_vot_5_8_id = 0x1000FF17;     // 辅助电压5-8的CAN ID
   CAN_ID_Handle.TX_ID.aux_vot_9_12_id = 0x1000FF18;    // 辅助电压9-12的CAN ID

   // 获取本机从控地址代码
   uint32_t address_code_counter = 0;  // 地址代码计数器
   if (HAL_GPIO_ReadPin(A0_GPIO_Port, A0_Pin) == GPIO_PIN_SET) {
      address_code_counter += 1;  // A0=1,地址代码加1
   }
   if (HAL_GPIO_ReadPin(A1_GPIO_Port, A1_Pin) == GPIO_PIN_SET) {
      address_code_counter += 1;  // A1=1,地址代码加1
   }
   if (HAL_GPIO_ReadPin(A2_GPIO_Port, A2_Pin) == GPIO_PIN_SET) {
      address_code_counter += 1;  // A2=1,地址代码加1
   }
   if (HAL_GPIO_ReadPin(A3_GPIO_Port, A3_Pin) == GPIO_PIN_SET) {
      address_code_counter += 1;  // A3=1,地址代码加1
   }

   // 根据地址代码重新设置ID的高16位
   CAN_ID_Handle.TX_ID.cell_vot_1_4_id = (address_code_counter << 16) | 0x1000FF11;
   CAN_ID_Handle.TX_ID.cell_vot_5_8_id = (address_code_counter << 16) | 0x1000FF12;
   CAN_ID_Handle.TX_ID.cell_vot_9_12_id = (address_code_counter << 16) | 0x1000FF13;
   CAN_ID_Handle.TX_ID.cell_vot_13_16_id = (address_code_counter << 16) | 0x1000FF14;
   CAN_ID_Handle.TX_ID.cell_vot_17_20_id = (address_code_counter << 16) | 0x1000FF15;
   CAN_ID_Handle.TX_ID.aux_vot_1_4_id = (address_code_counter << 16) | 0x1000FF16;
   CAN_ID_Handle.TX_ID.aux_vot_5_8_id = (address_code_counter << 16) | 0x1000FF17;
   CAN_ID_Handle.TX_ID.aux_vot_9_12_id = (address_code_counter << 16) | 0x1000FF18;
}

/**
 * @brief   发送电池电压CAN报文函数
 * @param   无
 * @return  无
 */
void send_cell_vot_can(void) {
   uint16_t* voltages_p[20];  // 指针数组,存储20个电芯电压数据地址,方便后续操作
   uint8_t cv01to04[8];       // 建立can报文发送缓冲
   uint8_t cv05to08[8];
   uint8_t cv09to12[8];
   uint8_t cv13to16[8];
   uint8_t cv17to20[8];
   uint8_t send_complete_flag = 0;  // 发送完成标志位
   uint32_t free_mailboxes = 0;     // 发送邮箱空闲数量

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

   send_complete_flag = 0;                                       // 发送完成标志位清零
   while (send_complete_flag == 0) {                             // 发送标志位不为0时跳出循环
      free_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // free_mailboxes的值为0-3，表示有多少个发送邮箱空闲
      if (free_mailboxes != 0) {
         can_tx_extid_8(CAN_ID_Handle.TX_ID.cell_vot_1_4_id, cv01to04);  // 发送电池电压1-4的CAN报文
         send_complete_flag = 1;                                         // 发送完成标志位置1
      }
   }

   send_complete_flag = 0;                                       // 发送完成标志位清零
   while (send_complete_flag == 0) {                             // 发送标志位不为0时跳出循环
      free_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // free_mailboxes的值为0-3，表示有多少个发送邮箱空闲
      if (free_mailboxes != 0) {
         can_tx_extid_8(CAN_ID_Handle.TX_ID.cell_vot_5_8_id, cv05to08);  // 发送电池电压5-8的CAN报文
         send_complete_flag = 1;                                         // 发送完成标志位置1
      }
   }

   send_complete_flag = 0;                                       // 发送完成标志位清零
   while (send_complete_flag == 0) {                             // 发送标志位不为0时跳出循环
      free_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // free_mailboxes的值为0-3，表示有多少个发送邮箱空闲
      if (free_mailboxes != 0) {
         can_tx_extid_8(CAN_ID_Handle.TX_ID.cell_vot_9_12_id, cv09to12);  // 发送电池电压9-12的CAN报文
         send_complete_flag = 1;                                          // 发送完成标志位置1
      }
   }

   send_complete_flag = 0;                                       // 发送完成标志位清零
   while (send_complete_flag == 0) {                             // 发送标志位不为0时跳出循环
      free_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // free_mailboxes的值为0-3，表示有多少个发送邮箱空闲
      if (free_mailboxes != 0) {
         can_tx_extid_8(CAN_ID_Handle.TX_ID.cell_vot_13_16_id, cv13to16);  // 发送电池电压13-16的CAN报文
         send_complete_flag = 1;                                           // 发送完成标志位置1
      }
   }

   send_complete_flag = 0;                                       // 发送完成标志位清零
   while (send_complete_flag == 0) {                             // 发送标志位不为0时跳出循环
      free_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // free_mailboxes的值为0-3，表示有多少个发送邮箱空闲
      if (free_mailboxes != 0) {
         can_tx_extid_8(CAN_ID_Handle.TX_ID.cell_vot_17_20_id, cv17to20);  // 发送电池电压17-20的CAN报文
         send_complete_flag = 1;                                           // 发送完成标志位置1
      }
   }
}

/**
 * @brief   发送辅助电压CAN报文函数
 * @param   无
 * @return  无
 */
void send_aux_vot_can(void) {
   uint16_t* voltages_p[16];  // 指针数组,存储16个辅助电压数据地址,方便后续操作
   uint8_t av01to04[8];       // 建立can报文发送缓冲
   uint8_t av05to08[8];
   uint8_t av09to12[8];
   uint8_t send_complete_flag = 0;  // 发送完成标志位
   uint32_t free_mailboxes = 0;     // 发送邮箱空闲数量

   // 初始化电压地址
   voltages_p[0] = &av_h_ltc6804[0].G1V;
   voltages_p[1] = &av_h_ltc6804[0].G2V;
   voltages_p[2] = &av_h_ltc6804[0].G3V;
   voltages_p[3] = &av_h_ltc6804[0].G4V;
   voltages_p[4] = &av_h_ltc6804[0].G5V;
   voltages_p[5] = &av_h_ltc6804[0].REF;

   voltages_p[6] = &av_h_ltc6804[1].G1V;
   voltages_p[7] = &av_h_ltc6804[1].G2V;
   voltages_p[8] = &av_h_ltc6804[1].G3V;
   voltages_p[9] = &av_h_ltc6804[1].G4V;
   voltages_p[10] = &av_h_ltc6804[1].G5V;
   voltages_p[11] = &av_h_ltc6804[1].REF;

   // can报文辅助电压发送缓冲区
   av01to04[0] = (uint8_t)(*voltages_p[0] >> 8);
   av01to04[1] = (uint8_t)*voltages_p[0];
   av01to04[2] = (uint8_t)(*voltages_p[1] >> 8);
   av01to04[3] = (uint8_t)*voltages_p[1];
   av01to04[4] = (uint8_t)(*voltages_p[2] >> 8);
   av01to04[5] = (uint8_t)*voltages_p[2];
   av01to04[6] = (uint8_t)(*voltages_p[3] >> 8);
   av01to04[7] = (uint8_t)*voltages_p[3];

   av05to08[0] = (uint8_t)(*voltages_p[4] >> 8);
   av05to08[1] = (uint8_t)*voltages_p[4];
   av05to08[2] = (uint8_t)(*voltages_p[5] >> 8);
   av05to08[3] = (uint8_t)*voltages_p[5];
   av05to08[4] = (uint8_t)(*voltages_p[6] >> 8);
   av05to08[5] = (uint8_t)*voltages_p[6];
   av05to08[6] = (uint8_t)(*voltages_p[7] >> 8);
   av05to08[7] = (uint8_t)*voltages_p[7];

   av09to12[0] = (uint8_t)(*voltages_p[8] >> 8);
   av09to12[1] = (uint8_t)*voltages_p[8];
   av09to12[2] = (uint8_t)(*voltages_p[9] >> 8);
   av09to12[3] = (uint8_t)*voltages_p[9];
   av09to12[4] = (uint8_t)(*voltages_p[10] >> 8);
   av09to12[5] = (uint8_t)*voltages_p[10];
   av09to12[6] = (uint8_t)(*voltages_p[11] >> 8);
   av09to12[7] = (uint8_t)*voltages_p[11];

   send_complete_flag = 0;                                       // 发送完成标志位清零
   while (send_complete_flag == 0) {                             // 发送标志位不为0时跳出循环
      free_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // free_mailboxes的值为0-3，表示有多少个发送邮箱空闲
      if (free_mailboxes != 0) {
         can_tx_extid_8(CAN_ID_Handle.TX_ID.aux_vot_1_4_id, av01to04);  // 发送辅助电压1-4的CAN报文
         send_complete_flag = 1;                                        // 发送完成标志位置1
      }
   }

   send_complete_flag = 0;                                       // 发送完成标志位清零
   while (send_complete_flag == 0) {                             // 发送标志位不为0时跳出循环
      free_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // free_mailboxes的值为0-3，表示有多少个发送邮箱空闲
      if (free_mailboxes != 0) {
         can_tx_extid_8(CAN_ID_Handle.TX_ID.aux_vot_5_8_id, av05to08);  // 发送辅助电压5-8的CAN报文
         send_complete_flag = 1;                                        // 发送完成标志位置1
      }
   }

   send_complete_flag = 0;                                       // 发送完成标志位清零
   while (send_complete_flag == 0) {                             // 发送标志位不为0时跳出循环
      free_mailboxes = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  // free_mailboxes的值为0-3，表示有多少个发送邮箱空闲
      if (free_mailboxes != 0) {
         can_tx_extid_8(CAN_ID_Handle.TX_ID.aux_vot_9_12_id, av09to12);  // 发送辅助电压9-12的CAN报文
         send_complete_flag = 1;                                         // 发送完成标志位置1
      }
   }
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
