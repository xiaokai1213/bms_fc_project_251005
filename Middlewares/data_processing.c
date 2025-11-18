#include "data_processing.h"
#include "ltc6804_1_task.h"

/**
 * @brief   电压数据处理函数,检测电压是否异常
 * @return uint8_t:错误值
 * 0:无错误
 * 1:电压异常
 */
uint8_t process_voltage_data(void) {
   uint16_t* voltages_p[cell_num];  // 指针数组,存储20个电芯电压数据地址,方便后续操作
   voltages_p[0] = &cv_h_ltc6804[0].C01V;
   voltages_p[1] = &cv_h_ltc6804[0].C02V;
   voltages_p[2] = &cv_h_ltc6804[0].C03V;
   voltages_p[3] = &cv_h_ltc6804[0].C04V;
   voltages_p[4] = &cv_h_ltc6804[0].C05V;

   voltages_p[5] = &cv_h_ltc6804[0].C07V;
   voltages_p[6] = &cv_h_ltc6804[0].C08V;
   voltages_p[7] = &cv_h_ltc6804[0].C09V;
   voltages_p[8] = &cv_h_ltc6804[0].C10V;
   voltages_p[9] = &cv_h_ltc6804[0].C11V;

   voltages_p[10] = &cv_h_ltc6804[1].C01V;
   voltages_p[11] = &cv_h_ltc6804[1].C02V;
   voltages_p[12] = &cv_h_ltc6804[1].C03V;
   voltages_p[13] = &cv_h_ltc6804[1].C04V;
   voltages_p[14] = &cv_h_ltc6804[1].C05V;

   voltages_p[15] = &cv_h_ltc6804[1].C07V;
   voltages_p[16] = &cv_h_ltc6804[1].C08V;
   voltages_p[17] = &cv_h_ltc6804[1].C09V;
   voltages_p[18] = &cv_h_ltc6804[1].C10V;
   voltages_p[19] = &cv_h_ltc6804[1].C11V;

   for (int i = 0; i < cell_num; i++) {                                              // 轮询每个电芯电压
      if (*voltages_p[i] > max_cell_voltage || *voltages_p[i] < min_cell_voltage) {  // 每个电压与阈值电压进行比较
         return 1;                                                                   // 有异常电压直接返回报警1,并退出函数
      }
   }
   return 0;  // 无异常电压返回0
}

/**
 * @brief   温度数据处理函数,检测温度是否异常
 * @return uint8_t:错误值
 * 0:无错误
 * 1:温度异常
 */
uint8_t process_temperature_data(void) {
   uint16_t* temperature_p[temp_num];
   temperature_p[0] = &av_h_ltc6804[0].G1V;
   temperature_p[1] = &av_h_ltc6804[0].G2V;
   temperature_p[2] = &av_h_ltc6804[0].G3V;
   temperature_p[3] = &av_h_ltc6804[0].G4V;
   temperature_p[4] = &av_h_ltc6804[0].G5V;

   temperature_p[5] = &av_h_ltc6804[1].G1V;
   temperature_p[6] = &av_h_ltc6804[1].G2V;
   temperature_p[7] = &av_h_ltc6804[1].G3V;
   temperature_p[8] = &av_h_ltc6804[1].G4V;
   temperature_p[9] = &av_h_ltc6804[1].G5V;

   for (int i = 0; i < temp_num; i++) {                // 轮询每个温度采集点
      if (*temperature_p[i] > max_cell_temperature) {  // 每个温度与阈值温度进行检测
         return 1;                                     // 有异常温度直接返回报警1,并退出函数
      }
   }
   return 0;  // 无异常温度返回0
}
