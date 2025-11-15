#include "data_processing.h"
#include "ltc6804_1_task.h"

/**
 * @brief   电压数据处理函数,检测电压是否异常
 * @return uint8_t:错误值
 * 0:无错误
 * 1:电压异常
 */
uint8_t process_voltage_data(void) {
   uint8_t error = 0;
   bat_pack_data[0].voltage_data = cv_h_ltc6804[0].C01V;
   bat_pack_data[1].voltage_data = cv_h_ltc6804[0].C02V;
   bat_pack_data[2].voltage_data = cv_h_ltc6804[0].C03V;
   bat_pack_data[3].voltage_data = cv_h_ltc6804[0].C04V;
   bat_pack_data[4].voltage_data = cv_h_ltc6804[0].C05V;

   bat_pack_data[5].voltage_data = cv_h_ltc6804[0].C07V;
   bat_pack_data[6].voltage_data = cv_h_ltc6804[0].C08V;
   bat_pack_data[7].voltage_data = cv_h_ltc6804[0].C09V;
   bat_pack_data[8].voltage_data = cv_h_ltc6804[0].C10V;
   bat_pack_data[9].voltage_data = cv_h_ltc6804[0].C11V;

   bat_pack_data[10].voltage_data = cv_h_ltc6804[1].C01V;
   bat_pack_data[11].voltage_data = cv_h_ltc6804[1].C02V;
   bat_pack_data[12].voltage_data = cv_h_ltc6804[1].C03V;
   bat_pack_data[13].voltage_data = cv_h_ltc6804[1].C04V;
   bat_pack_data[14].voltage_data = cv_h_ltc6804[1].C05V;

   bat_pack_data[15].voltage_data = cv_h_ltc6804[1].C07V;
   bat_pack_data[16].voltage_data = cv_h_ltc6804[1].C08V;
   bat_pack_data[17].voltage_data = cv_h_ltc6804[1].C09V;
   bat_pack_data[18].voltage_data = cv_h_ltc6804[1].C10V;
   bat_pack_data[19].voltage_data = cv_h_ltc6804[1].C11V;

   for (int i = 0; i < cell_num; i++) {                                                                            // 轮询每个电芯电压
      if (bat_pack_data[i].voltage_data > max_cell_voltage || bat_pack_data[i].voltage_data < min_cell_voltage) {  // 每个电压与阈值电压进行比较
         bat_pack_data[i].voltage_anomaly = 1;                                                                     // 有异常电压则标记
         error = 1;                                                                                                // 有异常电压返回错误1
      }
      bat_pack_data[i].voltage_can_send = 1;  // 每个电压数据标记发送
   }
   return error;  // 无异常电压返回0
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
