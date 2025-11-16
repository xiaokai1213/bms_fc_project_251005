#include "can_task.h"
#include "can.h"
#include "led_relay_gpio.h"
#include "ltc6804_1_task.h"

// 电池电压拆分can高低位数据示例
/*
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
*/

/**
 * @brief   通过can通讯发送电压数据
 * @return  uint8_t:发送状态
 * 0:发送
 */
uint8_t can_tx_voltage_data(void) {
   if (flag.voltage_can_tx_PF_10_ready_flag == 1) {  // 检查电压发送报文PF10状态
   }
   if (flag.voltage_can_tx_PF_11_ready_flag == 1) {  // 检查电压发送报文PF11状态
   }
   if (flag.voltage_can_tx_PF_12_ready_flag == 1) {  // 检查电压发送报文PF12状态
   }
   if (flag.voltage_can_tx_PF_13_ready_flag == 1) {  // 检查电压发送报文PF13状态
   }
   if (flag.voltage_can_tx_PF_14_ready_flag == 1) {  // 检查电压发送报文PF14状态
   }
}

/**
 * @brief 扩展can发送八位数据帧
 * @param extid:扩展标识符(29位)，0x1FFFFFFF是最大的29位数
 * @param data[8]: 要发送的数据指针
 */
void can_tx_extid_8(uint32_t extid, uint8_t data[8]) {
   CAN_TxHeaderTypeDef TxHeader;  // can发送消息句柄
   uint32_t TxMailbox;            // 用于返回使用的邮箱号
   uint8_t txdata[8] = {0};       // 发送数据缓冲区

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
   if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, txdata, &TxMailbox) != HAL_OK) {
      printf("can_tx_error\n");
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
