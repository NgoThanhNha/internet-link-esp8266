/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include <stdint.h>
#include "stk.h"

enum {
    /* KERNEL TASKS */
    TASK_TIMER_TICK_ID,

    /* APP TASKS */
    TASK_NETWORK_ID,
    TASK_MQTT_ID,
    LINK_PHY_ID,
    TASK_SM_ID,

    /* END OF TABLE */
    STK_EOT_TASK_ID,
};

extern task_t app_task_table[];

#endif /* __TASK_LIST_H__ */