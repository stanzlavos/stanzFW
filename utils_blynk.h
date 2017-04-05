#ifndef __UTILS_BLYNK__
#define __UTILS_BLYNK__

#include "config.h"

#ifdef BLYNK_IN_USE

void setup_blynk(void);
void stop_blynk(void);
bool is_blynk_connected(void);
void handle_blynk(void);

#endif // BLYNK_IN_USE

#endif // __UTILS_BLYNK__
