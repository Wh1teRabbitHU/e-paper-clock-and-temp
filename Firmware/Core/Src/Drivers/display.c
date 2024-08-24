#include "display.h"

static Display_state state = {.rotation = DISPLAY_ROTATION_0};

static void Display_drawAbsolutePixel(uint16_t x, uint16_t y, Display_PixelColor_t color) {
    if (x < 0 || x >= state.width || y < 0 || y >= state.height) {
        return;
    }

#if DISPLAY_INVERT_COLOR == 1
    if (color) {
        state.image[(x + y * state.width) / 8] |= 0x80 >> (x % 8);
    } else {
        state.image[(x + y * state.width) / 8] &= ~(0x80 >> (x % 8));
    }
#else
    if (color) {
        state.image[(x + y * state.width) / 8] &= ~(0x80 >> (x % 8));
    } else {
        state.image[(x + y * state.width) / 8] |= 0x80 >> (x % 8);
    }
#endif
}

void Display_init(uint8_t* image, uint16_t width, uint16_t height) {
    state.image = image;
    /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
    state.width = width % 8 ? width + 8 - (width % 8) : width;
    state.height = height;
}

Display_state* Display_getState(void) { return &state; }
void Display_setWidth(uint16_t width) { state.width = width; }
void Display_setHeight(uint16_t height) { state.height = height; }
void Display_setRotation(Display_Rotation_t rotation) { state.rotation = rotation; }

void Display_clear(Display_PixelColor_t color) {
    for (int x = 0; x < state.width; x++) {
        for (int y = 0; y < state.height; y++) {
            Display_drawAbsolutePixel(x, y, color);
        }
    }
}

void Display_drawPixel(uint16_t x, uint16_t y, Display_PixelColor_t color) {
    int point_temp;
    if (state.rotation == DISPLAY_ROTATION_0) {
        if (x < 0 || x >= state.width || y < 0 || y >= state.height) {
            return;
        }
        Display_drawAbsolutePixel(x, y, color);
    } else if (state.rotation == DISPLAY_ROTATION_90) {
        if (x < 0 || x >= state.height || y < 0 || y >= state.width) {
            return;
        }
        point_temp = x;
        x = state.width - y;
        y = point_temp;
        Display_drawAbsolutePixel(x, y, color);
    } else if (state.rotation == DISPLAY_ROTATION_180) {
        if (x < 0 || x >= state.width || y < 0 || y >= state.height) {
            return;
        }
        x = state.width - x;
        y = state.height - y;
        Display_drawAbsolutePixel(x, y, color);
    } else if (state.rotation == DISPLAY_ROTATION_270) {
        if (x < 0 || x >= state.height || y < 0 || y >= state.width) {
            return;
        }
        point_temp = x;
        x = y;
        y = state.height - point_temp;
        Display_drawAbsolutePixel(x, y, color);
    }
}

void Display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Display_PixelColor_t color) {
    /* Bresenham algorithm */
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while ((x0 != x1) && (y0 != y1)) {
        Display_drawPixel(x0, y0, color);
        if (2 * err >= dy) {
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Display_drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, Display_PixelColor_t color) {
    int i;
    for (i = x; i < x + width; i++) {
        Display_drawPixel(i, y, color);
    }
}

void Display_drawVerticalLine(uint16_t x, uint16_t y, uint16_t height, Display_PixelColor_t color) {
    int i;
    for (i = y; i < y + height; i++) {
        Display_drawPixel(x, i, color);
    }
}

void Display_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Display_PixelColor_t color) {
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    Display_drawHorizontalLine(min_x, min_y, max_x - min_x + 1, color);
    Display_drawHorizontalLine(min_x, max_y, max_x - min_x + 1, color);
    Display_drawVerticalLine(min_x, min_y, max_y - min_y + 1, color);
    Display_drawVerticalLine(max_x, min_y, max_y - min_y + 1, color);
}

void Display_drawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Display_PixelColor_t color) {
    int min_x, min_y, max_x, max_y;
    int i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    for (i = min_x; i <= max_x; i++) {
        Display_drawVerticalLine(i, min_y, max_y - min_y + 1, color);
    }
}

void Display_drawCircle(uint16_t x, uint16_t y, uint16_t radius, Display_PixelColor_t color) {
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do {
        Display_drawPixel(x - x_pos, y + y_pos, color);
        Display_drawPixel(x + x_pos, y + y_pos, color);
        Display_drawPixel(x + x_pos, y - y_pos, color);
        Display_drawPixel(x - x_pos, y - y_pos, color);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

void Display_drawFilledCircle(uint16_t x, uint16_t y, uint16_t radius, Display_PixelColor_t color) {
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do {
        Display_drawPixel(x - x_pos, y + y_pos, color);
        Display_drawPixel(x + x_pos, y + y_pos, color);
        Display_drawPixel(x + x_pos, y - y_pos, color);
        Display_drawPixel(x - x_pos, y - y_pos, color);
        Display_drawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, color);
        Display_drawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, color);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}