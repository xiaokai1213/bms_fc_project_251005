
500ms采集一次电压-->
	发送电压标志位置1
	电压阈值检测位置1
		检查发送邮箱是否有空为
			有就发送,并将发送标志位置0
			无则循环


定时器分配：
TIM1：高级定时器16位
TIM2：通用定时器32位：延时时钟
TIM3：通用定时器16位：定时任务时钟
TIM4：通用定时器16位


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
