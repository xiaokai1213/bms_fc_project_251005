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
#include "can_task.h"
#include "ltc6804_1_task.h"
// Middlewares头文件

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

   while (1) {
      ltc6804_cv();
      uint8_t cv[8];
      cv[0] = (uint8_t)(cv_h_ltc6804[0].C01V >> 8);
      cv[1] = (uint8_t)cv_h_ltc6804[0].C01V;
      cv[2] = (uint8_t)(cv_h_ltc6804[0].C02V >> 8);
      cv[3] = (uint8_t)cv_h_ltc6804[0].C02V;
      cv[4] = (uint8_t)(cv_h_ltc6804[0].C03V >> 8);
      cv[5] = (uint8_t)cv_h_ltc6804[0].C03V;
      cv[6] = (uint8_t)(cv_h_ltc6804[0].C04V >> 8);
      cv[7] = (uint8_t)cv_h_ltc6804[0].C04V;
      printf("\r\n");
      RELAY(0);
      can_tx_extid_8(0x123, cv);

      delay_ms(4000);
      RELAY(1);
      delay_ms(4000);
   }
}

/**
 * BMS从控状态机 - STM32F103
 * 文件名: bms_slave_sm.c
 * 功能: 电池管理系统从控状态机实现
 * 作者:
 * 版本: 1.0
 * 日期:
 */

/* ==================== 状态和事件定义 ==================== */

// 状态枚举定义 - 定义BMS从控可能的所有状态
typedef enum {
   STATE_INIT = 0,  // 初始化状态
   STATE_IDLE,      // 空闲状态，等待命令
   STATE_MEASURE,   // 测量状态，进行电压温度采集
   STATE_BALANCE,   // 均衡状态，进行电池均衡
   STATE_FAULT,     // 故障状态，处理系统故障
   STATE_SLEEP      // 睡眠状态，低功耗模式
} bms_state_t;

// 事件枚举定义 - 定义触发状态转换的所有事件
typedef enum {
   EVENT_POWER_ON = 0,    // 上电事件
   EVENT_MEASURE_CMD,     // 测量命令事件
   EVENT_BALANCE_CMD,     // 均衡命令事件
   EVENT_MEASURE_DONE,    // 测量完成事件
   EVENT_BALANCE_DONE,    // 均衡完成事件
   EVENT_FAULT_DETECTED,  // 故障检测事件
   EVENT_FAULT_CLEARED,   // 故障清除事件
   EVENT_ENTER_SLEEP,     // 进入睡眠事件
   EVENT_WAKE_UP          // 唤醒事件
} bms_event_t;

/* ==================== 数据结构定义 ==================== */

// BMS数据结构 - 存储电池相关数据
typedef struct {
   uint16_t cell_voltages[16];  // 16节电池电压，单位mV
   int16_t temperatures[8];     // 8个温度点，单位0.1°C
   uint8_t balance_status;      // 均衡状态，bitmap格式
   uint8_t fault_code;          // 故障代码，按位定义
} bms_data_t;

// 状态机结构 - 管理状态机运行所需的所有数据
typedef struct {
   bms_state_t current_state;   // 当前状态
   bms_data_t data;             // BMS数据
   uint32_t state_entry_time;   // 状态进入时间戳(ms)
   uint8_t measurement_count;   // 测量次数计数器
   uint32_t last_measure_time;  // 上次测量完成时间(ms)
} bms_state_machine_t;

/* ==================== 全局变量 ==================== */

// 全局状态机实例 - 整个系统只有一个状态机实例
bms_state_machine_t bms_sm;

/* ==================== 函数声明 ==================== */

// 状态机核心函数
static void bms_sm_init(void);
static void bms_sm_handle_event(bms_event_t event);
static void bms_sm_periodic_task(void);
static const char* bms_get_state_name(bms_state_t state);
static void bms_enter_state(bms_state_t new_state);

// 硬件抽象层函数
static void bms_hw_init(void);
static void bms_start_measurement(void);
static void bms_start_balancing(void);
static void bms_stop_balancing(void);
static uint8_t bms_check_faults(void);

/* ==================== 状态机核心函数实现 ==================== */

/**
 * @brief 状态机初始化函数
 * @param None
 * @return None
 * @note 在系统启动时调用，初始化所有状态机变量
 */
