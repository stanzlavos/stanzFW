#ifndef __UTILS_UI_TEST__
#define __UTILS_UI_TEST__

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  uint16_t color;
} ui_obj_t;

ui_obj_t rect1 = {94, 134, 50, 50, ILI9341_RED};

touch_event_t prev_event = {TS_UNDEFINED, TS_UNDEFINED, TS_UNDEFINED, T_UP};

void handle_ui(void)
{
  switch (touch_event.type)
  {
    case T_DOWN :
      {
        if ((touch_event.x > rect1.x) &&
            (touch_event.x < (rect1.x + rect1.w)) &&
            (touch_event.y > rect1.y) &&
            (touch_event.y < (rect1.y + rect1.h)))
        {
          prev_event = touch_event;
          tft.drawRect(rect1.x, rect1.y, rect1.w, rect1.h, ILI9341_BLUE);
        }
        break;
      }
    case T_HOLD :
      {
        if ((touch_event.x > rect1.x) &&
            (touch_event.x < (rect1.x + rect1.w)) &&
            (touch_event.y > rect1.y) &&
            (touch_event.y < (rect1.y + rect1.h)))
        {
          int16_t   x_diff     = touch_event.x - prev_event.x;
          uint16_t  x_diff_abs = abs(x_diff);
          int16_t   y_diff     = touch_event.y - prev_event.y;
          uint16_t  y_diff_abs = abs(y_diff);

          if ((x_diff_abs > 10) || (y_diff_abs > 10))
          {
            tft.drawRect(rect1.x, rect1.y, rect1.w, rect1.h, ILI9341_BLACK);
            rect1.x += x_diff;
            rect1.x = constrain(rect1.x, TFT_X_MIN, TFT_X_MAX);
            rect1.y += y_diff;
            rect1.y = constrain(rect1.y, TFT_Y_MIN, TFT_Y_MAX);
            tft.drawRect(rect1.x, rect1.y, rect1.w, rect1.h, ILI9341_BLUE);
            prev_event = touch_event;
          }
          break;
        }
      }
    case T_UP   :
      {
        prev_event.x    = TS_UNDEFINED;
        prev_event.y    = TS_UNDEFINED;
        prev_event.z    = TS_UNDEFINED;
        prev_event.type = T_UP;
        tft.drawRect(rect1.x, rect1.y, rect1.w, rect1.h, rect1.color);
        break;
      }
  }
}

#endif // __UTILS_UI_TEST__

