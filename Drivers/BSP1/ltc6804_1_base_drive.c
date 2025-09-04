#include "ltc6804_1_base_drive.h"

/**
 * @brief   写LTC6804菊花链的配置寄存器
 * @param   uint8_t total_ic: 菊花链中的芯片数量
 * @param   LTC6804_ConfigRegsTypeDef config_structs[]：存储配置数据的结构体数组。
 */
void write_LTC6804_Config(uint8_t total_ic, LTC6804_ConfigRegsTypeDef config_structs[]) {
   uint8_t config[total_ic][6];
   for (int ic = 0; ic < total_ic; ic++) {
      config[ic][0] = config_structs[ic].CFGR0.reg_val;
      config[ic][1] = config_structs[ic].CFGR1.reg_val;
      config[ic][2] = config_structs[ic].CFGR2.reg_val;
      config[ic][3] = config_structs[ic].CFGR3.reg_val;
      config[ic][4] = config_structs[ic].CFGR4.reg_val;
      config[ic][5] = config_structs[ic].CFGR5.reg_val;
   }
   LTC6804_wrcfg(total_ic, config);
}

/**
 * @brief   读取LTC6804菊花链的配置寄存器并存储到结构体
 * @param   uint8_t total_ic: 菊花链中的芯片数量
 * @param   LTC6804_ConfigRegsTypeDef r_config[]：存储读取配置数据的结构体数组。
 * @return  uint8_t pec_error:PEC状态。
 * 0: 读取的数据PEC匹配
 * 1: 读取的数据PEC不匹配
 */
uint8_t read_LTC6804_Config(uint8_t total_ic, LTC6804_ConfigRegsTypeDef r_config[]) {
   uint8_t r_config_2d[total_ic][6];
   uint8_t pec_error = LTC6804_rdcfg(total_ic, r_config_2d);
   if (pec_error == 0) {
      for (uint8_t ic = 0; ic < total_ic; ic++) {
         r_config[ic].CFGR0.reg_val = r_config_2d[ic][0];
         r_config[ic].CFGR1.reg_val = r_config_2d[ic][1];
         r_config[ic].CFGR2.reg_val = r_config_2d[ic][2];
         r_config[ic].CFGR3.reg_val = r_config_2d[ic][3];
         r_config[ic].CFGR4.reg_val = r_config_2d[ic][4];
         r_config[ic].CFGR5.reg_val = r_config_2d[ic][5];
      }
   }
   return pec_error;
}

/**
 * @brief   读取LTC6804的状态寄存器A和B，并解析到结构体中
 * @param   uint8_t total_ic; 菊花链中的芯片数量
 * @param   LTC6804_StatusRegs* status_regs; 存储所有芯片状态寄存器数据的结构体数组
 * @return  uint8_t, 错误状态
 * 0: 读取成功，无PEC错误
 * 1: 寄存器A读取出现PEC错误
 * 2: 寄存器B读取出现PEC错误
 * 3: 寄存器A和B都出现PEC错误
 */
uint8_t read_LTC6804_status_registers(uint8_t total_ic, LTC6804_StatusRegs status_regs[]) {
   uint8_t stat_data[total_ic][6];  // 存储原始状态数据的缓冲区
   uint8_t error_flag = 0;          // 错误标志
   uint8_t pec_a, pec_b;            // 寄存器A和B的PEC校验结果

   // 读取状态寄存器A
   pec_a = LTC6804_rdstat_reg(1, total_ic, stat_data);
   if (pec_a != 0) {
      error_flag |= 0x01;  // 标记寄存器A有PEC错误
   } else {
      // 将原始数据解析到状态寄存器A结构体
      for (uint8_t ic = 0; ic < total_ic; ic++) {
         status_regs[ic].groupA.STAR0.reg_val = stat_data[ic][0];
         status_regs[ic].groupA.STAR1.reg_val = stat_data[ic][1];
         status_regs[ic].groupA.STAR2.reg_val = stat_data[ic][2];
         status_regs[ic].groupA.STAR3.reg_val = stat_data[ic][3];
         status_regs[ic].groupA.STAR4.reg_val = stat_data[ic][4];
         status_regs[ic].groupA.STAR5.reg_val = stat_data[ic][5];
      }
   }

   // 读取状态寄存器B
   pec_b = LTC6804_rdstat_reg(2, total_ic, stat_data);
   if (pec_b != 0) {
      error_flag |= 0x02;  // 标记寄存器B有PEC错误
   } else {
      // 将原始数据解析到状态寄存器B结构体
      for (uint8_t ic = 0; ic < total_ic; ic++) {
         status_regs[ic].groupB.STBR0.reg_val = stat_data[ic][0];
         status_regs[ic].groupB.STBR1.reg_val = stat_data[ic][1];
         status_regs[ic].groupB.STBR2.reg_val = stat_data[ic][2];
         status_regs[ic].groupB.STBR3.reg_val = stat_data[ic][3];
         status_regs[ic].groupB.STBR4.reg_val = stat_data[ic][4];
         status_regs[ic].groupB.STBR5.reg_val = stat_data[ic][5];
      }
   }

   return error_flag;
}

/**
 * @brief   读取所有的电池电压并存储在电池电压句柄中
 * @param   uint8_t total_ic; 菊花链中的芯片数量
 * @param   BatteryVoltageRegs cv[];存储所有芯片电池电压数据的结构体数组
 * @return  uint8_t pec_error:PEC状态。
 * 0: 读取的数据PEC匹配
 * 1: 读取的数据PEC不匹配
 */
