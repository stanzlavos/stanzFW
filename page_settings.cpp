#include "config.h"

#ifdef TFT_IN_USE

#include "page_settings.h"
#include "ui_page.h"
#include "ui_button.h"
#include "utils_page.h"
#include "utils_debug.h"

void setup_settings_page(void)
{
  ui_page *settings_page = get_new_page(ID_SETTINGS, "Settings", (TOOLBAR_FLAG_HOME | TOOLBAR_FLAG_BACK));

  settings_page->set_work_area_type(WORK_ROWS);

  ui_button *button = NULL;
  uint8_t i = 0;
  for( ; i < 9; i++)
  {
    button = new ui_button(0, 0, 0, 0, (SETTINGS_OBJ_1 + i));
    char str[5];
    sprintf(str,"%d", (i+1));
    button->set_text(str);
    if(!settings_page->add_work_area_obj(button))
      PRINTLN("Settings page : Add obj failed !!!");
  }
}

#endif // TFT_IN_USE
