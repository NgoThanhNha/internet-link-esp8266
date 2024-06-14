/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#include "platform.h"

#include "stk.h"

#include "app.h"
#include "app_dbg.h"

void fatal_error(const char* fatal_type, uint8_t fatal_id) {
    Serial.print("[FATAL TYPE]:");
    Serial.print(fatal_type);
    Serial.print(" -> [FATAL ID]: ");
    Serial.print(fatal_id);
}

void console_init() {
    Serial.begin(115200);
}

void app_dbg(const char* str) {
    Serial.print("[APP_PRINT] ");
    Serial.print(str);
}

void sys_banner() {
    SYS_PRINT("\n");
    SYS_PRINT("\n");
    SYS_PRINT("\n");
    SYS_PRINT(" ___  ____  _  _ \n");
    SYS_PRINT("/ __)(_  _)( )/ )\n");
    SYS_PRINT("\\__ \\  )(  (   (\n");
    SYS_PRINT("(___/ (__) (_)\\_)\n");
    SYS_PRINT("\n");
    SYS_PRINT("Author: Nark\n");
    SYS_PRINT("Kernel version: ");
    SYS_PRINT(STK_VERSION);
    SYS_PRINT("\n");
    SYS_PRINT("Welcome to my application\n");
    SYS_PRINT("App version ");
    SYS_PRINT(APP_VERSION);
    SYS_PRINT("\n");
    SYS_PRINT("Application task created: ");
    SYS_PRINT(get_size_of_task_list());
    SYS_PRINT("\n");
    SYS_PRINT("\n");
    SYS_PRINT("\n");
    SYS_PRINT("\n");
}