static void bms_sm_init(void) {
   // 初始化状态为INIT
   bms_sm.current_state = STATE_INIT;

   // 清空BMS数据区域
   memset(&bms_sm.data, 0, sizeof(bms_data_t));

   // 初始化时间戳和计数器
   bms_sm.state_entry_time = 0;
   bms_sm.measurement_count = 0;
   bms_sm.last_measure_time = 0;

   // 初始化硬件外设
   bms_hw_init();
}

/**
 * @brief 进入新状态处理函数
 * @param new_state 要进入的新状态
 * @return None
 * @note 在状态转换时调用，执行状态进入动作和更新时间戳
 */
static void bms_enter_state(bms_state_t new_state) {
   // 更新当前状态
   bms_sm.current_state = new_state;

   // 记录状态进入时间，用于超时判断
   bms_sm.state_entry_time = HAL_GetTick();

   // 执行状态进入时的特定动作
   switch (new_state) {
      case STATE_MEASURE:
         // 进入测量状态，启动ADC转换
         bms_start_measurement();
         break;

      case STATE_BALANCE:
         // 进入均衡状态，启动均衡电路
         bms_start_balancing();
         break;

      case STATE_FAULT:
         // 进入故障状态，停止所有可能危险的操作
         bms_stop_balancing();
         // 可以添加其他故障处理，如关闭充放电MOSFET等
         break;

      case STATE_SLEEP:
         // 进入睡眠状态，配置低功耗模式
         // 实际应用中这里会调用 __WFI() 进入等待中断模式
         break;

      default:
         // 其他状态不需要特殊进入动作
         break;
   }
}

/**
 * @brief 状态机事件处理函数
 * @param event 要处理的事件
 * @return None
 * @note 这是状态机的核心，根据当前状态和事件决定状态转换
 */
static void bms_sm_handle_event(bms_event_t event) {
   bms_state_t new_state = bms_sm.current_state;  // 默认保持当前状态

   // 状态转换逻辑：根据当前状态和事件决定下一个状态
   switch (bms_sm.current_state) {
      case STATE_INIT:
         // 初始化状态只响应上电事件
         if (event == EVENT_POWER_ON) {
            new_state = STATE_IDLE;
         }
         break;

      case STATE_IDLE:
         // 空闲状态可以响应多种事件
         switch (event) {
            case EVENT_MEASURE_CMD:
               new_state = STATE_MEASURE;  // 测量命令 -> 进入测量状态
               break;
            case EVENT_BALANCE_CMD:
               new_state = STATE_BALANCE;  // 均衡命令 -> 进入均衡状态
               break;
            case EVENT_ENTER_SLEEP:
               new_state = STATE_SLEEP;  // 睡眠命令 -> 进入睡眠状态
               break;
            case EVENT_FAULT_DETECTED:
               new_state = STATE_FAULT;  // 检测到故障 -> 进入故障状态
               break;
            default:
               // 其他事件在空闲状态下不引起状态变化
               break;
         }
         break;

      case STATE_MEASURE:
         if (event == EVENT_MEASURE_DONE) {
            // 测量完成 -> 返回空闲状态
            bms_sm.measurement_count++;                // 增加测量计数
            bms_sm.last_measure_time = HAL_GetTick();  // 更新最后测量时间
            new_state = STATE_IDLE;
         } else if (event == EVENT_FAULT_DETECTED) {
            // 测量过程中检测到故障 -> 进入故障状态
            new_state = STATE_FAULT;
         }
         break;

      case STATE_BALANCE:
         if (event == EVENT_BALANCE_DONE || event == EVENT_FAULT_DETECTED) {
            // 均衡完成或检测到故障 -> 停止均衡返回空闲
            new_state = STATE_IDLE;
         }
         break;

      case STATE_FAULT:
         if (event == EVENT_FAULT_CLEARED) {
            // 故障清除 -> 返回空闲状态
            new_state = STATE_IDLE;
         }
         break;

      case STATE_SLEEP:
         if (event == EVENT_WAKE_UP) {
            // 被唤醒 -> 返回空闲状态
            new_state = STATE_IDLE;
         }
         break;
   }

   // 如果状态发生变化，执行状态进入操作
   if (new_state != bms_sm.current_state) {
      bms_enter_state(new_state);
   }
}

/**
 * @brief 状态机周期性任务处理函数
 * @param None
 * @return None
 * @note 在主循环中定期调用，处理超时等周期性检查
 */
