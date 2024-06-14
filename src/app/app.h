/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#ifndef __APP_H__
#define __APP_H__

#include "platform.h"

#define APP_VERSION         "1.0"
/** 
 * App version: 1.0
    * Get data from main MCU and post to Thingsboard  
**/

extern void main_app();
    
enum app_signals {
    /* TASK NETWORK */
    ESP_WL_GET_WIFI_INFO,
    ESP_WL_CHECK_CONNECT,
    ESP_WL_CONNECT,
    ESP_WL_WAITING_CONNECT,
    ESP_MQTT_INIT,
    ESP_MQTT_RECONNECT,
    ESP_MQTT_SEND_MSG,

    /* UART LINK */
    LINK_PHY_PARSER_BUFF,
    LINK_PHY_FW_MSG,
    LINK_PHY_PARSER_DATA,
    LINK_PHY_PARSER_WL_INFO,

    LINK_PHY_RES_WL_INFO,
};

enum task_sm_signals {
    SIG_SM_REQ_WIFI_INFO = 0x01,
    SIG_SM_RES_WIFI_INFO,

    SIG_SM_REQ_WIFI_STATUS,
    SIG_SM_RES_WIFI_STATUS,
    SIG_SM_REQ_WIFI_RECONNECT,
    SIG_SM_REQ_WIFI_RECHANGE,

    SIG_SM_RES_WIFI_CONNECTED,
    SIG_SM_RES_WIFI_DISCONNECTED,
};

enum wifi_status {
    WL_STATE_DISCONNECTED = 0x01,
    WL_STATE_CONNECTED,
    WL_STATE_RECONNECTING,
    WL_STATE_RECHANGING,
};

typedef struct {
    uint8_t wifi_status;
    char ssid[30];
    char password[30];
} wifi_attr_t;

typedef struct {
    const char* mqtt_broker;
    uint16_t port;
    const char* mqtt_access_token;
    const char* device_id;
    const char* publish_topic;
    uint8_t mqtt_initialized;
} mqtt_attr_t;

extern wifi_attr_t wifi_info;
extern mqtt_attr_t mqtt_info;

#endif /* __APP_H__ */