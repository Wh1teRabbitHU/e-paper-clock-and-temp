#include "paint.h"

static void Paint_drawAbsolutePixel(Paint_Section* section, uint16_t x, uint16_t y, Paint_Color color) {
    if (x >= section->width || y >= section->height) {
        return;
    }

#if PAINT_INVERT_COLOR == 1
    if (color == PAINT_COLOR_BLACK) {
        section->buffer[(x + y * section->width) / 8] |= 0x80 >> (x % 8);
    } else {
        section->buffer[(x + y * section->width) / 8] &= ~(0x80 >> (x % 8));
    }
#else
    if (color == PAINT_COLOR_BLACK) {
        section->buffer[(x + y * section->width) / 8] &= ~(0x80 >> (x % 8));
    } else {
        section->buffer[(x + y * section->width) / 8] |= 0x80 >> (x % 8);
    }
#endif
}

static void Paint_drawChar(Paint_Section* section, uint16_t x, uint16_t y, char character, Font_t* font,
                           Paint_Color color) {
    uint16_t i, j;
    uint16_t offset = (character - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const uint8_t* ptr = &font->table[offset];

    for (j = 0; j < font->Height; j++) {
        for (i = 0; i < font->Width; i++) {
            if (*ptr & (0x80 >> (i % 8))) {
                Paint_drawPixel(section, x + i, y + j, color);
            } else {
                Paint_drawPixel(section, x + i, y + j, !color);
            }

            if (i % 8 == 7) {
                ptr++;
            }
        }

        if (font->Width % 8 != 0) {
            ptr++;
        }
    }
}

void Paint_init(Paint_Section* section) {
    uint16_t sectionSize = section->width * section->height;
    uint8_t buffer[sectionSize];

    for (uint16_t i = 0; i < sectionSize; i++) {
        buffer[i] = 0xFF;
    }

    section->width = section->width % 8 ? section->width + 8 - (section->width % 8) : section->width;
    section->height = section->height;
    section->buffer = buffer;
}

void Paint_clear(Paint_Section* section, Paint_Color color) {
    uint16_t x, y;

    for (x = 0; x < section->width; x++) {
        for (y = 0; y < section->height; y++) {
            Paint_drawAbsolutePixel(section, x, y, color);
        }
    }
}

void Paint_drawPixel(Paint_Section* section, uint16_t x, uint16_t y, Paint_Color color) {
    uint16_t point_temp;

    if (section->rotation == PAINT_ROTATION_90) {
        point_temp = x;
        x = section->width - y;
        y = point_temp;
    } else if (section->rotation == PAINT_ROTATION_180) {
        x = section->width - x;
        y = section->height - y;
    } else if (section->rotation == PAINT_ROTATION_270) {
        point_temp = x;
        x = y;
        y = section->height - point_temp;
    }

    Paint_drawAbsolutePixel(section, x, y, color);
}

void Paint_drawLine(Paint_Section* section, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Paint_Color color) {
    /* Bresenham algorithm */
    int16_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int16_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy;

    while ((x0 != x1) && (y0 != y1)) {
        Paint_drawPixel(section, x0, y0, color);

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

void Paint_drawHorizontalLine(Paint_Section* section, uint16_t x, uint16_t y, uint16_t width, Paint_Color color) {
    uint16_t i;

    for (i = x; i < x + width; i++) {
        Paint_drawPixel(section, i, y, color);
    }
}

void Paint_drawVerticalLine(Paint_Section* section, uint16_t x, uint16_t y, uint16_t height, Paint_Color color) {
    uint16_t i;

    for (i = y; i < y + height; i++) {
        Paint_drawPixel(section, x, i, color);
    }
}

void Paint_drawRectangle(Paint_Section* section, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, Paint_Color color,
                         uint8_t filled) {
    uint16_t min_x = x1 > x0 ? x0 : x1;
    uint16_t max_x = x1 > x0 ? x1 : x0;
    uint16_t min_y = y1 > y0 ? y0 : y1;
    uint16_t max_y = y1 > y0 ? y1 : y0;

    if (filled) {
        uint16_t i;

        for (i = min_x; i <= max_x; i++) {
            Paint_drawVerticalLine(section, i, min_y, max_y - min_y + 1, color);
        }
    } else {
        Paint_drawHorizontalLine(section, min_x, min_y, max_x - min_x + 1, color);
        Paint_drawHorizontalLine(section, min_x, max_y, max_x - min_x + 1, color);
        Paint_drawVerticalLine(section, min_x, min_y, max_y - min_y + 1, color);
        Paint_drawVerticalLine(section, max_x, min_y, max_y - min_y + 1, color);
    }
}

void Paint_drawCircle(Paint_Section* section, uint16_t x, uint16_t y, uint16_t radius, Paint_Color color,
                      uint8_t filled) {
    /* Bresenham algorithm */
    int16_t x_pos = -radius;
    int16_t y_pos = 0;
    int16_t err = 2 - 2 * radius;
    int16_t e2;

    do {
        Paint_drawPixel(section, x - x_pos, y + y_pos, color);
        Paint_drawPixel(section, x + x_pos, y + y_pos, color);
        Paint_drawPixel(section, x + x_pos, y - y_pos, color);
        Paint_drawPixel(section, x - x_pos, y - y_pos, color);

        if (filled) {
            Paint_drawHorizontalLine(section, x + x_pos, y + y_pos, 2 * (-x_pos) + 1, color);
            Paint_drawHorizontalLine(section, x + x_pos, y - y_pos, 2 * (-x_pos) + 1, color);
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

void Paint_drawString(Paint_Section* section, uint16_t x, uint16_t y, char* text, Font_t* font, Paint_Color color) {
    const char* p_text = text;
    uint16_t refcolumn = x;

    while (*p_text != 0) {
        Paint_drawChar(section, refcolumn, y, *p_text, font, color);

        refcolumn += font->Width;
        p_text++;
    }
}