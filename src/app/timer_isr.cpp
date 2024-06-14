/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#include "timer_isr.h"

#include "Arduino.h"
#include "stk.h"
#include "message.h"
#include "timer.h"

static uint32_t current_millis;
static uint32_t previous_millis;

void get_tick_for_timer() {
    current_millis = millis();
    timer_tick(current_millis - previous_millis);
    previous_millis = current_millis;
}