static void bms_sm_periodic_task(void) {
   uint32_t current_time = HAL_GetTick();                         // 获取当前时间
   uint32_t state_time = current_time - bms_sm.state_entry_time;  // 计算状态持续时间

   // 根据当前状态执行不同的周期性任务
   switch (bms_sm.current_state) {
      case STATE_IDLE:
         // 空闲状态下定期自动测量（每5秒一次）
         if ((current_time - bms_sm.last_measure_time) > 5000) {
            bms_sm_handle_event(EVENT_MEASURE_CMD);
         }
         // 定期检查故障（每秒一次）
         if (state_time > 1000) {
            if (bms_check_faults()) {
               bms_sm_handle_event(EVENT_FAULT_DETECTED);
            }
         }
         break;

      case STATE_MEASURE:
         // 测量超时检查（500ms超时）
         if (state_time > 500) {
            bms_sm.data.fault_code = 0x80;  // 设置测量超时故障代码
            bms_sm_handle_event(EVENT_FAULT_DETECTED);
         }
         break;

      case STATE_BALANCE:
         // 均衡超时检查（60秒超时）
         if (state_time > 60000) {
            bms_sm_handle_event(EVENT_BALANCE_DONE);
         }
         // 均衡过程中也要定期检查故障（每秒一次）
         if ((state_time % 1000) == 0) {
            if (bms_check_faults()) {
               bms_sm_handle_event(EVENT_FAULT_DETECTED);
            }
         }
         break;

      case STATE_FAULT:
         // 故障状态下定期尝试恢复（10秒后尝试清除故障）
         if (state_time > 10000) {
            if (!bms_check_faults()) {
               bms_sm_handle_event(EVENT_FAULT_CLEARED);
            }
         }
         break;

      default:
         // 其他状态不需要特殊周期性处理
         break;
   }
}

/**
 * @brief 获取状态名称字符串
 * @param state 状态枚举值
 * @return 状态名称字符串
 * @note 用于调试和日志输出
 */
static const char* bms_get_state_name(bms_state_t state) {
   // 状态名称字符串数组，与状态枚举顺序对应
   static const char* state_names[] = {"INIT", "IDLE", "MEASURE", "BALANCE", "FAULT", "SLEEP"};

   // 检查状态值是否有效
   if (state <= STATE_SLEEP) {
      return state_names[state];
   }
   return "UNKNOWN";  // 无效状态返回未知
}

/* ==================== 硬件抽象层函数实现 ==================== */

/**
 * @brief 硬件初始化函数
 * @param None
 * @return None
 * @note 初始化所有需要用到的硬件外设
 */
static void bms_hw_init(void) {
   // ADC初始化 - 用于电压和温度测量
   // RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;      // 使能ADC1时钟
   // ADC1->CR2 = ADC_CR2_ADON;                // 开启ADC

   // GPIO初始化 - 用于均衡控制和其他数字IO
   // RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;      // 使能GPIOA时钟
   // GPIOA->CRL = ...;                        // 配置GPIO模式

   // I2C初始化 - 用于与主控通信
   // RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;      // 使能I2C1时钟

   // 定时器初始化 - 用于周期性任务和时间测量
   // RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;      // 使能TIM2时钟

   // 注：以上代码需要根据实际硬件连接进行配置
}

/**
 * @brief 开始测量函数
 * @param None
 * @return None
 * @note 启动ADC转换，开始采集电压和温度数据
 */
static void bms_start_measurement(void) {
   // 启动ADC转换
   // ADC1->CR2 |= ADC_CR2_SWSTART;            // 软件触发ADC转换

   // 实际应用中这里会依次启动所有电池电压和温度通道的ADC转换
   // 转换完成后会在ADC中断中触发EVENT_MEASURE_DONE事件
}

/**
 * @brief 开始均衡函数
 * @param None
 * @return None
 * @note 根据均衡策略开启相应的均衡开关
 */
static void bms_start_balancing(void) {
   // 根据均衡策略开启相应的均衡MOSFET
   // GPIOA->BSRR = balance_mask;              // 置位相应的均衡控制位

   bms_sm.data.balance_status = 0x01;  // 标记均衡进行中
                                       // 实际均衡策略需要根据电池电压差异来制定
}

/**
 * @brief 停止均衡函数
 * @param None
 * @return None
 * @note 关闭所有均衡开关，停止均衡操作
 */
static void bms_stop_balancing(void) {
   // 关闭所有均衡MOSFET
   // GPIOA->BRR = 0xFFFF;                     // 复位所有均衡控制位

   bms_sm.data.balance_status = 0x00;  // 清除均衡状态
}

/**
 * @brief 故障检查函数
 * @return 0-无故障, 1-有故障
 * @note 检查所有可能的故障条件，包括过压、欠压、过温等
 */
