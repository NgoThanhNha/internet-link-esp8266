/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <stdio.h>
#include <Arduino.h>

#define FATAL(type, id) fatal_error(type, id)

extern void console_init();
extern void sys_banner();
extern void app_dbg(const char* str);
extern void fatal_error(const char* fatal_type, uint8_t fatal_id);

#endif /* __PLATFORM_H__ */
