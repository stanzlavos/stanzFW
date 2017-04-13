#include "utils_ota.h"
#include "utils_debug.h"
#include "utils_config.h"
#include "utils_gen.h"

#if defined HTTP_OTA || defined HTTP_SPIFFS_OTA
#include <ESP8266httpUpdate.h>
#endif

char    update_ip[CONFIG_PARAM_LEN_MAX]     = "\0";
char    update_script[CONFIG_PARAM_LEN_MAX] = "\0";

#ifdef ARDUINO_OTA

#include <ArduinoOTA.h>

void setup_arduino_ota(void)
{
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  // ArduinoOTA.onStart([]() {
  //   PRINTLN("Start OTA")
  // });

  // ArduinoOTA.onEnd([]() {
  //   PRINTLN("\nEnd OTA")
  // });

  // ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  //  PRINT("Progress: %u%%\r", (progress / (total / 100)))
  // });

  // ArduinoOTA.onError([](ota_error_t error) {
  //   PRINT("Error[%u]: ", error)
  //   if (error == OTA_AUTH_ERROR)         PRINTLN("Auth Failed")
  //   else if (error == OTA_BEGIN_ERROR)   PRINTLN("Begin Failed")
  //   else if (error == OTA_CONNECT_ERROR) PRINTLN("Connect Failed")
  //   else if (error == OTA_RECEIVE_ERROR) PRINTLN("Receive Failed")
  //   else if (error == OTA_END_ERROR)     PRINTLN("End Failed")
  // });

  ArduinoOTA.begin();
}

void handle_arduino_ota(void)
{
	if (get_op_mode() == MODE_OTA)
	{
		ArduinoOTA.handle();

		set_op_mode(MODE_RUN);
	}
}

#endif

#ifdef HTTP_OTA

void handle_http_update(void)
{
  PRINTLN("Checking sketch update")
  
  t_httpUpdate_return ret = ESPhttpUpdate.update(update_ip, 80, update_script, SKETCH_VERSION);
  
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      //PRINT("HTTP_OTA_FAILD Error : ")
      PRINT(ESPhttpUpdate.getLastError())
      PRINT(" : ")
      PRINTLN(ESPhttpUpdate.getLastErrorString().c_str())
      break;

    case HTTP_UPDATE_NO_UPDATES:
      PRINTLN("HTTP_OTA_NO_UPDATES")
      break;

    case HTTP_UPDATE_OK:
      PRINTLN("HTTP_OTA_OK")
      break;
  }
}

void handle_http_update_loop(void)
{
	if (get_op_mode() == MODE_OTA)
	{
		handle_http_update();

		set_op_mode(MODE_RUN);
	}
}

#endif

#ifdef HTTP_SPIFFS_OTA

void handle_http_spiffs_update(void)
{
  PRINTLN("Checking SPIFFS update")

  t_httpUpdate_return retspiffs = ESPhttpUpdate.updateSpiffs(update_ip, 80, update_script, SPIFFS_VERSION);
  
  switch (retspiffs) {
    case HTTP_UPDATE_FAILED:
      //PRINT("HTTP_OTA_FAILD Error : ")
      PRINT(ESPhttpUpdate.getLastError())
      PRINT(" : ")
      break;

    case HTTP_UPDATE_NO_UPDATES:
      PRINTLN("HTTP_OTA_NO_UPDATES")
      break;

    case HTTP_UPDATE_OK:
      PRINTLN("HTTP_OTA_OK")
      //ESP.restart();
      break;
  }
}

void handle_http_spiffs_update_loop(void)
{
	if (get_op_mode() == MODE_SPIFFS_OTA)
	{
		handle_http_spiffs_update();

		set_op_mode(MODE_RUN);
	}
}

#endif
//mkspiffs -c directory_whose_contents_you_want_in_the_SPIFFS_image -b 8192 -s 3125248 image.spiffs.bin
