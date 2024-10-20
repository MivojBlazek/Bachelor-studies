#include "tft.h"
#include "spi.h"

void initTFT(spi_device_handle_t *spi)
{
    // Initialize GPIO
    gpio_set_direction(TFT_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(TFT_CS, GPIO_MODE_OUTPUT);
    gpio_set_direction(TFT_RST, GPIO_MODE_OUTPUT);

    // Display reset
    gpio_set_level(TFT_RST, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(TFT_RST, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);

    initSPI(spi);

    sendCommandTFT(*spi, 0x01);  // Software reset
    vTaskDelay(100 / portTICK_PERIOD_MS);
    sendCommandTFT(*spi, 0x11);  // Sleep out
    vTaskDelay(100 / portTICK_PERIOD_MS);
    sendCommandTFT(*spi, 0x29);  // Display on
}

void sendCommandTFT(spi_device_handle_t spi, const uint8_t command)
{
    gpio_set_level(TFT_DC, 0);
    sendDataSPI(spi, &command, 1);
}

void sendDataTFT(spi_device_handle_t spi, const uint8_t *data, int len)
{
    gpio_set_level(TFT_DC, 1);
    sendDataSPI(spi, data, len);
}

void setAddrWindowTFT(spi_device_handle_t spi, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[] = {
        x0 >> 8,
        x0 & 0xFF,
        x1 >> 8,
        x1 & 0xFF
    };
    sendCommandTFT(spi, 0x2A);
    sendDataTFT(spi, data, sizeof(data));

    uint8_t data2[] = {
        y0 >> 8,
        y0 & 0xFF,
        y1 >> 8,
        y1 & 0xFF
    };
    sendCommandTFT(spi, 0x2B);
    sendDataTFT(spi, data2, sizeof(data2));

    sendCommandTFT(spi, 0x2C);
}

void drawPixelTFT(spi_device_handle_t spi, uint16_t x, uint16_t y, uint16_t color)
{
    setAddrWindowTFT(spi, x, y, x, y);
    uint8_t data[] = {
        color >> 8,
        color & 0xFF
    };
    sendDataTFT(spi, data, sizeof(data));
}