#ifndef __UTILS_OTA__
#define __UTILS_OTA__

#include "config.h"

#ifdef ARDUINO_OTA
void setup_arduino_ota(void);
void handle_arduino_ota(void);
#endif

#ifdef HTTP_OTA
void handle_http_update(void);
void handle_http_update_loop(void);
#endif

#ifdef HTTP_SPIFFS_OTA
void handle_http_spiffs_update(void);
void handle_http_spiffs_update_loop(void);
#endif

#endif // __UTILS_OTA__
