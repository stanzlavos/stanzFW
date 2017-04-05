#ifndef __UTILS_TOUCH__
#define __UTILS_TOUCH__

#include "config.h"

/*
    (230,3820)   (230,300)
        +------------+
        |            |
        |            |
        |            |
        |            |
        |            |
        |            |
        |            |
        +------------+
   (3650,3820)  (3650,300)
*/

#ifdef TFT_IN_USE

#include "utils_defs.h"

#define TS_CS_PIN         D8

#define TS_X_MIN          230
#define TS_X_MAX          3650
#define TS_Y_MIN          3820
#define TS_Y_MAX          300

#define TS_UNDEFINED      999

#define TS_SAMPLES        3
#define TS_INTERVAL       10

#define TS_SWIPE          75
#define TS_SWIPE_INT_MIN  75
#define TS_SWIPE_INT_MAX  1000

typedef enum
{
  T_UP,           // Do not change position
  T_HOLD,         // Do not change position
  T_DOWN,         // Do not change position
  T_SWIPE_DOWN,
  T_SWIPE_UP,
  T_SWIPE_LEFT,
  T_SWIPE_RIGHT
} event_type;

typedef struct
{
  int16_t         x;
  int16_t         y;
  int16_t         z;
  event_type      type;
} touch_event_t;

void setup_TOUCH(void);
void handle_touch(void);
touch_event_t get_touch_event(void);

#endif // TFT_IN_USE

#endif // __UTILS_TOUCH__

