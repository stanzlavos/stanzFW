#include "utils_blynk.h"

#ifdef BLYNK_IN_USE

#include <BlynkSimpleEsp8266.h>

#include "utils_defs.h"
#include "utils_gen.h"
#include "utils_config.h"
#include "utils_debug.h"
#include "utils_timer.h"

#define BLYNK_CNCT_INTERVAL 		500		// in ms
#define BLYNK_TRIGGER_INTERVAL		10		// in s
#define BLYNK_LONG_PRESS_INTERVAL	3000	// in ms

bool blynk_enabled = false;
char blynk_auth[CONFIG_PARAM_LEN_MAX] = "\0";
uint32_t switch_time = 0;

bool is_blynk_connected(void)
{
	return Blynk.connected();
}

static inline void handle_blynk_setup(void *)
{
	setup_blynk();
}

void setup_blynk(void)
{
	if (blynk_enabled && !is_blynk_connected())
	{
		Blynk.config(blynk_auth);

		if (Blynk.connect(BLYNK_CNCT_INTERVAL) == false)
		{
			//T_PRINTLN("Blynk connect failed")
			add_timer(handle_blynk_setup, NULL, BLYNK_TRIGGER_INTERVAL, false);
		}
		else
		{
			//T_PRINTLN("Connected to Blynk server.")
			Blynk.syncAll();
		}
	}
}

void stop_blynk(void)
{
	Blynk.disconnect();
}

/*
   Virtual pin V0 used to move board to OTA mode
 */
BLYNK_WRITE(V0)
{
	// Blynk.syncAll() causes an initial write.
	// Ignore it !!!
	static bool first_time = true;
	if (first_time)
	{
		first_time = false;
		return;
	}

	// If switch was "long pressed" for 3 seconds,
	// move the board to OTA mode
	if (!switch_time)
	{
		switch_time = millis();
	}
	else
	{
		uint32_t high_time = millis() - switch_time;

		if (high_time > BLYNK_LONG_PRESS_INTERVAL)
		{
			set_op_mode(MODE_OTA);
			blink_led(5);
		}

		switch_time = 0;
	}
}

/*
   Virtual pin V1 used to move board to SPIFFS OTA mode
 */
BLYNK_WRITE(V1)
{
	// Blynk.syncAll() causes an initial write.
	// Ignore it !!!
	static bool first_time = true;
	if (first_time)
	{
		first_time = false;
		return;
	}

	// If switch was "long pressed" for 3 seconds,
	// move the board to SPIFFS OTA mode
	if (!switch_time)
	{
		switch_time = millis();
	}
	else
	{
		uint32_t high_time = millis() - switch_time;

		if (high_time > BLYNK_LONG_PRESS_INTERVAL)
		{
			set_op_mode(MODE_SPIFFS_OTA);
			blink_led(5);
		}

		switch_time = 0;
	}
}

void handle_blynk(void)
{
	Blynk.run();
}

#endif // BLYNK_IN_USE
