#ifndef _LTC6804_1_BASE_DRIVE_H
#define _LTC6804_1_BASE_DRIVE_H

#include "delay.h"
#include "main.h"
#include "spi.h"

// 电池电压寄存器选择
#define reg_cv_all 0
#define reg_cv_a   1
#define reg_cv_b   2
#define reg_cv_c   3
#define reg_cv_d   4

// 辅助寄存器选择
#define reg_ax_all 0
#define reg_ax_a   1
#define reg_ax_b   2

// 配置寄存器组结构体，使用位域来精准对应各个寄存器的每一位含义
// 联合体中数据是共享相同的存储空间，各种变量名都可以同时使用，操作也是共同生效。
typedef struct {
   // CFGR0 寄存器
   union {
      struct {
         uint8_t ADCOPT : 1;  // ADC模式选项位，0 -> 利用ADC转换命令中的MD[1:0]位选择27kHz、7kHz或26Hz模式；1 -> 利用ADC转换命令中的MD[1:0]位选择14kHz、3kHz或2kHz模式
         uint8_t SWTRD : 1;   // SWTEN引脚状态(只读数)，1 -> SWTEN引脚处于逻辑“1”；0 -> SWTEN引脚处于逻辑“0”
         uint8_t REFON : 1;   // 基准上电位，1 -> 基准保持上电状态直到看门狗超时时为止；0 -> 基准在转换操作之后关断
         uint8_t GPIO1 : 1;   // GPIO1引脚控制，写：0 -> GPIO1引脚下拉电路导通；1 -> GPIO1引脚下拉电路关断；读：0 -> GPIO1引脚处于逻辑“0”；1 -> GPIO1引脚处于逻辑“1”
         uint8_t GPIO2 : 1;   // GPIO2引脚控制，写：0 -> GPIO2引脚下拉电路导通；1 -> GPIO2引脚下拉电路关断；读：0 -> GPIO2引脚处于逻辑“0”；1 -> GPIO2引脚处于逻辑“1”
         uint8_t GPIO3 : 1;   // GPIO3引脚控制，写：0 -> GPIO3引脚下拉电路导通；1 -> GPIO3引脚下拉电路关断；读：0 -> GPIO3引脚处于逻辑“0”；1 -> GPIO3引脚处于逻辑“1”
         uint8_t GPIO4 : 1;   // GPIO4引脚控制，写：0 -> GPIO4引脚下拉电路导通；1 -> GPIO4引脚下拉电路关断；读：0 -> GPIO4引脚处于逻辑“0”；1 -> GPIO4引脚处于逻辑“1”
         uint8_t GPIO5 : 1;   // GPIO5引脚控制，写：0 -> GPIO5引脚下拉电路导通；1 -> GPIO5引脚下拉电路关断；读：0 -> GPIO5引脚处于逻辑“0”；1 -> GPIO5引脚处于逻辑“1”
      } bits;
      uint8_t reg_val;
   } CFGR0;

   // CFGR1 寄存器
   union {
      struct {
         uint8_t VUV0 : 1;  // 欠压比较电压0相关位
         uint8_t VUV1 : 1;  // 欠压比较电压1相关位
         uint8_t VUV2 : 1;  // 欠压比较电压2相关位
         uint8_t VUV3 : 1;  // 欠压比较电压3相关位
         uint8_t VUV4 : 1;  // 欠压比较电压4相关位
         uint8_t VUV5 : 1;  // 欠压比较电压5相关位
         uint8_t VUV6 : 1;  // 欠压比较电压6相关位
         uint8_t VUV7 : 1;  // 欠压比较电压7相关位
      } bits;
      uint8_t reg_val;
   } CFGR1;

   // CFGR2 寄存器
   union {
      struct {
         uint8_t VUV8 : 1;   // 欠压比较电压8相关位
         uint8_t VUV9 : 1;   // 欠压比较电压9相关位
         uint8_t VUV10 : 1;  // 欠压比较电压10相关位
         uint8_t VUV11 : 1;  // 欠压比较电压11相关位
         uint8_t VOV0 : 1;   // 过压比较电压0相关位
         uint8_t VOV1 : 1;   // 过压比较电压1相关位
         uint8_t VOV2 : 1;   // 过压比较电压2相关位
         uint8_t VOV3 : 1;   // 过压比较电压3相关位
      } bits;
      uint8_t reg_val;
   } CFGR2;

   // CFGR3 寄存器
   union {
      struct {
         uint8_t VOV4 : 1;   // 过压比较电压4相关位
         uint8_t VOV5 : 1;   // 过压比较电压5相关位
         uint8_t VOV6 : 1;   // 过压比较电压6相关位
         uint8_t VOV7 : 1;   // 过压比较电压7相关位
         uint8_t VOV8 : 1;   // 过压比较电压8相关位
         uint8_t VOV9 : 1;   // 过压比较电压9相关位
         uint8_t VOV10 : 1;  // 过压比较电压10相关位
         uint8_t VOV11 : 1;  // 过压比较电压11相关位
      } bits;
      uint8_t reg_val;
   } CFGR3;

   // CFGR4 寄存器
   union {
      struct {
         uint8_t DCC1 : 1;  // 对电池1 进行放电控制，1 -> 接通用于电池1 的短路开关；0 -> 断开用于电池1 的短路开关(默认)
         uint8_t DCC2 : 1;  // 对电池2 进行放电控制，1 -> 接通用于电池2 的短路开关；0 -> 断开用于电池2 的短路开关(默认)
         uint8_t DCC3 : 1;  // 对电池3 进行放电控制，1 -> 接通用于电池3 的短路开关；0 -> 断开用于电池3 的短路开关(默认)
         uint8_t DCC4 : 1;  // 对电池4 进行放电控制，1 -> 接通用于电池4 的短路开关；0 -> 断开用于电池4 的短路开关(默认)
         uint8_t DCC5 : 1;  // 对电池5 进行放电控制，1 -> 接通用于电池5 的短路开关；0 -> 断开用于电池5 的短路开关(默认)
         uint8_t DCC6 : 1;  // 对电池6 进行放电控制，1 -> 接通用于电池6 的短路开关；0 -> 断开用于电池6 的短路开关(默认)
         uint8_t DCC7 : 1;  // 对电池7 进行放电控制，1 -> 接通用于电池7 的短路开关；0 -> 断开用于电池7 的短路开关(默认)
         uint8_t DCC8 : 1;  // 对电池8 进行放电控制，1 -> 接通用于电池8 的短路开关；0 -> 断开用于电池8 的短路开关(默认)
      } bits;
      uint8_t reg_val;
   } CFGR4;

   // CFGR5 寄存器
   union {
      struct {
         uint8_t DCC9 : 1;   // 对电池9 进行放电控制，1 -> 接通用于电池9 的短路开关；0 -> 断开用于电池9 的短路开关(默认)
         uint8_t DCC10 : 1;  // 对电池10进行放电控制，1 -> 接通用于电池10的短路开关；0 -> 断开用于电池10的短路开关(默认)
         uint8_t DCC11 : 1;  // 对电池11进行放电控制，1 -> 接通用于电池11的短路开关；0 -> 断开用于电池11的短路开关(默认)
         uint8_t DCC12 : 1;  // 对电池12进行放电控制，1 -> 接通用于电池12的短路开关；0 -> 断开用于电池12的短路开关(默认)
         uint8_t DCTO0 : 1;  // 放电超时值0相关位
         uint8_t DCTO1 : 1;  // 放电超时值1相关位
         uint8_t DCTO2 : 1;  // 放电超时值2相关位
         uint8_t DCTO3 : 1;  // 放电超时值3相关位
      } bits;
      uint8_t reg_val;
   } CFGR5;
} LTC6804_ConfigRegsTypeDef;

