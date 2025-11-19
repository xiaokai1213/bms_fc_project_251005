/* ltc_fsm.h - Simple FSM wrapper for LTC6804 periodic sampling */
#ifndef _LTC_FSM_H
#define _LTC_FSM_H

#include <stdint.h>

typedef enum { LTE_NONE = 0, LTE_START_SAMPLING, LTE_ADC_READY, LTE_TIMEOUT, LTE_ERROR_EVENT } ltc_fsm_event_t;

/**
 * 初始化 LTC FSM 并注册必要的 TM 周期任务。
 * - 内部会注册一个短周期的 step（默认 50ms）用于处理事件/状态
 * - 内部会注册一个 500ms 的周期触发事件以保留原先每 500ms 采样逻辑
 */
void ltc_fsm_init(void);

/**
 * 向 FSM 投递事件（可在中断/任务中调用）
 */
void ltc_fsm_post_event(ltc_fsm_event_t ev);

#endif  // _LTC_FSM_H
