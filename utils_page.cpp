#include "utils_page.h"

#ifdef TFT_IN_USE

uint8_t prev_page_id              = UI_MAX_PAGES;
uint8_t next_page_id              = ID_HOME;
uint8_t page_stack[UI_MAX_PAGES]  = {0};
int8_t  stack_top                 = -1;

ui_page *ui_pages[UI_MAX_PAGES];

ui_page *get_new_page(uint8_t id, char *name, uint8_t flags)
{
  ui_page *page = new ui_page(id, name, flags);
  ui_pages[id] = page;
  return page;
}

ui_page *get_page(uint8_t id)
{
  return (ui_pages[id]);
}

bool push_page(uint8_t id)
{
  if((stack_top + 1) == UI_MAX_PAGES)
    return false;

  page_stack[++stack_top] = id;

  return true;
}

uint8_t pop_page(void)
{
  if(stack_top < 0)
    return UI_MAX_PAGES;

  return page_stack[stack_top--];   
}

void flush_stack(void)
{
  stack_top = -1;

  int i=0;
  for( ; i < UI_MAX_PAGES; i++)
    page_stack[i] = 0;    
}

void launch_page(uint8_t id)
{
  tft_clear_work_area();
  next_page_id = id;
  flush_stack();    
}

void launch_next_page(uint8_t id)
{
  if(push_page(prev_page_id))
  {
    tft_clear_work_area();
    next_page_id = id;
  }
}

void launch_prev_page(void)
{
  uint8_t id = UI_MAX_PAGES;
  if((id = pop_page()) != UI_MAX_PAGES)
  {
    tft_clear_work_area();
    next_page_id = id;
  }
}

#endif // TFT_IN_USE
