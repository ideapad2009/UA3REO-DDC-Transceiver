//Header files
#include "lcd_driver.h"
#include "main.h"
#include "fonts.h"
#include "settings.h"
#include "functions.h"

static bool _cp437 = false;
uint32_t LCD_FSMC_COMM_ADDR = 0;
uint32_t LCD_FSMC_DATA_ADDR = 0;
static uint16_t text_cursor_y = 0;
static uint16_t text_cursor_x = 0;
static bool wrap = false;

//***** Functions prototypes *****//
//Write command to LCD
inline void LCDDriver_SendCommand(uint16_t com)
{
	*(__IO uint16_t *)((uint32_t)(LCD_FSMC_COMM_ADDR)) = com;
}

//Write data to LCD
inline void LCDDriver_SendData(uint16_t data)
{
	*(__IO uint16_t *)((uint32_t)(LCD_FSMC_DATA_ADDR)) = data;
}

//Read command from LCD
inline uint16_t LCDDriver_ReadCommand(void)
{
	return *(__IO uint16_t *)((uint32_t)(LCD_FSMC_COMM_ADDR));
	//unsigned short data = *(unsigned short *)(LCD_FSMC_COMM_ADDR);
	//return data;
}
//Read data from LCD
inline uint16_t LCDDriver_ReadData(void)
{
	return *(__IO uint16_t *)((uint32_t)(LCD_FSMC_DATA_ADDR));
	//unsigned short data = * (unsigned short *)(LCD_FSMC_DATA_ADDR);
	//return (unsigned char)data;
}