uint8_t read_LTC6804_Battery_voltage_registers(uint8_t total_ic, BatteryVoltageRegs cv[]) {
   uint16_t cell_codes[total_ic][12];
   uint8_t pec_error = LTC6804_rdcv(reg_cv_all, total_ic, cell_codes);
   if (pec_error == 0) {
      for (uint8_t ic = 0; ic < total_ic; ic++) {
         cv[ic].C01V = cell_codes[ic][0];
         cv[ic].C02V = cell_codes[ic][1];
         cv[ic].C03V = cell_codes[ic][2];
         cv[ic].C04V = cell_codes[ic][3];
         cv[ic].C05V = cell_codes[ic][4];
         cv[ic].C06V = cell_codes[ic][5];
         cv[ic].C07V = cell_codes[ic][6];
         cv[ic].C08V = cell_codes[ic][7];
         cv[ic].C09V = cell_codes[ic][8];
         cv[ic].C10V = cell_codes[ic][9];
         cv[ic].C11V = cell_codes[ic][10];
         cv[ic].C12V = cell_codes[ic][11];
      }
   }
   return pec_error;
}

/**
 * @brief   读取所有辅助寄存器并存储到辅助寄存器句柄中
 * @param   uint8_t total_ic; 菊花链中的芯片数量
 * @param
 * @return  uint8_t pec_error:PEC状态。
 * 0: 读取的数据PEC匹配
 * 1: 读取的数据PEC不匹配
 */
uint8_t read_LTC6804_Auxiliary_voltage_registers(uint8_t total_ic, AuxVoltageRegs av[]) {
   uint16_t aux_codes[total_ic][6];
   uint8_t pec_error = LTC6804_rdaux(reg_ax_all, total_ic, aux_codes);
   if (pec_error == 0) {
      for (uint8_t ic = 0; ic < total_ic; ic++) {
         av[ic].G1V = aux_codes[ic][0];
         av[ic].G2V = aux_codes[ic][1];
         av[ic].G3V = aux_codes[ic][2];
         av[ic].G4V = aux_codes[ic][3];
         av[ic].G5V = aux_codes[ic][4];
         av[ic].REF = aux_codes[ic][5];
      }
   }
   return pec_error;
}

/**
 * @brief   启动电池电压 ADC 转换
 * @param   uint8_t MD：ADC转换模式
 * @param   uint8_t DCP：控制电池转换期间是否允许放电
 * @param   uint8_t CH：确定ADC转换时测量哪些电池通道
 */
void LTC6804_adcv(uint8_t MD, uint8_t DCP, uint8_t CH) {
   uint8_t md_bits;   // 用于暂存模式位的变量
   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   md_bits = (MD & 0x02) >> 1;                 // 提取MD的第1位，并右移1位
   cmd[0] = md_bits + 0x02;                    // 配置命令的第1个字节
   md_bits = (MD & 0x01) << 7;                 // 提取MD的第0位（左移7位）
   cmd[1] = 0x60 + md_bits + (DCP << 4) + CH;  // 配置命令第2字节（含模式、放电允许、电池通道）

   cmd_pec = pec15_calc(2, cmd);      // 计算2字节ADCV命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位存入cmd[2]
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位存入cmd[3]

   wakeup_sleep();           // 唤醒iso及LTC6804芯片
   cs_ltc6804(0);            // 拉低片选信号（使能LTC6804）
   spi_write_array(4, cmd);  // 通过SPI发送4字节命令（含PEC）
   cs_ltc6804(1);            // 拉高片选信号（禁用LTC6804）
}

/**
 * @brief   启动导线开路 ADC 转换
 * @param   uint8_t MD：ADC转换模式
 * @param   uint8_t PUP：导线开路转换的上拉/下拉电流
 * @param   uint8_t DCP：控制电池转换期间是否允许放电
 * @param   uint8_t CH：确定ADC转换时测量哪些电池通道
 */
void LTC6804_adow(uint8_t MD, uint8_t PUP, uint8_t DCP, uint8_t CH) {
   uint8_t md_bits;   // 用于暂存模式位的变量
   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   md_bits = (MD & 0x02) >> 1;                              // 提取MD的第1位，并右移1位
   cmd[0] = md_bits + 0x02;                                 // 配置命令的第1个字节
   md_bits = (MD & 0x01) << 7;                              // 提取MD的第0位（左移7位）
   cmd[1] = 0x28 + md_bits + (PUP << 6) + (DCP << 4) + CH;  // 配置命令第2字节（含模式、放电允许、电池通道、上下拉电流）

   cmd_pec = pec15_calc(2, cmd);      // 计算2字节ADCV命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位存入cmd[2]
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位存入cmd[3]

   wakeup_sleep();           // 唤醒iso及LTC6804芯片
   cs_ltc6804(0);            // 拉低片选信号（使能LTC6804）
   spi_write_array(4, cmd);  // 通过SPI发送4字节命令（含PEC）
   cs_ltc6804(1);            // 拉高片选信号（禁用LTC6804）
}

/**
 * @brief   启动自测试电池电压转换
 * @param   uint8_t MD：ADC转换模式
 * @param   uint8_t ST：自测试模式
 */
void LTC6804_cvst(uint8_t MD, uint8_t ST) {
   uint8_t md_bits;   // 用于暂存模式位的变量
   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   md_bits = (MD & 0x02) >> 1;           // 提取MD的第1位，并右移1位
   cmd[0] = md_bits + 0x02;              // 配置命令的第1个字节
   md_bits = (MD & 0x01) << 7;           // 提取MD的第0位（左移7位）
   cmd[1] = 0x07 + md_bits + (ST << 5);  // 配置命令第2字节

   cmd_pec = pec15_calc(2, cmd);      // 计算2字节ADCV命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位存入cmd[2]
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位存入cmd[3]

   wakeup_sleep();           // 唤醒iso及LTC6804芯片
   cs_ltc6804(0);            // 拉低片选信号（使能LTC6804）
   spi_write_array(4, cmd);  // 通过SPI发送4字节命令（含PEC）
   cs_ltc6804(1);            // 拉高片选信号（禁用LTC6804）
}

/**
 * @brief   启动 GPIO ADC 转换
 * @param   uint8_t MD：ADC转换模式
 * @param   uint8_t CHG：确定GPIO ADC转换时测量哪些通道
 */
