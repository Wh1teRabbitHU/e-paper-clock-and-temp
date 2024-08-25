
#ifndef INC_SHT40_H_
#define INC_SHT40_H_

#include "main.h"
#include "stdint.h"

#define SHT40_I2C_ADDR 0x44 << 1

#define SHT40_I2C_HEATER_20MW_0_1S 0x15
#define SHT40_I2C_HEATER_20MW_1_0S 0x1E
#define SHT40_I2C_HEATER_110MW_0_1S 0x24
#define SHT40_I2C_HEATER_110MW_1_0S 0x2F
#define SHT40_I2C_HEATER_200MW_0_1S 0x32
#define SHT40_I2C_HEATER_200MW_1_0S 0x39
#define SHT40_I2C_RESET 0x94
#define SHT40_I2C_SERIAL_NUMBER 0x89
#define SHT40_I2C_MEASURE_LOW_PRECISION 0xE0
#define SHT40_I2C_MEASURE_MEDIUM_PRECISION 0xF6
#define SHT40_I2C_MEASURE_HIGH_PRECISION 0xFD

typedef enum SHT40_Precision {
    SHT40_PRECISION_LOW = 0,
    SHT40_PRECISION_MEDIUM = 1,
    SHT40_PRECISION_HIGH = 2
} SHT40_Precision;

typedef enum SHT40_HeaterDuration {
    SHT40_HEATER_DURATION_0_1S = 0,
    SHT40_HEATER_DURATION_1_0S = 1
} SHT40_HeaterDuration;

typedef enum SHT40_HeaterStrength {
    SHT40_HEATER_STRENGTH_20MW = 0,
    SHT40_HEATER_STRENGTH_110MW = 1,
    SHT40_HEATER_STRENGTH_200MW = 2
} SHT40_HeaterStrength;

typedef struct SHT40_MeasurementResult {
    double temperature;
    double humidity;
} SHT40_MeasurementResult;

typedef struct SHT40_HeaterConfig {
    SHT40_HeaterStrength strength;
    SHT40_HeaterDuration duration;
} SHT40_HeaterConfig;

void SHT40_init(I2C_HandleTypeDef *i2cHandler);
void SHT40_reset(void);
uint32_t SHT40_getSerialNumber(void);
void SHT40_getMeasurement(SHT40_Precision precision, SHT40_MeasurementResult *result);
void SHT40_heater(SHT40_HeaterConfig *config);

#endif