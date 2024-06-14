/**
 ******************************************************************************
 * @author: Nark
 * @date:   05/06/2024
 ******************************************************************************
**/

#ifndef __TASK_POLLING_H__
#define __TASK_POLLING_H__

#include <stdint.h>
#include "stk.h"

extern void task_polling_run();
extern uint8_t get_size_byte_uart_receive();

#endif /* __TASK_POLLING_H__ */