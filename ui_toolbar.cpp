#include "ui_toolbar.h"

#ifdef TFT_IN_USE

#include "ui_defs.h"

bool ui_toolbar::add_obj(ui_obj *obj)
{
  if (obj_cnt == TOOLBAR_OBJ_MAX)
    return false;

  obj->x = ((TOOLBAR_OBJ_SIZE + TOOLBAR_OBJ_GAP_X) * obj_cnt) + TOOLBAR_OBJ_GAP_X;
  obj->y = (y + TOOLBAR_OBJ_GAP_Y);
  obj->h = obj->w = TOOLBAR_OBJ_SIZE;
  obj->set_text_xy();
  
  obj_list[obj_cnt++] = obj;

  return true;
}

ui_obj* ui_toolbar::find_touch(touch_event_t event)
{
  uint8_t i = 0;
  for ( ; i < obj_cnt; i++)
    if (obj_list[i]->obj_type != OBJ_NO_TOUCH)
      if (obj_list[i]->is_touched(event))
        return obj_list[i];
  
  return NULL;
}

void ui_toolbar::draw(void)
{
  ui_obj::draw();

  uint8_t i = 0;
  for ( ; i < obj_cnt; i++)
  {
    obj_list[i]->draw();
  }  
}

#endif // TFT_IN_USE
