#include "ui_button.h"

#ifdef TFT_IN_USE

bool ui_button::set_on_touch_handler(event_type type, on_touch_func_t f)
{
  bool ret = false;

  if (type <= T_DOWN)
  {
    onTouch_fn[type] = f;
    ret = true;
  }

  return ret;
}

bool ui_button::set_bmp(char *filename, event_type type)
{
  bool ret = false;

  if (type <= T_DOWN)
  {
    if (filename)
    {
      ui_obj::set_bmp(filename, type);

      ret = true;
    }
  }
  
  return ret;
}

void ui_button::change_state(event_type type)
{
  prevState   = state;
  state       = type;

  draw();

  state       = prevState;
}

void ui_button::on_touch(ui_page *page, ui_button *button, touch_event_t event)
{
  event_type type = event.type;
  if (type <= T_DOWN)
  {
    change_state(type);
    
    if(onTouch_fn[type])
      onTouch_fn[type](page, button, event);
  }
}

#endif // TFT_IN_USE