void LTC6804_adax(uint8_t MD, uint8_t CHG) {
   uint8_t md_bits;   // 用于暂存模式位的变量
   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   md_bits = (MD & 0x02) >> 1;     // 提取MD的第1位，并右移1位
   cmd[0] = md_bits + 0x04;        // 配置命令的第1个字节
   md_bits = (MD & 0x01) << 7;     // 提取MD的第0位（左移7位）
   cmd[1] = 0x60 + md_bits + CHG;  // 配置命令第2字节（含模式、通道）

   cmd_pec = pec15_calc(2, cmd);      // 计算2字节ADCV命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位存入cmd[2]
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位存入cmd[3]

   wakeup_sleep();           // 唤醒iso及LTC6804芯片
   cs_ltc6804(0);            // 拉低片选信号（使能LTC6804）
   spi_write_array(4, cmd);  // 通过SPI发送4字节命令（含PEC）
   cs_ltc6804(1);            // 拉高片选信号（禁用LTC6804）
}

/**
 * @brief   启动自测试 GPIO 转换
 * @param   uint8_t MD：ADC转换模式
 * @param   uint8_t ST：自测试模式
 */
void LTC6804_axst(uint8_t MD, uint8_t ST) {
   uint8_t md_bits;   // 用于暂存模式位的变量
   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   md_bits = (MD & 0x02) >> 1;           // 提取MD的第1位，并右移1位
   cmd[0] = md_bits + 0x04;              // 配置命令的第1个字节
   md_bits = (MD & 0x01) << 7;           // 提取MD的第0位（左移7位）
   cmd[1] = 0x07 + md_bits + (ST << 5);  // 配置命令第2字节

   cmd_pec = pec15_calc(2, cmd);      // 计算2字节ADCV命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位存入cmd[2]
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位存入cmd[3]

   wakeup_sleep();           // 唤醒iso及LTC6804芯片
   cs_ltc6804(0);            // 拉低片选信号（使能LTC6804）
   spi_write_array(4, cmd);  // 通过SPI发送4字节命令（含PEC）
   cs_ltc6804(1);            // 拉高片选信号（禁用LTC6804）
}

/**
 * @brief   启动状态组 ADC 转换
 * @param   uint8_t MD：ADC转换模式
 * @param   uint8_t CHST：确定状态组 ADC 转换时测量哪些通道
 */
void LTC6804_adstat(uint8_t MD, uint8_t CHST) {
   uint8_t md_bits;   // 用于暂存模式位的变量
   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   md_bits = (MD & 0x02) >> 1;      // 提取MD的第1位，并右移1位
   cmd[0] = md_bits + 0x04;         // 配置命令的第1个字节
   md_bits = (MD & 0x01) << 7;      // 提取MD的第0位（左移7位）
   cmd[1] = 0x68 + md_bits + CHST;  // 配置命令第2字节（含模式、通道）

   cmd_pec = pec15_calc(2, cmd);      // 计算2字节ADCV命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位存入cmd[2]
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位存入cmd[3]

   wakeup_sleep();           // 唤醒iso及LTC6804芯片
   cs_ltc6804(0);            // 拉低片选信号（使能LTC6804）
   spi_write_array(4, cmd);  // 通过SPI发送4字节命令（含PEC）
   cs_ltc6804(1);            // 拉高片选信号（禁用LTC6804）
}

/**
 * @brief   启动自测试状态组转换
 * @param   uint8_t MD：ADC转换模式
 * @param   uint8_t ST：自测试模式
 */
void LTC6804_statst(uint8_t MD, uint8_t ST) {
   uint8_t md_bits;   // 用于暂存模式位的变量
   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   md_bits = (MD & 0x02) >> 1;           // 提取MD的第1位，并右移1位
   cmd[0] = md_bits + 0x04;              // 配置命令的第1个字节
   md_bits = (MD & 0x01) << 7;           // 提取MD的第0位（左移7位）
   cmd[1] = 0x0f + md_bits + (ST << 5);  // 配置命令第2字节

   cmd_pec = pec15_calc(2, cmd);      // 计算2字节ADCV命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位存入cmd[2]
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位存入cmd[3]

   wakeup_sleep();           // 唤醒iso及LTC6804芯片
   cs_ltc6804(0);            // 拉低片选信号（使能LTC6804）
   spi_write_array(4, cmd);  // 通过SPI发送4字节命令（含PEC）
   cs_ltc6804(1);            // 拉高片选信号（禁用LTC6804）
}

/**
 * @brief   启动组合电池电压以及 GPIO1、GPIO2 转换
 * @param   uint8_t MD：ADC转换模式
 * @param   uint8_t DCP：控制转换期间是否允许放电
 */
void LTC6804_adcvax(uint8_t MD, uint8_t DCP) {
   uint8_t md_bits;   // 用于暂存模式位的变量
   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   md_bits = (MD & 0x02) >> 1;            // 提取MD的第1位，并右移1位
   cmd[0] = md_bits + 0x04;               // 配置命令的第1个字节
   md_bits = (MD & 0x01) << 7;            // 提取MD的第0位（左移7位）
   cmd[1] = 0x6f + md_bits + (DCP << 4);  // 配置命令第2字节

   cmd_pec = pec15_calc(2, cmd);      // 计算2字节ADCV命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位存入cmd[2]
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位存入cmd[3]

   wakeup_sleep();           // 唤醒iso及LTC6804芯片
   cs_ltc6804(0);            // 拉低片选信号（使能LTC6804）
   spi_write_array(4, cmd);  // 通过SPI发送4字节命令（含PEC）
   cs_ltc6804(1);            // 拉高片选信号（禁用LTC6804）
}

/**
 * @brief   写入LTC6804的配置寄存器,该命令会写入菊花链中LTC6804-1的配置寄存器。配置按降序写入，因此最后一个器件的配置最先写入。
 * @param   uint8_t total_ic; 要写入的芯片数量。
 * @param   uint8_t config[][6]：存储配置数据的二维数组，数组应包含每个芯片的6字节配置。
 */
