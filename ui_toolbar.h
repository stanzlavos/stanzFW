#ifndef __UI_TOOLBAR__
#define __UI_TOOLBAR__

#include "config.h"

#ifdef TFT_IN_USE

#include "ui_obj.h"
#include "ui_defs.h"

#define DEFAULT_TOOLBAR_BMP   "/toolbar.palette"

class ui_toolbar : public ui_obj
{
  public :

    ui_obj  *obj_list[TOOLBAR_OBJ_MAX];
    uint8_t obj_cnt;

    //Constructor
    ui_toolbar(uint16_t x1, uint16_t y1, uint16_t w1, uint16_t h1, uint8_t id, char *text1 = NULL, obj_type_t type = OBJ_NO_TOUCH) : ui_obj(x1, y1, w1, h1, id, text1, type)
    {
      obj_cnt = 0;

      uint8_t i=0;
      for( ;i<TOOLBAR_OBJ_MAX; i++)
      {
        obj_list[i] = NULL;
      }
    }

    bool    add_obj(ui_obj *);
    ui_obj  *find_touch(touch_event_t event);
    void    draw(void);
};

#endif // TFT_IN_USE

#endif // __UI_TOOLBAR__
