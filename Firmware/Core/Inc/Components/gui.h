#ifndef INC_GUI_H_
#define INC_GUI_H_

#include "er_epm027.h"
#include "math.h"
#include "paint.h"
#include "sht40.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#define GUI_FORCE_UPDATE_AFTER 60

#define GUI_TEMPERATURE_X (ER_EPM027_WIDTH / 2)
#define GUI_TEMPERATURE_Y (((ER_EPM027_HEIGHT) / 2) - (17 * 6 / 2))
#define GUI_TEMPERATURE_WIDTH (24)
#define GUI_TEMPERATURE_HEIGHT (17 * 6)
#define GUI_TEMPERATURE_ROTATION (PAINT_ROTATION_90)

#define GUI_HUMIDITY_X ((ER_EPM027_WIDTH / 2) - 24)
#define GUI_HUMIDITY_Y (((ER_EPM027_HEIGHT) / 2) - (17 * 6 / 2))
#define GUI_HUMIDITY_WIDTH (24)
#define GUI_HUMIDITY_HEIGHT (17 * 6)
#define GUI_HUMIDITY_ROTATION (PAINT_ROTATION_90)

typedef struct GUI_MeasurementState {
    double temperature;
    double humidity;
    uint8_t updateIn;
} GUI_MeasurementState;

void GUI_init(void);
void GUI_drawStaticElements(void);
void GUI_drawMeasurements(SHT40_MeasurementResult *measurement);
void GUI_updateMeasurements();

#endif