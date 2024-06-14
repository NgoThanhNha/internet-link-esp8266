/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#include "app.h"

#include "stk.h"
#include "message.h"
#include "timer.h"

#include "platform.h"

#include "app_dbg.h"
#include "task_list.h"
#include "task_network.h"

/* arduino framwork */
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

wifi_attr_t wifi_info;
mqtt_attr_t mqtt_info;

void main_app() {
    delay(1000);
    /* stk init */
    msg_init();
    timer_init();
    task_create(app_task_table);

    /* uart init */
    console_init();

    /* banner */
    sys_banner();

    task_post_pure_msg(TASK_NETWORK_ID, ESP_WL_GET_WIFI_INFO);
    task_post_pure_msg(TASK_NETWORK_ID, ESP_WL_CONNECT);
}