//Initialise function
void LCDDriver_Init(void)
{
	//init remap
#if FMC_REMAP
	if (hsram1.Init.NSBank == FMC_NORSRAM_BANK1)
		LCD_FSMC_COMM_ADDR = 0xC0000000;
	if (hsram1.Init.NSBank == FMC_NORSRAM_BANK2)
		LCD_FSMC_COMM_ADDR = 0xCA000000;
	if (hsram1.Init.NSBank == FMC_NORSRAM_BANK3)
		LCD_FSMC_COMM_ADDR = 0xCB000000;
	if (hsram1.Init.NSBank == FMC_NORSRAM_BANK4)
		LCD_FSMC_COMM_ADDR = 0xCC000000;
#else
	if (hsram1.Init.NSBank == FMC_NORSRAM_BANK1)
		LCD_FSMC_COMM_ADDR = 0x60000000;
	if (hsram1.Init.NSBank == FMC_NORSRAM_BANK2)
		LCD_FSMC_COMM_ADDR = 0x6A000000;
	if (hsram1.Init.NSBank == FMC_NORSRAM_BANK3)
		LCD_FSMC_COMM_ADDR = 0x6B000000;
	if (hsram1.Init.NSBank == FMC_NORSRAM_BANK4)
		LCD_FSMC_COMM_ADDR = 0x6C000000;
#endif
	LCD_FSMC_DATA_ADDR = LCD_FSMC_COMM_ADDR + (1 << (FSMC_REGISTER_SELECT + 1));
	
#if (defined(LCD_ILI9481) || defined(LCD_HX8357B))
	#define ILI9481_COMM_DELAY 20
	
	LCDDriver_SendCommand(LCD_COMMAND_SOFT_RESET); //0x01
	HAL_Delay(ILI9481_COMM_DELAY);

	LCDDriver_SendCommand(LCD_COMMAND_EXIT_SLEEP_MODE); //0x11
	HAL_Delay(ILI9481_COMM_DELAY);

	LCDDriver_SendCommand(LCD_COMMAND_NORMAL_MODE_ON); //0x13
	
	LCDDriver_SendCommand(LCD_COMMAND_POWER_SETTING); //(0xD0);
	LCDDriver_SendData(0x07);
	LCDDriver_SendData(0x42);
	LCDDriver_SendData(0x18);
	
	LCDDriver_SendCommand(LCD_COMMAND_VCOM); //(0xD1);
	LCDDriver_SendData(0x00);
	LCDDriver_SendData(0x07);
	LCDDriver_SendData(0x10);
	
	LCDDriver_SendCommand(LCD_COMMAND_NORMAL_PWR_WR); //(0xD2);
	LCDDriver_SendData(0x01);
	LCDDriver_SendData(0x02);
	HAL_Delay(ILI9481_COMM_DELAY);

#if defined(LCD_HX8357B)	
	LCDDriver_SendCommand(LCD_COMMAND_PANEL_DRV_CTL); //(0xC0);
	LCDDriver_SendData(0x10);
	LCDDriver_SendData(0x3B);
	LCDDriver_SendData(0x00);
	LCDDriver_SendData(0x02);
	LCDDriver_SendData(0x11);
	HAL_Delay(ILI9481_COMM_DELAY);
#endif
	
	LCDDriver_SendCommand(LCD_COMMAND_FR_SET); //(0xC5);
	LCDDriver_SendData(0x03);
	HAL_Delay(ILI9481_COMM_DELAY);
	
	LCDDriver_SendCommand(LCD_COMMAND_GAMMAWR); //(0xC8);
	LCDDriver_SendData(0x00);
	LCDDriver_SendData(0x32);
	LCDDriver_SendData(0x36);
	LCDDriver_SendData(0x45);
	LCDDriver_SendData(0x06);
	LCDDriver_SendData(0x16);
	LCDDriver_SendData(0x37);
	LCDDriver_SendData(0x75);
	LCDDriver_SendData(0x77);
	LCDDriver_SendData(0x54);
	LCDDriver_SendData(0x0C);
	LCDDriver_SendData(0x00);
	HAL_Delay(ILI9481_COMM_DELAY);
	
	LCDDriver_SendCommand(LCD_COMMAND_MADCTL); //(0x36);
	LCDDriver_SendData(LCD_PARAM_MADCTL_BGR);
	HAL_Delay(ILI9481_COMM_DELAY);
	
	LCDDriver_SendCommand(LCD_COMMAND_PIXEL_FORMAT); //(0x3A);
	LCDDriver_SendData(0x55);
	HAL_Delay(ILI9481_COMM_DELAY);
	
	LCDDriver_SendCommand(LCD_COMMAND_COLUMN_ADDR); //(0x2A);
	LCDDriver_SendData(0x00);
	LCDDriver_SendData(0x00);
	LCDDriver_SendData(0x01);
	LCDDriver_SendData(0x3F);
	HAL_Delay(ILI9481_COMM_DELAY);
	
	LCDDriver_SendCommand(LCD_COMMAND_PAGE_ADDR); //(0x2B);
	LCDDriver_SendData(0x00);
	LCDDriver_SendData(0x00);
	LCDDriver_SendData(0x01);
	LCDDriver_SendData(0xDF);
	HAL_Delay(ILI9481_COMM_DELAY);
	
#if defined(LCD_HX8357B)	
	LCDDriver_SendCommand(LCD_COMMAND_COLOR_INVERSION_ON); //(0x21);
	HAL_Delay(ILI9481_COMM_DELAY);
#endif

	LCDDriver_SendCommand(LCD_COMMAND_IDLE_OFF);		   //(0x38);
	HAL_Delay(ILI9481_COMM_DELAY);
	
	LCDDriver_SendCommand(LCD_COMMAND_DISPLAY_ON);		   //(0x29);
	HAL_Delay(ILI9481_COMM_DELAY);
#endif
}

//Set screen rotation
void LCDDriver_setRotation(uint8_t rotate)
{
#if (defined(LCD_ILI9481) || defined(LCD_HX8357B))
	LCDDriver_SendCommand(LCD_COMMAND_MADCTL);
	switch (rotate)
	{
	case 1: // Portrait
		LCDDriver_SendData(LCD_PARAM_MADCTL_BGR | LCD_PARAM_MADCTL_MX);
		break;
	case 2: // Landscape (Portrait + 90)
		LCDDriver_SendData(LCD_PARAM_MADCTL_BGR | LCD_PARAM_MADCTL_MV);
		break;
	case 3: // Inverter portrait
		LCDDriver_SendData(LCD_PARAM_MADCTL_BGR | LCD_PARAM_MADCTL_MY);
		break;
	case 4: // Inverted landscape
		LCDDriver_SendData(LCD_PARAM_MADCTL_BGR | LCD_PARAM_MADCTL_MV | LCD_PARAM_MADCTL_MX | LCD_PARAM_MADCTL_MY);
		//LCDDriver_SendData(LCD_PARAM_MADCTL_BGR | LCD_PARAM_MADCTL_MV | LCD_PARAM_MADCTL_SS | LCD_PARAM_MADCTL_GS);
		break;
	}
	HAL_Delay(120);
#endif
}

