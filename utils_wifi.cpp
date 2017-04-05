#include "utils_wifi.h"
#include "utils_debug.h"
#include "utils_config.h"
#include "utils_timer.h"
#include <ESP8266WiFi.h>

#define WIFI_RETRY_COUNT 	      3
#define WIFI_TRIGGER_INTERVAL	  2	    // 3 seconds
#define MAX_WIFI_FUNCS			    8

char wifi_ssid[CONFIG_PARAM_LEN_MAX]    	= "\0";
char wifi_pass[CONFIG_PARAM_LEN_MAX]    	= "\0";
char wifi_ip[CONFIG_PARAM_LEN_MAX]      	= "\0";
char wifi_gw[CONFIG_PARAM_LEN_MAX]      	= "\0";
char wifi_subnet[CONFIG_PARAM_LEN_MAX]  	= "\0";
bool wifi_connected                     	= false;
bool wifi_enabled                       	= false;

uint8_t	on_cnctd_funcs						= 0;
uint8_t	on_discnctd_funcs					= 0;
uint8_t	wifi_handler_funcs					= 0;
wifi_func_t	on_cnctd[MAX_WIFI_FUNCS] 		= {NULL};
wifi_func_t	on_discnctd[MAX_WIFI_FUNCS] 	= {NULL};
wifi_func_t	wifi_handler[MAX_WIFI_FUNCS] 	= {NULL};

bool add_wifi_dependent_func(wifi_func_t f1, wifi_func_t f2, wifi_func_t f3)
{
	if((on_cnctd_funcs 	   == MAX_WIFI_FUNCS) ||
			(on_discnctd_funcs  == MAX_WIFI_FUNCS) ||
			(wifi_handler_funcs == MAX_WIFI_FUNCS))
	{
		return false;
	}

	if(f1)
		on_cnctd[on_cnctd_funcs++] 			= f1;

	if(f2)
		on_discnctd[on_discnctd_funcs++] 	= f2;

	if(f3)
		wifi_handler[wifi_handler_funcs++] 	= f3;

	return true;
}

static inline void run_wifi_funcs(wifi_func_t f[])
{
	uint8_t i = 0;
	while(f[i])
	{
		f[i++]();
	}
}

static inline void handle_on_cnct(void *)
{
	run_wifi_funcs(on_cnctd);
	wifi_connected = true;
}

static inline void handle_on_discnct(void *)
{
	run_wifi_funcs(on_discnctd);
}

void handle_wifi_funcs(void)
{
	if(is_wifi_connected)
		run_wifi_funcs(wifi_handler);
}

static inline void wifi_event_handler(WiFiEvent_t event)
{
	switch (event)
	{
	case WIFI_EVENT_STAMODE_GOT_IP:
		//Serial.println("WiFi connected");
		add_timer(handle_on_cnct, NULL, WIFI_TRIGGER_INTERVAL, false);
		break;
	case WIFI_EVENT_STAMODE_DISCONNECTED:
		if(wifi_connected)
		{
			//Serial.println("WiFi connection lost");
			wifi_connected = false;
			add_timer(handle_on_discnct, NULL, WIFI_TRIGGER_INTERVAL, false);
		}
		break;
	}
}

bool ena_dis_wifi(bool state)
{
	wifi_connected = state;
	if(state)
		setup_wifi();
	else
		WiFi.disconnect();
}

bool is_wifi_connected(void)
{
	return wifi_connected;
}

static inline void handle_wifi_setup(void *)
{
	setup_wifi();
}

void setup_wifi(void)
{
	if (wifi_enabled && !WiFi.isConnected())
	{
		static bool first_time = true;

		WiFi.mode(WIFI_STA);

		IPAddress ip;
		ip.fromString(wifi_ip);
		IPAddress gateway;
		gateway.fromString(wifi_gw);
		IPAddress subnet;
		subnet.fromString(wifi_subnet);

		WiFi.config(ip, gateway, subnet);
		WiFi.onEvent(wifi_event_handler);
		WiFi.begin(wifi_ssid, wifi_pass);

		if(first_time)
		{
			T_PRINT("Connecting to : ")
			T_PRINTLN(wifi_ssid)
		}

		uint8_t retry_count = 0;
		while(WiFi.waitForConnectResult() != WL_CONNECTED)
		{
			if(++retry_count == WIFI_RETRY_COUNT)
				break;
			if(first_time)
				T_PRINTLN("Connection Failed !!! Retrying")
		}

		if(retry_count == WIFI_RETRY_COUNT)
		{
			if(first_time)
				T_PRINTLN("WiFI Connection Failed")
			wifi_connected = false;
		}
		else
		{
			if(first_time)
				T_PRINTLN("Wifi Connected")
			wifi_connected = true;
		}

		first_time = false;
	}
}
