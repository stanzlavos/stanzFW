#ifndef __UTILS_TFT__
#define __UTILS_TFT__

#include "config.h"

/*
 *  (0,0)       (239,0)
 *    +------------+
 *    |            |
 *    |            |
 *    |            |
 *    |            |
 *    |            |
 *    |            |
 *    |            |
 *    +------------+
 * (0,319)     (239,319)
 * 
 */
 
#ifdef TFT_IN_USE

#include "stdint.h"
#include "Arduino.h"

#include "WString.h"
#include "Printable.h"

#define COLOR_INVALID       0x0001  // Just picked one !!!
#define COLOR_MAGENTA		0xF81F
#define COLOR_BLACK			0x0000
#define COLOR_WHITE			0xFFFF
#define COLOR_YELLOW		0xFFE0
#define COLOR_BLUE			0x001F

#define TFT_PORTRAIT        0

#define TFT_X_MIN           0
#define TFT_X_MAX           (TFT_WIDTH - 1)
#define TFT_Y_MIN           0
#define TFT_Y_MAX           (TFT_HEIGHT - 1)

#define TFT_DC              D1
#define TFT_CS              D2

#define TFT_CMD_MAX_PARAMS  12

extern  int16_t TFT_HEIGHT;
extern  int16_t TFT_WIDTH;

void setup_tft(void);

void tft_clear_work_area(void);
void tft_scroll(void);
void tft_clear(void);
void tft_get_text_bounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void tft_set_cursor(int16_t x, int16_t y);
void tft_set_text_size(uint8_t s);
void tft_set_text_color(uint16_t c);
void tft_start_write(void);
void tft_end_write(void);
void tft_set_addr_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void tft_write_pixel(uint16_t c);
void tft_write_pixels(uint16_t *colors, uint32_t len);
void tft_fill_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void tft_draw_round_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void tft_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void tft_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void tft_fill_traingle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

void tft_print(const char str[]);
void tft_print(char c);
void tft_print(unsigned char, int = DEC);
void tft_print(int, int = DEC);
void tft_print(unsigned int, int = DEC);
void tft_print(long, int = DEC);
void tft_print(unsigned long, int = DEC);
void tft_print(double, int = 2);
void tft_print(const String &);
void tft_printf(const char * format, ...)  __attribute__ ((format (printf, 1, 2)));
void tft_print(const __FlashStringHelper *);
void tft_print(const Printable&);

void tft_println(const char str[]);
void tft_println(const __FlashStringHelper *);
void tft_println(const String &s);
void tft_println(char);
void tft_println(unsigned char, int = DEC);
void tft_println(int, int = DEC);
void tft_println(unsigned int, int = DEC);
void tft_println(long, int = DEC);
void tft_println(unsigned long, int = DEC);
void tft_println(double, int = 2);
void tft_println(const Printable&);
void tft_println(void);

#endif // TFT_IN_USE

#endif // __UTILS_TFT__
