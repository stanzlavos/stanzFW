#ifndef __CONFIG__
#define __CONFIG__

#define SKETCH_VERSION     "test_v1.0"
#define SPIFFS_VERSION     "test_v1.0"

#define BLYNK_IN_USE

//#define ARDUINO_OTA
#define HTTP_OTA
#define HTTP_SPIFFS_OTA

#define DEBUG_PRINT
//#define DEBUG_TELNET

#define TFT_IN_USE      // Implicitley decides if touch is in use or not
//#define TFT_SCROLL_ON   // Tesxt size 1 only !!!

#define NTP_IN_USE

#endif // __CONFIG__
