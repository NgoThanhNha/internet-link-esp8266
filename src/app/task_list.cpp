/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#include "task_list.h"

#include "stk.h"
#include "message.h"
#include "timer.h"

#include "link_phy.h"
#include "task_network.h"
#include "task_mqtt.h"
#include "task_sm.h"

task_t app_task_table[] = {
    /*************************************************************************/
	/* SYSTEM TASKS */
	/*************************************************************************/
    {TASK_TIMER_TICK_ID,              task_timer_handler},

    /*************************************************************************/
	/* APP TASKS */
	/*************************************************************************/
    {TASK_NETWORK_ID,                 task_network_handler},
    {TASK_MQTT_ID,                    task_mqtt_handler},
    {LINK_PHY_ID,                     link_phy_handler},
    {TASK_SM_ID,                      task_sm_handler},

    /*************************************************************************/
	/* END OF TABLE */
	/*************************************************************************/
    {STK_EOT_TASK_ID,                 (pf_task)0},
};