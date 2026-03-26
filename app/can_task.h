#ifndef _CAN_TASK_H
#define _CAN_TASK_H

#include "main.h"

#define A0_Pin       GPIO_PIN_12
#define A0_GPIO_Port GPIOB
#define A1_Pin       GPIO_PIN_13
#define A1_GPIO_Port GPIOB
#define A2_Pin       GPIO_PIN_14
#define A2_GPIO_Port GPIOB
#define A3_Pin       GPIO_PIN_15
#define A3_GPIO_Port GPIOB

typedef struct {
   struct {
      uint32_t cell_vot_1_4_id;    // 电池电压1-4的CAN ID
      uint32_t cell_vot_5_8_id;    // 电池电压5-8的CAN ID
      uint32_t cell_vot_9_12_id;   // 电池电压9-12的CAN ID
      uint32_t cell_vot_13_16_id;  // 电池电压13-16的CAN ID
      uint32_t cell_vot_17_20_id;  // 电池电压17-20的CAN ID
      uint32_t aux_vot_1_4_id;     // 辅助电压1-4的CAN ID
      uint32_t aux_vot_5_8_id;     // 辅助电压5-8的CAN ID
      uint32_t aux_vot_9_12_id;    // 辅助电压9-12的CAN ID
   } TX_ID;                        // 发送ID结构体定义

   struct {
      uint32_t receive_cell_vot_can;
   } RX_ID;  // 接收ID结构体定义

} CAN_ID_HandleTypeDef;                     // CAN ID句柄结构体定义
extern CAN_ID_HandleTypeDef CAN_ID_Handle;  // CAN ID句柄

void CAN_Address_init(void);   // CAN地址初始化函数
void send_cell_vot_can(void);  // 发送电池电压CAN报文函数
void send_aux_vot_can(void);   // 发送辅助电压CAN报文函数

#endif  // _CAN_TASK_H
