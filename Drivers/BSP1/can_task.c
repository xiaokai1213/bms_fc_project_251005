#include "can_task.h"
#include "can.h"
#include "ltc6804_1_task.h"

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
