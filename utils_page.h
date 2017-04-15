#ifndef __UTILS_PAGE__
#define __UTILS_PAGE__

#include "config.h"

#ifdef TFT_IN_USE

#include "ui_page.h"

extern uint8_t prev_page_id;
extern uint8_t next_page_id;

ui_page *get_new_page(uint8_t id, char *name, uint8_t flags = 0);
ui_page *get_page(uint8_t id);
ui_page *get_active_page(void);
bool is_page_redraw_needed(void);
void trigger_page_redraw(void);

void flush_stack(void);
void launch_page(uint8_t id);
void launch_next_page(uint8_t id);
void launch_prev_page(void);

#endif // TFT_IN_USE

#endif // __UTILS_PAGE__
