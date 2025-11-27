#ifndef _CAN_TASK_H
#define _CAN_TASK_H

#include "main.h"

// can发送电压数据报文id设置
#define can_tx_id_voltage_01_to_04     (uint32_t)0x18105000
#define can_tx_id_voltage_05_to_08     (uint32_t)0x18115000
#define can_tx_id_voltage_09_to_12     (uint32_t)0x18125000
#define can_tx_id_voltage_13_to_16     (uint32_t)0x18135000
#define can_tx_id_voltage_17_to_20     (uint32_t)0x18145000

// can发送温度数据报文id设置
#define can_tx_id_temperature_01_to_04 (uint32_t)0x18106000
#define can_tx_id_temperature_05_to_08 (uint32_t)0x18116000
#define can_tx_id_temperature_09_to_10 (uint32_t)0x18126000

typedef enum {
   CAN_RESP_OK = 0,    // 发送成功
   CAN_RESP_ERROR,     // 发送错误
   CAN_RESP_BUS_BUSY,  // 发送总线忙
   CAN_RESP
} CAN_ResponseEnum_t;  // CAN响应状态枚举定义

typedef enum {
   CAN_EVENT_NONE = 0,             // 无事件
   CAN_EVENT_TX_VOLTAGE_DATA,      // 发送电压数据事件
   CAN_EVENT_TX_TEMPERATURE_DATA,  // 发送温度数据事件
   CAN_EVENT
} CAN_EventEnum_t;  // CAN事件枚举定义

typedef enum {
   CAN_STATE_IDLE = 0,             // 空闲状态
   CAN_STATE_TX_VOLTAGE_DATA,      // 发送电压数据状态
   CAN_STATE_TX_TEMPERATURE_DATA,  // 发送温度数据状态
   CAN_STATE
} CAN_StateEnum_t;  // CAN状态机状态枚举定义

// CAN发送句柄管理
typedef struct {
   uint8_t can_free_mailbox_counter;                    // can发送空邮箱计数器
   uint8_t can_send_busy : 1;                           // CAN发送邮箱忙指示,0有空邮箱,1无空邮箱
   uint8_t voltage_can_tx_01_to_04_ready_flag : 1;      // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_05_to_08_ready_flag : 1;      // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_09_to_12_ready_flag : 1;      // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_13_to_16_ready_flag : 1;      // 电压发送报文10挂起标志位
   uint8_t voltage_can_tx_17_to_20_ready_flag : 1;      // 电压发送报文10挂起标志位
   uint8_t temperature_can_tx_01_to_04_ready_flag : 1;  // 温度发送报文10挂起标志位
   uint8_t temperature_can_tx_05_to_08_ready_flag : 1;  // 温度发送报文10挂起标志位
   uint8_t temperature_can_tx_09_to_10_ready_flag : 1;  // 温度发送报文10挂起标志位
} CAN_TX_FLAG_t;

uint8_t can_tx_voltage_data(void);                        // 通过can通讯发送电压数据
uint8_t can_tx_temperature_data(void);                    // 通过can通讯发送温度数据
uint8_t can_tx_extid_8(uint32_t extid, uint8_t data[8]);  // 扩展can发送八位数据帧

#endif  // _CAN_TASK_H
