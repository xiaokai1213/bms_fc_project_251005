#include "delay.h"

/*

// 毫秒级的延时
void delay_ms(u16 time) {
   u16 i = 0;
   while (time--) {
      i = 12000;
      while (i--)
         ;
   }
}
*/

// 粗延时函数，微秒
void delay_us(uint32_t time) {
   u16 i = 0;
   while (time--) {
      i = 10;
      while (i--)
         ;
   }
}

// 毫秒延时
void delay_ms(uint16_t time) {
   u16 i = 0;
   while (time--) {
      i = 12000;
      while (i--)
         ;
   }
}
