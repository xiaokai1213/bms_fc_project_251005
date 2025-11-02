#include "can.h"

CAN_HandleTypeDef hcan1;  // can外设初始化句柄

/**
 * @brief      初始化CAN1外设，初始化波特率为500kHz
 * @param      无
 * @retval     无
 */
void CAN_Init(void) {
   hcan1.Instance = CAN1;                      // 指定CAN1外设实例
   hcan1.Init.Prescaler = 9;                   // 设置位时序分频器
   hcan1.Init.Mode = CAN_MODE_NORMAL;          // 设置为正常工作模式
   hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;     // 同步跳转宽度为1个时间量子
   hcan1.Init.TimeSeg1 = CAN_BS1_5TQ;          // 位段1长度为5个时间量子
   hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;          // 位段2长度为2个时间量子
   hcan1.Init.TimeTriggeredMode = DISABLE;     // 禁用时间触发模式
   hcan1.Init.AutoBusOff = ENABLE;             // 启用自动总线关闭恢复
   hcan1.Init.AutoWakeUp = ENABLE;             // 启用自动唤醒
   hcan1.Init.AutoRetransmission = ENABLE;     // 启用自动重传
   hcan1.Init.ReceiveFifoLocked = DISABLE;     // 禁用接收FIFO锁定模式
   hcan1.Init.TransmitFifoPriority = DISABLE;  // 禁用发送FIFO优先级

   if (HAL_CAN_Init(&hcan1) != HAL_OK) {  // 初始化CAN外设
      printf("can_not_ok\n");
      // Error_Handler();  // 初始化失败时调用错误处理
   }

   CAN_FilterTypeDef canFilter;  // 过滤器结构体

   // 配置过滤器0:32位列表模式,关联至FIFO0
   canFilter.FilterBank = 0;                           // 滤波器编号（0~13，F103最多14个）
   canFilter.FilterMode = CAN_FILTERMODE_IDMASK;       // 模式，掩码模式（IDMASK）或列表模式（IDLIST）
   canFilter.FilterScale = CAN_FILTERSCALE_32BIT;      // 尺寸，16位模式 or 32位模式
   canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;  // 分配到哪个 FIFO，取FIFO0 或 FIFO1
   canFilter.FilterActivation = ENABLE;                // 启用或禁用该滤波器
   // 配置为接收所有扩展数据帧
   uint32_t ext_id = 0x00000000;  // 基准ID（任意值，掩码会放开所有位）
   uint32_t mask = 0x00000000;    // 掩码=0：所有位不关心（全部接收）

   // 拆分29位扩展ID到FilterIdHigh和FilterIdLow（含IDE位）
   canFilter.FilterIdHigh = (ext_id >> 13) & 0xFFFF;  // 高16位：29位ID的第18~28位（11位）
   canFilter.FilterIdLow = (ext_id & 0x1FFF) << 3;    // 低16位：29位ID的第0~17位（18位）左移3位
   canFilter.FilterIdLow |= CAN_ID_EXT;               // 强制IDE=1（扩展帧标志，关键！）
   // 注：RTR默认0（数据帧），若需接收远程帧可改为CAN_RTR_REMOTE

   // 拆分掩码到FilterMaskIdHigh和FilterMaskIdLow（放开所有位）
   canFilter.FilterMaskIdHigh = (mask >> 13) & 0xFFFF;  // 掩码高16位
   canFilter.FilterMaskIdLow = (mask & 0x1FFF) << 3;    // 掩码低16位（左移3位）
   canFilter.FilterMaskIdLow |= CAN_ID_EXT;             // 掩码放开IDE位（只接收扩展帧，关键！）

   HAL_CAN_ConfigFilter(&hcan1, &canFilter);  // 配置过滤器

   // 启用所需的中断
   // HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY);      // 发送邮箱空中断（当发送邮箱为空时，即发送完成）
   // HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);  // FIFO0消息挂起中断（FIFO0中有新消息）
   // HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_FULL);         // FIFO0满中断
   // HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_OVERRUN);      // FIFO0溢出中断
   // HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING);  // FIFO1消息挂起中断
   // HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_FULL);         // FIFO1满中断
   // HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_OVERRUN);      // FIFO1溢出中断
   // HAL_CAN_ActivateNotification(&hcan1, CAN_IT_ERROR);                 // 错误中断

   // 开启can外设
   if (HAL_CAN_Start(&hcan1) != HAL_OK) {
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
      GPIO_InitStruct.Pin = GPIO_PIN_11;             // 配置RX引脚(PA11)
      GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;     // 设置为输入模式
      GPIO_InitStruct.Pull = GPIO_PULLUP;            // 使用上拉电阻
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 设置为高速模式
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        // 应用RX引脚配置

      GPIO_InitStruct.Pin = GPIO_PIN_12;             // 配置TX引脚(PA12)
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        // 设置为复用推挽输出
      GPIO_InitStruct.Pull = GPIO_NOPULL;            // 不使用上下拉电阻
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 设置为高速模式
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        // 应用TX引脚配置
   }
}
