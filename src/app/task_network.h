/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#ifndef __TASK_NETWORK_H__
#define __TASK_NETWORK_H__

#include "stk.h"

#include <ESP8266WebServer.h>

#define EEPROM_SIZE                     (96)
#define WIFI_CONNECT_TIMEOUT            (60)

extern ESP8266WebServer server;

extern void read_wifi_info_eeprom();
extern void save_wifi_config(uint8_t length_ssid, uint8_t length_password);
extern void task_network_handler(stk_msg_t* msg);

#endif /* __TASK_NETWORK_H__ */