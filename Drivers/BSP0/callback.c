#include "callback.h"
#include "delay.h"
#include "spi.h"
#include "tim.h"

/**
 * @brief 基本定时器周期结束回调函数
 * @param 定时器句柄，传入一个定时器
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
   if (htim->Instance == TIM2) {  // 定时器2，延时定时器,1ms触发一次
      if (delaytime != 0) {       // 判断定时器计数器是否为零
         delaytime--;             // 若不为零则递减
      }
   }

   if (htim->Instance == TIM4) {
      // 定时器4，后台定时器,1ms触发一次
   }
}

/**
 * @brief SPI发送完成回调函数
 * @param hspi: SPI句柄指针
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
   if (hspi->Instance == SPI1) {
      spi1_tx_it_end_sign = 1;  // 设置发送完成标志
   }
}

/**
 * @brief spi接收完成回调函数
 * @param hspi：spi句柄
 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi) {
   if (hspi->Instance == SPI1) {
      spi1_rx_it_end_sign = 1;  // 设置接收完成标志
   }
}
