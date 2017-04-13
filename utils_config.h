#ifndef __UTILS_CONFIG__
#define __UTILS_CONFIG__

#include <ArduinoJson.h>
#include <FS.h>

#define CONFIG_FILE_NAME      "/config/config.json"
#define CONFIG_FILE_SIZE      1024
#define CONFIG_PARAM_LEN_MAX  64

void create_json_cfg(void);
bool read_json_cfg(void);

#endif // __UTILS_CONFIG__