//Set cursor position
void LCDDriver_SetCursorAreaPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
#if (defined(LCD_ILI9481) || defined(LCD_HX8357B))
	LCDDriver_SendCommand(LCD_COMMAND_COLUMN_ADDR);
	LCDDriver_SendData(x1 >> 8);
	LCDDriver_SendData(x1 & 0xFF);
	LCDDriver_SendData(x2 >> 8);
	LCDDriver_SendData(x2 & 0xFF);
	LCDDriver_SendCommand(LCD_COMMAND_PAGE_ADDR);
	LCDDriver_SendData(y1 >> 8);
	LCDDriver_SendData(y1 & 0xFF);
	LCDDriver_SendData(y2 >> 8);
	LCDDriver_SendData(y2 & 0xFF);
	LCDDriver_SendCommand(LCD_COMMAND_GRAM);
#endif
}

static void LCDDriver_SetCursorPosition(uint16_t x, uint16_t y)
{
#if (defined(LCD_ILI9481) || defined(LCD_HX8357B))
	LCDDriver_SendCommand(LCD_COMMAND_COLUMN_ADDR);
	LCDDriver_SendData(x >> 8); //-V760
	LCDDriver_SendData(x & 0xFF);
	LCDDriver_SendData(x >> 8);
	LCDDriver_SendData(x & 0xFF);
	LCDDriver_SendCommand(LCD_COMMAND_PAGE_ADDR);
	LCDDriver_SendData(y >> 8); //-V760
	LCDDriver_SendData(y & 0xFF);
	LCDDriver_SendData(y >> 8);
	LCDDriver_SendData(y & 0xFF);
	LCDDriver_SendCommand(LCD_COMMAND_GRAM);
#endif
	text_cursor_y = y;
	text_cursor_x = x;
}

uint16_t LCDDriver_GetCurrentXOffset(void)
{
	return text_cursor_x;
}

//Write data to a single pixel
static void LCDDriver_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	LCDDriver_SetCursorPosition(x, y);
	LCDDriver_SendData(color);
}

