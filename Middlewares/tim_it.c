#include "tim_it.h"

uint16_t see = 0;

/**
 * @brief 基本定时器周期结束回调函数,定时器周期为1ms，即1ms触发一次定时器
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
   if (htim->Instance == TIM2) {
      see++;
      if (see == 2500) {
         HAL_GPIO_TogglePin(LEDG_GPIO_Port, LEDG_Pin);
         // HAL_GPIO_TogglePin(RELAY_GPIO_Port, RELAY_Pin);
         see = 0;
      }
   }
}
