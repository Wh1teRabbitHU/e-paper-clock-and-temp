
#ifndef INC_PAINT_H_
#define INC_PAINT_H_

#include "fonts.h"
#include "stdint.h"

#define PAINT_INVERT_COLOR 0

typedef enum Paint_Rotation_t {
    PAINT_ROTATION_0 = 0,
    PAINT_ROTATION_90 = 1,
    PAINT_ROTATION_180 = 2,
    PAINT_ROTATION_270 = 3
} Paint_Rotation_t;

typedef enum Paint_PixelColor_t { PAINT_COLOR_WHITE = 0, PAINT_COLOR_BLACK = 1 } Paint_PixelColor_t;

typedef struct Paint_section {
    Paint_Rotation_t rotation;
    uint8_t* buffer;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} Paint_section;

void Paint_init(Paint_section* section, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void Paint_clear(Paint_section* section, Paint_PixelColor_t color);
void Paint_drawPixel(Paint_section* section, uint16_t x, uint16_t y, Paint_PixelColor_t color);
void Paint_drawLine(Paint_section* section, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                    Paint_PixelColor_t color);
void Paint_drawHorizontalLine(Paint_section* section, uint16_t x, uint16_t y, uint16_t width, Paint_PixelColor_t color);
void Paint_drawVerticalLine(Paint_section* section, uint16_t x, uint16_t y, uint16_t height, Paint_PixelColor_t color);
void Paint_drawRectangle(Paint_section* section, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                         Paint_PixelColor_t color, uint8_t filled);
void Paint_drawCircle(Paint_section* section, uint16_t x, uint16_t y, uint16_t radius, Paint_PixelColor_t color,
                      uint8_t filled);
void Paint_drawString(Paint_section* section, uint16_t x, uint16_t y, const char* text, Font_t* font,
                      Paint_PixelColor_t color);

#endif
