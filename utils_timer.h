#ifndef __UTILS_TIMER__
#define __UTILS_TIMER__

#include <stdint.h>

typedef void (*timer_func_t)(void *);

void init_timer_ring(void);
void handle_timer_events(void);
bool add_timer(timer_func_t func, void *arg, uint8_t intrvl, bool repeat, uint16_t repeat_cnt = 0);

#endif // __UTILS_TIMER__