static uint8_t bms_check_faults(void) {
   uint8_t fault_detected = 0;  // 故障检测标志

   // 检查过压故障 - 任何单节电池电压超过4.2V
   for (int i = 0; i < 16; i++) {
      if (bms_sm.data.cell_voltages[i] > 4200) {
         bms_sm.data.fault_code = 0x01;  // 设置过压故障代码
         fault_detected = 1;
         break;  // 发现一个过压就立即返回
      }
   }

   // 检查欠压故障 - 任何单节电池电压低于3.0V
   for (int i = 0; i < 16; i++) {
      if (bms_sm.data.cell_voltages[i] < 3000) {
         bms_sm.data.fault_code = 0x02;  // 设置欠压故障代码
         fault_detected = 1;
         break;  // 发现一个欠压就立即返回
      }
   }

   // 检查温度故障 - 任何温度点超过60°C
   for (int i = 0; i < 8; i++) {
      if (bms_sm.data.temperatures[i] > 60) {
         bms_sm.data.fault_code = 0x04;  // 设置过温故障代码
         fault_detected = 1;
         break;  // 发现一个过温就立即返回
      }
   }

   // 可以继续添加其他故障检查，如通信故障、硬件故障等

   return fault_detected;
}

/* ==================== 中断服务程序 ==================== */

/**
 * @brief 系统定时器中断服务程序
 * @param None
 * @return None
 * @note 1ms周期中断，用于HAL库的时间基准
 */
void SysTick_Handler(void) {
   HAL_IncTick();  // 递增HAL库时间计数器
}

/**
 * @brief ADC转换完成中断服务程序
 * @param None
 * @return None
 * @note ADC转换完成后触发，处理采集到的数据
 */
void ADC1_2_IRQHandler(void) {
   // 检查ADC转换完成标志
   // if(ADC1->SR & ADC_SR_EOC) {
   // 读取ADC转换结果
   // uint16_t adc_value = ADC1->DR;

   // 处理ADC数据，转换为实际电压/温度值
   // ...

   // 所有通道转换完成后触发测量完成事件
   bms_sm_handle_event(EVENT_MEASURE_DONE);
   // }
}

/**
 * @brief I2C事件中断服务程序
 * @param None
 * @return None
 * @note 处理来自主控的I2C命令
 */
void I2C1_EV_IRQHandler(void) {
   // 解析主控发送的命令
   // uint8_t command = I2C1->DR;

   // 根据命令触发相应的事件
   // switch(command) {
   //     case 0x01:
   //         bms_sm_handle_event(EVENT_MEASURE_CMD);
   //         break;
   //     case 0x02:
   //         bms_sm_handle_event(EVENT_BALANCE_CMD);
   //         break;
   //     case 0x03:
   //         bms_sm_handle_event(EVENT_ENTER_SLEEP);
   //         break;
   // }
}

/* ==================== 主程序和系统初始化 ==================== */

/**
 * @brief 主函数
 * @param None
 * @return int 程序退出码
 * @note 程序入口，初始化系统并进入主循环
 */
int main(void) {
   // HAL库初始化
   HAL_Init();

   // 系统时钟配置
   SystemClock_Config();

   // BMS状态机初始化
   bms_sm_init();

   // 触发上电事件，启动状态机
   bms_sm_handle_event(EVENT_POWER_ON);

   // 主循环
   while (1) {
      // 执行状态机周期性任务
      bms_sm_periodic_task();

      // 其他后台任务...
      // 如：数据上传、LED指示、看门狗喂狗等

      // 延时10ms，控制主循环频率
      HAL_Delay(10);
   }

   return 0;  // 正常情况下不会执行到这里
}

/**
 * @brief 系统时钟配置函数
 * @param None
 * @return None
 * @note 配置STM32F103的系统时钟，使用HSE和PLL达到72MHz
 */
void SystemClock_Config(void) {
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

   // 配置HSE（外部高速时钟）和PLL（锁相环）
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_ON;                 // 开启HSE
   RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;  // HSE预分频
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;             // 开启PLL
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;     // PLL源为HSE
   RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;             // PLL倍频x9
   HAL_RCC_OscConfig(&RCC_OscInitStruct);

   // 配置系统时钟和总线时钟
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;  // 系统时钟源为PLL
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;         // AHB不分频
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;          // APB1 2分频(36MHz)
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;          // APB2不分频(72MHz)
   HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);  // Flash等待状态
}
