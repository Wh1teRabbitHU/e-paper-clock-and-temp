#include "paint.h"

static Paint_state state = {.rotation = PAINT_ROTATION_0};

static void Paint_drawAbsolutePixel(uint16_t x, uint16_t y, Paint_PixelColor_t color) {
    if (x >= state.width || y >= state.height) {
        return;
    }

#if PAINT_INVERT_COLOR == 1
    if (color == PAINT_COLOR_BLACK) {
        state.image[(x + y * state.width) / 8] |= 0x80 >> (x % 8);
    } else {
        state.image[(x + y * state.width) / 8] &= ~(0x80 >> (x % 8));
    }
#else
    if (color == PAINT_COLOR_BLACK) {
        state.image[(x + y * state.width) / 8] &= ~(0x80 >> (x % 8));
    } else {
        state.image[(x + y * state.width) / 8] |= 0x80 >> (x % 8);
    }
#endif
}

void Paint_init(uint8_t* image, uint16_t width, uint16_t height) {
    state.image = image;
    state.width = width % 8 ? width + 8 - (width % 8) : width;
    state.height = height;
}

Paint_state* Paint_getState(void) { return &state; }

void Paint_setWidth(uint16_t width) { state.width = width; }

void Paint_setHeight(uint16_t height) { state.height = height; }

void Paint_setRotation(Paint_Rotation_t rotation) { state.rotation = rotation; }

void Paint_clear(Paint_PixelColor_t color) {
    uint16_t x, y;

    for (x = 0; x < state.width; x++) {
        for (y = 0; y < state.height; y++) {
            Paint_drawAbsolutePixel(x, y, color);
        }
    }
}

void Paint_drawPixel(uint16_t x, uint16_t y, Paint_PixelColor_t color) {
    uint16_t point_temp;

    if (state.rotation == PAINT_ROTATION_90) {
        point_temp = x;
        x = state.width - y;
        y = point_temp;
    } else if (state.rotation == PAINT_ROTATION_180) {
        x = state.width - x;
        y = state.height - y;
    } else if (state.rotation == PAINT_ROTATION_270) {
        point_temp = x;
        x = y;
        y = state.height - point_temp;
    }

    Paint_drawAbsolutePixel(x, y, color);
}

void Paint_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Paint_PixelColor_t color) {
    /* Bresenham algorithm */
    int16_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;

    while ((x0 != x1) && (y0 != y1)) {
        Paint_drawPixel(x0, y0, color);

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

void Paint_drawHorizontalLine(uint16_t x, uint16_t y, uint16_t width, Paint_PixelColor_t color) {
    uint16_t i;

    for (i = x; i < x + width; i++) {
        Paint_drawPixel(i, y, color);
    }
}

void Paint_drawVerticalLine(uint16_t x, uint16_t y, uint16_t height, Paint_PixelColor_t color) {
    uint16_t i;

    for (i = y; i < y + height; i++) {
        Paint_drawPixel(x, i, color);
    }
}

void Paint_drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Paint_PixelColor_t color, uint8_t filled) {
    uint16_t min_x = x1 > x0 ? x0 : x1;
    uint16_t max_x = x1 > x0 ? x1 : x0;
    uint16_t min_y = y1 > y0 ? y0 : y1;
    uint16_t max_y = y1 > y0 ? y1 : y0;

    if (filled) {
        uint16_t i;

        for (i = min_x; i <= max_x; i++) {
            Paint_drawVerticalLine(i, min_y, max_y - min_y + 1, color);
        }
    } else {
        Paint_drawHorizontalLine(min_x, min_y, max_x - min_x + 1, color);
        Paint_drawHorizontalLine(min_x, max_y, max_x - min_x + 1, color);
        Paint_drawVerticalLine(min_x, min_y, max_y - min_y + 1, color);
        Paint_drawVerticalLine(max_x, min_y, max_y - min_y + 1, color);
    }
}

void Paint_drawCircle(uint16_t x, uint16_t y, uint16_t radius, Paint_PixelColor_t color, uint8_t filled) {
    /* Bresenham algorithm */
    int16_t x_pos = -radius;
    int16_t y_pos = 0;
    int16_t err = 2 - 2 * radius;
    int16_t e2;

    do {
        Paint_drawPixel(x - x_pos, y + y_pos, color);
        Paint_drawPixel(x + x_pos, y + y_pos, color);
        Paint_drawPixel(x + x_pos, y - y_pos, color);
        Paint_drawPixel(x - x_pos, y - y_pos, color);

        if (filled) {
            Paint_drawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, color);
            Paint_drawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, color);
        }

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