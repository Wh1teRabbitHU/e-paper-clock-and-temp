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