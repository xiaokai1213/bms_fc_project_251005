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

   // 配置地址输入GPIO口
   GPIO_InitStruct.Pin = A0_Pin | A1_Pin | A2_Pin | A3_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief   读取地址函数
 * @return  uint8_t address:地址,后四位有效
 */
uint8_t read_address_init(void) {
   uint8_t address = 0;  // 定义地址
                         // 读取第一个GPIO引脚状态
   if (HAL_GPIO_ReadPin(A0_GPIO_Port, A0_Pin) == GPIO_PIN_SET) {
      address |= 0x01;  // 设置bit0
   }

   // 读取第二个GPIO引脚状态
   if (HAL_GPIO_ReadPin(A1_GPIO_Port, A1_Pin) == GPIO_PIN_SET) {
      address |= 0x02;  // 设置bit1
   }

   // 读取第三个GPIO引脚状态
   if (HAL_GPIO_ReadPin(A2_GPIO_Port, A2_Pin) == GPIO_PIN_SET) {
      address |= 0x04;  // 设置bit2
   }

   // 读取第四个GPIO引脚状态
   if (HAL_GPIO_ReadPin(A3_GPIO_Port, A3_Pin) == GPIO_PIN_SET) {
      address |= 0x08;  // 设置bit3
   }

   // 确保只保留低4位
   address &= 0x0F;

   return address;  // 返回地址数据
}
