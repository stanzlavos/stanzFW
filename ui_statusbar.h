/*
 * ui_statusbar.h
 *
 *  Created on: 02-Apr-2017
 *      Author: stanzlavos
 */

#ifndef __UI_STATUSBAR__
#define __UI_STATUSBAR__

#include "ui_defs.h"
#include "ui_page.h"
#include "utils_tft.h"

#define OBJ_STATUS_CHK_INTRVL	1000	// In ms

typedef bool (*status_func_t)(void);

class ui_statusbar
{
public:

	ui_obj *obj[STATUSBAR_OBJ_MAX];
	status_func_t obj_status_func[STATUSBAR_OBJ_MAX];
	uint8_t  obj_cnt;
	uint8_t  obj_status_bifield;
	uint32_t obj_status_time;
	uint16_t text_size; uint16_t text_color;
	uint16_t text_y;  uint16_t text_h;
	uint16_t time_x;  uint16_t time_w;
	uint16_t title_x; uint16_t title_w;
	uint16_t left_x;  uint16_t right_x;

	//Constructor
	ui_statusbar()
	{
		uint8_t i = 0;
		for( ; i < STATUSBAR_OBJ_MAX; i++)
		{
			obj[i] = NULL;
			obj_status_func[i] = NULL;
		}

		obj_cnt = 0;
		obj_status_bifield = 0;
		obj_status_time = millis();

		uint16_t tmp_x, tmp_y;
		tft_get_text_bounds("00:00", 0, 0, (int16_t*)&tmp_x, (int16_t*)&tmp_y, &time_w, &text_h);

		text_y  = (STATUSBAR_H - text_h)/2;
		time_x  = STATUSBAR_W - time_w - STATUSBAR_OBJ_GAP_X;
		right_x = time_x;
		title_x = title_w = left_x = 0;
		text_size = 1; text_color = COLOR_WHITE;
	}

	bool add_obj(ui_obj *obj, status_func_t f=NULL);
	bool is_obj_redraw_needed(void);
	void draw(ui_page *);
};

void setup_statusbar(void);
void update_statusbar(ui_page *active_page);

#endif //__UI_STATUSBAR__
