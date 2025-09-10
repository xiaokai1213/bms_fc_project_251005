#include "can.h"

CAN_HandleTypeDef hcan;  // can外设初始化句柄

/**
 * @brief      初始化CAN1外设，初始化波特率为500kHz
 * @param      无
 * @retval     无
 */
void CAN_Init(void) {
   hcan.Instance = CAN1;                      // 指定CAN1外设实例
   hcan.Init.Prescaler = 9;                   // 设置位时序分频器
   hcan.Init.Mode = CAN_MODE_NORMAL;          // 设置为正常工作模式
   hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;     // 同步跳转宽度为1个时间量子
   hcan.Init.TimeSeg1 = CAN_BS1_5TQ;          // 位段1长度为5个时间量子
   hcan.Init.TimeSeg2 = CAN_BS2_2TQ;          // 位段2长度为2个时间量子
   hcan.Init.TimeTriggeredMode = DISABLE;     // 禁用时间触发模式
   hcan.Init.AutoBusOff = ENABLE;             // 启用自动总线关闭恢复
   hcan.Init.AutoWakeUp = ENABLE;             // 禁用自动唤醒
   hcan.Init.AutoRetransmission = ENABLE;     // 启用自动重传
   hcan.Init.ReceiveFifoLocked = DISABLE;     // 禁用接收FIFO锁定模式
   hcan.Init.TransmitFifoPriority = DISABLE;  // 禁用发送FIFO优先级

   if (HAL_CAN_Init(&hcan) != HAL_OK)  // 初始化CAN外设
   {
      printf("can_not_ok\n");
      // Error_Handler();  // 初始化失败时调用错误处理
   }

   if (HAL_CAN_Start(&hcan) != HAL_OK) {
      printf("can_not_ok\n");
   }
}

/**
 * @brief      CAN1底层硬件初始化
 * @param      canHandle CAN句柄指针
 * @retval     无
 */
void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle) {
   GPIO_InitTypeDef GPIO_InitStruct = {0};  // 定义GPIO初始化结构体

   if (canHandle->Instance == CAN1)  // 判断是否为CAN1外设
   {
      __HAL_RCC_CAN1_CLK_ENABLE();   // 使能CAN1外设时钟
      __HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA端口时钟

      // CAN引脚配置说明
      //  PA11  ------> CAN_RX     PA11作为CAN接收引脚
      //  PA12  ------> CAN_TX     PA12作为CAN发送引脚
      GPIO_InitStruct.Pin = GPIO_PIN_11;       // 配置RX引脚(PA11)
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // 设置为输入模式
      GPIO_InitStruct.Pull = GPIO_NOPULL;      // 不使用上下拉电阻
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  // 应用RX引脚配置

      GPIO_InitStruct.Pin = GPIO_PIN_12;             // 配置TX引脚(PA12)
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        // 设置为复用推挽输出
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 设置为高速模式
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        // 应用TX引脚配置
   }
}
