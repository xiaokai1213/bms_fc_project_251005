/**
 * @file    task_manager.h
 * @brief   简单任务管理器头文件（基于 HAL_GetTick 的轮询调度）
 *
 * 说明：
 * - 该模块适用于裸机主循环调用场景，任务在 TM_Run 中同步执行。
 * - 若任务可能阻塞或耗时，请将其改为只置位标志或使用更高阶的调度机制。
 */

#ifndef __TASK_MANAGER_H
#define __TASK_MANAGER_H

#include <stddef.h>
#include <stdint.h>

/** 最大任务数，可按需调整 */
#define TASK_MANAGER_MAX_TASKS 16

/** 任务函数原型：接收一个 void* 参数指针 */
typedef void (*TaskFunc)(void* arg);

/** 任务描述结构体 */
typedef struct {
   TaskFunc func;      /**< 任务回调 */
   void* arg;          /**< 回调参数指针（可为 NULL） */
   uint32_t period_ms; /**< 周期（ms），为0表示立即执行一次 */
   uint64_t next_run;  /**< 下次运行时间（以 `sys_time` 为基准，单位 ms） */
   uint8_t enabled;    /**< 启用标志（1=启用） */
   uint8_t repeat;     /**< 是否周期执行（1=周期，0=一次性） */
} TM_Task;

/**
 * @brief  初始化任务管理器
 * @note   在 HAL tick 可用后调用（例如系统初始化完成）
 * @retval 无
 */
void TM_Init(void);

/**
 * @brief  注册任务
 * @param  func       任务回调函数，不能为空
 * @param  arg        传递给任务的参数指针（可为 NULL）
 * @param  period_ms  周期（ms），为 0 表示立即执行一次
 * @param  repeat     是否周期执行（1=周期，0=一次性）
 * @retval int        返回 task id(>=0) 表示成功；返回 -1 表示失败（无可用插槽或参数错误）
 */
/**
 * @brief  注册任务
 * @param  func       任务回调函数，不能为空
 * @param  arg        传递给任务的参数指针（可为 NULL）
 * @param  period_ms  周期（ms），为 0 表示一次性任务
 * @param  repeat     是否周期执行（1=周期，0=一次性）
 * @param  delay_ms   首次延迟（ms），0 表示立即按 period_ms/next_run 计算
 * @retval int        返回 task id(>=0) 表示成功；返回 -1 表示失败（无可用插槽或参数错误）
 */
int TM_Register(TaskFunc func, void* arg, uint32_t period_ms, uint8_t repeat, uint32_t delay_ms);

/**
 * @brief  注销任务
 * @param  task_id    任务 ID（由 TM_Register 返回）
 * @retval 无
 */
void TM_Unregister(int task_id);

/**
 * @brief  启用任务
 * @param  task_id    任务 ID
 * @retval 无
 */
void TM_Enable(int task_id);

/**
 * @brief  禁用任务
 * @param  task_id    任务 ID
 * @retval 无
 */
void TM_Disable(int task_id);

/**
 * @brief  获取任务指针（用于调试/扩展）
 * @param  task_id    任务 ID
 * @retval TM_Task*   返回任务指针，若不存在则返回 NULL
 */
TM_Task* TM_GetTask(int task_id);

/**
 * @brief  在主循环中调用，调度并执行已到期的任务
 * @retval 无
 */
void TM_Run(void);

#endif /* __TASK_MANAGER_H */