//Fill the entire screen with a background color
void LCDDriver_Fill(uint16_t color)
{
	LCDDriver_Fill_RectXY(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

//Rectangle drawing functions
static IRAM2 uint16_t fillxy_color;
void LCDDriver_Fill_RectXY(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	if (x1 > (LCD_WIDTH - 1))
		x1 = LCD_WIDTH - 1;
	if (y1 > (LCD_HEIGHT - 1))
		y1 = LCD_HEIGHT - 1;
	uint32_t n = ((x1 + 1) - x0) * ((y1 + 1) - y0);
	if (n > LCD_PIXEL_COUNT)
		n = LCD_PIXEL_COUNT;
	LCDDriver_SetCursorAreaPosition(x0, y0, x1, y1);

	fillxy_color = color;
	if (n > 50)
	{
		SCB_CleanDCache_by_Addr((uint32_t *)&fillxy_color, sizeof(fillxy_color));
		const uint32_t part_size = 32000;
		uint32_t estamated = n;
		while (estamated > 0)
		{
			if (estamated >= part_size)
			{
				HAL_MDMA_Start(&hmdma_mdma_channel43_sw_0, (uint32_t)&fillxy_color, LCD_FSMC_DATA_ADDR, part_size * 2, 1);
				HAL_MDMA_PollForTransfer(&hmdma_mdma_channel43_sw_0, HAL_MDMA_FULL_TRANSFER, HAL_MAX_DELAY);
				estamated -= part_size;
			}
			else
			{
				HAL_MDMA_Start(&hmdma_mdma_channel43_sw_0, (uint32_t)&fillxy_color, LCD_FSMC_DATA_ADDR, estamated * 2, 1);
				HAL_MDMA_PollForTransfer(&hmdma_mdma_channel43_sw_0, HAL_MDMA_FULL_TRANSFER, HAL_MAX_DELAY);
				estamated = 0;
			}
		}
	}
	else
	{
		while (n--)
		{
			LCDDriver_SendData(color);
		}
	}
}

void LCDDriver_Fill_RectWH(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	LCDDriver_Fill_RectXY(x, y, x + w, y + h, color);
}

//Line drawing functions
void LCDDriver_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		uswap(x0, y0)
			uswap(x1, y1)
	}

	if (x0 > x1)
	{
		uswap(x0, x1)
			uswap(y0, y1)
	}

	int16_t dx, dy;
	dx = (int16_t)(x1 - x0);
	dy = (int16_t)abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep)
		{
			LCDDriver_DrawPixel(y0, x0, color);
		}
		else
		{
			LCDDriver_DrawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

void LCDDriver_drawFastHLine(uint16_t x, uint16_t y, int16_t w, uint16_t color)
{
	int16_t x2 = x + w;
	if (x2 < 0)
		x2 = 0;
	if (x2 > (LCD_WIDTH - 1))
		x2 = LCD_WIDTH - 1;

	if (x2 < x)
		LCDDriver_Fill_RectXY((uint16_t)x2, y, x, y, color);
	else
		LCDDriver_Fill_RectXY(x, y, (uint16_t)x2, y, color);
}

void LCDDriver_drawFastVLine(uint16_t x, uint16_t y, int16_t h, uint16_t color)
{
	int16_t y2 = y + h - 1;
	if (y2 < 0)
		y2 = 0;
	if (y2 > (LCD_HEIGHT - 1))
		y2 = LCD_HEIGHT - 1;

	if (y2 < y)
		LCDDriver_Fill_RectXY(x, (uint16_t)y2, x, y, color);
	else
		LCDDriver_Fill_RectXY(x, y, x, (uint16_t)y2, color);
}

void LCDDriver_drawRectXY(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	LCDDriver_drawFastHLine(x0, y0, (int16_t)(x1 - x0), color);
	LCDDriver_drawFastHLine(x0, y1, (int16_t)(x1 - x0), color);
	LCDDriver_drawFastVLine(x0, y0, (int16_t)(y1 - y0), color);
	LCDDriver_drawFastVLine(x1, y0, (int16_t)(y1 - y0), color);
}

//Text printing functions
void LCDDriver_drawChar(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
	uint8_t line;
	if ((x >= LCD_WIDTH) ||			// Clip right
		(y >= LCD_HEIGHT) ||		// Clip bottom
		((x + 6 * size - 1) < 0) || // Clip left
		((y + 8 * size - 1) < 0))	// Clip top
		return;

	if (!_cp437 && (c >= 176))
		c++;																   // Handle 'classic' charset behavior
	LCDDriver_SetCursorAreaPosition(x, y, x + 6 * size - 1, y + 8 * size - 1); //char area

	for (int8_t j = 0; j < 8; j++)
	{											//y line out
		for (int8_t s_y = 0; s_y < size; s_y++) //y size scale
			for (int8_t i = 0; i < 6; i++)
			{ //x line out
				{
					if (i == 5)
						line = 0x0;
					else
						line = pgm_read_byte(rastr_font + (c * 5) + i); //read font
					line >>= j;
					for (int8_t s_x = 0; s_x < size; s_x++) //x size scale
					{
						if (line & 0x1)
							LCDDriver_SendData(color); //font pixel
						else
							LCDDriver_SendData(bg); //background pixel
					}
				}
			}
	}
}

void LCDDriver_printText(char text[], uint16_t x, uint16_t y, uint16_t color, uint16_t bg, uint8_t size)
{
	uint16_t offset;
	offset = size * 6;
	for (uint16_t i = 0; i < 40 && text[i] != NULL; i++)
	{
		LCDDriver_drawChar(x + (offset * i), y, text[i], color, bg, size);
		text_cursor_x = x + (offset * (i + 1));
	}
}

void LCDDriver_drawCharFont(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, GFXfont *gfxFont)
{
	c -= (uint8_t)pgm_read_byte(&gfxFont->first);
	GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
	uint8_t *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

	uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
	uint8_t w = pgm_read_byte(&glyph->width), h = pgm_read_byte(&glyph->height), xa = pgm_read_byte(&glyph->xAdvance);
	int8_t xo = (int8_t)pgm_read_byte(&glyph->xOffset);
	int8_t yo = (int8_t)pgm_read_byte(&glyph->yOffset);
	uint8_t xx, yy, bits = 0, bit = 0;
	int16_t ys1 = y + yo;
	int16_t ys2 = y + yo + h - 1;
	if (ys1 < 0)
		ys1 = 0;
	if (ys2 < 0)
		ys2 = 0;
	LCDDriver_SetCursorAreaPosition(x, (uint16_t)ys1, x + xa - 1, (uint16_t)ys2); //char area

	for (yy = 0; yy < h; yy++)
	{
		for (xx = 0; xx < xa; xx++)
		{
			if (xx < xo || xx >= (xo + w))
			{
				LCDDriver_SendData(bg); //background pixel
				continue;
			}
			if (!(bit++ & 7))
			{
				bits = pgm_read_byte(&bitmap[bo++]);
			}
			if (bits & 0x80)
			{
				LCDDriver_SendData(color); //font pixel
			}
			else
			{
				LCDDriver_SendData(bg); //background pixel
			}
			bits <<= 1;
		}
	}
}

void LCDDriver_printTextFont(char text[], uint16_t x, uint16_t y, uint16_t color, uint16_t bg, GFXfont *gfxFont)
{
	uint8_t c = 0;
	text_cursor_x = x;
	text_cursor_y = y;
	for (uint16_t i = 0; i < 40 && text[i] != NULL; i++)
	{
		c = text[i];
		if (c == '\n')
		{
			text_cursor_x = 0;
			text_cursor_y += (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
		}
		else if (c != '\r')
		{
			uint8_t first = pgm_read_byte(&gfxFont->first);
			uint8_t last = pgm_read_byte(&gfxFont->last);
			if ((c >= first) && (c <= last))
			{
				GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c - first]);
				uint8_t w = pgm_read_byte(&glyph->width);
				uint8_t h = pgm_read_byte(&glyph->height);
				if ((w > 0) && (h > 0))
				{
					int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset);
					if (wrap && ((text_cursor_x + (xo + w)) > LCD_WIDTH))
					{
						text_cursor_x = 0;
						text_cursor_y += (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
					}
					LCDDriver_drawCharFont(text_cursor_x, text_cursor_y, c, color, bg, gfxFont);
				}
				text_cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance);
			}
		}
	}
}

