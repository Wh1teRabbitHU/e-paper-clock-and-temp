#include "er_epm027.h"

static uint8_t spiBuffer[1];
static SPI_HandleTypeDef *hspi;

static void ER_EPM027_spiTransfer(uint8_t command) {
    spiBuffer[0] = command;

    HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, 0);
    HAL_SPI_Transmit(hspi, spiBuffer, 1, 1000);
    HAL_GPIO_WritePin(DISPLAY_CS_GPIO_Port, DISPLAY_CS_Pin, 1);
}

static void ER_EPM027_reset(void) {
    HAL_GPIO_WritePin(DISPLAY_RST_GPIO_Port, DISPLAY_RST_Pin, 0);
    HAL_Delay(200);
    HAL_GPIO_WritePin(DISPLAY_RST_GPIO_Port, DISPLAY_RST_Pin, 1);
    HAL_Delay(200);
}

static void ER_EPM027_WaitUntilIdle(void) {
    while (HAL_GPIO_ReadPin(DISPLAY_BUSY_GPIO_Port, DISPLAY_BUSY_Pin) == ER_EPM027_STATE_BUSY) {
        HAL_Delay(10);
    }
}

static void ER_EPM027_sendCommand(uint8_t command) {
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, 0);
    ER_EPM027_spiTransfer(command);
}

static void ER_EPM027_sendData(uint8_t data) {
    HAL_GPIO_WritePin(DISPLAY_DC_GPIO_Port, DISPLAY_DC_Pin, 1);
    ER_EPM027_spiTransfer(data);
}

void ER_EPM027_powerOn(void) {
    HAL_GPIO_WritePin(DISPLAY_PWR_GPIO_Port, DISPLAY_PWR_Pin, 1);
    HAL_Delay(100);
}

void ER_EPM027_powerOff(void) { HAL_GPIO_WritePin(DISPLAY_PWR_GPIO_Port, DISPLAY_PWR_Pin, 0); }

void ER_EPM027_sleep(void) {
    ER_EPM027_sendCommand(ER_EPM027_CMD_POWER_OFF);
    ER_EPM027_WaitUntilIdle();
    ER_EPM027_sendCommand(ER_EPM027_CMD_DEEP_SLEEP);
    ER_EPM027_sendData(0xa5);  // check code
}

void ER_EPM027_start(void) { /* EPD hardware init start */
    ER_EPM027_powerOn();
    ER_EPM027_reset();
    ER_EPM027_sendCommand(ER_EPM027_CMD_POWER_SETTING);
    ER_EPM027_sendData(0x03);  // VDS_EN, VDG_EN
    ER_EPM027_sendData(0x00);  // VCOM_HV, VGHL_LV[1], VGHL_LV[0]
    ER_EPM027_sendData(0x2b);  // VDH
    ER_EPM027_sendData(0x2b);  // VDL
    ER_EPM027_sendData(0x09);  // VDHR

    ER_EPM027_sendCommand(ER_EPM027_CMD_BOOSTER_SOFT_START);
    ER_EPM027_sendData(0x07);
    ER_EPM027_sendData(0x07);
    ER_EPM027_sendData(0x17);
    // Power optimization
    ER_EPM027_sendCommand(0xF8);
    ER_EPM027_sendData(0x60);
    ER_EPM027_sendData(0xA5);
    // Power optimization
    ER_EPM027_sendCommand(0xF8);
    ER_EPM027_sendData(0x73);
    ER_EPM027_sendData(0x23);
    // Power optimization
    ER_EPM027_sendCommand(0xF8);
    ER_EPM027_sendData(0x7C);
    ER_EPM027_sendData(0x00);

    ER_EPM027_sendCommand(ER_EPM027_CMD_PARTIAL_DISPLAY_REFRESH);
    ER_EPM027_sendData(0x00);
    ER_EPM027_sendCommand(ER_EPM027_CMD_POWER_ON);
    ER_EPM027_WaitUntilIdle();

    ER_EPM027_sendCommand(ER_EPM027_CMD_PANEL_SETTING);
    ER_EPM027_sendData(0x1F);  // KW-BF   KWR-AF    BWROTP 0f
    ER_EPM027_sendCommand(ER_EPM027_CMD_PLL_CONTROL);
    ER_EPM027_sendData(0x3A);  // 3A 100HZ   29 150Hz 39 200HZ    31 171HZ

    ER_EPM027_sendCommand(ER_EPM027_CMD_TCON_RESOLUTION);
    ER_EPM027_sendData(0x00);
    ER_EPM027_sendData(0xB0);
    ER_EPM027_sendData(0x01);
    ER_EPM027_sendData(0x08);
}

