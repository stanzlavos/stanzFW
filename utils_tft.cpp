#include "utils_tft.h"

#ifdef TFT_IN_USE

#include "SPI.h"
#include "ui_defs.h"

#ifdef _TFT_eSPI_
#include <TFT_eSPI.h>
#undef TFT_HEIGHT
#undef TFT_WIDTH
#else
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#endif

#ifdef _TFT_eSPI_
TFT_eSPI tft = TFT_eSPI();
#else
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
#endif

int16_t TFT_HEIGHT  =       0;
int16_t TFT_WIDTH   =       0;

#ifdef _TFT_eSPI_
TFT_eSPI *get_tft_instance(void)
#else
Adafruit_ILI9341 *get_tft_instance(void)
#endif
{
	return &tft;
}

void configure_tft(uint8_t cmd, uint8_t params[TFT_CMD_MAX_PARAMS], uint8_t num)
{
	if(num <= TFT_CMD_MAX_PARAMS)
	{
		SPI.beginTransaction(SPISettings(24000000, MSBFIRST, SPI_MODE0));

		SPI.transfer(cmd);

		for(int cnt=0; cnt<num; cnt++)
		{
			SPI.transfer(params[cnt]);
		}

		SPI.endTransaction();
	}
}

void setup_tft(void)
{
	uint8_t params[TFT_CMD_MAX_PARAMS] = {0};

	tft.begin();
	tft.setRotation(TFT_PORTRAIT);

	TFT_HEIGHT  = tft.height();
	TFT_WIDTH   = tft.width();

#ifdef TFT_SCROLL_ON
	params[0] = 0x00;
	params[1] = 0x00;
	params[2] = 0x01;
	params[3] = 0x40;
	params[4] = 0x00;
	params[5] = 0x00;
	configure_TFT(0x33, params, 6);
#endif

	tft.fillScreen(COLOR_BLACK);
	tft.setCursor(0, 0);
	tft.setTextColor(COLOR_WHITE);
	tft.setTextSize(1);
}

void tft_scroll(void)
{
#ifdef TFT_SCROLL_ON
	static uint16_t offset    = 0;
	static uint16_t last_line = 312;
	uint8_t params[2]         = {0};

	int16_t y = tft.getCursorY();

	static bool scroll = false;
	if (y > last_line || scroll)
	{
		scroll = true;

		last_line += 8;
		if (last_line > 312)
			last_line = 0;

		offset += 8;
		if (offset > 312)
			offset = 0;

		params[0] = offset >> 8;
		params[1] = offset;
		configure_TFT(0x37, params, 2);

		tft.setCursor(0, last_line);
		tft.fillRect(0, last_line, 240, 8, COLOR_BLACK);
	}
#endif
}

void tft_clear(void)
{
	tft.fillScreen(ILI9341_BLACK);
	tft.setCursor(0, 0);
}

void tft_clear_work_area(void)
{
	tft.fillRect(WORK_AREA_FRAME_X + 1, WORK_AREA_FRAME_Y + 1, WORK_AREA_FRAME_W - 2,
			WORK_AREA_FRAME_H - 2, COLOR_BLACK);
}

void tft_get_text_bounds(char *str, int16_t x, int16_t y,
		int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)
{
#ifdef _TFT_eSPI_
	*x1 = x;
	*y1 = y;
	*w  = tft.textWidth(str);
	*h  = 8;
#else  
	tft.getTextBounds(str, x, y, x1, y1, w, h);
#endif 
}

void tft_set_cursor(int16_t x, int16_t y)
{
	tft.setCursor(x, y);
}

void tft_set_text_size(uint8_t s)
{
	tft.setTextSize(s);
}

void tft_set_text_color(uint16_t c)
{
	tft.setTextColor(c);
}

void tft_start_write(void)
{
#ifdef _TFT_eSPI_
	return;
#else  
	tft.startWrite();
#endif 
}

void tft_end_write(void)
{
#ifdef _TFT_eSPI_
	return;
#else
	tft.endWrite();
#endif 
}

void tft_set_addr_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
#ifdef _TFT_eSPI_
	tft.setWindow(x, y, x+w-1, y+h-1);
#else
	tft.setAddrWindow(x, y, w, h);
#endif
}

void tft_write_pixel(uint16_t c)
{
#ifdef _TFT_eSPI_
	tft.pushColor(c);
#else  
	tft.writePixel(c);
#endif  
}

void tft_write_pixels(uint16_t *colors, uint32_t len)
{
#ifdef _TFT_eSPI_
	tft.pushColors(colors, len);
#else  
	tft.writePixels(colors, len);
#endif 
}

void tft_fill_round_rect(int16_t x, int16_t y, int16_t w,
		int16_t h, int16_t r, uint16_t color)
{
	tft.fillRoundRect(x, y, w, h, r, color);
}

void tft_draw_round_rect(int16_t x, int16_t y, int16_t w, int16_t h,
		int16_t r, uint16_t color)
{
	tft.drawRoundRect(x, y, w, h, r, color);
}

void tft_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	tft.fillRect(x, y, w, h, color);
}

void tft_draw_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	tft.drawRect(x, y, w, h, color);
}

void tft_fill_traingle(int16_t x0, int16_t y0,
		int16_t x1, int16_t y1,
		int16_t x2, int16_t y2, uint16_t color)
{
	tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void tft_print(const char str[])
{
	tft.print(str);
}

void tft_print(char c)
{
	tft.print(c);
}

void tft_print(unsigned char c, int t)
{
	tft.print(c, t);
}

void tft_print(int v, int t)
{
	tft.print(v, t);
}

void tft_print(unsigned int v, int t)
{
	tft.print(v, t);
}

void tft_print(long v, int t)
{
	tft.print(v, t);
}

void tft_print(unsigned long v, int t)
{
	tft.print(v, t);
}

void tft_print(double v, int t)
{
	tft.print(v, t);
}

void tft_print(const String &str)
{
	tft.print(str);
}

void tft_print(const __FlashStringHelper *str)
{
	tft.print(str);
}

void tft_print(const Printable &str)
{
	tft.print(str);
}

void tft_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	tft.printf(format, args);
	va_end(args);
}


void tft_println(const char str[])
{
	tft.println(str);
}

void tft_println(const __FlashStringHelper *str)
{
	tft.println(str);
}

void tft_println(const String &str)
{
	tft.println(str);
}

void tft_println(char c)
{
	tft.println(c);
}

void tft_println(unsigned char c, int t)
{
	tft.println(c, t);
}

void tft_println(int v, int t)
{
	tft.println(v, t);
}

void tft_println(unsigned int v, int t)
{
	tft.println(v, t);
}

void tft_println(long v, int t)
{
	tft.println(v, t);
}

void tft_println(unsigned long v, int t)
{
	tft.println(v, t);
}

void tft_println(double v, int t)
{
	tft.println(v, t);
}

void tft_println(const Printable &str)
{
	tft.println(str);
}

void tft_println(void)
{
	tft.println();
}


#endif // TFT_IN_USE
