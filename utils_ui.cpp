#include "utils_ui.h"

#ifdef TFT_IN_USE

#include "utils_page.h"
#include "page_home.h"
#include "page_settings.h"
#include "utils_touch.h"
#include "utils_ntp.h"
#include "ui_statusbar.h"

void setup_ui(void)
{
	setup_statusbar();
	setup_home_page();
	setup_settings_page();
}

touch_event_t 	prev_event    = {TS_UNDEFINED, TS_UNDEFINED, TS_UNDEFINED, T_UP};
extern ui_page 	*active_page;

void handle_ui(void)
{
	update_statusbar(NULL);

	if(is_page_redraw_needed())
	{
		prev_page_id = next_page_id;
		active_page  = get_page(next_page_id);
		update_statusbar(active_page);
		active_page->draw();
	}

	touch_event_t t_event = get_touch_event();
	if((t_event.type == T_HOLD) || (t_event.type != prev_event.type))
	{
		prev_event = t_event;
		active_page->handle_touch(active_page, t_event);
		if(t_event.type == T_UP)
			active_page->active_obj = NULL;
	}
}

#endif // TFT_IN_USE
