
#ifndef INC_PAINT_H_
#define INC_PAINT_H_

#include "stdint.h"

#define PAINT_INVERT_COLOR 1

typedef enum Paint_Rotation_t {
    PAINT_ROTATION_0 = 0,
    PAINT_ROTATION_90 = 1,
    PAINT_ROTATION_180 = 2,
    PAINT_ROTATION_270 = 3
} Paint_Rotation_t;

typedef enum Paint_PixelColor_t { PAINT_COLOR_WHITE = 0, PAINT_COLOR_BLACK = 1 } Paint_PixelColor_t;

typedef struct Paint_state {
    Paint_Rotation_t rotation;
    uint8_t* image;
    uint16_t width;
    uint16_t height;
} Paint_state;

void Paint_init(uint8_t* image, uint16_t width, uint16_t height);

Paint_state* Paint_getState(void);
void Paint_setWidth(uint16_t width);
void Paint_setHeight(uint16_t height);
void Paint_setRotation(Paint_Rotation_t rotation);

void Paint_clear(Paint_PixelColor_t color);
void Paint_drawPixel(uint16_t x, uint16_t y, Paint_PixelColor_t color);
void Paint_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Paint_PixelColor_t color);
void Paint_drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, Paint_PixelColor_t color);
void Paint_drawVerticalLine(uint16_t x, uint16_t y, uint16_t height, Paint_PixelColor_t color);
void Paint_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Paint_PixelColor_t color, uint8_t filled);
void Paint_drawCircle(uint16_t x, uint16_t y, uint16_t radius, Paint_PixelColor_t color, uint8_t filled);
// void Paint_drawCharAt(uint16_t x, uint16_t y, char ascii_char, sFONT* font, Paint_PixelColor_t color);
// void Paint_drawStringAt(uint16_t x, uint16_t y, const char* text, sFONT* font, Paint_PixelColor_t color);

#endif
