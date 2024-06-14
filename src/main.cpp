#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "stk.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "task_list.h"
#include "task_polling.h"
#include "timer_isr.h"

void setup() {
    main_app();
}

void loop() {
    /* kernel start */
    task_scheduler();
    task_polling_run();
    get_tick_for_timer();
}
