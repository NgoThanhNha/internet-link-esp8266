/**
 ******************************************************************************
 * @author: Nark
 * @date:   04/06/2024
 ******************************************************************************
**/

#ifndef __APP_DBG_H__
#define __APP_DBG_H__

#include "platform.h"
#include <Arduino.h>

#define DBG_EN (0)

#if DBG_EN
    #define APP_DBG(str)            app_dbg(str)
    #define APP_PRINTLN(str)        Serial.println(str)
    #define APP_PRINT(str)          Serial.print(str)
    #define SYS_PRINT(str)          Serial.print(str)
#else
    #define APP_DBG(str)            
    #define APP_PRINTLN(str)        
    #define APP_PRINT(str)          
    #define SYS_PRINT(str)         
#endif

#endif /* __APP_DBG_H__ */