// 状态寄存器组 A 相关结构体
typedef struct {
   // STAR0 寄存器（RD）
   union {
      struct {
         uint8_t SOC0 : 1;  // 第 0 位：SOC[0]
         uint8_t SOC1 : 1;  // 第 1 位：SOC[1]
         uint8_t SOC2 : 1;  // 第 2 位：SOC[2]
         uint8_t SOC3 : 1;  // 第 3 位：SOC[3]
         uint8_t SOC4 : 1;  // 第 4 位：SOC[4]
         uint8_t SOC5 : 1;  // 第 5 位：SOC[5]
         uint8_t SOC6 : 1;  // 第 6 位：SOC[6]
         uint8_t SOC7 : 1;  // 第 7 位：SOC[7]
      } bits;
      uint8_t reg_val;
   } STAR0;

   // STAR1 寄存器（RD）
   union {
      struct {
         uint8_t SOC8 : 1;   // 第 0 位：SOC[8]
         uint8_t SOC9 : 1;   // 第 1 位：SOC[9]
         uint8_t SOC10 : 1;  // 第 2 位：SOC[10]
         uint8_t SOC11 : 1;  // 第 3 位：SOC[11]
         uint8_t SOC12 : 1;  // 第 4 位：SOC[12]
         uint8_t SOC13 : 1;  // 第 5 位：SOC[13]
         uint8_t SOC14 : 1;  // 第 6 位：SOC[14]
         uint8_t SOC15 : 1;  // 第 7 位：SOC[15]
      } bits;
      uint8_t reg_val;
   } STAR1;

   // STAR2 寄存器（RD）
   union {
      struct {
         uint8_t ITMP0 : 1;  // 第 0 位：ITMP[0]
         uint8_t ITMP1 : 1;  // 第 1 位：ITMP[1]
         uint8_t ITMP2 : 1;  // 第 2 位：ITMP[2]
         uint8_t ITMP3 : 1;  // 第 3 位：ITMP[3]
         uint8_t ITMP4 : 1;  // 第 4 位：ITMP[4]
         uint8_t ITMP5 : 1;  // 第 5 位：ITMP[5]
         uint8_t ITMP6 : 1;  // 第 6 位：ITMP[6]
         uint8_t ITMP7 : 1;  // 第 7 位：ITMP[7]
      } bits;
      uint8_t reg_val;
   } STAR2;

   // STAR3 寄存器（RD）
   union {
      struct {
         uint8_t ITMP8 : 1;   // 第 0 位：ITMP[8]
         uint8_t ITMP9 : 1;   // 第 1 位：ITMP[9]
         uint8_t ITMP10 : 1;  // 第 2 位：ITMP[10]
         uint8_t ITMP11 : 1;  // 第 3 位：ITMP[11]
         uint8_t ITMP12 : 1;  // 第 4 位：ITMP[12]
         uint8_t ITMP13 : 1;  // 第 5 位：ITMP[13]
         uint8_t ITMP14 : 1;  // 第 6 位：ITMP[14]
         uint8_t ITMP15 : 1;  // 第 7 位：ITMP[15]
      } bits;
      uint8_t reg_val;
   } STAR3;

   // STAR4 寄存器（RD）
   union {
      struct {
         uint8_t VA0 : 1;  // 第 0 位：VA[0]
         uint8_t VA1 : 1;  // 第 1 位：VA[1]
         uint8_t VA2 : 1;  // 第 2 位：VA[2]
         uint8_t VA3 : 1;  // 第 3 位：VA[3]
         uint8_t VA4 : 1;  // 第 4 位：VA[4]
         uint8_t VA5 : 1;  // 第 5 位：VA[5]
         uint8_t VA6 : 1;  // 第 6 位：VA[6]
         uint8_t VA7 : 1;  // 第 7 位：VA[7]
      } bits;
      uint8_t reg_val;
   } STAR4;

   // STAR5 寄存器（RD）
   union {
      struct {
         uint8_t VA8 : 1;   // 第 0 位：VA[8]
         uint8_t VA9 : 1;   // 第 1 位：VA[9]
         uint8_t VA10 : 1;  // 第 2 位：VA[10]
         uint8_t VA11 : 1;  // 第 3 位：VA[11]
         uint8_t VA12 : 1;  // 第 4 位：VA[12]
         uint8_t VA13 : 1;  // 第 5 位：VA[13]
         uint8_t VA14 : 1;  // 第 6 位：VA[14]
         uint8_t VA15 : 1;  // 第 7 位：VA[15]
      } bits;
      uint8_t reg_val;
   } STAR5;
} LTC6804_StatusRegsGroupA;