void LTC6804_wrcfg(uint8_t total_ic, uint8_t config[][6]) {
   /*
   WRCFG流程：
   1. 加载写入配置命令和PEC到cmd数组
   2. 加载LTC6804配置数据到cmd
   3. 计算传输的LTC6804配置数据的PEC
   4. 唤醒isoSPI（可选）
   5. 向LTC6804菊花链写入配置数据
   */

   const uint8_t BYTES_IN_REG = 6;              // 每个寄存器的字节数
   const uint8_t CMD_LEN = 4 + (8 * total_ic);  // 命令总长度（4字节基础命令+每个芯片8字节数据）

   uint8_t* cmd;       // 指向命令数组的指针
   uint16_t cfg_pec;   // 配置数据的PEC值
   uint8_t cmd_index;  // 命令计数器

   cmd = (uint8_t*)malloc(CMD_LEN * sizeof(uint8_t));  // 分配命令缓冲区

   // 1. 设置写入配置命令和PEC
   cmd[0] = 0x00;  // 命令高字节
   cmd[1] = 0x01;  // 命令低字节（写入配置命令）
   cmd[2] = 0x3d;  // 预计算的PEC高8位
   cmd[3] = 0x6e;  // 预计算的PEC低8位

   // 2. 加载配置数据到cmd数组
   cmd_index = 4;                                                                    // 从第4字节开始存储配置数据
   for (uint8_t current_ic = total_ic; current_ic > 0; current_ic--) {               // 按芯片编号从高到低遍历（因配置按降序写入）
      for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++) {  // 遍历每个配置字节
         cmd[cmd_index] = config[current_ic - 1][current_byte];                      // 将配置数据添加到发送数组
         cmd_index = cmd_index + 1;                                                  // 计数器+1
      }

      // 3. 计算每个芯片配置数据的PEC
      cfg_pec = (uint16_t)pec15_calc(BYTES_IN_REG, &config[current_ic - 1][0]);  // 计算6字节配置数据的PEC
      cmd[cmd_index] = (uint8_t)(cfg_pec >> 8);                                  // PEC高8位
      cmd[cmd_index + 1] = (uint8_t)cfg_pec;                                     // PEC低8位
      cmd_index = cmd_index + 2;                                                 // 计数器+2
   }

   // 4. 唤醒isoSPI
   wakeup_sleep();  // 唤醒iso及LTC6804芯片

   // 5. 发送命令
   cs_ltc6804(0);                  // 拉低片选
   spi_write_array(CMD_LEN, cmd);  // 通过SPI发送完整命令（含所有配置数据和PEC）
   cs_ltc6804(1);                  // 拉高片选
   free(cmd);                      // 释放命令缓冲区
}

/**
 * @brief   读取LTC6804菊花链的配置寄存器
 * @param   uint8_t total_ic: 菊花链中的芯片数量
 * @param   uint8_t r_config[][6]：存储读取配置数据的二维数组。
 * @return  uint8_t pec_error:PEC状态。
 * 0: 读取的数据PEC匹配
 * 1: 读取的数据PEC不匹配
 */
uint8_t LTC6804_rdcfg(uint8_t total_ic, uint8_t r_config[][6]) {
   /*
   RDCFG流程：
   1. 加载读取配置命令和PEC到cmd数组
   2. 唤醒isoSPI（可选）
   3. 发送命令并读取配置数据
   4. 对每个芯片：
      a. 将配置数据加载到r_config数组
      b. 计算接收数据的PEC并与接收的PEC比较
   5. 返回PEC错误
   */

   const uint8_t BYTES_IN_REG = 8;  // 每个寄存器的总字节数（6字节配置+2字节PEC）（声明常量）

   uint8_t cmd[4];         // 命令数组（2字节命令+2字节PEC）
   uint8_t* rx_data;       // 指向接收数据的指针
   uint8_t pec_error = 0;  // PEC错误标志
   uint16_t data_pec;      // 计算的PEC值
   uint16_t received_pec;  // 接收的PEC值

   rx_data = (uint8_t*)malloc((8 * total_ic) * sizeof(uint8_t));  // 分配接收缓冲区（每个芯片8字节×芯片数）

   // 1. 设置读取配置命令
   cmd[0] = 0x00;  // 命令高字节
   cmd[1] = 0x02;  // 命令低字节（读取配置命令）
   cmd[2] = 0x2b;  // 预计算的PEC高8位
   cmd[3] = 0x0A;  // 预计算的PEC低8位

   // 2. 唤醒isoSPI
   wakeup_sleep();  // 唤醒iso及LTC6804芯片

   // 3. 发送命令并接收数据
   cs_ltc6804(0);                                               // 拉低片选
   spi_write_read(cmd, 4, rx_data, (BYTES_IN_REG * total_ic));  // 发送4字节命令，接收总字节数=每个寄存器字节数×芯片数
   cs_ltc6804(1);                                               // 拉高片选

   for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) {  // 遍历每个芯片
      // 4.a 将接收数据加载到r_config数组
      for (uint8_t current_byte = 0; current_byte < 6; current_byte++) {                            // 只加载每芯片数据的前六个字节，不加载对应pec
         r_config[current_ic][current_byte] = rx_data[current_byte + (current_ic * BYTES_IN_REG)];  // 按芯片和字节索引存储
      }
      // 4.b 校验PEC
      received_pec = (rx_data[6 + (current_ic * BYTES_IN_REG)] << 8) + rx_data[7 + (current_ic * BYTES_IN_REG)];  // 提取接收的PEC
      data_pec = pec15_calc(6, &r_config[current_ic][0]);                                                         // 计算6字节配置数据的PEC
      if (received_pec != data_pec) {
         pec_error = 1;  // 标记错误
      }
   }

   free(rx_data);  // 释放接收缓冲区并返回PEC错误标志
   return (pec_error);
}

