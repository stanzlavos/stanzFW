#include "ui_popup.h"

#ifdef TFT_IN_USE

#include "utils_page.h"
#include "utils_debug.h"

void ui_popup::set_msg(char *text)
{
	if (text)
	{
		uint8_t msg_gap = 0;
		char *token = strtok(text, "\n");
		while((token != NULL) && (num_lines != POPUP_MSG_LINES))
		{
			uint16_t tmp_x, tmp_y, tmp_w, tmp_h;

			uint16_t str_len = strlen(token);
			str_len = (str_len > (POPUP_MSG_MAX - 1)) ? (POPUP_MSG_MAX - 1) : str_len;
			strncpy(msg[num_lines], token, str_len);
			msg[num_lines][str_len] = '\0';

			tft_get_text_bounds(msg[num_lines], 0, 0, (int16_t*)&tmp_x, (int16_t*)&tmp_y, &tmp_w, &tmp_h);
			msg_h += tmp_h + msg_gap;

			msg_gap = POPUP_MSG_GAP;

			num_lines++;
			token = strtok (NULL, "\n");
		}
	}
}

void ui_popup::set_timer(uint8_t val)
{
	timer_val = val;
}

void ui_popup::set_objs(uint8_t obj_mask)
{
	flags = obj_mask;

	uint8_t i = 0;
	for( ; i < POPUP_OBJ_MAX; i++)
	{
		if(flags & (1 << i))
		{
			num_obj++;
		}
	}
}

void ui_popup::set_text_size(uint8_t size)
{
	text_size = size;
}

void ui_popup::set_text_color(uint16_t color)
{
	text_color = color;
}

void ui_popup::draw_text(void)
{
	uint16_t tmp_x, tmp_y, tmp_w, tmp_h, obj_h = 0, msg_x, msg_y;

	if(flags)
		obj_h = POPUP_OBJ_H;

	msg_y = POPUP_Y + ((POPUP_H - obj_h - POPUP_OBJ_GAP_2 - msg_h)/2);

	tft_set_text_size(text_size);
	tft_set_text_color(text_color);

	uint8_t i = 0;
	for( ; i < num_lines; i ++)
	{
		tft_get_text_bounds(msg[i], 0, 0, (int16_t*)&tmp_x, (int16_t*)&tmp_y, &tmp_w, &tmp_h);
		msg_x = POPUP_X + ((POPUP_W - tmp_w)/2);
		tft_set_cursor(msg_x, msg_y);
		msg_y += tmp_h + POPUP_MSG_GAP;

		tft_print(msg[i]);
	}
}

void ui_popup::draw_obj(void)
{
	uint16_t tmp_x = POPUP_X + POPUP_OBJ_GAP_1;
	if(num_obj == POPUP_OBJ_MAX)
		tmp_x = POPUP_X + POPUP_OBJ_GAP_2;

	uint8_t i = 0;
	for( ; i < POPUP_OBJ_MAX; i++)
	{
		if(flags & (1 << i))
		{
			tmp_x = tmp_x + (i * (POPUP_OBJ_W + POPUP_OBJ_GAP_2));
			obj[i]->x = tmp_x;
			obj[i]->y = POPUP_OBJ_Y;
			obj[i]->draw();
		}
	}
}

void ui_popup::draw_boundary(void)
{
	tft_draw_round_rect(POPUP_X, POPUP_Y, POPUP_W, POPUP_H, POPUP_R, COLOR_MAGENTA);
}

void ui_popup::draw(void)
{
	if(msg[0][0] != '\0')
	{
		draw_boundary();

		draw_text();

		draw_obj();
	}
}

ui_obj* ui_popup::find_touch(touch_event_t event)
{
	uint8_t i = 0;
	for ( ; i < POPUP_OBJ_MAX; i++)
		if (flags & (1 << i))
			if (obj[i]->is_touched(event))
				return obj[i];

	return NULL;
}

void ui_popup::set_on_touch_handler(uint8_t obj_mask, event_type type, on_touch_func_t f)
{
	uint8_t i = 0;
	for ( ; i < POPUP_OBJ_MAX; i++)
		if (obj_mask & (1 << i))
			obj[i]->set_on_touch_handler(type, f);
}

ui_popup popup;   // Global popup object

void popup_set(char *msg, uint8_t flags)
{
	char str[256];
	strcpy(str, msg);
  
	popup.set_msg(str);
	popup.set_objs(flags);
}

void popup_clear(void)
{
	uint8_t i = 0;
	for( ; i<POPUP_MSG_LINES; i++)
	{
		popup.msg[i][0] = '\0';
	}
	popup.flags 	   = 0;
	popup.num_obj	   = 0;
	popup.num_lines  = 0;
	popup.timer_val    = 0;
	popup.msg_h		   = 0;
	popup.text_size    = 1;
	popup.text_color   = COLOR_WHITE;
}

void popup_set_on_touch_handler(uint8_t obj, event_type type, on_touch_func_t f)
{
	popup.set_on_touch_handler(obj, type, f);
}

void popup_launch(void)
{
	get_active_page()->popup = &popup;
	trigger_page_redraw();
}

void popup_destroy(void)
{
	get_active_page()->popup = NULL;
	popup_clear();
	tft_clear_work_area();
	trigger_page_redraw();
}

#endif // TFT_IN_USE