// 状态寄存器组 B 相关结构体
typedef struct {
   // STBR0 寄存器（RD）
   union {
      struct {
         uint8_t VD0 : 1;  // 第 0 位：VD[0]
         uint8_t VD1 : 1;  // 第 1 位：VD[1]
         uint8_t VD2 : 1;  // 第 2 位：VD[2]
         uint8_t VD3 : 1;  // 第 3 位：VD[3]
         uint8_t VD4 : 1;  // 第 4 位：VD[4]
         uint8_t VD5 : 1;  // 第 5 位：VD[5]
         uint8_t VD6 : 1;  // 第 6 位：VD[6]
         uint8_t VD7 : 1;  // 第 7 位：VD[7]
      } bits;
      uint8_t reg_val;
   } STBR0;

   // STBR1 寄存器（RD）
   union {
      struct {
         uint8_t VD8 : 1;   // 第 0 位：VD[8]
         uint8_t VD9 : 1;   // 第 1 位：VD[9]
         uint8_t VD10 : 1;  // 第 2 位：VD[10]
         uint8_t VD11 : 1;  // 第 3 位：VD[11]
         uint8_t VD12 : 1;  // 第 4 位：VD[12]
         uint8_t VD13 : 1;  // 第 5 位：VD[13]
         uint8_t VD14 : 1;  // 第 6 位：VD[14]
         uint8_t VD15 : 1;  // 第 7 位：VD[15]
      } bits;
      uint8_t reg_val;
   } STBR1;

   // STBR2 寄存器（RD）
   union {
      struct {
         uint8_t C1UV : 1;  // 第 0 位：C1UV
         uint8_t C10V : 1;  // 第 1 位：C10V
         uint8_t C2UV : 1;  // 第 2 位：C2UV
         uint8_t C20V : 1;  // 第 3 位：C20V
         uint8_t C3UV : 1;  // 第 4 位：C3UV
         uint8_t C30V : 1;  // 第 5 位：C30V
         uint8_t C4UV : 1;  // 第 6 位：C4UV
         uint8_t C40V : 1;  // 第 7 位：C40V
      } bits;
      uint8_t reg_val;
   } STBR2;

   // STBR3 寄存器（RD）
   union {
      struct {
         uint8_t C5UV : 1;  // 第 0 位：C5UV
         uint8_t C50V : 1;  // 第 1 位：C50V
         uint8_t C6UV : 1;  // 第 2 位：C6UV
         uint8_t C60V : 1;  // 第 3 位：C60V
         uint8_t C7UV : 1;  // 第 4 位：C7UV
         uint8_t C70V : 1;  // 第 5 位：C70V
         uint8_t C8UV : 1;  // 第 6 位：C8UV
         uint8_t C80V : 1;  // 第 7 位：C80V
      } bits;
      uint8_t reg_val;
   } STBR3;

   // STBR4 寄存器（RD）
   union {
      struct {
         uint8_t C9UV : 1;   // 第 0 位：C9UV
         uint8_t C90V : 1;   // 第 1 位：C90V
         uint8_t C10UV : 1;  // 第 2 位：C10UV
         uint8_t C100V : 1;  // 第 3 位：C100V
         uint8_t C11UV : 1;  // 第 4 位：C11UV
         uint8_t C110V : 1;  // 第 5 位：C110V
         uint8_t C12UV : 1;  // 第 6 位：C12UV
         uint8_t C120V : 1;  // 第 7 位：C120V
      } bits;
      uint8_t reg_val;
   } STBR4;

   // STBR5 寄存器（RD）
   union {
      struct {
         uint8_t THSD : 1;     // 第 0 位：THSD
         uint8_t MUXFAIL : 1;  // 第 1 位：MUXFAIL
         uint8_t RSVD0 : 1;    // 第 2 位：RSVD（保留位，回读值始终为0）
         uint8_t RSVD1 : 1;    // 第 3 位：RSVD（保留位，回读值始终为0）
         uint8_t REV0 : 1;     // 第 4 位：REV[0]
         uint8_t REV1 : 1;     // 第 5 位：REV[1]
         uint8_t REV2 : 1;     // 第 6 位：REV[2]
         uint8_t REV3 : 1;     // 第 7 位：REV[3]
      } bits;
      uint8_t reg_val;
   } STBR5;
} LTC6804_StatusRegsGroupB;

