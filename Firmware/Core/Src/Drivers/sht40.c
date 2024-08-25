#include "sht40.h"

static I2C_HandleTypeDef* hi2c;

static HAL_StatusTypeDef SHT40_writeI2C(uint8_t data) {
    uint8_t buffer[1];

    buffer[0] = data;

    return HAL_I2C_Master_Transmit(hi2c, SHT40_I2C_ADDR, buffer, 1, 100);
}

static uint32_t SHT40_readI2C(uint8_t registerAddress) {
    uint8_t buffer[6] = {0};
    uint32_t result = 0;

    HAL_StatusTypeDef response = SHT40_writeI2C(registerAddress);

    if (response != HAL_OK) {
        // TODO
        return 0;
    }

    HAL_Delay(100);

    response = HAL_I2C_Master_Receive(hi2c, SHT40_I2C_ADDR, buffer, 6, 100);

    // HAL_I2C_Mem_Read

    if (response != HAL_OK) {
        // TODO
        return 0;
    }

    result |= buffer[0] << 24;
    result |= buffer[1] << 16;
    result |= buffer[3] << 8;
    result |= buffer[4];

    return result;
}

void SHT40_init(I2C_HandleTypeDef* i2cHandler) { hi2c = i2cHandler; }

void SHT40_reset(void) {}

uint32_t SHT40_getSerialNumber(void) { return SHT40_readI2C(SHT40_I2C_SERIAL_NUMBER); }

void SHT40_getMeasurement(SHT40_Precision precision, SHT40_MeasurementResult* result) {
    uint32_t rawValue = 0;
    uint16_t rawTemperature = 0;
    uint16_t rawHumidity = 0;

    switch (precision) {
        case SHT40_PRECISION_LOW:
            rawValue = SHT40_readI2C(SHT40_I2C_MEASURE_LOW_PRECISION);
            break;
        case SHT40_PRECISION_MEDIUM:
            rawValue = SHT40_readI2C(SHT40_I2C_MEASURE_MEDIUM_PRECISION);
            break;
        case SHT40_PRECISION_HIGH:
            rawValue = SHT40_readI2C(SHT40_I2C_MEASURE_HIGH_PRECISION);
            break;
    }

    rawTemperature = (rawValue >> 16) & 0xFFFF;
    rawHumidity = rawValue & 0xFFFF;

    result->temperature = -45 + 175 * (rawTemperature / 65535.0);
    result->humidity = -6 + 125 * (rawHumidity / 65535.0);
}

void SHT40_heater(SHT40_HeaterConfig* config) {
    // TODO
}