/**
 * @brief   读取并解析LTC6804的电池电压寄存器。该函数用于读取LTC6804的电池编码值。函数会发送读取命令、解析数据，并将电池电压存储在cell_codes变量中。
 * @param   uint8_t reg; 控制读取哪个电池电压寄存器。
 * 0: 读取所有电池寄存器
 * 1: 读取电池组A
 * 2: 读取电池组B
 * 3: 读取电池组C
 * 4: 读取电池组D
 * @param   uint8_t total_ic; 菊花链中的芯片数量
 * @param   uint16_t cell_codes[][12]; 解析后的电池编码值数组（从低到高排序）。
 * @return  int8_t, PEC状态。
 * 0: 未检测到PEC错误
 * 1: 检测到PEC错误，请重试读取
 */
uint8_t LTC6804_rdcv(uint8_t reg, uint8_t total_ic, uint16_t cell_codes[][12]) {
   /*
   LTC6804_rdcv流程：
   1. 分支处理：
      a. reg = 0（读取所有寄存器）
         i. 读取菊花链中每个芯片的电池电压寄存器A-D
         ii. 解析原始数据到cell_codes数组
         iii. 校验每个寄存器的PEC（接收值vs计算值）
      b. reg ≠ 0（读取单个寄存器）
         i. 读取菊花链中所有芯片的指定寄存器
         ii. 解析原始数据到cell_codes数组
         iii. 校验PEC
   2. 返回PEC错误标志
   */

   const uint8_t NUM_RX_BYT = 8;   // 每个芯片的接收字节数（6字节数据+2字节PEC）
   const uint8_t BYT_IN_REG = 6;   // 每个寄存器的数据字节数
   const uint8_t CELL_IN_REG = 3;  // 每个寄存器包含的电池数量（6字节=3个16位值）

   uint8_t* cell_data;        // 指向接收数据的指针
   uint8_t pec_error = 0;     // PEC错误标志（0=无错误，1=错误）
   uint16_t parsed_cell;      // 解析后的电池电压值
   uint16_t received_pec;     // 接收的PEC值
   uint16_t data_pec;         // 计算的PEC值
   uint8_t data_counter = 0;  // 数据计数器

   cell_data = (uint8_t*)malloc((NUM_RX_BYT * total_ic) * sizeof(uint8_t));  // 分配接收缓冲区（总字节数=每个芯片8字节×芯片数）

   if (reg == 0) {  // 1.a 若读取所有寄存器
      // a.i 遍历每个LTC6804的电池电压寄存器A-D
      for (uint8_t cell_reg = 1; cell_reg < 5; cell_reg++) {
         data_counter = 0;                                                    // 重置数据计数器
         LTC6804_rdcv_reg(cell_reg, total_ic, cell_data);                     // 读取指定寄存器的原始数据到cell_data
         for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) {  // 遍历每个芯片
            // a.ii 解析寄存器中的每个电池电压值
            for (uint8_t current_cell = 0; current_cell < CELL_IN_REG; current_cell++) {
               parsed_cell = cell_data[data_counter] + (cell_data[data_counter + 1] << 8);           // 拼接低8位和高8位，得到16位电池值
               cell_codes[current_ic][current_cell + ((cell_reg - 1) * CELL_IN_REG)] = parsed_cell;  // 按芯片和电池编号存储
               data_counter = data_counter + 2;                                                      // 计数器+2（每个电池值占2字节）
            }
            // a.iii 校验PEC
            received_pec = (cell_data[data_counter] << 8) + cell_data[data_counter + 1];  // 提取接收的PEC
            data_pec = pec15_calc(BYT_IN_REG, &cell_data[current_ic * NUM_RX_BYT]);       // 计算6字节数据的PEC
            if (received_pec != data_pec) {
               pec_error = 1;  // 标记错误
            }
            data_counter = data_counter + 2;  // 计数器+2（跳过PEC字节）
         }
      }
   } else {  // 1.b 若读取单个寄存器
      // b.i 读取指定寄存器
      LTC6804_rdcv_reg(reg, total_ic, cell_data);
      for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) {  // 遍历每个芯片
         // b.ii 解析每个电池电压值
         for (uint8_t current_cell = 0; current_cell < CELL_IN_REG; current_cell++) {
            parsed_cell = cell_data[data_counter] + (cell_data[data_counter + 1] << 8);                   // 拼接16位电池值
            cell_codes[current_ic][current_cell + ((reg - 1) * CELL_IN_REG)] = 0x0000FFFF & parsed_cell;  // 存储（确保16位）
            data_counter = data_counter + 2;                                                              // 计数器+2
         }
         // b.iii 校验PEC
         received_pec = (cell_data[data_counter] << 8) + cell_data[data_counter + 1];  // 提取接收的PEC
         data_pec = pec15_calc(BYT_IN_REG, &cell_data[current_ic * NUM_RX_BYT]);       // 计算PEC
         if (received_pec != data_pec) {
            pec_error = 1;  // 标记错误
         }
         data_counter = data_counter + 2;  // 计数器+2
      }
   }

   // 2. 释放缓冲区并返回PEC状态
   free(cell_data);
   return (pec_error);
}

/**
 * @brief   从LTC6804电池电压寄存器读取原始数据,该函数读取单个电池电压寄存器，并将原始数据存储到*data指向的字节数组。此函数很少在LTC6804_rdcv()之外单独使用。
 * @param   uint8_t reg; 控制读取哪个电池电压寄存器。
 * 1: 读取电池组A
 * 2: 读取电池组B
 * 3: 读取电池组C
 * 4: 读取电池组D
 * @param   uint8_t total_ic; 菊花链中的芯片数量
 * @param   uint8_t *data; 存储未解析电池编码值的数组
 */
