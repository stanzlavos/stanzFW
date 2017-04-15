#ifndef __UI_BUTTON__
#define __UI_BUTTON__

#include "config.h"

#ifdef TFT_IN_USE

#include "ui_obj.h"

#define MAX_BUTTON_TOUCH_STATES MAX_BMPS

class ui_button;

typedef void (*on_touch_func_t)(ui_obj *active_obj, ui_button *button, touch_event_t event);

class ui_button : public ui_obj
{
  public :

    uint8_t         prevState;
    on_touch_func_t onTouch_fn[MAX_BUTTON_TOUCH_STATES];

    //Constructor
    ui_button(uint16_t x1, uint16_t y1, uint16_t w1, uint16_t h1, uint8_t id, char *text1 = NULL, obj_type_t type = OBJ_TOUCH) : ui_obj(x1, y1, w1, h1, id, text1, type)
    {
      int i = 0;
      for ( ; i < MAX_BUTTON_TOUCH_STATES; i++)
      {
        onTouch_fn[i] = NULL;
      }

      prevState = T_UP;
    }

    bool set_on_touch_handler(event_type, on_touch_func_t);
    bool set_bmp(char *, event_type);
    void on_touch(ui_obj *active_obj, ui_button *button, touch_event_t event);
    void change_state(event_type);
};

#endif // TFT_IN_USE

#endif // __UI_TOOLBAR__
