#include "config.h"

extern "C" {
#include "user_interface.h"
}

#include "utils_debug.h"
#include "utils_config.h"
#include "utils_touch.h"
#include "utils_gen.h"
#include "utils_wifi.h"
#include "utils_blynk.h"
#include "utils_ota.h"
#include "utils_ui.h"
#include "utils_defs.h"
#include "utils_ntp.h"
#include "utils_timer.h"

void setup() {
#ifdef DEBUG_PRINT
  Serial.begin(115200);
  Serial.flush();
#endif

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  init_timer_ring();

#ifdef TFT_IN_USE
  setup_tft();
#endif

#ifdef TOUCH_IN_USE
  setup_TOUCH();
#endif

#ifdef BLYNK_IN_USE
  add_wifi_dependent_func(setup_blynk, 			stop_blynk, 	handle_blynk);
#endif
#ifdef DEBUG_TELNET
  add_wifi_dependent_func(setup_telnet, 		NULL, 			handle_telnet);
#endif
#ifdef ARDUINO_OTA
  add_wifi_dependent_func(setup_arduino_ota, 	NULL, 			handle_arduino_ota);
#endif  
#ifdef HTTP_OTA
  add_wifi_dependent_func(NULL, 				NULL, 			handle_http_update_loop);
#endif
#ifdef HTTP_SPIFFS_OTA
  add_wifi_dependent_func(NULL, 				NULL, 			handle_http_spiffs_update_loop);
#endif
  
  SPIFFS.begin();
  if (read_json_cfg())
    setup_wifi();

  bool ret = system_update_cpu_freq(160);
  if (ret)
  {
    T_PRINT("CPU Freq : ") 
    T_PRINTLN(system_get_cpu_freq())
  }

  T_PRINTLN("Ready")
  blink_led(2);

#ifdef TFT_IN_USE
  tft_clear();
  setup_ui();
#endif
}

void loop()
{
  handle_timer_events();
  
  handle_wifi_funcs();

#ifdef TFT_IN_USE
  handle_touch();

  handle_ui();
#endif
}