/**************************************************************************/
/*!
	@brief    Helper to determine size of a character with current font/size.
	   Broke this out as it's used by both the PROGMEM- and RAM-resident getTextBounds() functions.
	@param    c     The ascii character in question
	@param    x     Pointer to x location of character
	@param    y     Pointer to y location of character
	@param    minx  Minimum clipping value for X
	@param    miny  Minimum clipping value for Y
	@param    maxx  Maximum clipping value for X
	@param    maxy  Maximum clipping value for Y
*/
/**************************************************************************/
static void LCDDriver_charBounds(char c, uint16_t *x, uint16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy, GFXfont *gfxFont)
{
	if (c == '\n')
	{			// Newline?
		*x = 0; // Reset x to zero, advance y by one line
		*y += (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
	}
	else if (c != '\r')
	{ // Not a carriage return; is normal char
		uint8_t first = pgm_read_byte(&gfxFont->first),
				last = pgm_read_byte(&gfxFont->last);
		if ((c >= first) && (c <= last))
		{ // Char present in this font?
			GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(
				&gfxFont->glyph))[c - first]);
			uint8_t gw = pgm_read_byte(&glyph->width),
					gh = pgm_read_byte(&glyph->height),
					xa = pgm_read_byte(&glyph->xAdvance);
			int8_t xo = (int8_t)pgm_read_byte(&glyph->xOffset),
				   yo = (int8_t)pgm_read_byte(&glyph->yOffset);
			if (wrap && ((*x + (((int16_t)xo + gw))) > LCD_WIDTH))
			{
				*x = 0; // Reset x to zero, advance y by one line
				*y += (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
			}
			int16_t x1 = *x + xo,
					y1 = *y + yo,
					x2 = x1 + gw - 1,
					y2 = y1 + gh - 1;
			if (x1 < *minx)
				*minx = x1;
			if (y1 < *miny)
				*miny = y1;
			if (x2 > *maxx)
				*maxx = x2;
			if (y2 > *maxy)
				*maxy = y2;
			*x += xa;
		}
	}
}