void LTC6804_rdcv_reg(uint8_t reg, uint8_t total_ic, uint8_t* data) {
   /*
   LTC6804_rdcv_reg函数流程：
   1. 确定命令并初始化命令数组
   2. 计算命令的PEC
   3. 唤醒isoSPI（可选）
   4. 向LTC6804菊花链发送全局命令
   */

   const uint8_t REG_LEN = 8;  // 每个寄存器的总字节数（6字节数据+2字节PEC）

   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   // 1. 根据寄存器号设置命令
   if (reg == 1) {  // 读取电池组A
      cmd[0] = 0x00;
      cmd[1] = 0x04;
   } else if (reg == 2) {  // 读取电池组B
      cmd[0] = 0x00;
      cmd[1] = 0x06;
   } else if (reg == 3) {  // 读取电池组C
      cmd[0] = 0x00;
      cmd[1] = 0x08;
   } else if (reg == 4) {  // 读取电池组D
      cmd[0] = 0x00;
      cmd[1] = 0x0A;
   }

   // 2. 计算命令的PEC
   cmd_pec = pec15_calc(2, cmd);      // 计算2字节命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位

   // 3. 唤醒isoSPI（可选）
   wakeup_sleep();  // 唤醒iso及LTC6804芯片

   // 4. 发送命令并接收数据
   cs_ltc6804(0);                                       // 拉低片选
   spi_write_read(cmd, 4, data, (REG_LEN * total_ic));  // 发送4字节命令，接收总字节数=每个寄存器字节数×芯片数
   cs_ltc6804(1);                                       // 拉高片选
}

/**
 * @brief   读取并解析LTC6804的辅助寄存器。该函数用于读取LTC6804的GPIO编码值。函数会发送读取命令、解析数据，并将GPIO电压存储在aux_codes变量中。
 * @param   uint8_t reg; 控制读取哪个GPIO电压寄存器。
 * 0: 读取所有辅助寄存器
 * 1: 读取辅助组A
 * 2: 读取辅助组B
 * @param   uint8_t total_ic; 菊花链中的芯片数量
 * @param   uint16_t aux_codes[][6]; GPIO电压编码的二维数组。
 * @return  uint8_t, PEC状态
 * 0: 未检测到PEC错误
 * 1: 检测到PEC错误，请重试读取
 */
uint8_t LTC6804_rdaux(uint8_t reg, uint8_t total_ic, uint16_t aux_codes[][6]) {
   /*
   LTC6804_rdaux流程：
   1. 分支处理：
      a. reg = 0（读取所有寄存器）
         i. 读取菊花链中每个芯片的GPIO寄存器A-D
         ii. 解析原始GPIO数据到cell_codes数组
         iii. 校验PEC（接收值vs计算值）
      b. reg ≠ 0（读取单个寄存器）
         i. 读取菊花链中所有芯片的指定寄存器
         ii. 解析原始GPIO数据到cell_codes数组
         iii. 校验PEC
   2. 返回PEC错误标志
   */

   const uint8_t NUM_RX_BYT = 8;   // 每个芯片的接收字节数（6字节数据+2字节PEC）
   const uint8_t BYT_IN_REG = 6;   // 每个寄存器的数据字节数
   const uint8_t GPIO_IN_REG = 3;  // 每个寄存器包含的GPIO数量（6字节=3个16位值）

   uint8_t* data;                                                       // 指向接收数据的指针
   uint8_t data_counter = 0;                                            // 数据计数器
   uint8_t pec_error = 0;                                               // PEC错误标志
   uint16_t parsed_aux;                                                 // 解析后的GPIO值
   uint16_t received_pec;                                               // 接收的PEC值
   uint16_t data_pec;                                                   // 计算的PEC值
   data = (uint8_t*)malloc((NUM_RX_BYT * total_ic) * sizeof(uint8_t));  // 分配接收缓冲区
   // 1.a 若读取所有寄存器
   if (reg == 0) {
      // a.i 遍历每个辅助寄存器A-B
      for (uint8_t gpio_reg = 1; gpio_reg < 3; gpio_reg++) {
         data_counter = 0;                                                    // 重置计数器
         LTC6804_rdaux_reg(gpio_reg, total_ic, data);                         // 读取原始辅助寄存器数据到data
         for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) {  // 遍历每个芯片
            // a.ii 解析每个GPIO值
            for (uint8_t current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++) {
               parsed_aux = data[data_counter] + (data[data_counter + 1] << 8);                    // 拼接16位GPIO值
               aux_codes[current_ic][current_gpio + ((gpio_reg - 1) * GPIO_IN_REG)] = parsed_aux;  // 按芯片和GPIO编号存储
               data_counter = data_counter + 2;                                                    // 计数器+2
            }
            // a.iii 校验PEC
            received_pec = (data[data_counter] << 8) + data[data_counter + 1];  // 提取接收的PEC
            data_pec = pec15_calc(BYT_IN_REG, &data[current_ic * NUM_RX_BYT]);  // 计算6字节数据的PEC
            if (received_pec != data_pec) {
               pec_error = 1;  // 标记错误
            }
            data_counter = data_counter + 2;  // 计数器+2（跳过PEC）
         }
      }
   } else {
      // b.i 读取指定寄存器
      LTC6804_rdaux_reg(reg, total_ic, data);                          // 读取原始辅助寄存器数据到data
      for (int current_ic = 0; current_ic < total_ic; current_ic++) {  // 遍历每个芯片
         // b.ii 解析每个GPIO值
         for (int current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++) {
            parsed_aux = (data[data_counter] + (data[data_counter + 1] << 8));             // 拼接16位GPIO值
            aux_codes[current_ic][current_gpio + ((reg - 1) * GPIO_IN_REG)] = parsed_aux;  // 存储
            data_counter = data_counter + 2;                                               // 计数器+2
         }
         // b.iii 校验PEC
         received_pec = (data[data_counter] << 8) + data[data_counter + 1];  // 提取接收的PEC
         data_pec = pec15_calc(BYT_IN_REG, &data[current_ic * NUM_RX_BYT]);  // 计算PEC
         if (received_pec != data_pec) {
            pec_error = 1;  // 标记错误
         }
         data_counter = data_counter + 2;  // 计数器+2（跳过PEC）
      }
   }

   // 2. 释放缓冲区并返回PEC状态
   free(data);          // 释放缓冲区
   return (pec_error);  // 返回PEC状态
}

