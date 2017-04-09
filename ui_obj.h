#ifndef __UI_OBJ__
#define __UI_OBJ__

#include "config.h"

#ifdef TFT_IN_USE

#include "utils_touch.h"
#include "utils_tft.h"
#include "ui_defs.h"

#define BMP_FROM_SPIFFS

#ifdef BMP_FROM_SPIFFS
#include "FS.h"

#define SPIFFS_DEF_READ_CNT 255
#endif

#define MAX_TEXT_SIZE       64
#define MAX_BMPS            3
#define MAX_FILENAME_SIZE   32

#define OBJ_DEFAULT         0

typedef enum
{
  ALLIGN_LEFT,
  ALLIGN_RIGHT,
  ALLIGN_CENTER
} text_allign_t;

typedef enum
{
  OBJ_TOUCH,
  OBJ_MOVE,     // Implicitely OBJ_TOUCH
  OBJ_NO_TOUCH
} obj_type_t;

class ui_obj
{
  public :

    uint16_t      x;
    uint16_t      y;
    uint16_t      w;
    uint16_t      h;

    obj_type_t    obj_type;
    uint8_t       obj_id;
    char          text[MAX_TEXT_SIZE];
    int16_t       text_x;
    int16_t       text_y;
    uint16_t      text_color[MAX_BMPS];
    text_allign_t text_allign;
    uint8_t       text_size;

#ifdef BMP_FROM_SPIFFS
    char          bmp_filename[MAX_BMPS][MAX_FILENAME_SIZE];
    uint8_t       state;
    bool          bmp;
#else
    uint8_t       *bmp;
    uint32_t      bmp_cnt;
#endif
    uint16_t      fill_color[MAX_BMPS];         // Used if no BMP is specified

    // Contructor
    ui_obj(uint16_t x1, uint16_t y1, uint16_t w1, uint16_t h1, uint8_t id, char *text1 = NULL, obj_type_t type = OBJ_TOUCH)
    {
      x = x1; y = y1; w = w1; h = h1;

      obj_type  = type;
      obj_id    = id;

      text[0] = '\0';
      if (text1)
      {
        strcpy(text, text1);
        text[MAX_TEXT_SIZE - 1] = '\0';
      }

      text_color[T_UP]    = COLOR_BLACK;
      text_color[T_HOLD]  = COLOR_INVALID;
      text_color[T_DOWN]  = COLOR_WHITE;
      text_allign = ALLIGN_CENTER;
      text_x      = -1;
      text_y      = -1;
      text_size   = 1;
#ifdef BMP_FROM_SPIFFS
      int i = 0;
      for ( ; i < MAX_BMPS; i++)
        bmp_filename[i][0] = '\0';
      state       = 0;
      bmp         = false;
#else
      bmp         = NULL;
      bmp_cnt     = 0;
#endif
      fill_color[T_UP]    = COLOR_YELLOW;
      fill_color[T_HOLD]  = COLOR_INVALID;
      fill_color[T_DOWN]  = COLOR_BLUE;
    }

    void set_text(char *);
    void set_text_color(uint16_t, uint8_t state = 0);
    void set_text_allign(uint8_t);
    void set_text_size(uint8_t);
    void set_text_xy(void);
#ifdef BMP_FROM_SPIFFS
    void set_bmp(char *, uint8_t state = 0);             // Add "/" at the beginning of the filename
    void read_bmp_spiffs(File, uint16_t, uint16_t *);
    File open_palette_file(char *);
    void close_palette_file(File);
#else
    void set_bmp(uint8_t *);
    uint16_t read_bmp_progmem(void);
#endif
    void set_fill_color(uint16_t, uint8_t state = 0);
    // Draw self
    void draw(void);
    void draw_text(void);
    void draw_bmp(uint8_t x, uint16_t y);

    bool is_touched(touch_event_t event);
};

#endif // TFT_IN_USE

#endif // __UI_OBJ__ 
