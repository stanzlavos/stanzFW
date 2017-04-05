#ifndef __UTILS_NTP__
#define __UTILS_NTP__

#include "config.h"

#ifdef NTP_IN_USE

#include "stdint.h"

bool get_ntp_time_str_if_mod(char *str);
void get_curr_ntp_time(uint8_t *hours, uint8_t *minutes);

#endif // NTP_IN_USE

#endif // __UTILS_NTP__
