/**
 ***********************************************************************************************************************
 * @author: Nark
 * @date:   03/05/2024
 * STK (Simple Tasker Kernel) is a task scheduling mechanism based on the event-driven programming model includes:
    * Post signals to tasks
    * Execute tasks based on signal queue
 ***********************************************************************************************************************
**/

#ifndef __STK_H__
#define __STK_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdio.h>
#include <stdint.h>

#include "message.h"

#define STK_VERSION             "1.0"

#define STK_DISABLE             (0x00)
#define STK_ENABLE              (0x01)

typedef void (*pf_task)(stk_msg_t*);
typedef uint8_t task_id_t;

typedef struct {
	stk_msg_t* qhead;
	stk_msg_t* qtail;
} signal_queue_t;

typedef struct {
    task_id_t task_id;
    pf_task task_handler;
} task_t;

extern void task_post(task_id_t task_id, stk_msg_t* msg);
extern void task_post_pure_msg(task_id_t des_task_id, uint8_t signal);

extern void task_create(task_t* task_table_create);
extern void task_scheduler();
extern uint8_t get_size_of_task_list();

#ifdef __cplusplus
}
#endif

#endif /* __STK_H__ */