void ER_EPM027_init(SPI_HandleTypeDef *spiHandler) {
    hspi = spiHandler;

    ER_EPM027_start();
}

void ER_EPM027_sendSection(const uint8_t *buffer, uint16_t x, uint16_t y, uint16_t w, uint16_t l) {
    if (buffer == NULL) {
        return;
    }

    ER_EPM027_sendCommand(ER_EPM027_CMD_PARTIAL_DATA_START_TRANSMISSION_2);
    ER_EPM027_sendData(x >> 8);
    ER_EPM027_sendData(x & 0xf8);  // x should be the multiple of 8, the last 3 bit will always be ignored
    ER_EPM027_sendData(y >> 8);
    ER_EPM027_sendData(y & 0xff);
    ER_EPM027_sendData(w >> 8);
    ER_EPM027_sendData(w & 0xf8);  // w (width) should be the multiple of 8, the last 3 bit will always be ignored
    ER_EPM027_sendData(l >> 8);
    ER_EPM027_sendData(l & 0xff);

    HAL_Delay(2);

    for (int i = 0; i < w / 8 * l; i++) {
        ER_EPM027_sendData(buffer[i]);
    }

    HAL_Delay(2);
}

void ER_EPM027_drawSection(uint16_t x, uint16_t y, uint16_t w, uint16_t l) {
    ER_EPM027_sendCommand(ER_EPM027_CMD_PARTIAL_DISPLAY_REFRESH);

    ER_EPM027_sendData(x >> 8);
    ER_EPM027_sendData(x & 0xf8);  // x should be the multiple of 8, the last 3 bit will always be ignored
    ER_EPM027_sendData(y >> 8);
    ER_EPM027_sendData(y & 0xff);
    ER_EPM027_sendData(w >> 8);
    ER_EPM027_sendData(w & 0xf8);  // w (width) should be the multiple of 8, the last 3 bit will always be ignored
    ER_EPM027_sendData(l >> 8);
    ER_EPM027_sendData(l & 0xff);

    ER_EPM027_WaitUntilIdle();
}

void ER_EPM027_sendScreen(const uint8_t *imageBuffer, uint16_t width, uint16_t height) {
    if (imageBuffer == NULL) {
        return;
    }

    ER_EPM027_sendCommand(ER_EPM027_CMD_DATA_START_TRANSMISSION_1);

    HAL_Delay(2);

    for (int i = 0; i < width / 8 * height; i++) {
        ER_EPM027_sendData(0xFF);
    }

    HAL_Delay(2);

    ER_EPM027_sendCommand(ER_EPM027_CMD_DATA_START_TRANSMISSION_2);

    HAL_Delay(2);

    for (int i = 0; i < width / 8 * height; i++) {
        ER_EPM027_sendData(imageBuffer[i]);
    }

    HAL_Delay(2);

    ER_EPM027_sendCommand(ER_EPM027_CMD_DISPLAY_REFRESH);
    ER_EPM027_WaitUntilIdle();
}

void ER_EPM027_drawScreen(void) {
    ER_EPM027_sendCommand(ER_EPM027_CMD_DISPLAY_REFRESH);
    ER_EPM027_WaitUntilIdle();
}

void ER_EPM027_clearScreen(void) {
    ER_EPM027_sendCommand(ER_EPM027_CMD_DATA_START_TRANSMISSION_1);

    HAL_Delay(2);

    for (int i = 0; i < ER_EPM027_WIDTH * ER_EPM027_HEIGHT / 8; i++) {
        ER_EPM027_sendData(0xFF);
    }

    HAL_Delay(2);

    ER_EPM027_sendCommand(ER_EPM027_CMD_DATA_START_TRANSMISSION_2);

    HAL_Delay(2);

    for (int i = 0; i < ER_EPM027_WIDTH * ER_EPM027_HEIGHT / 8; i++) {
        ER_EPM027_sendData(0xFF);
    }

    HAL_Delay(2);

    ER_EPM027_drawScreen();
}