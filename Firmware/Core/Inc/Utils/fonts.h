#ifndef __FONTS_H
#define __FONTS_H

#include <stdint.h>

#define FONT_MAX_HEIGHT 24
#define FONT_MAX_WIDTH 17
#define FONT_OFFSET_BITMAP 54

typedef struct Font_t {
    const uint8_t *table;
    uint16_t Width;
    uint16_t Height;

} Font_t;

extern Font_t Font24;
extern Font_t Font12;

#endif
