#include "stm32f1xx_it.h"
#include "can.h"
#include "spi.h"
#include "tim.h"

// volatile避免编译器优化(在中断服务程序中使用)

void NVIC_Init(void) {
   // 设置优先级分组为Group2 (2位抢占, 2位响应)
   HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

   // 设置滴答定时器中断优先级（最高）
   HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(SysTick_IRQn);  // 使能滴答定时器中断

   // 设置CAN中断优先级 (最高)
   HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 1, 1);  // 抢占优先级0, 响应优先级0
   HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
   HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);  // 抢占优先级0, 响应优先级0
   HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
   HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);  // 抢占优先级0, 响应优先级0
   HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);

   // 设置SPI中断优先级
   HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);  // 抢占优先级1, 响应优先级0
   HAL_NVIC_EnableIRQ(SPI1_IRQn);

   // 设置定时器中断优先级
   HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);  // 延时定时器 - 抢占优先级2, 响应优先级0
   HAL_NVIC_EnableIRQ(TIM2_IRQn);

   HAL_NVIC_SetPriority(TIM4_IRQn, 2, 1);  // 后台定时器 - 抢占优先级2, 响应优先级1
   HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

/**
 * @brief 系统滴答定时器中断函数
 */
void SysTick_Handler(void) {
   HAL_IncTick();
   sys_time++;  // 系统时间变量自增1ms
}

/**
 * @brief   can发送中断函数入口
 */
void USB_HP_CAN1_TX_IRQHandler(void) {
   HAL_CAN_IRQHandler(&hcan1);  // can中断处理公共函数
}

/**
 * @brief can接收邮箱0中断函数入口
 */
void USB_LP_CAN1_RX0_IRQHandler(void) {
   HAL_CAN_IRQHandler(&hcan1);  // can中断处理公共函数
}

/**
 * @brief can接收邮箱1中断函数入口
 */
void CAN1_RX1_IRQHandler(void) {
   HAL_CAN_IRQHandler(&hcan1);  // can中断处理公共函数
}

/**
 * @brief 定时器2中断响应函数（延时定时器）
 */
void TIM2_IRQHandler(void) {
   HAL_TIM_IRQHandler(&htim2);  // hal库定时器中断公共处理函数
}

/**
 * @brief 定时器4中断响应函数（后台定时器）
 */
void TIM4_IRQHandler(void) {
   HAL_TIM_IRQHandler(&htim4);  // hal库定时器中断公共处理函数
}

/**
 * @brief SPI1通讯外设中断响应函数(与ltc6804通讯,硬件连接ltc6820转换成isoSPI)
 */
void SPI1_IRQHandler(void) {
   HAL_SPI_IRQHandler(&hspi1);
}

/**
 * @brief SPI2通讯外设中断响应函数
 */
void SPI2_IRQHandler(void) {
   HAL_SPI_IRQHandler(&hspi2);
}
