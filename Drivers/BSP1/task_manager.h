#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

#include <stdint.h>

// 最大任务数，可按需调整
#ifndef TASK_MANAGER_MAX_TASKS
#define TASK_MANAGER_MAX_TASKS 16
#endif

// 任务函数类型定义：空函数指针，参数为 void*，无返回值
typedef void (*TaskFunc)(void* arg);

/**
 * @brief  任务描述结构
 * @note   该结构由任务管理器内部使用，包含运行信息与控制位
 */
typedef struct {
   TaskFunc func;       // 任务函数（回调），签名 void func(void *arg)
   void* arg;           // 传递给任务的参数指针
   uint32_t period_ms;  // 任务周期（毫秒），为 0 表示立即执行一次
   uint32_t last_run;   // 上次运行的时间戳（HAL_GetTick 单位毫秒）
   uint8_t enabled;     // 任务是否被启用（1=启用，0=禁用）
   uint8_t repeat;      // 是否为周期任务（1=周期，0=一次性）
} TM_Task;

/**
 * @brief   初始化任务管理器
 * @note    在系统启动并且 HAL tick（HAL_GetTick）可用后调用
 */
void TM_Init(void);

/**
 * @brief   注册任务
 * @param   func        任务回调函数，不能为空
 * @param   arg         传递给任务的参数指针（可为 NULL）
 * @param   period_ms   周期（毫秒），为 0 表示立即执行一次
 * @param   repeat      是否周期执行（1=周期，0=一次性）
 * @return  int         返回任务 ID（>=0）表示注册成功，返回 -1 表示失败（无可用插槽）
 */
int TM_Register(TaskFunc func, void* arg, uint32_t period_ms, uint8_t repeat);

/**
 * @brief   注销任务
 * @param   task_id     任务 ID（由 TM_Register 返回）
 */
void TM_Unregister(int task_id);

/**
 * @brief   启用任务
 * @param   task_id     任务 ID
 */
void TM_Enable(int task_id);

/**
 * @brief   禁用任务
 * @param   task_id     任务 ID
 */
void TM_Disable(int task_id);

/**
 * @brief   调度并执行已到期的任务
 * @note    该函数应在主循环中频繁调用；任务在此处同步调用执行
 */
void TM_Run(void);

/**
 * @brief   获取任务描述（仅用于调试/扩展）
 * @param   task_id     任务 ID
 * @return  TM_Task*    返回任务指针，若不存在则返回 NULL
 */
TM_Task* TM_GetTask(int task_id);

#endif  // __TASK_MANAGER_H