/**
 * @brief   从LTC6804辅助寄存器读取原始数据,该函数读取单个GPIO电压寄存器，并将原始数据存储到*data指向的字节数组。此函数很少在LTC6804_rdaux()之外单独使用。
 * @param   uint8_t reg; 控制读取哪个GPIO电压寄存器。
 * 1: 读取辅助组A
 * 2: 读取辅助组B
 * @param   uint8_t total_ic; 菊花链中的芯片数量
 * @param   uint8_t *data; 存储未解析辅助编码值的数组
 */
void LTC6804_rdaux_reg(uint8_t reg, uint8_t total_ic, uint8_t* data) {
   /*
   LTC6804_rdaux_reg函数流程：
   1. 确定命令并初始化命令数组
   2. 计算命令的PEC
   3. 唤醒isoSPI（可选）
   4. 向LTC6804菊花链发送全局命令
   */

   const uint8_t REG_LEN = 8;  // 每个寄存器的总字节数（6字节数据+2字节PEC）
   uint8_t cmd[4];             // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;           // 命令的PEC值

   // 1. 根据寄存器号设置命令
   if (reg == 1) {  // 读取辅助组A
      cmd[0] = 0x00;
      cmd[1] = 0x0C;
   } else if (reg == 2) {  // 读取辅助组B
      cmd[0] = 0x00;
      cmd[1] = 0x0E;
   } else {  // 默认读取辅助组A
      cmd[0] = 0x00;
      cmd[1] = 0x0C;
   }

   // 2. 计算命令的PEC
   cmd_pec = pec15_calc(2, cmd);      // 计算2字节命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位

   // 3. 唤醒isoSPI（可选）
   wakeup_sleep();  // 唤醒iso及LTC6804芯片

   // 4. 发送命令并接收数据
   cs_ltc6804(0);                                       // 拉低片选
   spi_write_read(cmd, 4, data, (REG_LEN * total_ic));  // 发送4字节命令，接收总字节数=每个寄存器字节数×芯片数
   cs_ltc6804(1);                                       // 拉高片选
}

/**
 * @brief   从LTC6804的状态寄存器读取原始数据
 * @param   uint8_t reg; 控制读取哪个状态寄存器
 * 1: 读取状态寄存器A
 * 2: 读取状态寄存器B
 * @param   uint8_t total_ic; 菊花链中的芯片数量
 * @param   uint8_t stat_data[][6]; 存储所有芯片上状态数据的二维数组
 * @return  uint8_t, PEC状态
 * 0: 未检测到PEC错误
 * 1: 检测到PEC错误，请重试读取
 */
uint8_t LTC6804_rdstat_reg(uint8_t reg, uint8_t total_ic, uint8_t stat_data[][6]) {
   /*
   LTC6804_rdstat_reg函数流程：
   1. 确定命令并初始化命令数组
   2. 计算命令的PEC
   3. 唤醒isoSPI（可选）
   4. 向LTC6804菊花链发送全局命令并接收数据
   5. 校验PEC并存储数据
   */

   const uint8_t reg_len = 8;  // 每个寄存器的总字节数（6字节数据+2字节PEC）
   uint8_t cmd[4];             // 命令数组（2字节命令+2字节PEC）
   uint8_t* rx_data;           // 接收数据的指针
   uint8_t pec_error = 0;      // PEC错误标志
   uint16_t cmd_pec;           // 命令的PEC值
   uint16_t received_pec;      // 接收的PEC值
   uint16_t data_pec;          // 计算的PEC值

   rx_data = (uint8_t*)malloc(reg_len * total_ic * sizeof(uint8_t));  // 分配接收缓冲区

   // 1. 根据寄存器号设置命令
   if (reg == 1) {  // 读取状态组A
      cmd[0] = 0x00;
      cmd[1] = 0x10;
   } else if (reg == 2) {  // 读取状态组B
      cmd[0] = 0x00;
      cmd[1] = 0x12;
   } else {  // 默认读取辅助组A
      cmd[0] = 0x00;
      cmd[1] = 0x10;
   }

   // 2. 计算命令的PEC
   cmd_pec = pec15_calc(2, cmd);      // 计算2字节命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位

   // 3. 唤醒isoSPI（可选）
   wakeup_sleep();  // 唤醒iso及LTC6804芯片

   // 4. 发送命令并接收数据
   cs_ltc6804(0);                                          // 拉低片选
   spi_write_read(cmd, 4, rx_data, (reg_len * total_ic));  // 发送4字节命令，接收总字节数=每个寄存器字节数×芯片数
   cs_ltc6804(1);                                          // 拉高片选

   // 5. 校验PEC并存储数据
   for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) {  // 遍历每个芯片
      // 将接收到的数据加载到stat_data数组
      for (uint8_t current_byte = 0; current_byte < 6; current_byte++) {                        // 只加载每芯片数据的前六个字节，不加载对应pec
         stat_data[current_ic][current_byte] = rx_data[current_byte + (current_ic * reg_len)];  // 按照芯片和字节索引存储
      }
      // 校验PEC
      received_pec = (rx_data[6 + (current_ic * reg_len)] << 8) + rx_data[7 + (current_ic * reg_len)];  // 提取接收的PEC
      data_pec = pec15_calc(6, &stat_data[current_ic][0]);                                              // 计算6字节状态数据的PEC
      if (received_pec != data_pec) {
         pec_error = 1;  // 标记错误
      }
   }

   free(rx_data);  // 释放接收缓冲区并返回PEC错误标志
   return (pec_error);
}

/**
 * @brief   清除LTC6804的电池电压寄存器,该命令会清除电池电压寄存器，并将所有值初始化为1。命令发送后，寄存器读取值为0xFF（十六进制）。
 */