// 如果需要把两组状态寄存器放到一个大的结构体里统一管理，可再封装一层
typedef struct {
   LTC6804_StatusRegsGroupA groupA;  // 状态寄存器组A
   LTC6804_StatusRegsGroupB groupB;  // 状态寄存器组B
} LTC6804_StatusRegs;

typedef struct {
   uint16_t C01V;
   uint16_t C02V;
   uint16_t C03V;
   uint16_t C04V;
   uint16_t C05V;
   uint16_t C06V;
   uint16_t C07V;
   uint16_t C08V;
   uint16_t C09V;
   uint16_t C10V;
   uint16_t C11V;
   uint16_t C12V;
} BatteryVoltageRegs;  // 电池电压句柄

typedef struct {
   uint16_t G1V;
   uint16_t G2V;
   uint16_t G3V;
   uint16_t G4V;
   uint16_t G5V;
   uint16_t REF;
} AuxVoltageRegs;  // 辅助寄存器句柄

void write_LTC6804_Config(uint8_t total_ic, LTC6804_ConfigRegsTypeDef config_structs[]);    // 写配置寄存器
uint8_t read_LTC6804_Config(uint8_t total_ic, LTC6804_ConfigRegsTypeDef r_config[]);        // 读配置寄存器
uint8_t read_LTC6804_status_registers(uint8_t total_ic, LTC6804_StatusRegs status_regs[]);  // 读状态寄存器
uint8_t read_LTC6804_Battery_voltage_registers(uint8_t total_ic, BatteryVoltageRegs cv[]);  // 读电池电压寄存器
uint8_t read_LTC6804_Auxiliary_voltage_registers(uint8_t total_ic, AuxVoltageRegs av[]);    // 读辅助寄存器

