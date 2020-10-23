#ifndef _LCDDRIVER_H_
#define _LCDDRIVER_H_

//List of includes
#include <stdbool.h>
#include "stm32h7xx_hal.h"
#include <stdlib.h>
#include <stdio.h>
#include "settings.h"
#include "images.h"

//LCD dimensions defines
#if (defined(LCD_ILI9481) || defined(LCD_HX8357B))
	#define LCD_WIDTH 480
	#define LCD_HEIGHT 320
#endif

extern uint32_t LCD_FSMC_COMM_ADDR;
extern uint32_t LCD_FSMC_DATA_ADDR;

#define LCD_PIXEL_COUNT (LCD_WIDTH * LCD_HEIGHT)

//ILI9481 LCD commands
#if (defined(LCD_ILI9481) || defined(LCD_HX8357B))
#define LCD_COMMAND_COLUMN_ADDR 0x2A
#define LCD_COMMAND_PAGE_ADDR 0x2B
#define LCD_COMMAND_GRAM 0x2C
#define LCD_COMMAND_SOFT_RESET 0x01
#define LCD_COMMAND_NORMAL_MODE_ON 0x13
#define LCD_COMMAND_VCOM 0xD1
#define LCD_COMMAND_NORMAL_PWR_WR 0xD2
#define LCD_COMMAND_PANEL_DRV_CTL 0xC0
#define LCD_COMMAND_FR_SET 0xC5
#define LCD_COMMAND_GAMMAWR 0xC8
#define LCD_COMMAND_PIXEL_FORMAT 0x3A
#define LCD_COMMAND_DISPLAY_ON 0x29
#define LCD_COMMAND_EXIT_SLEEP_MODE 0x11
#define LCD_COMMAND_POWER_SETTING 0xD0
#define LCD_COMMAND_COLOR_INVERSION_OFF 0x20
#define LCD_COMMAND_COLOR_INVERSION_ON 0x21
#define LCD_COMMAND_TEARING_OFF 0x34
#define LCD_COMMAND_TEARING_ON 0x35
#define LCD_COMMAND_MADCTL 0x36
#define LCD_COMMAND_IDLE_OFF 0x38
#define LCD_COMMAND_TIMING_NORMAL 0xC1
#define LCD_PARAM_MADCTL_MY 0x80
#define LCD_PARAM_MADCTL_MX 0x40
#define LCD_PARAM_MADCTL_MV 0x20
#define LCD_PARAM_MADCTL_ML 0x10
#define LCD_PARAM_MADCTL_RGB 0x00
#define LCD_PARAM_MADCTL_BGR 0x08
#define LCD_PARAM_MADCTL_SS 0x02
#define LCD_PARAM_MADCTL_GS 0x01
#endif

//List of colors
#define COLOR_BLACK 0x0000
#define COLOR_NAVY 0x000F
#define COLOR_DGREEN 0x03E0
#define COLOR_DCYAN 0x03EF
#define COLOR_MAROON 0x7800
#define COLOR_PURPLE 0x780F
#define COLOR_OLIVE 0x7BE0
#define COLOR_LGRAY 0xC618
#define COLOR_DGRAY 0x7BEF
#define COLOR_BLUE 0x001F
#define COLOR_BLUE2 0x051D
#define COLOR_GREEN 0x07E0
#define COLOR_GREEN2 0xB723
#define COLOR_GREEN3 0x8000
#define COLOR_CYAN 0x07FF
#define COLOR_RED 0xF800
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW 0xFFE0
#define COLOR_WHITE 0xFFFF
#define COLOR_ORANGE 0xFD20
#define COLOR_GREENYELLOW 0xAFE5
#define COLOR_BROWN 0XBC40

/// Font data stored PER GLYPH
typedef struct
{
	uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
	uint8_t width;		   ///< Bitmap dimensions in pixels
	uint8_t height;		   ///< Bitmap dimensions in pixels
	uint8_t xAdvance;	   ///< Distance to advance cursor (x axis)
	int8_t xOffset;		   ///< X dist from cursor pos to UL corner
	int8_t yOffset;		   ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct
{
	const uint8_t *bitmap;	///< Glyph bitmaps, concatenated
	const GFXglyph *glyph;	///< Glyph array
	const uint8_t first;	///< ASCII extents (first char)
	const uint8_t last;		///< ASCII extents (last char)
	const uint8_t yAdvance; ///< Newline distance (y axis)
} GFXfont;

//Functions defines Macros
#define uswap(a, b)     \
	{                   \
		uint16_t t = a; \
		a = b;          \
		b = t;          \
	}
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
#define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif
#define min(a, b) (((a) < (b)) ? (a) : (b))

extern void LCDDriver_SendData(uint16_t data);
extern void LCDDriver_SetCursorAreaPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
extern uint16_t LCDDriver_GetCurrentXOffset(void);
extern void LCDDriver_Init(void);
extern void LCDDriver_Fill(uint16_t color);
extern void LCDDriver_Fill_RectXY(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
extern void LCDDriver_Fill_RectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
extern void LCDDriver_drawPixel(uint16_t x, uint16_t y, uint16_t color);
extern void LCDDriver_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
extern void LCDDriver_drawFastHLine(uint16_t x, uint16_t y, int16_t w, uint16_t color);
extern void LCDDriver_drawFastVLine(uint16_t x, uint16_t y, int16_t h, uint16_t color);
extern void LCDDriver_drawRectXY(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
extern void LCDDriver_drawChar(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
extern void LCDDriver_drawCharFont(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, GFXfont *gfxFont);
extern void LCDDriver_printText(char text[], uint16_t x, uint16_t y, uint16_t color, uint16_t bg, uint8_t size);
extern void LCDDriver_printTextFont(char text[], uint16_t x, uint16_t y, uint16_t color, uint16_t bg, GFXfont *gfxFont);
extern void LCDDriver_getTextBounds(char text[], uint16_t x, uint16_t y, uint16_t *x1, uint16_t *y1, uint16_t *w, uint16_t *h, GFXfont *gfxFont);
extern void LCDDriver_printImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
extern void LCDDriver_printImage_RLECompressed(uint16_t x, uint16_t y, const tIMAGE *image);
extern void LCDDriver_setRotation(uint8_t rotate);
extern void LCDDriver_setBrightness(uint8_t percent);

extern uint16_t rgb888torgb565(uint_fast8_t red, uint_fast8_t green, uint_fast8_t blue);

#endif
