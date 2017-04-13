#ifndef __UI_POPUP__
#define __UI_POPUP__

#include "config.h"

#ifdef TFT_IN_USE

#define POPUP_MSG_MAX	64
#define POPUP_OBJS		2

#define POPUP_OK		0x01
#define POPUP_CANCEL	0x02

#include "ui_defs.h"

class ui_popup
{
  public :

	char 	msg[POPUP_MSG_MAX];
	uint8_t	timer_val;
	uint8_t	flags;		//bit field
	ui_obj	*obj[POPUP_OBJS];

	// Constructor
	ui_popup()
  	{
		msg[0] 		= '\0';
		timer_val	= 0;
		flags		= 0;

		obj[0] = new ui_button(0, 0, 0, 0, ID_HOME_BUTTON);
  	}
};

#endif // TFT_IN_USE

#endif // __UI_POPUP__
