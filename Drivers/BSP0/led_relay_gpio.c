#include "led_relay_gpio.h"

/**
 * @brief   LED与继电器控制GPIO初始化函数
 * @param   无
 * @retval  无
 */
void LED_RELAY_GPIO_Init(void) {
   GPIO_InitTypeDef GPIO_InitStruct = {0};  // 定义GPIO初始化结构体，并将所有成员初始化为0

   // GPIO外设工作前必须先使能时钟
   __HAL_RCC_GPIOC_CLK_ENABLE();  // 使能GPIOC端口时钟
   __HAL_RCC_GPIOD_CLK_ENABLE();  // 使能GPIOD端口时钟
   __HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA端口时钟
   __HAL_RCC_GPIOB_CLK_ENABLE();  // 使能GPIOB端口时钟

   HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);  // 初始化RELAY引脚输出低电平（复位状态）

   HAL_GPIO_WritePin(GPIOB, LEDR_Pin | LEDG_Pin, GPIO_PIN_SET);  // 初始化LEDR和LEDG引脚输出高电平（置位状态）

   GPIO_InitStruct.Pin = RELAY_Pin;                   // 配置目标引脚为RELAY_Pin（继电器控制引脚）
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;        // 模式设置为推挽输出（可直接输出高低电平，驱动能力强）
   GPIO_InitStruct.Pull = GPIO_PULLDOWN;              // 下拉电阻使能（引脚空闲时默认拉低）
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;       // 引脚速度为低速（适用于对响应速度要求不高的场景）
   HAL_GPIO_Init(RELAY_GPIO_Port, &GPIO_InitStruct);  // 应用配置到RELAY所在的GPIO端口

   GPIO_InitStruct.Pin = LEDR_Pin | LEDG_Pin;     // 配置目标引脚为LEDR_Pin和LEDG_Pin（LED控制引脚）
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;    // 模式设置为开漏输出（需外部上拉电阻才能输出高电平，支持线与逻辑）
   GPIO_InitStruct.Pull = GPIO_PULLUP;            // 上拉电阻使能（引脚空闲时默认拉高）
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 引脚速度为高速（适用于对响应速度要求高的场景）
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);        // 应用配置到GPIOB端口（LEDR和LEDG所在端口）

   RELAY(1);
   LEDR(0);
   LEDG(0);
}
