// 混合架构示例：时间片轮询 + 状态机 + 事件驱动
int main(void)
{
    System_Init();
    
    while(1)
    {
        // 时间片调度周期性任务
        TimeSlice_Scheduler();
        
        // 处理事件队列
        Event_Processor();
        
        // 状态机更新
        StateMachine_Update();
    }
}
架构思路：

使用时间片轮询调度周期性任务（如电压采集、温度采集）。

使用事件驱动处理异步事件（如接收到通信命令、故障信号）。

使用状态机管理主要的工作模式（如正常模式、故障模式、休眠模式）。

假设硬件平台为STM32，使用HAL库。

步骤：
a. 定义系统状态（状态机）
b. 定义事件类型和事件队列
c. 定义任务控制块（时间片轮询）
d. 主循环中处理事件和任务调度

bms_slave/
├── bms_main.c          # 主程序文件
├── bms_main.h          # 主头文件
├── scheduler.c         # 时间片调度器
├── scheduler.h         # 调度器头文件
├── state_machine.c     # 状态机实现
├── state_machine.h     # 状态机头文件
├── event_queue.c       # 事件队列管理
├── event_queue.h       # 事件队列头文件
├── drivers/            # 驱动层
│   ├── adc_driver.c
│   ├── adc_driver.h
│   ├── can_driver.c
│   ├── can_driver.h
│   ├── gpio_driver.c
│   └── gpio_driver.h
└── application/        # 应用层
    ├── battery_monitor.c
    ├── battery_monitor.h
    ├── communication.c
    └── communication.h

延时函数只能在主循环中用，在中断中不可使用延时函数

优化spi发送逻辑，使用中断发送

使用简易任务调度器，在后台中断定时器处进行任务添加，
主函数头执行任务执行分配？


从控任务：
10ms采集一次电压
10ms采集一次温度
每采集一次更新电压和温度报警状态
采集6次电压做均值滤波
均值滤波后进行can通讯挂起发送操作
中断接收均衡控制消息

定时器分配：
TIM1：高级定时器16位
TIM2：通用定时器32位：延时时钟
TIM3：通用定时器16位：定时任务时钟
TIM4：通用定时器16位



embedded_project/
├── bsp/               # 板级支持包（引脚配置、时钟）
├── drivers/           # 硬件驱动
│   ├── spi.c/.h       # SPI驱动
│   ├── i2c.c/.h       # I2C驱动
│   └── adc.c/.h       # ADC驱动
├── middleware/        # 中间件（通信协议、算法）
│   ├── mqtt/          # MQTT协议栈
│   └── algorithm/     # 数据处理算法
├── application/       # 应用层
│   ├── main.c         # 主函数
│   ├── task1.c        # 任务1（如传感器采集）
│   └── task2.c        # 任务2（如数据上传）
└── config/            # 配置参数（宏定义、设备地址）


与ltc6820通讯的SPI外设MX配置参数：
Mode：选择Full - Duplex Master。
硬件片选。
Data Size：选择 8 - bit。
MSB First：通常选择 MSB First，符合大多数设备的数据传输习惯。
Prescaler：64，spi预分频系数
CPOL：高
CPHA：2 Edge
CRC：Disabled

与W25Q128通讯的SPI外设MX配置参数：
Mode：选择Full - Duplex Master。
硬件片选。
Data Size：选择 8 - bit。
MSB First：通常选择 MSB First，符合大多数设备的数据传输习惯。
Prescaler：16，spi预分频系数
CPOL：低
CPHA：1 Edge
CRC：Disabled

USART1串口通讯配置：
波特率：115200。
word length：8位。
parity：none。
stop bits：1位。
data direction：Rx and Tx。

I2C设置：
speed mode:	standard mode
clock speed: 100kHz
Clock No Stretch Mode：关闭。时钟无拉伸模式，当前状态为 Disabled（已禁用 ），I2C 通信里 “时钟拉伸（Clock Stretch）” 是从设备让主设备时钟暂停的机制，此模式禁用则关闭该功能。
Primary Address Length selection：7位。主地址长度选择，设为 7 - bit（7 位 ），即 I2C 从设备地址采用 7 位格式。
Dual Address Acknowledged：关闭。双地址应答，状态 Disabled（已禁用 ），若使能，从设备可响应两个不同地址。
Primary slave address：设置为0。主从设备地址，值为 0 ，即该 I2C 从设备的主地址设为 0（实际使用中需按需合理设置 ）。
General Call address detection：关闭。广播呼叫地址检测，状态 Disabled（已禁用 ），使能后从设备可响应广播地址（全设备呼叫地址）通信。


CAN总线设置：
位时序参数（Bit Timings Parameters）
	Prescaler (for Time Quantum)：分频器（用于时间量子），值为 18
	Time Quantum：时间量子，时长 500.0 纳秒
	Time Quanta in Bit Segment 1：位段 1 中的时间量子数，为 1 个时间量子
	Time Quanta in Bit Segment 2：位段 2 中的时间量子数，为 2 个时间量子
	Time for one Bit：一位的时间，为 2000 纳秒
	Baud Rate：波特率，为 500000 位 / 秒
	ReSynchronization Jump Width：重新同步跳转宽度，为 1 个时间量子
基本参数（Basic Parameters）
	Time Triggered Communication Mode：时间触发通信模式，状态为禁用
	Automatic Bus - Off Management：自动总线离线管理，状态为启用
	Automatic Wake - Up Mode：自动唤醒模式，状态为禁用
	Automatic Retransmission：自动重传，状态为启用
	Receive Fifo Locked Mode：接收 FIFO 锁定模式，状态为禁用
	Transmit Fifo Priority：发送 FIFO 优先级，状态为禁用
高级参数（Advanced Parameters）
	Test Mode：测试模式，当前为正常（Normal）


GPIO输出设置：
PC13：继电器控制端口
	GPIO输出模式。
	初始输出：低。
	模式：推挽输出。
	拉电阻：下拉电阻
	输出速度：低。
PB9：绿灯控制口
	GPIO输出模式。
	初始输出：高。
	模式：开漏输出。
	拉电阻：上拉电阻
	输出速度：高。
PB8：红灯控制口
	GPIO输出模式。
	初始输出：高。
	模式：开漏输出。
	拉电阻：上拉电阻
	输出速度：高。

TIM2设置：
Clock Source选择Internal Clock
计数器设置（Counter Settings）
	Prescaler (PSC - 16 bits value)：7200-1									预分频器（PSC - 16 位数值）
	Counter Mode：向上 														计数器模式，为向上计数（Up）
	Counter Period (AutoReload Register - 16 bits value )：1000-1			计数器周期（自动重装载寄存器 - 16 位数值 ），值为 65535
	Internal Clock Division (CKD)：无										内部时钟分频（CKD），为不分频（No Division）
	auto - reload preload：启用												自动重装载预加载
触发输出（TRGO）参数（Trigger Output (TRGO) Parameters）
	Master/Slave Mode (MSM bit)：禁用										主 / 从模式（MSM 位），为禁用（触发输入效果无延迟）[Disable (Trigger input effect not delayed)]
	Trigger Event Selection：												触发事件选择，为复位（来自 TIMx_EGR 的 UG 位）[Reset (UG bit from TIMx_EGR)]


定时器中断服务函数为：TIMx_IRQHandler 等，当发生中断的时候，程序就会执行中断服
务函数。HAL 库提供了一个定时器中断公共处理函数 HAL_TIM_IRQHandler，该函数又会调用
HAL_TIM_PeriodElapsedCallback 等一些回调函数，需要用户根据中断类型选择重定义对应的中
断回调函数来处理中断程序。
