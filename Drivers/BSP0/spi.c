#include "spi.h"

SPI_HandleTypeDef hspi1;  // spi1初始化句柄
SPI_HandleTypeDef hspi2;  // spi2初始化句柄

/**
 * @brief  初始化SPI1外设
 * @param  无
 * @retval 无
 */
void SPI1_Init(void) {
   hspi1.Instance = SPI1;                                    // 指定SPI1外设实例
   hspi1.Init.Mode = SPI_MODE_MASTER;                        // 配置为SPI主机模式
   hspi1.Init.Direction = SPI_DIRECTION_2LINES;              // 采用双线全双工模式（MOSI和MISO独立）
   hspi1.Init.DataSize = SPI_DATASIZE_8BIT;                  // 数据帧长度为8位
   hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;               // 时钟极性为高电平（空闲时SCK为高）
   hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;                    // 时钟相位为第二个边沿采样（SCK第二个跳变沿读写数据）
   hspi1.Init.NSS = SPI_NSS_SOFT;                            // 片选信号NSS由软件引脚控制（输出模式）
   hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;  // 波特率分频系数为64（SPI时钟 = 外设时钟/64）
   hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;                   // 数据传输时先发送高位（MSB）
   hspi1.Init.TIMode = SPI_TIMODE_DISABLE;                   // 禁用TI模式（普通SPI模式）
   hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   // 禁用CRC校验功能
   hspi1.Init.CRCPolynomial = 10;                            // CRC多项式值（禁用时无效）

   if (HAL_SPI_Init(&hspi1) != HAL_OK)  // 初始化SPI1外设
   {
      printf("SPI1_init_error");
   }
}

/**
 * @brief  初始化SPI2外设
 * @param  无
 * @retval 无
 */
void SPI2_Init(void) {
   hspi2.Instance = SPI2;                                    // 指定SPI2外设实例
   hspi2.Init.Mode = SPI_MODE_MASTER;                        // 配置为SPI主机模式
   hspi2.Init.Direction = SPI_DIRECTION_2LINES;              // 采用双线全双工模式（MOSI和MISO独立）
   hspi2.Init.DataSize = SPI_DATASIZE_8BIT;                  // 数据帧长度为8位
   hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;                // 时钟极性为低电平（空闲时SCK为低）
   hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;                    // 时钟相位为第一个边沿采样（SCK第一个跳变沿读写数据）
   hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;                     // 片选信号NSS由硬件引脚控制（输出模式）
   hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;  // 波特率分频系数为16（SPI时钟 = 外设时钟/16）
   hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;                   // 数据传输时先发送高位（MSB）
   hspi2.Init.TIMode = SPI_TIMODE_DISABLE;                   // 禁用TI模式（普通SPI模式）
   hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   // 禁用CRC校验功能
   hspi2.Init.CRCPolynomial = 10;                            // CRC多项式值（禁用时无效）

   if (HAL_SPI_Init(&hspi2) != HAL_OK)  // 初始化SPI2外设
   {
      printf("SPI2_init_error");
   }
}

/**
 * @brief  SPI底层硬件初始化（时钟、引脚配置）
 * @param  spiHandle SPI句柄指针
 * @retval 无
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle) {
   GPIO_InitTypeDef GPIO_InitStruct = {0};  // 定义GPIO初始化结构体并清零

   if (spiHandle->Instance == SPI1)  // 判断是否为SPI1外设
   {
      /* SPI1时钟使能 */
      __HAL_RCC_SPI1_CLK_ENABLE();   // 使能SPI1外设时钟
      __HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA端口时钟

      // 引脚功能说明
      // PA5  ------> SPI1_SCK    PA5作为SPI1的时钟信号SCK
      // PA6  ------> SPI1_MISO   PA6作为SPI1的主机输入/从机输出信号MISO
      // PA7  ------> SPI1_MOSI   PA7作为SPI1的主机输出/从机输入信号MOSI

      GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;  // PA5（SCK）、PA7（MOSI）
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         // 模式为复用推挽输出（SPI外设控制引脚）
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   // 引脚速度为高速（50MHz）
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);         // 初始化GPIOA的上述引脚

      GPIO_InitStruct.Pin = GPIO_PIN_6;        // 配置PA6（MISO）
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // 模式为输入（接收SPI数据）
      GPIO_InitStruct.Pull = GPIO_NOPULL;      // 不启用上下拉电阻（由外部电路决定）
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  // 初始化GPIOA的PA6引脚

   } else if (spiHandle->Instance == SPI2)  // 判断是否为SPI2外设
   {
      /* SOI2时钟使能 */
      __HAL_RCC_SPI2_CLK_ENABLE();   // 使能SPI2外设时钟
      __HAL_RCC_GPIOB_CLK_ENABLE();  // 使能GPIOB端口时钟
      // 引脚功能说明
      // PB12  ------> SPI2_NSS   PB12作为SPI2的片选信号NSS
      // PB13  ------> SPI2_SCK   PB13作为SPI2的时钟信号SCK
      // PB14  ------> SPI2_MISO  PB14作为SPI2的主机输入/从机输出信号MISO
      // PB15  ------> SPI2_MOSI  PB15作为SPI2的主机输出/从机输入信号MOSI

      GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;  // 配置PB12（NSS）、PB13（SCK）、PB15（MOSI）
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                         // 模式为复用推挽输出（SPI外设控制引脚）
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;                   // 引脚速度为高速（50MHz）
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);                         // 初始化GPIOB的上述引脚

      GPIO_InitStruct.Pin = GPIO_PIN_14;       // 配置PB14（MISO）
      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // 模式为输入（接收SPI数据）
      GPIO_InitStruct.Pull = GPIO_NOPULL;      // 不启用上下拉电阻（由外部电路决定）
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  // 初始化GPIOB的PB14引脚
   }
}

/**
 * @brief SPI1 读写一个字节数据
 * @param txdata : 要发送的数据(1 字节)
 * @retval 接收到的数据(1 字节)
 */
uint8_t spi1_read_write_byte(uint8_t txdata) {
   uint8_t rxdata;
   HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 1000);
   return rxdata; /* 返回收到的数据 */
}
