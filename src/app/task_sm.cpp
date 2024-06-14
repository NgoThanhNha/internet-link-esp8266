/**
 ******************************************************************************
 * @author: Nark
 * @date:   06/06/2024
 ******************************************************************************
**/

#include "task_sm.h"

#include "stk.h"
#include "message.h"
#include "timer.h"

#include "platform.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_network.h"
#include "link_phy.h"

#include <ESP8266WiFi.h>

void task_sm_handler(stk_msg_t* msg) {
    switch (msg->sig) {
    case SIG_SM_REQ_WIFI_STATUS:
        APP_DBG("[TASK_SM] SIG_SM_REQ_WIFI_STATUS\n");
        task_post_pure_msg(TASK_SM_ID, SIG_SM_RES_WIFI_STATUS);
        break;

    case SIG_SM_RES_WIFI_STATUS:
        APP_DBG("[TASK_SM] SIG_SM_RES_WIFI_STATUS\n");
        if (WiFi.status() == WL_CONNECTED) {
            LINK_PHY_FORWARD_MSG_OUT(TASK_SM_ID, SIG_SM_RES_WIFI_CONNECTED);
        }
        else {
            LINK_PHY_FORWARD_MSG_OUT(TASK_SM_ID, SIG_SM_RES_WIFI_DISCONNECTED);
        }
        break;

    case SIG_SM_REQ_WIFI_INFO:
        APP_DBG("[TASK_SM] SIG_SM_REQ_CHECK_WIFI_CONNECT\n");
        task_post_pure_msg(LINK_PHY_ID, SIG_SM_REQ_WIFI_INFO);
        break;

    case SIG_SM_REQ_WIFI_RECONNECT:
        APP_DBG("[TASK_SM] SIG_SM_REQ_WIFI_RECONNECT\n");
        task_post_pure_msg(TASK_NETWORK_ID, ESP_WL_CONNECT);
        break;

    case SIG_SM_REQ_WIFI_RECHANGE:
        APP_DBG("[TASK_SM] SIG_SM_REQ_WIFI_RECHANGE\n");
        task_post_pure_msg(TASK_NETWORK_ID, SIG_SM_REQ_WIFI_RECHANGE);
        break;

    default:
        break;
    }
}