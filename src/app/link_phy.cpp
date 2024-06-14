/**
 ******************************************************************************
 * @author: Nark
 * @date:   05/06/2024
 ******************************************************************************
**/

#include "link_phy.h"

#include "stk.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_network.h"
#include "task_polling.h"

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>

uint8_t link_phy_buffer_receive[BUFFER_UART_RECEIVE_SIZE];
uint8_t link_phy_buffer_trans[BUFFER_UART_TRANS_SIZE];
uint8_t get_length_byte_uart;

stk_msg_parser_t* get_msg = (stk_msg_parser_t*)0;
data_parser_t* get_data = (data_parser_t*)0;
wifi_info_parser_t* get_wl_info = (wifi_info_parser_t*)0;

data_parser_t data_for_send_msg_mqtt;
uint8_t get_wifi_state;
uint8_t length_ssid;
uint8_t length_password;

static void link_phy_response_wl_info();

void link_phy_handler(stk_msg_t* msg) {
    switch (msg->sig) {
    case LINK_PHY_PARSER_BUFF:
        APP_DBG("[LINK_PHY] LINK_PHY_PARSER_BUFF\n");

        get_length_byte_uart = get_size_byte_uart_receive();

        switch (link_phy_buffer_receive[1]) {
        case TASK_POST:
            task_post_pure_msg(LINK_PHY_ID, LINK_PHY_FW_MSG);
            break;
        
        case PARSER_DATA:
            task_post_pure_msg(LINK_PHY_ID, LINK_PHY_PARSER_DATA);
            break;

        case PARSER_WL_INFO:
            task_post_pure_msg(LINK_PHY_ID, LINK_PHY_PARSER_WL_INFO);
            break;

        default:
            break;
        }
        break;
    
    case LINK_PHY_FW_MSG:
        APP_DBG("[LINK_PHY] LINK_PHY_FW_MSG\n");
        get_msg = (stk_msg_parser_t*)(&link_phy_buffer_receive[2]);
        APP_PRINT("Destination task id: ");
        APP_PRINTLN(get_msg->des_task_id);
        APP_PRINT("Signal: ");
        APP_PRINTLN(get_msg->sig);
        task_post_pure_msg(get_msg->des_task_id, get_msg->sig);
        break;

    case LINK_PHY_PARSER_DATA:
        APP_DBG("[LINK_PHY] LINK_PHY_PARSER_DATA\n");
        if (sizeof(link_phy_buffer_receive) >= 2 + sizeof(data_parser_t)) {
            get_data = (data_parser_t*)(&link_phy_buffer_receive[2]);

            data_for_send_msg_mqtt.voltage = get_data->voltage;
            data_for_send_msg_mqtt.current = get_data->current;
            data_for_send_msg_mqtt.speed_level = get_data->speed_level;
            data_for_send_msg_mqtt.weight = get_data->weight;
 
            APP_PRINT("Voltage: ");
            APP_PRINTLN(get_data->voltage);
            APP_PRINT("Current: ");
            APP_PRINTLN(get_data->current);
            APP_PRINT("Speed: ");
            APP_PRINTLN(get_data->speed_level);
            APP_PRINT("Weight: ");
            APP_PRINTLN(get_data->weight);
        } 
        else {
            APP_DBG("[LINK_PHY] Buffer receive size error\n");
        }
        break;

    case LINK_PHY_PARSER_WL_INFO:
        APP_DBG("[LINK_PHY] LINK_PHY_PARSER_WL_INFO\n");

        get_wl_info = (wifi_info_parser_t*)(&link_phy_buffer_receive[2]);

        /* clear buffer */
        memset(wifi_info.ssid, '\0', sizeof(wifi_info.ssid));
        memset(wifi_info.password, '\0', sizeof(wifi_info.password));
        strncpy(wifi_info.ssid, (const char*)(&link_phy_buffer_receive[4]), get_wl_info->length_char_ssid);
        strncpy(wifi_info.password, (const char*)(&link_phy_buffer_receive[4 + get_wl_info->length_char_ssid]), get_wl_info->length_char_password);
        APP_PRINT("PARSER WIFI SSID: ");
        APP_PRINT(wifi_info.ssid);
        APP_PRINT("\n");
        APP_PRINT("PARSER WIFI_PASSWORD: ");
        APP_PRINT(wifi_info.password);
        APP_PRINT("\n");
        
        /* reconnect wifi with new wifi */
        save_wifi_config(get_wl_info->length_char_ssid, get_wl_info->length_char_password);
        task_post_pure_msg(TASK_NETWORK_ID, ESP_WL_GET_WIFI_INFO);
        task_post_pure_msg(TASK_NETWORK_ID, ESP_WL_CONNECT);
        break;
    
    case SIG_SM_REQ_WIFI_INFO:
        APP_DBG("[LINK_PHY] SIG_SM_REQ_WIFI_INFO\n");
        task_post_pure_msg(LINK_PHY_ID, LINK_PHY_RES_WL_INFO);
        break;
        
    case LINK_PHY_RES_WL_INFO:
        APP_DBG("[LINK_PHY] LINK_PHY_RES_WL_INFO\n");
        link_phy_response_wl_info();
        break;

    default:
        break;
    }
}

void link_phy_send_data(uint8_t* buffer, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        UART_LINK.write((uint8_t)buffer[i]);
    }
}

void link_phy_fw_msg(uint8_t des_task_id, uint8_t sig) {
    link_phy_buffer_trans[0] = 0xFD;
    link_phy_buffer_trans[1] = TASK_POST;
    link_phy_buffer_trans[2] = des_task_id;
    link_phy_buffer_trans[3] = sig;
    link_phy_buffer_trans[4] = 0xFE;
    link_phy_send_data(&link_phy_buffer_trans[0], 5);
}

void link_phy_response_wl_info() {
    /* start condition  */
    link_phy_buffer_trans[0] = 0xFD;
    link_phy_buffer_trans[1] = SIG_SM_RES_WIFI_INFO;

    /* read wifi info */
    EEPROM.begin(EEPROM_SIZE);
    length_ssid = EEPROM.read(0);
    length_password = EEPROM.read(1);
    link_phy_buffer_trans[2] = wifi_info.wifi_status;
    link_phy_buffer_trans[3] = length_ssid;
    link_phy_buffer_trans[4] = length_password;

    for (uint8_t i = 0; i < length_ssid; i++) {
        link_phy_buffer_trans[5 + i] = EEPROM.read(2 + i);
    }
    for (uint8_t i = 0; i < (length_password); i++) {
        link_phy_buffer_trans[5 + length_ssid + i] = EEPROM.read(2 + length_ssid + i);
    }
    EEPROM.end();

    /* stop condition */
    link_phy_buffer_trans[5 + length_ssid + length_password] = 0xFE;

    /* transmission */
    link_phy_send_data((uint8_t*)&link_phy_buffer_trans[0], (5 + length_ssid + length_password + 1));
}