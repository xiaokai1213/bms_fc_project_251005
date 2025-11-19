# Copilot 使用说明 — bms_fc_project_251005

下面是能让 AI 编码代理（Copilot / GPT）快速上手本仓库的关键信息、约定和可操作指引。请仅根据仓库中可发现的内容作出改变；不要引入与本仓库工具链不一致的新流程。

## 要旨（Big picture）
- **嵌入式 STM32F103 项目**：基于 STM32F1 系列，使用 ST HAL 驱动（见 `Drivers/STM32F1xx_HAL_Driver/`）。
- **工程文件**：Keil µVision 工程文件位于根目录（例如 `Template_project.uvprojx` / `.uvprojx`），常用 IDE 为 Keil µVision，项目没有显式的 cross-platform build script 或 CI。
- **主要模块边界**：
  - 硬件抽象与外设驱动：`Drivers/`（HAL + BSP0/BSP1）
  - 业务逻辑任务：`User/`（例如 `can_task.c`、`ltc6804_1_task.c`）
  - 编译产物 / 中间文件：`Output/`（包含 `.crf`, `.d` 等——这些是构建输出/依赖相关文件，不应当编辑）

## 入口与初始化顺序（快速参考）
- 程序入口：`User/main.c`。
- `main()` 中的典型初始化顺序（请保持或谨慎修改，因为模块间依赖明显）：
  - `HAL_Init()`
  - `Stm32_Clock_Init()`
  - `uart_init(115200)` （串口用于调试输出）
  - `LED_RELAY_GPIO_Init()`
  - `TIM2_Init()` / `TIM4_Init()` （定时器分别用于延时与后台时钟）
  - `SPI1_Init()` / `CAN_Init()` / `NVIC_Init()`
  - `LTC6804_init()`（电池监测芯片初始化）

示例：`User/main.c` 展示了完整的初始化序列，修改驱动或任务时注意依赖顺序。

## 设备和外设约定（从 `readme.txt` 可发现）
- SPI（LTC6820、W25Q128）有不同的预分频与极性：不要随意统一 SPI 配置，优先查找并复用对应驱动文件（`spi.c` / `spi.h`）。
- CAN 波特率与位时序有明确设定（readme 中列出参数，实际初始化在 `can.c` / `can.h`）。
- GPIO 用途示例：`PC13` 控制继电器，`PB8/PB9` 控制指示灯（详见 `readme.txt`）。

## 代码组织与约定
- 驱动与 HAL 位于 `Drivers/`（不要在 `Drivers/STM32F1xx_HAL_Driver/` 中改造 HAL 源文件，除非确知需要）
- 板级支持包按 `BSP0/` 和 `BSP1/` 分组，任务以 `_task` 后缀命名（例如 `can_task.c`, `ltc6804_1_task.c`）。
- 中断相关文件：`stm32f1xx_it.*` 与 `stm32f1xx_hal_*` 实现中断处理与回调，回调函数名遵循 HAL 约定（如 `HAL_TIM_PeriodElapsedCallback`）。

## 构建与调试工作流（可操作说明）
- 首选工具：**Keil µVision** 打开 `Template_project.uvprojx` 并使用 IDE 的 Build/Debug。仓库内存在 `DebugConfig/*.dbgconf`，可在 IDE 中导入或直接使用作为调试配置。
- 本仓库未发现 CI 构建脚本或 makefile；如需要自动化构建，请与仓库维护者确认 target 工具链与许可证。
- 输出文件位于 `Output/`，包含依赖（`.d`）和其他构建中间产物（`.crf`），这些不应作为源代码编辑对象。

## 代码修改建议（AI 代理行动指南）
- 在修改任何硬件驱动或时序敏感代码前，先阅读 `readme.txt` 中对定时器与外设的配置说明（例如 TIM 分配、SPI/USART/CAN 参数）。
- 修改外围设备初始化时，保持 `main.c` 中的调用顺序不变，或在 PR 中明确说明改变顺序的原因与影响范围。
- 若要添加新任务：在 `User/` 下新增 `_task.c`/`_task.h`，并在 `main.c` 中的初始化序列中注册或初始化（按现有模式）。
- 避免直接编辑 `Output/` 下文件或 `Template_project.*.uvprojx` 之外的工程元文件，除非你要变更项目配置并已测试。

## 常见查找小贴士（示例命令 / 搜索路径）
- 查找入口与任务：`User/main.c`, `User/*.c`。
- 查找 HAL 和 BSP：`Drivers/STM32F1xx_HAL_Driver/`, `Drivers/BSP0/`, `Drivers/BSP1/`。
- 构建/调试配置：`Template_project.uvprojx`, `DebugConfig/*.dbgconf`。

