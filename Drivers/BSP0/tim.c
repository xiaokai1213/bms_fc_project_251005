#include "tim.h"

TIM_HandleTypeDef htim2;  // 定义定时器2初始化句柄
TIM_HandleTypeDef htim4;  // 定义定时器4初始化句柄

/**
 * @brief   初始化TIm2定时器
 * @param   无
 * @retval  无
 */
void TIM2_Init(void) {
   TIM_ClockConfigTypeDef sClockSourceConfig = {0};  // 定义时钟源配置结构体并初始化为0
   TIM_MasterConfigTypeDef sMasterConfig = {0};      // 定义主模式配置结构体并初始化为0

   /*
   定时器中断的频率计算公式为：
   中断频率 = 定时器时钟频率 / (预分频器 + 1) / (自动重载值 + 1)
   */
   htim2.Instance = TIM2;                                         // 设置TIM2外设实例
   htim2.Init.Prescaler = 7200 - 1;                               // 设置预分频器值
   htim2.Init.CounterMode = TIM_COUNTERMODE_UP;                   // 配置为向上计数模式
   htim2.Init.Period = 10 - 1;                                    // 设置自动重载值
   htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;             // 设置时钟分频系数为1（不分频）
   htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  // 启用自动重载预装载功能
   HAL_TIM_Base_Init(&htim2);                                     // 初始化TIM2基本定时器功能

   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;  // 配置TIM2的时钟源为内部时钟
   HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);     // 应用时钟源配置

   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;             // 配置主模式输出触发为复位事件
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;    // 禁用主从模式同步
   HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);  // 应用主模式同步配置

   HAL_TIM_Base_Start_IT(&htim2);  // 以基础中断模式开启定时器2
}

/**
 * @brief   初始化TIm4定时器
 * @param   无
 * @retval  无
 */
void TIM4_Init(void) {
   TIM_ClockConfigTypeDef sClockSourceConfig = {0};
   TIM_MasterConfigTypeDef sMasterConfig = {0};

   /*
   定时器中断的频率计算公式为：
   中断频率 = 定时器时钟频率 / (预分频器 + 1) / (自动重载值 + 1)
   */
   htim4.Instance = TIM4;                                         // 设置TIM4外设实例
   htim4.Init.Prescaler = 7200 - 1;                               // 设置预分频器值
   htim4.Init.CounterMode = TIM_COUNTERMODE_UP;                   // 配置为向上计数模式
   htim4.Init.Period = 10 - 1;                                    // 设置自动重载值
   htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;             // 设置时钟分频系数为1（不分频）
   htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  // 启用自动重载预装载功能
   HAL_TIM_Base_Init(&htim4);                                     // 初始化TIM4基本定时器功能

   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;  // 配置TIM2的时钟源为内部时钟
   HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);     // 应用时钟源配置

   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;             // 配置主模式输出触发为复位事件
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;    // 禁用主从模式同步
   HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);  // 应用主模式同步配置

   HAL_TIM_Base_Start_IT(&htim4);  // 以基础中断模式开启定时器4
}

/**
 * @brief   TIM2底层硬件初始化函数,被HAL_TIM_Base_Init调用
 * @param   tim_baseHandle TIM句柄指针
 * @retval  无
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle) {
   if (tim_baseHandle->Instance == TIM2) {         // 判断是否为TIM2外设
      __HAL_RCC_TIM2_CLK_ENABLE();                 // 使能TIM2外设时钟
   } else if (tim_baseHandle->Instance == TIM4) {  // 判断是否为TIM4外设
      __HAL_RCC_TIM4_CLK_ENABLE();                 // 使能TIM4外设时钟
   }
}
