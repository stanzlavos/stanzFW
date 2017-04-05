#include "utils_config.h"
#include "utils_debug.h"

extern char    wifi_ssid[CONFIG_PARAM_LEN_MAX];
extern char    wifi_pass[CONFIG_PARAM_LEN_MAX];
extern char    wifi_ip[CONFIG_PARAM_LEN_MAX];
extern char    wifi_gw[CONFIG_PARAM_LEN_MAX];
extern char    wifi_subnet[CONFIG_PARAM_LEN_MAX];
extern bool    wifi_enabled;

extern bool    blynk_enabled;
extern char    blynk_auth[CONFIG_PARAM_LEN_MAX];

extern char    update_ip[CONFIG_PARAM_LEN_MAX];
extern char    update_script[CONFIG_PARAM_LEN_MAX];

void create_json_cfg(void)
{
  StaticJsonBuffer<CONFIG_FILE_SIZE> jsonBuffer;
  char buff[CONFIG_FILE_SIZE];

  JsonObject& root = jsonBuffer.createObject();

  JsonObject& data = root.createNestedObject("wifi");
  data["enabled"]       = 1;
  data["wifi_ssid"]     = "";
  data["wifi_pass"]     = "";
  data["my_ip"]         = "";
  data["my_gw"]         = "";
  data["my_subnet"]     = "";

  JsonObject& data1 = root.createNestedObject("blynk");
  data1["enabled"]       = 1;
  data1["blynk_auth"]    = "";

  JsonObject& data2 = root.createNestedObject("ota");
  data2["update_ip"]     = "";
  data2["update_script"] = "";

  root.printTo(buff, sizeof(buff));

  //PRINTLN(buff);

  if (SPIFFS.exists(CONFIG_FILE_NAME))
  {
    PRINTLN("Config file found !!!")
  }
  else
  {
    PRINTLN("Config file not found !!!")
  }


  File fp = SPIFFS.open(CONFIG_FILE_NAME, "w+");
  if (!fp)
  {
    PRINTLN("Config file open failed !!!")
    return;
  }

  fp.write((uint8_t *)buff, (strlen(buff) + 1));
  fp.close();

  PRINTLN("JSON file create done.")
}

void testSPIFFs(void)
{
  FSInfo fs_info;
  SPIFFS.info(fs_info);

  Serial.println(" ");
  Serial.print("totalBytes    :"); Serial.println(fs_info.totalBytes);
  Serial.print("usedBytes     :"); Serial.println(fs_info.usedBytes);
  Serial.print("blockSize     :"); Serial.println(fs_info.blockSize);
  Serial.print("pageSize      :"); Serial.println(fs_info.pageSize);
  Serial.print("maxOpenFiles  :"); Serial.println(fs_info.maxOpenFiles);
  Serial.print("maxPathLength :"); Serial.println(fs_info.maxPathLength);

  Serial.println("All files : ");
  Serial.println(" ");
  String str = "";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\r\n";
  }
  Serial.print(str);
  Serial.println(" ");
}

bool read_json_cfg(void)
{
  StaticJsonBuffer<CONFIG_FILE_SIZE> jsonBuffer;
  char buff[CONFIG_FILE_SIZE];

  File fp = SPIFFS.open(CONFIG_FILE_NAME, "r");
  if (!fp)
  {
    T_PRINTLN("Config file open failed !!!")
    return false;
  }

  fp.read((uint8_t *)buff, sizeof(buff));
  fp.close();

  JsonObject& root = jsonBuffer.parseObject(buff);

  if (!root.success()) {
    T_PRINTLN("parseObject() failed")
    return false;
  }

  //root.prettyPrintTo(Serial);

  wifi_enabled      =   root["wifi"]["enabled"];
  strcpy(wifi_ssid,     root["wifi"]["wifi_ssid"]);
  strcpy(wifi_pass,     root["wifi"]["wifi_pass"]);
  strcpy(wifi_ip,         root["wifi"]["wifi_ip"]);
  strcpy(wifi_gw,         root["wifi"]["wifi_gw"]);
  strcpy(wifi_subnet,     root["wifi"]["wifi_subnet"]);

  blynk_enabled     =   root["blynk"]["enabled"];
  strcpy(blynk_auth,    root["blynk"]["blynk_auth"]);

  strcpy(update_ip,     root["ota"]["update_ip"]);
  strcpy(update_script, root["ota"]["update_script"]);

  T_PRINTLN("Config file parse done")

  return true;
}
