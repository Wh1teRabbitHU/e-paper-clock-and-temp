#include "gui.h"

static char textBuffer[32];
static GUI_MeasurementState state = {.humidity = 0.0, .temperature = 0.0, .updateIn = GUI_FORCE_UPDATE_AFTER};

static inline void GUI_sendTemperatureSection(double temperature) {
    Paint_Section *section = malloc(sizeof(Paint_Section));

    section->x = GUI_TEMPERATURE_X;
    section->y = GUI_TEMPERATURE_Y;
    section->width = GUI_TEMPERATURE_WIDTH;
    section->height = GUI_TEMPERATURE_HEIGHT;
    section->rotation = GUI_TEMPERATURE_ROTATION;

    Paint_init(section);

    uint8_t decimal = temperature;
    double f = temperature - decimal;
    int fraction = trunc(f * 100);

    sprintf(textBuffer, "%d.%dC", decimal, fraction);
    Paint_drawString(section, 0, 0, textBuffer, &Font24, PAINT_COLOR_BLACK);

    ER_EPM027_sendSection(section);

    free(section);
}

static inline void GUI_sendHumiditySection(double humidity) {
    Paint_Section *section = malloc(sizeof(Paint_Section));

    section->x = GUI_HUMIDITY_X;
    section->y = GUI_HUMIDITY_Y;
    section->width = GUI_HUMIDITY_WIDTH;
    section->height = GUI_HUMIDITY_HEIGHT;
    section->rotation = GUI_HUMIDITY_ROTATION;

    Paint_init(section);

    uint8_t decimal = humidity;
    double f = humidity - decimal;
    int fraction = trunc(f * 100);

    sprintf(textBuffer, "%d.%d%%", decimal, fraction);
    Paint_drawString(section, 0, 0, textBuffer, &Font24, PAINT_COLOR_BLACK);

    ER_EPM027_sendSection(section);

    free(section);
}

static inline uint8_t GUI_shouldUpdate(SHT40_MeasurementResult *measurement) {
    double temperatureDifference = fabs(measurement->temperature - state.temperature);
    double humidityDifference = fabs(measurement->humidity - state.humidity);

    if (temperatureDifference > 1.0 || humidityDifference > 1.0) {
        state.updateIn = 0;
    } else if (temperatureDifference > 0.5 || humidityDifference > 0.5) {
        if (state.updateIn < GUI_FORCE_UPDATE_AFTER / 2) {
            state.updateIn = 0;
        } else {
            state.updateIn -= GUI_FORCE_UPDATE_AFTER / 2;
        }
    } else if (temperatureDifference > 0.1 || humidityDifference > 0.1) {
        if (state.updateIn < GUI_FORCE_UPDATE_AFTER / 4) {
            state.updateIn = 0;
        } else {
            state.updateIn -= GUI_FORCE_UPDATE_AFTER / 4;
        }
    } else {
        state.updateIn--;
    }

    if (state.updateIn == 0) {
        state.updateIn = GUI_FORCE_UPDATE_AFTER;

        return 1;
    }

    return 0;
}

void GUI_init(void) {
    SHT40_MeasurementResult *measurement = malloc(sizeof(SHT40_MeasurementResult));

    SHT40_getMeasurement(SHT40_PRECISION_MEDIUM, measurement);

    GUI_drawStaticElements();
    GUI_drawMeasurements(measurement);

    free(measurement);
}

void GUI_drawStaticElements(void) {}

void GUI_drawMeasurements(SHT40_MeasurementResult *measurement) {
    GUI_sendTemperatureSection(measurement->temperature);
    GUI_sendHumiditySection(measurement->humidity);

    ER_EPM027_drawPartialScreen(GUI_HUMIDITY_X, GUI_HUMIDITY_Y, GUI_TEMPERATURE_WIDTH + GUI_HUMIDITY_WIDTH,
                                GUI_HUMIDITY_HEIGHT);
}

void GUI_updateMeasurements() {
    SHT40_MeasurementResult *measurement = malloc(sizeof(SHT40_MeasurementResult));
    SHT40_getMeasurement(SHT40_PRECISION_MEDIUM, measurement);

    uint8_t update = GUI_shouldUpdate(measurement);

    if (update) {
        GUI_drawMeasurements(measurement);

        state.temperature = measurement->temperature;
        state.humidity = measurement->humidity;
    }

    free(measurement);
}
