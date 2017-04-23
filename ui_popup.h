#ifndef __UI_POPUP__
#define __UI_POPUP__

#include "config.h"

#ifdef TFT_IN_USE

#include "ui_button.h"
#include "ui_defs.h"

#define POPUP_MSG_MAX		33				// Max 32 characters in a line
#define POPUP_MSG_LINES		3

#define POPUP_OK			0x01
#define POPUP_CANCEL		0x02

class ui_popup
{
public :

	char 		msg[POPUP_MSG_LINES][POPUP_MSG_MAX];
	uint8_t		msg_h;
	uint8_t		timer_val;
	uint8_t		flags;		//bit field
	ui_button	*obj[POPUP_OBJ_MAX];
	uint8_t		num_obj;
	uint8_t		num_lines;
	uint16_t    text_color;
	uint8_t     text_size;

	// Constructor
	ui_popup()
	{
		uint8_t i = 0;
		for( ; i<POPUP_MSG_LINES; i++)
		{
			msg[i][0] = '\0';
		}

		msg_h		= 0;
		timer_val	= 0;
		flags		= 0;
		num_obj		= 0;
		num_lines	= 0;
		text_size   = 1;
		text_color  = COLOR_WHITE;

		obj[0] = new ui_button(0, 0, POPUP_OBJ_W, POPUP_OBJ_H, ID_POPUP_OK);
		obj[0]->set_bmp(OK_UP_BMP, T_UP);
		obj[0]->set_bmp(OK_DOWN_BMP, T_DOWN);
		//obj[0]->set_on_touch_handler(T_UP, on_home_button_up);

		obj[1] = new ui_button(0, 0, POPUP_OBJ_W, POPUP_OBJ_H, ID_POPUP_CANCEL);
		obj[1]->set_bmp(CANCEL_UP_BMP, T_UP);
		obj[1]->set_bmp(CANCEL_DOWN_BMP, T_DOWN);
		//obj[0]->set_on_touch_handler(T_UP, on_home_button_up);
	}

	void 	set_msg(char *);
	void 	set_timer(uint8_t);
	void 	set_objs(uint8_t);
	void	set_text_size(uint8_t);
	void	set_text_color(uint16_t);
	void 	set_on_touch_handler(uint8_t, event_type, on_touch_func_t);

	void	draw_text(void);
	void	draw_obj(void);
	void	draw_boundary(void);
	void 	draw(void);
	ui_obj* find_touch(touch_event_t event);
};

void popup_set(char *msg, uint8_t flags = 0);
void popup_set_on_touch_handler(uint8_t obj_mask, event_type type, on_touch_func_t f);
bool popup_launch(void);
void popup_destroy(void);
void popup_set_timer(uint8_t);

#endif // TFT_IN_USE

#endif // __UI_POPUP__
