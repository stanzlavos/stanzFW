#ifndef __UI_PAGE__
#define __UI_PAGE__

#include "config.h"

#ifdef TFT_IN_USE

#include "ui_toolbar.h"
#include "ui_defs.h"

#define PAGE_ID_INVALID   UI_MAX_PAGES

#define TOOLBAR_FLAG_HOME 0x01
#define TOOLBAR_FLAG_BACK 0x02

typedef enum
{
  WORK_GRID,        // 3x3
  WORK_ROWS,
  WORK_FREE
} work_area_type_t;

typedef enum
{
  ARROW_UP,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT
} work_area_arrow_type_t;

class ui_page;

typedef void (*touch_func_t)(ui_page *page, touch_event_t event);
void default_touch_handler(ui_page *page, touch_event_t event);     // Assumes objects are buttons. If there are other types, please set new handler !!!
bool default_swipe_handler(ui_page *page, touch_event_t event);

class ui_page
{
  public :

    ui_toolbar        *toolbar;
    ui_obj            *active_obj;
    ui_obj            *obj_touch_list[WORK_AREA_PER_PAGE][WORK_AREA_OBJ_MAX];
    ui_obj            *obj_list_work_area[WORK_AREA_PER_PAGE][WORK_AREA_OBJ_MAX];
    uint8_t           obj_cnt;
    uint8_t           obj_touch_cnt[WORK_AREA_PER_PAGE];
    uint8_t           page_id;
    char              page_name[MAX_TEXT_SIZE];
    touch_func_t      handle_touch_fn;
    work_area_type_t  work_area_type;
    uint8_t           work_area_obj_cnt[WORK_AREA_PER_PAGE];
    uint8_t           active_work_area;
    uint8_t           free_work_area;

    // Constructor
    ui_page(uint8_t id = PAGE_ID_INVALID, char *name = NULL, uint8_t flags = 0)
    {
      toolbar = new ui_toolbar(TOOLBAR_X, TOOLBAR_Y, TOOLBAR_W, TOOLBAR_H, OBJ_DEFAULT);
      toolbar->set_bmp(DEFAULT_TOOLBAR_BMP);

      page_id       = id;
      page_name[0]  = '\0';
      if (name)
      {
        strcpy(page_name, name);
        page_name[MAX_TEXT_SIZE - 1] = '\0';
      }

      active_obj        = NULL;
      obj_cnt           = 0;
      handle_touch_fn   = default_touch_handler;
      work_area_type    = WORK_GRID;
      active_work_area  = 0;
      free_work_area    = 0;

      uint8_t i = 0, j = 0;
      for ( ; i < WORK_AREA_PER_PAGE; i++)
      {
        work_area_obj_cnt[i] = 0;
        obj_touch_cnt[i]     = 0;
        for(j = 0; j < WORK_AREA_OBJ_MAX; j++)
        {
          obj_touch_list[i][j]     = NULL;
          obj_list_work_area[i][j] = NULL;
        }
      }

      handle_flags(flags);
    }

    void    set_page_id(uint8_t);
    void    set_page_name(char *);
    void    draw(void);
    void    draw_work_area(void);
    void    clear_work_area(void);
    void    draw_arrow(work_area_arrow_type_t, uint16_t);
    void    draw_work_area_arrows(void);
    bool    add_toolbar_obj(ui_obj *);
    bool    add_work_area_obj(ui_obj *);
    void    set_work_area_type(work_area_type_t);
    ui_obj  *find_touch(touch_event_t event);
    void    set_touch_handler(touch_func_t);
    void    handle_touch(ui_page *page, touch_event_t event);
    void    handle_flags(uint8_t);
};

#endif // TFT_IN_USE

#endif // __UI_PAGE__
