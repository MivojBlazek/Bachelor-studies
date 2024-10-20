#ifndef _TFT_H_
#define _TFT_H_

#include "driver/spi_master.h"
#include "driver/gpio.h"

#define TFT_CS      GPIO_NUM_5
#define TFT_RST     GPIO_NUM_17
#define TFT_DC      GPIO_NUM_27
#define TFT_SDA     GPIO_NUM_23
#define TFT_SCK     GPIO_NUM_18

void initTFT(spi_device_handle_t *spi);
void sendCommandTFT(spi_device_handle_t spi, const uint8_t cmd);
void sendDataTFT(spi_device_handle_t spi, const uint8_t *data, int len);
void setAddrWindowTFT(spi_device_handle_t spi, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void drawPixelTFT(spi_device_handle_t spi, uint16_t x, uint16_t y, uint16_t color);

#endif //_TFT_H_
