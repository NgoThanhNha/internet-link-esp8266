/**
 ******************************************************************************
 * @author: Nark
 * @date:   05/06/2024
 ******************************************************************************
**/

#ifndef __TASK_MQTT_H__
#define __TASK_MQTT_H__

#include "stk.h"

#define MQTT_UNINITIALIZED            (0x00)
#define MQTT_INITIALIZED              (0x01)

extern void task_mqtt_handler(stk_msg_t* msg);
extern void polling_mqtt();

#endif /* __TASK_MQTT_H__ */