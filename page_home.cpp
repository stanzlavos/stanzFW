#include "config.h"

#ifdef TFT_IN_USE

#include "page_home.h"
#include "utils_page.h"
#include "ui_button.h"
#include "ui_popup.h"

// Example page touch handler
void home_touch_handler(ui_page *page, touch_event_t event)
{
  ui_obj *obj = page->find_touch(event);

  if (!obj)
  {
    Serial.println("HOME : NO OBJ !!!");
    return;
  }

  switch (obj->obj_id)
  {
    case  ID_HOME_BUTTON :
      {
        ui_button *b = (ui_button *)obj;
        b->on_touch(page->active_obj, b, event);
        Serial.print("HOME BUTTON : ");
        if (event.type == T_DOWN)
          Serial.println("DOWN");
        else if (event.type == T_UP)
          Serial.println("UP");
        else if (event.type == T_HOLD)
          Serial.println("HOLD");

        break;
      }

    case  ID_BACK_BUTTON :
      {
        ui_button *b = (ui_button *)obj;
        b->on_touch(page->active_obj, b, event);
        Serial.print("BACK BUTTON : ");
        if (event.type == T_DOWN)
          Serial.println("DOWN");
        else if (event.type == T_UP)
          Serial.println("UP");
        else if (event.type == T_HOLD)
          Serial.println("HOLD");

        break;
      }

    default :
      {
        Serial.print("DEFAULT ");
        Serial.print(obj->obj_id);
        Serial.print(" : ");
        if (event.type == T_DOWN)
          Serial.println("DOWN");
        else if (event.type == T_UP)
          Serial.println("UP");
        else if (event.type == T_HOLD)
          Serial.println("HOLD");
      }
  }
}

void on_settings_button_up(ui_obj *active_obj, ui_button *button, touch_event_t event)
{
  if(active_obj == (ui_obj *)button)
  {
    launch_next_page(ID_SETTINGS);
  }  
}

void setup_home_page(void)
{
  ui_page *home_page = get_new_page(ID_HOME, "Home");

  // x,y,h and w will get updated automatically when added to toolbar
  // h,w will be updated later picked from BMP itself
  //ui_button *settings_button = new ui_button(0, 0, 0, 0, ID_SETTINGS_BUTTON);
  //settings_button->set_bmp(SETTINGS_UP_BMP, T_UP);
  //settings_button->set_bmp(SETTINGS_DOWN_BMP, T_DOWN);
  //home_page->add_toolbar_obj(back_button);

  //home_page->set_work_area_type(WORK_ROWS);

  // x,y,h and w will get updated automatically when added to work area
  ui_button *settings_button = new ui_button(0, 0, 0, 0, (ID_SETTINGS_BUTTON));
  settings_button->set_bmp(SETTINGS_UP_BMP, T_UP);
  settings_button->set_bmp(SETTINGS_DOWN_BMP, T_DOWN);
  settings_button->set_on_touch_handler(T_UP, on_settings_button_up);

  home_page->add_work_area_obj(settings_button);
  //home_page->set_touch_handler(home_touch_handler);
}

#endif // TFT_IN_USE
