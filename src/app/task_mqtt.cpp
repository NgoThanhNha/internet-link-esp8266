/**
 ******************************************************************************
 * @author: Nark
 * @date:   05/06/2024
 ******************************************************************************
**/

#include "task_mqtt.h"

#include "stk.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "link_phy.h"
#include "task_list.h"
#include "task_polling.h"

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

WiFiClient esp8266_wifi_client;
PubSubClient mqtt_client(esp8266_wifi_client);

StaticJsonDocument<200> json_publish_to_mqtt;
char json_buffer[512];

void mqtt_callback(char* topic, byte* payload, unsigned int length);
void mqtt_reconnect();

void task_mqtt_handler(stk_msg_t* msg) {
    switch (msg->sig) {
    case ESP_MQTT_INIT:
        APP_DBG("[TASK_MQTT] ESP_MQTT_INIT\n");
        mqtt_info.mqtt_broker = "demo.thingsboard.io";
        mqtt_info.port = 1883;
        mqtt_info.publish_topic = "v1/devices/me/telemetry";
        mqtt_info.mqtt_access_token = "lnFquHFL3tDoDaH9Z63z";
        mqtt_info.device_id = "94e621c0-1ef7-11ef-a435-ab3a1d535f3e";
        mqtt_client.setServer(mqtt_info.mqtt_broker, mqtt_info.port);
        mqtt_client.setCallback(mqtt_callback);
        mqtt_info.mqtt_initialized = MQTT_INITIALIZED;
        break;
        
    case ESP_MQTT_RECONNECT:
        APP_DBG("[TASK_MQTT] ESP_MQTT_RECONNECT\n");
        mqtt_reconnect();
        break;

    case ESP_MQTT_SEND_MSG:
        APP_DBG("[TASK_MQTT] ESP_MQTT_SEND_MSG\n");
        if (mqtt_info.mqtt_initialized) {
            /* update data */
            json_publish_to_mqtt["voltage"] = data_for_send_msg_mqtt.voltage;
            json_publish_to_mqtt["current"] = data_for_send_msg_mqtt.current * 10;
            json_publish_to_mqtt["speed"] = data_for_send_msg_mqtt.speed_level;
            json_publish_to_mqtt["weight"] = data_for_send_msg_mqtt.weight;

            /* create json string */
            serializeJson(json_publish_to_mqtt, json_buffer);
            APP_DBG("[TASK_MQTT] Publishing message: ");
            APP_PRINTLN(json_buffer);

            /* publish */
            mqtt_client.publish(mqtt_info.publish_topic, json_buffer);
        }
        break;

    default:
        break;
    }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    APP_DBG("[MQTT_CALLBACK] Message arrived from topic [");
    APP_PRINT(topic);
    APP_PRINT("] ");
    for (unsigned int i = 0; i < length; i++) {
        APP_PRINT((char)payload[i]);
    }
    APP_PRINTLN();
}

void mqtt_reconnect() {
    APP_DBG("[TASK_MQTT] Attempting MQTT connection...\n");
    if (mqtt_client.connect(mqtt_info.device_id, mqtt_info.mqtt_access_token, "")) {
        APP_DBG("[TASK_MQTT] Connected to Thingsboard MQTT Broker!\n");
        mqtt_client.subscribe("v1/devices/me/attributes/response/+");
    } 
    else {
        APP_DBG("[TASK_MQTT] Failed connection to MQTT Broker, err = ");
        APP_PRINTLN(mqtt_client.state());
        APP_DBG("[TASK_MQTT] Try again in 5 seconds");
    }
}

void polling_mqtt() {
    if (mqtt_info.mqtt_initialized) {
        if (!mqtt_client.connected()) {
            mqtt_reconnect();
        }
        mqtt_client.loop();
    }
}