// ADC 模式
#define MD_FAST      1  // 宏定义：ADC快速转换模式
#define MD_NORMAL    2  // 宏定义：ADC正常转换模式
#define MD_FILTERED  3  // 宏定义：ADC滤波转换模式

// 放电使能
#define DCP_DISABLED 0  // 宏定义：禁用放电（转换期间不允许放电）
#define DCP_ENABLED  1  // 宏定义：使能放电（转换期间允许放电）

// 针对 ADC 转换的电池选择
#define CH_ALL       0  // 宏定义：转换所有电池通道
#define CH_1and7     1  // 宏定义：转换电池1和电池7通道
#define CH_2and8     2  // 宏定义：转换电池2和电池8通道
#define CH_3and9     3  // 宏定义：转换电池3和电池9通道
#define CH_4and10    4  // 宏定义：转换电池4和电池10通道
#define CH_5and11    5  // 宏定义：转换电池5和电池11通道
#define CH_6and12    6  // 宏定义：转换电池6和电池12通道

// 用于导线开路转换的上拉/下拉电流
#define PUP_DOWN     0  // 宏定义：下拉电流
#define PUP_UP       1  // 宏定义：上拉电流

// 自测试模式选择
#define ST1          1  // 宏定义：自测试1
#define ST2          2  // 宏定义：自测试2

// 针对 ADC 转换的 GPIO 选择
#define CHG_ALL      0  // 宏定义：转换所有辅助通道（GPIO和Vref2）
#define CHG_GPIO1    1  // 宏定义：转换GPIO1通道
#define CHG_GPIO2    2  // 宏定义：转换GPIO2通道
#define CHG_GPIO3    3  // 宏定义：转换GPIO3通道
#define CHG_GPIO4    4  // 宏定义：转换GPIO4通道
#define CHG_GPIO5    5  // 宏定义：转换GPIO5通道
#define CHG_VREF2    6  // 宏定义：转换参考电压2通道

// 状态组通道选择
#define CHST_ALL     0  // 宏定义：选择所有通道
#define CHST_SOC     1  // 宏定义：电池测量的总和
#define CHST_ITMP    2  // 宏定义：内部芯片温度
#define CHST_VA      3  // 宏定义：模拟电源电压
#define CHST_VD      4  // 宏定义：数字电源电压

// ADC转换函数命令组
void LTC6804_adcv(uint8_t MD, uint8_t DCP, uint8_t CH);               // 启动电池电压 ADC 转换
void LTC6804_adow(uint8_t MD, uint8_t PUP, uint8_t DCP, uint8_t CH);  // 启动导线开路 ADC 转换
void LTC6804_cvst(uint8_t MD, uint8_t ST);                            // 启动自测试电池电压转换
void LTC6804_adax(uint8_t MD, uint8_t CHG);                           // 启动 GPIO ADC 转换
void LTC6804_axst(uint8_t MD, uint8_t ST);                            // 启动自测试 GPIO 转换
void LTC6804_adstat(uint8_t MD, uint8_t CHST);                        // 启动状态组 ADC 转换
void LTC6804_statst(uint8_t MD, uint8_t ST);                          // 启动自测试状态组转换
void LTC6804_adcvax(uint8_t MD, uint8_t DCP);                         // 启动组合电池电压以及 GPIO1、GPIO2 转换

