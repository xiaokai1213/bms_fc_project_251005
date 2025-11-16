#include "data_processing.h"
#include "ltc6804_1_task.h"

/**
 * @brief   电压数据处理函数,检测电压是否异常
 * @return uint8_t:错误值
 * 0:无错误
 * 1:电压异常
 */
uint8_t process_voltage_data(void) {
   uint16_t* voltages[cell_num];  // 指针数组,存储20个电芯电压数据地址,方便后续操作
   voltages[0] = &cv_h_ltc6804[0].C01V;
   voltages[1] = &cv_h_ltc6804[0].C02V;
   voltages[2] = &cv_h_ltc6804[0].C03V;
   voltages[3] = &cv_h_ltc6804[0].C04V;
   voltages[4] = &cv_h_ltc6804[0].C05V;

   voltages[5] = &cv_h_ltc6804[0].C07V;
   voltages[6] = &cv_h_ltc6804[0].C08V;
   voltages[7] = &cv_h_ltc6804[0].C09V;
   voltages[8] = &cv_h_ltc6804[0].C10V;
   voltages[9] = &cv_h_ltc6804[0].C11V;

   voltages[10] = &cv_h_ltc6804[1].C01V;
   voltages[11] = &cv_h_ltc6804[1].C02V;
   voltages[12] = &cv_h_ltc6804[1].C03V;
   voltages[13] = &cv_h_ltc6804[1].C04V;
   voltages[14] = &cv_h_ltc6804[1].C05V;

   voltages[15] = &cv_h_ltc6804[1].C07V;
   voltages[16] = &cv_h_ltc6804[1].C08V;
   voltages[17] = &cv_h_ltc6804[1].C09V;
   voltages[18] = &cv_h_ltc6804[1].C10V;
   voltages[19] = &cv_h_ltc6804[1].C11V;

   for (int i = 0; i < cell_num; i++) {                                          // 轮询每个电芯电压
      if (*voltages[i] > max_cell_voltage || *voltages[i] < min_cell_voltage) {  // 每个电压与阈值电压进行比较
         return 1;                                                               // 有异常电压直接返回报警1,并退出函数
      }
   }
   return 0;  // 无异常电压返回0
}

/*
uint16_t* voltages[cell_num];         // 指针数组
    voltages[0]
      voltages[1]
      voltages[2]
      voltages[3]
      voltages[4]
      voltages[5]
      voltages[6]
      voltages[7]
      voltages[8]
      voltages[9]
      voltages[10]
      voltages[11]
      voltages[12]
      voltages[13]
      voltages[14]
      voltages[15]
      voltages[16]
      voltages[17]
      voltages[18]
      voltages[19]
*/
