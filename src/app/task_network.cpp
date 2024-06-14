/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#include "task_network.h"

#include "stk.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_mqtt.h"
#include "task_polling.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

ESP8266WebServer server(80);

const char* AP_MODE_SSID     = "HandmockSwingREV1.0";
const char* AP_MODE_PASSWORD = "stkadmin@";

uint32_t wifi_connect_timeout;
uint8_t index_read_ssid;
uint8_t index_read_password;

static void ap_mode_start();
void read_wifi_info_eeprom();
void save_wifi_config(uint8_t length_ssid, uint8_t length_password);

void task_network_handler(stk_msg_t* msg) {
    switch (msg->sig) {
    case ESP_WL_GET_WIFI_INFO:
        APP_DBG("[TASK_NETWORK] ESP_WL_GET_WIFI_INFO\n");
        read_wifi_info_eeprom();
        APP_DBG("[TASK_NETWORK] WIFI SSID: ");
        APP_PRINTLN(wifi_info.ssid);
        APP_DBG("[TASK_NETWORK] WIFI_PASSWORD: ");
        APP_PRINTLN(wifi_info.password);
        break;

    case ESP_WL_CONNECT:
        APP_DBG("[TASK_NETWORK] ESP_WL_CONNECT\n");
        APP_PRINTLN();
        APP_DBG("[TASK_NETWORK] Connecting to WIFI_SSID: ");
        APP_PRINTLN(wifi_info.ssid);

        /* remove prev timer */
        timer_remove(TASK_NETWORK_ID, ESP_WL_WAITING_CONNECT);

        /* wifi connect */
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifi_info.ssid, wifi_info.password);

        /* timer set for waiting wifi connected */
        wifi_connect_timeout = WIFI_CONNECT_TIMEOUT;
        timer_set(TASK_NETWORK_ID, ESP_WL_WAITING_CONNECT, 1000, TIMER_PERIODIC);
        break;
    
    case ESP_WL_CHECK_CONNECT:
        if (WiFi.status() == WL_CONNECTED) {
            APP_DBG("[TASK_NETWORK] WIFI_CONNECTED\n");
        }
        else {
            APP_DBG("[TASK_NETWORK] WIFI_DISCONNECTED\n");
        }
        break;

    case ESP_WL_WAITING_CONNECT:
        APP_DBG("[TASK_NETWORK] ESP_WL_WAITING_CONNECT\n");
        if ((WiFi.status() == WL_CONNECTED)) {
            /* remove timer */
            timer_remove(TASK_NETWORK_ID, ESP_WL_WAITING_CONNECT);
            APP_PRINTLN("");
            APP_DBG("[TASK_NETWORK] WIFI_CONNECTED\n");
            APP_DBG("[TASK_NETWORK] IP_ADDRESS: ");
            APP_PRINTLN(WiFi.localIP());
            
            wifi_info.wifi_status = WL_STATE_CONNECTED;
            task_post_pure_msg(LINK_PHY_ID, SIG_SM_REQ_WIFI_INFO);
            task_post_pure_msg(TASK_MQTT_ID, ESP_MQTT_INIT);
        }
        else {
            if (wifi_connect_timeout == 0) {
                wifi_info.wifi_status = WL_STATE_DISCONNECTED;
                timer_remove(TASK_NETWORK_ID, ESP_WL_WAITING_CONNECT);
                task_post_pure_msg(LINK_PHY_ID, SIG_SM_REQ_WIFI_INFO);
                APP_PRINTLN("[TASK_NETWORK] WIFI IS NOT CONNECTED");
            }
            else {
                wifi_connect_timeout--;
            }
        }
        break;
        
    case SIG_SM_REQ_WIFI_RECHANGE:
        mqtt_info.mqtt_initialized = MQTT_UNINITIALIZED;
        timer_remove(TASK_MQTT_ID, ESP_MQTT_RECONNECT);
        timer_remove(TASK_NETWORK_ID, ESP_WL_WAITING_CONNECT);
        APP_DBG("[TASK_NETWORK] SIG_SM_REQ_WIFI_RECHANGE\n");
        ap_mode_start();
        break;

    default:
        break;
    }
}