## 不要做的事情（仓库敏感点）
- 不要删除或编辑 `Output/` 中生成的文件。
- 在不了解硬件影响的情况下，不要随意修改定时器 / CAN / SPI 波特/极性等配置。

## 后续迭代与反馈
- 我已把这些信息写入此文件；请告诉我：
  - 是否希望补充更多文件级别的示例（如 `spi.c` / `can.c` 的具体片段）？
  - 是否需要把“如何在命令行无 Keil 时构建”加入说明（这需要你确认可用的交叉编译工具链）？

—— 结束

## 具体驱动配置示例（摘录）
下面为代码中可直接参考的关键配置片段与使用约定，已从 `Drivers/BSP0/spi.c`、`Drivers/BSP0/can.c`、`Drivers/BSP1/can_task.c` 摘录：

- SPI1（用于 LTC68xx 系列） — `Drivers/BSP0/spi.c`：
  - 配置要点：`Mode = SPI_MODE_MASTER`, `DataSize = 8bit`, `CLKPolarity = SPI_POLARITY_HIGH`, `CLKPhase = SPI_PHASE_2EDGE`, `BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64`, `FirstBit = MSB`。
  - 引脚：`PA5` = SCK, `PA6` = MISO, `PA7` = MOSI（在 `HAL_SPI_MspInit` 中初始化）。
  - 交互模式：文件提供了中断方式发送/接收接口 `spi1_tx_it()` / `spi1_rx_it()`，它们调用 `HAL_SPI_Transmit_IT` / `HAL_SPI_Receive_IT` 并在本地标志 `spi1_tx_it_end_sign`/`spi1_rx_it_end_sign` 上等待完成。

- SPI2（用于 W25Q128 等） — `Drivers/BSP0/spi.c`：
  - 配置要点：`CLKPolarity = SPI_POLARITY_LOW`, `CLKPhase = SPI_PHASE_1EDGE`, `BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16`, `NSS = SPI_NSS_HARD_OUTPUT`。
  - 引脚：`PB12` = NSS, `PB13` = SCK, `PB14` = MISO, `PB15` = MOSI。
  - 说明：不同外设使用不同 CPOL/CPHA/Prescaler，切勿盲目统一 SPI 配置；按驱动内约定调用对应 SPI 接口。

- CAN1 初始化与过滤（`Drivers/BSP0/can.c`）：
  - 波特/位时序（文件内设置）：`Prescaler = 9`, `SyncJumpWidth = CAN_SJW_1TQ`, `TimeSeg1 = CAN_BS1_5TQ`, `TimeSeg2 = CAN_BS2_2TQ`。
  - 行为：`Mode = CAN_MODE_NORMAL`, `AutoBusOff = ENABLE`, `AutoWakeUp = ENABLE`, `AutoRetransmission = ENABLE`。
  - 滤波器示例：FilterBank=0, FilterMode=IDMASK, FilterScale=32bit, Mask=0x00000000 → 当前配置接收“所有扩展帧”。注意代码中将 `FilterIdLow |= CAN_ID_EXT` 与 `FilterMaskIdLow |= CAN_ID_EXT`，这会限定为扩展帧（IDE=1）；如需接收标准帧（11 位），需修改相应 FilterIdLow/MaskLow。
  - 引脚：`PA11` = CAN_RX, `PA12` = CAN_TX（在 `HAL_CAN_MspInit` 中配置）。
  - 中断：文件中 `HAL_CAN_ActivateNotification(...)` 的多行调用被注释掉 —— 如果要用中断驱动接收/错误处理，请启用并在 `stm32f1xx_it.c` / 回调中实现处理逻辑。

- CAN 发送示例（`Drivers/BSP1/can_task.c`）：
  - 函数 `can_tx_extid_8(uint32_t extid, uint8_t data[8])`：构造 `CAN_TxHeaderTypeDef`，设置 `IDE = CAN_ID_EXT`, `RTR = CAN_RTR_DATA`, `DLC = 8`，调用 `HAL_CAN_AddTxMessage(&hcan1, &TxHeader, txdata, &TxMailbox)`。
  - 使用约定：发送扩展帧时请使用 `can_tx_extid_8`；检查 `HAL_CAN_AddTxMessage` 返回值以确认是否上报错误。

注意要点：
- SPI 的不同设备使用不同的时序与片选方法（软件/硬件），修改或新增 SPI 外设时务必复用已有 `SPIx_Init`/`HAL_SPI_MspInit` 约定。
- 当前 CAN 滤波器配置默认只接收扩展帧，且中断未启用；若新增功能依赖标准帧或中断，请在 `can.c` 中调整滤波器并启用 `HAL_CAN_ActivateNotification`。

将这些摘录作为“快速参考”添加到指南可以帮助 AI 代理在修改驱动或编写任务时避免常见误用。
