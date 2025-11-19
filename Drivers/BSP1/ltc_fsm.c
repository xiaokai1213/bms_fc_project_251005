/* ltc_fsm.c - Lightweight FSM to manage LTC6804 non-blocking sampling */
#include "ltc_fsm.h"
#include <string.h>
#include "ltc6804_1_task.h"
#include "main.h"
#include "task_manager.h"

extern volatile uint64_t sys_time;  // 从 main.c 导出

#define EVENT_QUEUE_SIZE     8
#define FSM_STEP_PERIOD_MS   50
#define FSM_SAMPLE_PERIOD_MS 500
#define LTC_ADC_DELAY_MS     10

static volatile ltc_fsm_event_t event_q[EVENT_QUEUE_SIZE];
static volatile uint8_t q_head = 0, q_tail = 0;

static inline int event_queue_full(void) {
   return ((q_tail + 1) % EVENT_QUEUE_SIZE) == q_head;
}

static inline int event_queue_empty(void) {
   return q_head == q_tail;
}

void ltc_fsm_post_event(ltc_fsm_event_t ev) {
   __disable_irq();
   if (!event_queue_full()) {
      event_q[q_tail] = ev;
      q_tail = (q_tail + 1) % EVENT_QUEUE_SIZE;
   }
   __enable_irq();
}

static ltc_fsm_event_t event_dequeue(void) {
   ltc_fsm_event_t ev = LTE_NONE;
   __disable_irq();
   if (!event_queue_empty()) {
      ev = event_q[q_head];
      q_head = (q_head + 1) % EVENT_QUEUE_SIZE;
   }
   __enable_irq();
   return ev;
}

typedef enum { S_IDLE = 0, S_TRIGGER_ADC, S_WAIT_ADC, S_READ_ADC, S_PROCESS, S_ERROR } ltc_state_t;

typedef struct {
   ltc_state_t state;
   uint64_t wait_until;
} ltc_ctx_t;

static ltc_ctx_t g_ctx;

static void ltc_start_conversion(void) {
   // 触发电池电压 ADC（不阻塞）
   LTC6804_adcv(MD_NORMAL, DCP_DISABLED, CH_ALL);
}

// 一次性延迟读取任务，由 TM 在 delay 到期时执行
static void ltc_read_task(void* arg) {
   (void)arg;
   uint8_t pec = read_LTC6804_Battery_voltage_registers(total_ic, cv_h_ltc6804);
   if (pec != 0) {
      // 将错误作为事件投递
      ltc_fsm_post_event(LTE_ERROR_EVENT);
      return;
   }
   ltc_cv_updated = 1;
   ltc_fsm_post_event(LTE_ADC_READY);
}

// 周期触发器：每 FSM_SAMPLE_PERIOD_MS 投递一个 START 事件
static void ltc_periodic_trigger(void* arg) {
   (void)arg;
   ltc_fsm_post_event(LTE_START_SAMPLING);
}

// FSM step：由 TM 定期调用，短时间内驱动状态迁移
static void ltc_fsm_step(void* arg) {
   (void)arg;
   ltc_fsm_event_t ev = event_dequeue();
   uint64_t now = sys_time;

   switch (g_ctx.state) {
      case S_IDLE:
         if (ev == LTE_START_SAMPLING) {
            ltc_start_conversion();
            // 注册一次性延迟读任务，delay = LTC_ADC_DELAY_MS
            TM_Register(ltc_read_task, NULL, 0, 0, LTC_ADC_DELAY_MS);
            g_ctx.state = S_WAIT_ADC;
            g_ctx.wait_until = now + LTC_ADC_DELAY_MS + 5;  // 余量
         }
         break;

      case S_WAIT_ADC:
         if (ev == LTE_ADC_READY) {
            g_ctx.state = S_PROCESS;
         } else if (now >= g_ctx.wait_until) {
            // 超时，尝试读取一次（容错）
            TM_Register(ltc_read_task, NULL, 0, 0, 0);
            g_ctx.state = S_WAIT_ADC;  // 继续等待事件
         }
         break;

      case S_PROCESS:
         // 读取已完成并设置了 ltc_cv_updated，处理后回到空闲
         g_ctx.state = S_IDLE;
         break;

      case S_ERROR:
         // 简单错误恢复：清状态回到空闲
         g_ctx.state = S_IDLE;
         break;

      default:
         g_ctx.state = S_IDLE;
         break;
   }
}

void ltc_fsm_init(void) {
   memset(&g_ctx, 0, sizeof(g_ctx));
   g_ctx.state = S_IDLE;
   // 注册 FSM step（周期短，频率可调）
   TM_Register(ltc_fsm_step, NULL, FSM_STEP_PERIOD_MS, 1, 0);
   // 注册周期触发器（保留原先每 500ms 采样行为）
   TM_Register(ltc_periodic_trigger, NULL, FSM_SAMPLE_PERIOD_MS, 1, 0);
}
