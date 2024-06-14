/**
 ******************************************************************************
 * @author: Nark
 * @date:   05/06/2024
 ******************************************************************************
**/

#ifndef __LINK_PHY_H__
#define __LINK_PHY_H__

#include "stk.h"
#include "Arduino.h"

#define LINK_PHY_FORWARD_MSG_OUT(des_task_id, sig)          link_phy_fw_msg(des_task_id, sig)

#define UART_LINK           Serial
#define BUFFER_UART_RECEIVE_SIZE            (32)
#define BUFFER_UART_TRANS_SIZE              (32)

/* stk msg from main mcu */
typedef struct {
    uint8_t des_task_id;
    uint8_t sig;
} stk_msg_parser_t ;

/* data of handmock machine */
typedef struct {
    uint8_t voltage;
    uint8_t current;
    uint8_t speed_level;
    uint8_t weight;
} data_parser_t;

typedef struct {
    uint8_t length_char_ssid;
    uint8_t length_char_password;
} wifi_info_parser_t;

/* type of function call by link phy */
typedef enum {
    TASK_POST = 0x01,
    PARSER_DATA,
    PARSER_WL_INFO,
} link_dect_t;

extern data_parser_t data_for_send_msg_mqtt;
extern uint8_t link_phy_buffer_trans[BUFFER_UART_TRANS_SIZE];
extern uint8_t link_phy_buffer_receive[BUFFER_UART_RECEIVE_SIZE];

extern void link_phy_send_data(uint8_t* buffer, uint8_t length);
extern void link_phy_fw_msg(uint8_t des_task_id, uint8_t sig);
extern void link_phy_handler(stk_msg_t* msg);

#endif /* __LINK_PHY_H__ */