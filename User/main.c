// 主头文件
#include "main.h"
// SYSTEM头文件
#include "delay.h"
#include "sys.h"
#include "usart.h"
// BSP0头文件
#include "can.h"
#include "led_relay_gpio.h"
#include "spi.h"
#include "stm32f1xx_it.h"
#include "tim.h"
// BSP1头文件
#include "ltc6804_1_task.h"
// Middlewares头文件
#include "tim_it.h"

CAN_TxHeaderTypeDef TxHeader;
uint8_t TxData[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
uint32_t TxMailbox;

void CAN_SendMessage(void) {
   TxHeader.StdId = 0x123;
   TxHeader.ExtId = 0x00;
   TxHeader.RTR = CAN_RTR_DATA;
   TxHeader.IDE = CAN_ID_STD;
   TxHeader.DLC = 8;

   TxData[0] = (uint8_t)(cv_h_ltc6804[0].C01V);
   TxData[1] = (uint8_t)(cv_h_ltc6804[0].C01V >> 8);

   if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
      printf("can_tx_error\n");
   }
}

int main(void) {
   // 外设初始化
   HAL_Init();             // HAL库初始化
   Stm32_Clock_Init();     // 时钟初始化
   uart_init(115200);      // 串口1初始化，中断收发，支持打印函数
   LED_RELAY_GPIO_Init();  // LED与继电器初始化
   TIM2_Init();            // 定时器2初始化（延时定时器）
   TIM4_Init();            // 定时器4初始化（后台定时器）
   SPI1_Init();            // spi1初始化
   CAN_Init();             // can外设初始化
   NVIC_Init();            // 中断初始化；中断统一管理

   // 外设驱动初始化
   LTC6804_init();

   // uint8_t TxData[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

   // HAL_CAN_Start(&hcan);
   // HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
   // CAN_FilterConfig();

   while (1) {
      CAN_SendMessage();

      ltc6804_cv();
      printf("\r\n");
      delay_ms(4000);
   }
}