// 配置寄存器操作函数
void LTC6804_wrcfg(uint8_t total_ic, uint8_t config[][6]);       // 写配置寄存器组的函数声明
uint8_t LTC6804_rdcfg(uint8_t total_ic, uint8_t r_config[][6]);  // 读取配置寄存器的函数声明

// 电压寄存器操作函数
uint8_t LTC6804_rdcv(uint8_t reg, uint8_t total_ic, uint16_t cell_codes[][12]);  // 读取并解析电池电压寄存器的函数声明
void LTC6804_rdcv_reg(uint8_t reg, uint8_t total_ic, uint8_t* data);             // 从电池电压寄存器读取原始数据的函数声明
uint8_t LTC6804_rdaux(uint8_t reg, uint8_t total_ic, uint16_t aux_codes[][6]);   // 读取并解析辅助寄存器的函数声明
void LTC6804_rdaux_reg(uint8_t reg, uint8_t total_ic, uint8_t* data);            // 从辅助寄存器读取原始数据的函数声明

// 状态寄存器组操作函数
uint8_t LTC6804_rdstat_reg(uint8_t reg, uint8_t total_ic, uint8_t stat_data[][6]);  // 从状态寄存器读取原始数据的函数声明

// 清除电池电压、辅助电压、状态寄存器命令
void LTC6804_clrcell(void);  // 清除电池电压寄存器的函数声明
void LTC6804_clraux(void);   // 清除辅助电压寄存器的函数声明
void LTC6804_clrstat(void);  // 清除状态寄存器的函数声明

// 片选端口操作(PA4)
#define cs_ltc6804(x)                                                                                                \
   do {                                                                                                              \
      x ? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); \
   } while (0)

// 通讯函数
void spi_write_array(uint8_t len, uint8_t data[]);                                         // 通过SPI发送字节数组的函数声明
void spi_write_read(uint8_t tx_Data[], uint8_t tx_len, uint8_t* rx_data, uint8_t rx_len);  // 通过SPI读写字节的函数声明

// 唤醒函数
void wakeup_sleep(void);  // 唤醒ltc6804

// CRC校验函数
uint16_t pec15_calc(uint8_t len, uint8_t* data);  // 计算CRC15的函数声明

// 静态常量CRC15表，用于LTC6804的PEC（校验和）计算（预计算）
static const unsigned int crc15Table[256] = {
    0x0,    0xc599, 0xceab, 0xb32,  0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac, 0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1, 0xbbf3, 0x7e6a,
    0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x83fd, 0x5b2e, 0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b, 0x77e6, 0xb27f, 0xb94d, 0x7cd4,
    0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd, 0x2544, 0x2be,  0xc727, 0xcc15, 0x98c,  0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c, 0x3d6e, 0xf8f7, 0x2b0a, 0xee93, 0xe5a1, 0x2038,
    0x7c2,  0xc25b, 0xc969, 0xcf0,  0xdf0d, 0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf, 0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8,
    0xa8eb, 0x6d72, 0x6640, 0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba, 0x4a88, 0x8f11, 0x57c,  0xc0e5, 0xcbd7, 0xe4e,  0xddb3, 0x182a, 0x1318, 0xd681,
    0xf17b, 0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921, 0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070, 0x85e9,
    0xf84,  0xca1d, 0xc12f, 0x4b6,  0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528, 0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59, 0x2ac0,
    0xd3a,  0xc8a3, 0xc391, 0x608,  0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01, 0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9, 0x7350,
    0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a, 0xee3,  0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25, 0x2fbc,
    0x846,  0xcddf, 0xc6ed, 0x374,  0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8,  0xcf61, 0xc453, 0x1ca,  0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b, 0x2d02,
    0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3, 0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095};

#endif  // _LTC6804_1_BASE_DRIVE_H
