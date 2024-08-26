
#ifndef INC_PAINT_H_
#define INC_PAINT_H_

#include "fonts.h"
#include "stdint.h"

#define PAINT_INVERT_COLOR 0

typedef enum Paint_Rotation {
    PAINT_ROTATION_0 = 0,
    PAINT_ROTATION_90 = 1,
    PAINT_ROTATION_180 = 2,
    PAINT_ROTATION_270 = 3
} Paint_Rotation;

typedef enum Paint_Color { PAINT_COLOR_WHITE = 0, PAINT_COLOR_BLACK = 1 } Paint_Color;

typedef struct Paint_Section {
    uint8_t* buffer;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    Paint_Rotation rotation;
} Paint_Section;

void Paint_init(Paint_Section* section);
void Paint_clear(Paint_Section* section, Paint_Color color);
void Paint_drawPixel(Paint_Section* section, uint16_t x, uint16_t y, Paint_Color color);
void Paint_drawLine(Paint_Section* section, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Paint_Color color);
void Paint_drawHorizontalLine(Paint_Section* section, uint16_t x, uint16_t y, uint16_t width, Paint_Color color);
void Paint_drawVerticalLine(Paint_Section* section, uint16_t x, uint16_t y, uint16_t height, Paint_Color color);
void Paint_drawRectangle(Paint_Section* section, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Paint_Color color,
                         uint8_t filled);
void Paint_drawCircle(Paint_Section* section, uint16_t x, uint16_t y, uint16_t radius, Paint_Color color,
                      uint8_t filled);
void Paint_drawString(Paint_Section* section, uint16_t x, uint16_t y, char* text, Font_t* font, Paint_Color color);

#endif
