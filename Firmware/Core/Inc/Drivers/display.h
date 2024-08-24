
#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "stdint.h"

#define DISPLAY_INVERT_COLOR 1

typedef enum Display_Rotation_t {
    DISPLAY_ROTATION_0 = 0,
    DISPLAY_ROTATION_90 = 1,
    DISPLAY_ROTATION_180 = 2,
    DISPLAY_ROTATION_270 = 3
} Display_Rotation_t;

typedef enum Display_PixelColor_t { DISPLAY_COLOR_WHITE = 0, DISPLAY_COLOR_BLACK = 1 } Display_PixelColor_t;

typedef struct Display_state {
    Display_Rotation_t rotation;
    uint8_t* image;
    uint16_t width;
    uint16_t height;
} Display_state;

void Display_init(uint8_t* image, uint16_t width, uint16_t height);

Display_state* Display_getState(void);
void Display_setWidth(uint16_t width);
void Display_setHeight(uint16_t height);
void Display_setRotation(Display_Rotation_t rotation);

void Display_clear(Display_PixelColor_t color);
void Display_drawPixel(uint16_t x, uint16_t y, Display_PixelColor_t color);
void Display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Display_PixelColor_t color);
void Display_drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, Display_PixelColor_t color);
void Display_drawVerticalLine(uint16_t x, uint16_t y, uint16_t height, Display_PixelColor_t color);
void Display_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Display_PixelColor_t color);
void Display_drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Display_PixelColor_t color);
void Display_drawCircle(uint16_t x, uint16_t y, uint16_t radius, Display_PixelColor_t color);
void Display_drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, Display_PixelColor_t color);
// void Display_drawCharAt(uint16_t x, uint16_t y, char ascii_char, sFONT* font, Display_PixelColor_t color);
// void Display_drawStringAt(uint16_t x, uint16_t y, const char* text, sFONT* font, Display_PixelColor_t color);

#endif