void read_wifi_info_eeprom() {
    EEPROM.begin(EEPROM_SIZE);
    uint8_t length_ssid = EEPROM.read(0);
    uint8_t length_password = EEPROM.read(1);
    for (uint8_t i = 0; i < length_ssid; i++) {
        wifi_info.ssid[i] = EEPROM.read(2 + i);
    }
    for (uint8_t i = 0; i < (length_password); i++) {
        wifi_info.password[i] = EEPROM.read(2 + length_ssid + i);
    }
    EEPROM.get((2 + length_ssid), wifi_info.password);
    EEPROM.end();
}

void clear_wifi_info_eeprom() {
    for (int i = 0; i < EEPROM_SIZE; i++) {
        EEPROM.write(i, '\0');
    }
    EEPROM.commit();
}

void save_wifi_config(uint8_t length_ssid, uint8_t length_password) {
    EEPROM.begin(EEPROM_SIZE);
    clear_wifi_info_eeprom();
    EEPROM.put(0, length_ssid);
    EEPROM.put(1, length_password);
    EEPROM.put(2, wifi_info.ssid);
    EEPROM.put(2 + length_ssid, wifi_info.password);
    EEPROM.commit();
    EEPROM.end();
}

void ap_mode_view() {
    String html = "<html><head>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }";
    html += "h1 { color: #333; }";
    html += "form { background-color: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); }";
    html += "input[type=text], input[type=password] { width: 100%; padding: 10px; margin: 10px 0; border: 1px solid #ccc; border-radius: 4px; }";
    html += "input[type=submit] { background-color: #4CAF50; color: white; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; }";
    html += "input[type=submit]:hover { background-color: #45a049; }";
    html += "</style>";
    html += "</head><body>";
    html += "<form action=\"/commit\" method=\"POST\">";
    html += "<h1>Automatic Handmock Swing - Wifi Web Config</h1>";
    html += "SSID: <input type=\"text\" name=\"ssid\"><br>";
    html += "Password: <input type=\"text\" name=\"password\"><br>";
    html += "<input type=\"submit\" value=\"Commit\">";
    html += "</form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void ap_mode_commit() {
    if (server.method() == HTTP_POST) {
        /* get wl info */
        String ssid = server.arg("ssid");
        String password = server.arg("password");

        /* clear buffer */
        memset(wifi_info.ssid, '\0', sizeof(wifi_info.ssid));
        memset(wifi_info.password, '\0', sizeof(wifi_info.password));

        /* save wifi config */
        ssid.toCharArray(wifi_info.ssid, ssid.length() + 1);
        password.toCharArray(wifi_info.password, password.length() + 1);
        save_wifi_config(ssid.length() + 1, password.length() + 1);

        String string_view = "<html><head>";
        string_view += "<style>";
        string_view += "body { font-family: Arial, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; display: flex; justify-content: center; align-items: center; height: 100vh; }";
        string_view += ".info { background-color: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); text-align: center; }";
        string_view += "</style>";
        string_view += "</head><body>";
        string_view += "<div class=\"info\">";
        string_view += "<h1>Wifi Configurated!</h1>";
        string_view += "<p><strong>Wifi Info</strong></p>";
        string_view += "<p>SSID: " + (String)(wifi_info.ssid) + "</p>";
        string_view += "<p>Password: " + (String)(wifi_info.password) + "</p>";
        string_view += "</div>";
        string_view += "</body></html>";
        server.send(200, "text/html", string_view);
        delay(3000);
        wifi_info.wifi_status = WL_STATE_RECHANGING;
        task_post_pure_msg(LINK_PHY_ID, LINK_PHY_RES_WL_INFO);
        task_post_pure_msg(TASK_NETWORK_ID, ESP_WL_GET_WIFI_INFO);
        task_post_pure_msg(TASK_NETWORK_ID, ESP_WL_CONNECT);
    }
}

void ap_mode_start() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_MODE_SSID, AP_MODE_PASSWORD);

    server.on("/", ap_mode_view);
    server.on("/commit", ap_mode_commit);
    server.begin();
}