/**************************************************************************/
/*!
	@brief    Helper to determine size of a string with current font/size. Pass string and a cursor position, returns UL corner and W,H.
	@param    str     The ascii string to measure
	@param    x       The current cursor X
	@param    y       The current cursor Y
	@param    x1      The boundary X coordinate, set by function
	@param    y1      The boundary Y coordinate, set by function
	@param    w      The boundary width, set by function
	@param    h      The boundary height, set by function
*/
/**************************************************************************/
void LCDDriver_getTextBounds(char text[], uint16_t x, uint16_t y, uint16_t *x1, uint16_t *y1, uint16_t *w, uint16_t *h, GFXfont *gfxFont)
{
	uint8_t c; // Current character

	*x1 = x;
	*y1 = y;
	*w = *h = 0;

	int16_t minx = LCD_WIDTH, miny = LCD_HEIGHT, maxx = 0, maxy = 0;

	for (uint16_t i = 0; i < 40 && text[i] != NULL; i++)
	{
		c = text[i];
		LCDDriver_charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy, gfxFont);
	}

	if (maxx >= minx)
	{
		*x1 = (uint16_t)minx;
		*w = (uint16_t)(maxx - minx + 1);
	}
	if (maxy >= miny)
	{
		*y1 = (uint16_t)miny;
		*h = (uint16_t)(maxy - miny + 1);
	}
}

//Image print (RGB 565, 2 bytes per pixel)
void LCDDriver_printImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
	uint32_t n = w * h * 2;
	LCDDriver_SetCursorAreaPosition(x, y, w + x - 1, h + y - 1);
	for (uint32_t i = 0; i < n; i += 2)
	{
		LCDDriver_SendData((uint16_t)((data[i] << 8) | data[i + 1]));
	}
}

void LCDDriver_printImage_RLECompressed(uint16_t x, uint16_t y, const tIMAGE *image)
{
	uint32_t pixels = image->width * image->height;
	uint32_t i = 0;
	uint32_t decoded = 0;

	LCDDriver_SetCursorAreaPosition(x, y, image->width + x - 1, image->height + y - 1);
	while (true)
	{
		if ((int16_t)image->data[i] < 0) // no repeats
		{
			uint16_t count = (-(int16_t)image->data[i]);
			i++;
			for (uint16_t p = 0; p < count; p++)
			{
				LCDDriver_SendData(image->data[i]);
				decoded++;
				i++;
				if (pixels <= decoded)
					return;
			}
		}
		else //repeats
		{
			uint16_t count = ((int16_t)image->data[i]);
			i++;
			for (uint16_t p = 0; p < count; p++)
			{
				LCDDriver_SendData(image->data[i]);
				decoded++;
				if (pixels <= decoded)
					return;
			}
			i++;
		}
	}
}

void LCDDriver_drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	LCDDriver_SetCursorAreaPosition(x, y, x, y);
	LCDDriver_SendData(color);
}

inline uint16_t rgb888torgb565(uint_fast8_t r, uint_fast8_t g, uint_fast8_t b)
{
	return (uint16_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xFF) >> 3));
}

void LCDDriver_setBrightness(uint8_t percent)
{
	uint32_t perc = 65535 - 65535 * percent / 100;
	if (TIM3->CCR4 != perc)
		TIM3->CCR4 = perc;
}
