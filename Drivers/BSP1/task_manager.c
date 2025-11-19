/**
 * @file    task_manager.c
 * @brief   简单任务管理器实现（基于 HAL_GetTick 的轮询调度）
 *
 * 说明：
 * - 该模块适用于裸机主循环调用场景，任务在 TM_Run 中同步执行。
 * - 若任务可能阻塞或耗时，请将其改为只置位标志或使用更高阶的调度机制。
 */

#include "task_manager.h"
#include <string.h>
#include "stm32f1xx_hal.h"

/**
 * @brief   管理器内部结构
 */
typedef struct {
   TM_Task tasks[TASK_MANAGER_MAX_TASKS];
   uint8_t used[TASK_MANAGER_MAX_TASKS];
} TM_Manager;

// 定义一个静态的任务管理器实例
static TM_Manager tm;

/**
 * @brief   初始化任务管理器
 * @note    在 HAL tick 可用后调用（例如系统初始化完成）
 */
void TM_Init(void) {
   // 清零管理器状态
   memset(&tm, 0, sizeof(tm));
   // memset作用是将tm结构体的内存空间全部设置为0，初始化所有任务槽为空闲状态。
}

/**
 * @brief   注册任务
 * @param   func        任务回调函数，不能为空
 * @param   arg         传递给任务的参数指针（可为 NULL）
 * @param   period_ms   周期（毫秒），为 0 表示立即执行一次
 * @param   repeat      是否周期执行（1=周期，0=一次性）
 * @return  int         返回任务 ID（>=0）表示注册成功，返回 -1 表示失败（无可用插槽）
 */
int TM_Register(TaskFunc func, void* arg, uint32_t period_ms, uint8_t repeat) {
   if (!func)
      return -1;
   for (int i = 0; i < TASK_MANAGER_MAX_TASKS; i++) {
      if (!tm.used[i]) {
         tm.used[i] = 1;
         tm.tasks[i].func = func;
         tm.tasks[i].arg = arg;
         tm.tasks[i].period_ms = period_ms;
         tm.tasks[i].last_run = HAL_GetTick();
         tm.tasks[i].enabled = 1;
         tm.tasks[i].repeat = repeat ? 1 : 0;
         return i;
      }
   }
   return -1;  // 无空闲插槽
}

/**
 * @brief   注销任务
 * @param   task_id     任务 ID（由 TM_Register 返回）
 */
void TM_Unregister(int task_id) {
   if (task_id < 0 || task_id >= TASK_MANAGER_MAX_TASKS)
      return;
   tm.used[task_id] = 0;
   tm.tasks[task_id].func = NULL;
}

/**
 * @brief   启用任务
 * @param   task_id     任务 ID
 */
void TM_Enable(int task_id) {
   if (task_id < 0 || task_id >= TASK_MANAGER_MAX_TASKS)
      return;
   if (tm.used[task_id])
      tm.tasks[task_id].enabled = 1;
}

/**
 * @brief   禁用任务
 * @param   task_id     任务 ID
 */
void TM_Disable(int task_id) {
   if (task_id < 0 || task_id >= TASK_MANAGER_MAX_TASKS)
      return;
   if (tm.used[task_id])
      tm.tasks[task_id].enabled = 0;
}

/**
 * @brief   获取任务描述（仅用于调试/扩展）
 * @param   task_id     任务 ID
 * @return  TM_Task*    返回任务指针，若不存在则返回 NULL
 */
TM_Task* TM_GetTask(int task_id) {
   if (task_id < 0 || task_id >= TASK_MANAGER_MAX_TASKS)
      return NULL;
   if (!tm.used[task_id])
      return NULL;
   return &tm.tasks[task_id];
}

/**
 * @brief   调度并执行已到期的任务
 * @note    该函数应在主循环中频繁调用；任务在此处同步调用执行
 */
void TM_Run(void) {
   uint32_t now = HAL_GetTick();
   for (int i = 0; i < TASK_MANAGER_MAX_TASKS; i++) {
      if (!tm.used[i])
         continue;
      TM_Task* t = &tm.tasks[i];
      if (!t->enabled || !t->func)
         continue;

      uint32_t elapsed = now - t->last_run;
      if (t->period_ms == 0) {
         // 一次性任务：立即执行并注销（若 repeat==0）
         t->last_run = now;
         t->func(t->arg);
         if (!t->repeat) {
            tm.used[i] = 0;
         }
         continue;
      }

      if (elapsed >= t->period_ms) {
         t->last_run = now;
         t->func(t->arg);
         if (!t->repeat) {
            // 一次性任务执行后禁用/注销
            tm.used[i] = 0;
         }
      }
   }
}
