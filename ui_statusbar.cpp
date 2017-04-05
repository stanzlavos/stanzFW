/*
 * ui_statusbar.cpp
 *
 *  Created on: 02-Apr-2017
 *      Author: stanzlavos
 */

#include "ui_statusbar.h"
#include "utils_ntp.h"
#include "utils_wifi.h"
#include "utils_blynk.h"

ui_statusbar *statusbar = NULL;

bool ui_statusbar::add_obj(ui_obj *_obj, status_func_t _f)
{
	if(obj_cnt == STATUSBAR_OBJ_MAX)
		return false;

	_obj->y = STATUSBAR_OBJ_Y;
	_obj->w = STATUSBAR_OBJ_SIZE;
	_obj->h = STATUSBAR_OBJ_SIZE;

	obj[obj_cnt] = _obj;
	obj_status_func[obj_cnt++] = _f;

	return true;
}

bool ui_statusbar::is_obj_redraw_needed(void)
{
	bool    ret = false;
	uint8_t i   = 0;

	if((millis() - obj_status_time) < OBJ_STATUS_CHK_INTRVL)
		return ret;
	obj_status_time = millis();

	for( ; i < obj_cnt; i++)
	{
		uint8_t new_bit_val = 1;

		if(obj_status_func[i])
			new_bit_val = obj_status_func[i]();

		uint8_t new_bit_mask = (new_bit_val << i);
		uint8_t bit_mask = (1 << i);

		if(new_bit_mask != (obj_status_bifield & bit_mask))
		{
			ret = true;
			obj_status_bifield = (obj_status_bifield & ~(bit_mask)) | (new_bit_mask);
		}
	}

	return ret;
}

void ui_statusbar::draw(ui_page *active_page)
{
	// Update Time
	char time[6] = {'\0'};
	if(get_ntp_time_str_if_mod(time))
	{
		tft_fill_rect(time_x, text_y, time_w, text_h, COLOR_BLACK);
		tft_set_cursor(time_x, text_y);
		tft_set_text_size(text_size);
		tft_set_text_color(text_color);
		tft_print(time);
	}

	// Redraw Objects
	if(is_obj_redraw_needed())
	{
		if(left_x)
			tft_fill_rect(STATUSBAR_OBJ_GAP_X, STATUSBAR_OBJ_GAP_Y, (left_x + STATUSBAR_OBJ_SIZE), STATUSBAR_OBJ_SIZE, COLOR_BLACK);

		uint8_t i = 0, cnt = 0;
		for( ; i < obj_cnt; i++)
		{
			if(obj_status_bifield & (1 << i))
			{
				left_x = obj[i]->x = ((STATUSBAR_OBJ_SIZE + STATUSBAR_OBJ_GAP_X) * cnt++) + STATUSBAR_OBJ_GAP_X;
				obj[i]->draw();
			}
		}
	}

	// Update title
	if(active_page)
	{
		if(title_x)
			tft_fill_rect(title_x, text_y, title_w, text_h, COLOR_BLACK);

		uint16_t tmp_x, tmp_y, tmp_h;
		tft_get_text_bounds(active_page->page_name, 0, 0, (int16_t*)&tmp_x, (int16_t*)&tmp_y, &title_w, &tmp_h);
		title_x = (STATUSBAR_W - title_w)/2;

		tft_set_cursor(title_x, text_y);
		tft_set_text_size(text_size);
		tft_set_text_color(text_color);
		tft_print(active_page->page_name);
	}
}

void setup_statusbar(void)
{
	statusbar = new ui_statusbar;

	ui_obj *obj = new ui_obj(0, 0, 0, 0, OBJ_DEFAULT);
	obj->set_bmp(STATUSBAR_WIFI_BMP);
	statusbar->add_obj(obj, is_wifi_connected);

	obj = new ui_obj(0, 0, 0, 0, OBJ_DEFAULT);
	obj->set_bmp(STATUSBAR_BLYNK_BMP);
	statusbar->add_obj(obj, is_blynk_connected);
}

void update_statusbar(ui_page *active_page)
{
	statusbar->draw(active_page);
}
