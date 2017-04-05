#ifndef __UTILS_PAGE_HOME__
#define __UTILS_PAGE_HOME__

#include "ui_defs.h"

/*HOME PAGE*/

typedef enum
{
  ID_SETTINGS_BUTTON = ID_USER_START,
} home_page_obj_id_t;

#define SETTINGS_UP_BMP     "/settingsUP.palette"
#define SETTINGS_DOWN_BMP   "/settingsDOWN.palette"

void setup_home_page(void);

#endif // __UTILS_PAGE_HOME__
