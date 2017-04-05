#include "utils_touch.h"

#ifdef TFT_IN_USE

#include <XPT2046_Touchscreen.h>
#include "utils_tft.h"

XPT2046_Touchscreen ts(TS_CS_PIN);

typedef struct
{
  touch_event_t event;
  uint32_t      t;
} swipe_event_t;

touch_event_t touch_event = {TS_UNDEFINED, TS_UNDEFINED, TS_UNDEFINED, T_UP};
swipe_event_t swipe_event = {{TS_UNDEFINED, TS_UNDEFINED, TS_UNDEFINED, T_UP}, 0};

void setup_TOUCH(void)
{
  ts.begin();
}

touch_event_t get_touch_event(void)
{
  return touch_event;
}

touch_event_t get_touch(void)
{
  touch_event_t ret = {TS_UNDEFINED, TS_UNDEFINED, TS_UNDEFINED, T_UP};
  uint16_t      x=0, y=0, z=0;
  uint8_t       cnt=0;
  for (cnt = 0; cnt < TS_SAMPLES; cnt++)
  {
    if (ts.touched()) 
    {
      TS_Point p = ts.getPoint();

      uint16_t tmp_x = map(p.y, TS_Y_MIN, TS_Y_MAX, TFT_X_MIN, TFT_X_MAX);
      x += constrain(tmp_x, TFT_X_MIN, TFT_X_MAX);
      uint16_t tmp_y = map(p.x, TS_X_MIN, TS_X_MAX, TFT_Y_MIN, TFT_Y_MAX);
      y += constrain(tmp_y, TFT_Y_MIN, TFT_Y_MAX);
      z += p.z;
    }
    else
    {
      break;
    }
    
    delay(TS_INTERVAL);
  }

  if(cnt == TS_SAMPLES)
  {
    ret.x     = x/TS_SAMPLES;
    ret.y     = y/TS_SAMPLES;
    ret.z     = z/TS_SAMPLES;
    ret.type  = T_DOWN;
  }

  return ret;
}

void handle_touch(void)
{
  touch_event_t t_event = get_touch();

  if(t_event.x != TS_UNDEFINED)
  {
    event_type prev = touch_event.type;
    touch_event     = t_event;
    if((prev == T_DOWN) || (prev == T_HOLD))
    {
      touch_event.type = T_HOLD;
    }
    else if(prev == T_UP)
    {
      swipe_event.event = t_event;
      swipe_event.t     = millis();
    }
  }
  else
  {
    event_type type = touch_event.type;
    touch_event.type = T_UP;

    if((type == T_DOWN || (type == T_HOLD)) && (swipe_event.t))
    {
      int16_t   x_diff      = swipe_event.event.x - touch_event.x;
      uint16_t  x_diff_abs  = abs(x_diff);
      int16_t   y_diff      = swipe_event.event.y - touch_event.y;
      uint16_t  y_diff_abs  = abs(y_diff);
      uint32_t  t_diff      = millis() - swipe_event.t;

      if((t_diff > TS_SWIPE_INT_MIN) && (t_diff < TS_SWIPE_INT_MAX))
      {
        if(x_diff_abs > TS_SWIPE)
        {
          if(x_diff < 0)
            touch_event.type = T_SWIPE_RIGHT;
          else
            touch_event.type = T_SWIPE_LEFT;
        }
        else if(y_diff_abs > TS_SWIPE)
        {
          if(y_diff < 0)
            touch_event.type = T_SWIPE_DOWN;
          else
            touch_event.type = T_SWIPE_UP;
        }
      }
    }

    swipe_event.event.x = TS_UNDEFINED;
    swipe_event.event.y = TS_UNDEFINED;
    swipe_event.event.z = TS_UNDEFINED;
    swipe_event.t       = 0;      
  }
}

#endif // TFT_IN_USE
