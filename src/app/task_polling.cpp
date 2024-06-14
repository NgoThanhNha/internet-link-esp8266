/**
 ******************************************************************************
 * @author: Nark
 * @date:   05/06/2024
 ******************************************************************************
**/

#include "task_polling.h"

#include "stk.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_network.h"
#include "task_mqtt.h"
#include "link_phy.h"

#include <ESP8266WiFi.h>

uint8_t length_of_byte_receive;
uint8_t index_receive = 0;
uint8_t flag_start_transmiss;

static void polling_link_uart();
static void polling_ap_mode();

void task_polling_run() {
    polling_link_uart();
    polling_mqtt();
    polling_ap_mode();
}

void polling_link_uart() {
    if (UART_LINK.available()) {
        /* receive byte */
        link_phy_buffer_receive[index_receive] = UART_LINK.read();

        /* start transmission*/
        if (link_phy_buffer_receive[index_receive] == 0xFD) {
            flag_start_transmiss = 1;
        }

        /* end of transmission */
        if (flag_start_transmiss) {
            if (link_phy_buffer_receive[index_receive] == 0xFE) {
                task_post_pure_msg(LINK_PHY_ID, LINK_PHY_PARSER_BUFF);
                length_of_byte_receive = index_receive;
                index_receive = 0;
                flag_start_transmiss = 0;
            }
            else {
                index_receive++;
            }
        }
    }
}

uint8_t get_size_byte_uart_receive() {
    return length_of_byte_receive;
}

void polling_ap_mode() {
    if (WiFi.getMode() == WIFI_AP) {
        server.handleClient();
    }
}