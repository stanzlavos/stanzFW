#ifndef __UTILS_WIFI__
#define __UTILS_WIFI__

typedef void (*wifi_func_t)(void);

bool is_wifi_connected(void);
void setup_wifi(void);
void handle_wifi_funcs(void);
bool ena_dis_wifi(bool state);
bool add_wifi_dependent_func(wifi_func_t on_cncted, wifi_func_t on_discnctd, wifi_func_t wifi_handler);

#endif // __UTILS_WIFI__
