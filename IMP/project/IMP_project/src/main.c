#include <stdio.h>
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "tft.h"

void app_main()
{
    spi_device_handle_t spi;
    initTFT(&spi);

    // Clear display
    for (int x = 0; x < 128; x++)
    {
        for (int y = 0; y < 128; y++)
        {
            drawPixelTFT(spi, x, y, 0xFFFF);
        }
    }

    uint16_t color = 0x001F;
    for (int x = 40; x < 54; x++)
    {
        for (int y = 40; y < 54; y++)
        {
            drawPixelTFT(spi, x, y, color);
        }
    }

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}