void LTC6804_clrcell(void) {
   /*
   LTC6804_clrcell函数流程：
   1. 将clrcell命令加载到cmd数组
   2. 计算clrcell命令的PEC并加载到cmd数组
   3. 唤醒isoSPI端口（若已知状态，可省略）
   4. 向LTC6804菊花链发送广播clrcell命令
   */

   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   // 1. 设置清除电池寄存器命令
   cmd[0] = 0x07;  // 命令高字节
   cmd[1] = 0x11;  // 命令低字节（清除电池寄存器命令）

   // 2. 计算命令的PEC
   cmd_pec = pec15_calc(2, cmd);      // 计算2字节命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位

   // 3. 唤醒isoSPI（可选）
   wakeup_sleep();  // 唤醒iso及LTC6804芯片

   // 4. 发送命令
   cs_ltc6804(0);                 // 拉低片选
   spi_write_read(cmd, 4, 0, 0);  // 发送4字节命令（无接收数据）
   cs_ltc6804(1);                 // 拉高片选
}

/**
 * @brief   清除LTC6804的辅助寄存器,该命令会清除辅助寄存器，并将所有值初始化为1。命令发送后，寄存器读取值为0xFF（十六进制）。
 */
void LTC6804_clraux(void) {
   /*
   LTC6804_clraux函数流程：
   1. 将clraux命令加载到cmd数组
   2. 计算clraux命令的PEC并加载到cmd数组
   3. 唤醒isoSPI端口（若已知状态，可省略）
   4. 向LTC6804菊花链发送广播clraux命令
   */

   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   // 1. 设置清除辅助寄存器命令
   cmd[0] = 0x07;  // 命令高字节
   cmd[1] = 0x12;  // 命令低字节（清除辅助寄存器命令）

   // 2. 计算命令的PEC
   cmd_pec = pec15_calc(2, cmd);      // 计算2字节命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位

   // 3. 唤醒isoSPI（可选）
   wakeup_sleep();  // 唤醒iso及LTC6804芯片

   // 4. 发送命令
   cs_ltc6804(0);                 // 拉低片选
   spi_write_read(cmd, 4, 0, 0);  // 发送4字节命令（无接收数据）
   cs_ltc6804(1);                 // 拉高片选
}

/**
 * @brief   清除LTC6804的状态寄存器,该命令会清除状态寄存器，并将所有值初始化为1。命令发送后，寄存器读取值为0xFF（十六进制）。
 */
void LTC6804_clrstat(void) {
   /*
   LTC6804_clrstat函数流程：
   1. 将clrstat命令加载到cmd数组
   2. 计算clrstat命令的PEC并加载到cmd数组
   3. 唤醒isoSPI端口（若已知状态，可省略）
   4. 向LTC6804菊花链发送广播clrstat命令
   */

   uint8_t cmd[4];    // 命令数组（2字节命令+2字节PEC）
   uint16_t cmd_pec;  // 命令的PEC值

   // 1. 设置清除状态寄存器命令
   cmd[0] = 0x07;  // 命令高字节
   cmd[1] = 0x13;  // 命令低字节（清除状态寄存器命令）

   // 2. 计算命令的PEC
   cmd_pec = pec15_calc(2, cmd);      // 计算2字节命令的PEC
   cmd[2] = (uint8_t)(cmd_pec >> 8);  // PEC高8位
   cmd[3] = (uint8_t)(cmd_pec);       // PEC低8位

   // 3. 唤醒isoSPI（可选）
   wakeup_sleep();  // 唤醒iso及LTC6804芯片

   // 4. 发送命令
   cs_ltc6804(0);                 // 拉低片选
   spi_write_read(cmd, 4, 0, 0);  // 发送4字节命令（无接收数据）
   cs_ltc6804(1);                 // 拉高片选
}

/**
 * @brief   通过SPI端口发送字节数组
 * @param   uint8_t len：要发送的字节长度
 * @param   uint8_t data[]：要发送的数据数组
 */
void spi_write_array(uint8_t len, uint8_t data[]) {
   for (uint8_t i = 0; i < len; i++) {
      spi1_read_write_byte((uint8_t)data[i]);  // 逐个字节发送数据
   }
}

/**
 * @brief   通过SPI端口读写字节
 * @param   uint8_t tx_data[]：要发送的数据数组
 * @param   uint8_t tx_len：发送数据的长度
 * @param   uint8_t rx_data：存储接收数据的数组
 * @param   uint8_t rx_len：要接收的字节数
 */
void spi_write_read(uint8_t tx_Data[], uint8_t tx_len, uint8_t* rx_data, uint8_t rx_len) {
   for (uint8_t i = 0; i < tx_len; i++) {  // 逐个发送字节
      spi1_read_write_byte(tx_Data[i]);    // 发送一个字节
   }
   for (uint8_t i = 0; i < rx_len; i++) {                // 逐个接收字节
      rx_data[i] = (uint8_t)spi1_read_write_byte(0xFF);  // 读取一个字节（发送0xFF作为填充）
   }
}

/**
 * @brief  将isoSPI与LTC6804从空闲状态唤醒
 */
void wakeup_sleep(void) {
   cs_ltc6804(0);  // 拉低片选
   delay_ms(3);    // 延时1ms（确保LTC6804进入待机模式）
   cs_ltc6804(1);  // 拉高片选
}

/**
 * @brief   计算并返回CRC15校验和
 * @param   uint8_t len: 用于计算PEC的数据数组长度
 * @param   uint8_t data[] : 用于生成PEC的数据数组
 * @return  计算得到的15位PEC（无符号整数）
 */
uint16_t pec15_calc(uint8_t len, uint8_t* data) {
   uint16_t remainder, addr;  // remainder=余数，addr=CRC表地址
   remainder = 16;            // 初始化PEC

   for (uint8_t i = 0; i < len; i++) {                  // 遍历数据数组的每个字节
      addr = ((remainder >> 7) ^ data[i]) & 0xff;       // 计算CRC表地址
      remainder = (remainder << 8) ^ crc15Table[addr];  // 更新余数（查表法）
   }
   return (remainder * 2);  // CRC15的最低位为0，因此余数需×2
}
