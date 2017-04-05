#include "ui_obj.h"

#ifdef TFT_IN_USE

void ui_obj::set_text(char *text1)
{
  if (text1)
  {
    strcpy(text, text1);
    text[MAX_TEXT_SIZE - 1] = '\0';
    set_text_xy();
  }
}

void ui_obj::set_text_color(uint16_t val, uint8_t state)
{
  text_color[state] = val;
}

void ui_obj::set_text_size(uint8_t val)
{
  text_size = val;
  set_text_xy();
}

void ui_obj::set_text_allign(uint8_t val)
{
  text_allign = (text_allign_t)val;
  set_text_xy();
}

#ifdef BMP_FROM_SPIFFS
void ui_obj::set_bmp(char *filename, uint8_t state)                           // Add "/" at the beginning of the filename
{
  if (filename)
  {
    strcpy(bmp_filename[state], filename);
    bmp_filename[state][MAX_FILENAME_SIZE - 1] = '\0';
    bmp = true;
  }
}

File ui_obj::open_palette_file(char *filename)
{
  return (SPIFFS.open(filename, "r"));
}

void ui_obj::close_palette_file(File fp)
{
  fp.close();
}
#else
void ui_obj::set_bmp(uint8_t *ptr)
{
  if (ptr)
    bmp = ptr;
}
#endif

void ui_obj::set_fill_color(uint16_t val, uint8_t state)
{
  fill_color[state] = val;
}

void ui_obj::set_text_xy(void)
{
  if (text[0] != '\0')
  {
    uint16_t tmp_x, tmp_y, tmp_w, tmp_h;

    tft_get_text_bounds(text, 0, 0, (int16_t*)&tmp_x, (int16_t*)&tmp_y, &tmp_w, &tmp_h);

    uint16_t x_diff = (w - tmp_w) / 2;
    uint16_t y_diff = (h - tmp_h) / 2;

     text_y = y + y_diff;
    if (text_allign == ALLIGN_CENTER)
      text_x = x + x_diff;
    else if (text_allign == ALLIGN_LEFT)
      text_x = x + 2;
    else
      text_x = x + x_diff + (x_diff - 2);
  }
}

void ui_obj::draw_text(void)
{
  if ((text[0] != '\0') && (text_color[state] != COLOR_INVALID))
  {
	tft_set_cursor(text_x, text_y);
	tft_set_text_size(text_size);
	tft_set_text_color(text_color[state]);
	tft_print(text);
  }
}

#ifdef BMP_FROM_SPIFFS
void ui_obj::read_bmp_spiffs(File fp, uint16_t col, uint16_t *ptr)
{
  fp.read((uint8_t *)ptr, col * 2);
}
#else
uint16_t ui_obj::read_bmp_progmem(void)
{
  uint16_t result = pgm_read_word_near(bmp + bmp_cnt++);
  bmp_cnt++;
  return result;
}
#endif

// ptr points to an array storing palette information that was generated from a BMP
void ui_obj::draw_bmp(uint8_t x1, uint16_t y1)
{
  //uint16_t w, h;
  uint16_t tft_width  = TFT_WIDTH;
  uint16_t tft_height = TFT_HEIGHT;
  if ((x1 >= tft_width) || (y1 >= tft_height)) return;

#ifdef BMP_FROM_SPIFFS
  File fp = open_palette_file(bmp_filename[state]);
  if (!fp)
    return;
  read_bmp_spiffs(fp, 1, &w);
  read_bmp_spiffs(fp, 1, &h);
#else
  bmp_cnt = 0;
  // First 4 bytes gives height and width
  w = read_bmp_progmem();
  h = read_bmp_progmem();
#endif

  // Crop area to be loaded
  //if ((x1 + w - 1) >= tft_width)  w = tft_width  - x1;
  //if ((y1 + h - 1) >= tft_height) h = tft_height - y1;

  // Set TFT address window to clipped image bounds
  tft_start_write();
  tft_set_addr_window(x1, y1, w, h);

  uint32_t pixel_cnt = w * h, read_cnt = 0;

#ifndef BMP_FROM_SPIFFS
  for ( ; read_cnt < pixel_cnt; read_cnt++)
	  tft_write_pixel(read_bmp_progmem());
#else
  read_cnt = (pixel_cnt < SPIFFS_DEF_READ_CNT) ? pixel_cnt : SPIFFS_DEF_READ_CNT;
  uint16_t palette[read_cnt];

  while (pixel_cnt)
  {
	read_bmp_spiffs(fp, read_cnt, palette);

	tft_write_pixels(palette, read_cnt);

    pixel_cnt -= read_cnt;
    if (pixel_cnt < read_cnt)
      read_cnt = pixel_cnt;
  }

  close_palette_file(fp);
#endif

  tft_end_write();
}

void ui_obj::draw(void)
{
  if (!bmp)
  {
    if (fill_color[state] != COLOR_INVALID)
    {
      tft_fill_round_rect(x, y, w, h, 4, fill_color[state]);
      tft_draw_round_rect(x, y, w, h, 4, COLOR_MAGENTA);
    }
  }
  else
  {
    draw_bmp(x, y);
  }

  draw_text();
}

bool ui_obj::is_touched(touch_event_t event)
{
  if ((event.x > x) &&
      (event.x < (x + w)) &&
      (event.y > y) &&
      (event.y < (y + h)))
  {
    return true;
  }

  return false;
}

#endif // TFT_IN_USE
