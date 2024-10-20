#include <stdio.h>
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// TFT Display Pins
#define TFT_CS      GPIO_NUM_5   // Chip Select
#define TFT_RST     GPIO_NUM_17  // Reset
#define TFT_DC      GPIO_NUM_2   // Data/Command
#define TFT_SDA     GPIO_NUM_23   // MOSI
#define TFT_SCK     GPIO_NUM_18   // Clock
#define LED_PIN     GPIO_NUM_27    // LED Pin

#define TFT_WIDTH   128
#define TFT_HEIGHT  128

static spi_device_handle_t tft_spi;

static const char *TAG = "TFT_DISPLAY";

// Function Prototypes
void tft_init(void);
void tft_send_command(uint8_t cmd);
void tft_send_data(uint8_t data);
void tft_fill_screen(uint16_t color);
void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

void app_main(void) {
    ESP_LOGI(TAG, "Starting TFT display example...");

    // Initialize the TFT display
    tft_init();

    // Fill screen with white color
    tft_fill_screen(0xFFFF);
    draw_rectangle(10, 10, 100, 50, 0xF800); // Red color
     // Initialize LED pin as output
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

        // Turn on the LED
    gpio_set_level(LED_PIN, 1); // High to turn on

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void tft_init(void) {
    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .miso_io_num = -1,      // Not used
        .mosi_io_num = TFT_SDA, // MOSI pin
        .sclk_io_num = TFT_SCK, // Clock pin
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000
    };

    // Initialize the SPI bus
    spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH2);

    // Device configuration
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_MASTER_FREQ_8M, // Clock out at 8 MHz
        .mode = 0,                             // SPI mode 0
        .spics_io_num = TFT_CS,                // CS pin
        .queue_size = 1,
    };

    // Add the device to the SPI bus
    spi_bus_add_device(HSPI_HOST, &devcfg, &tft_spi);

    // Reset the display
    gpio_set_direction(TFT_RST, GPIO_MODE_OUTPUT);
    gpio_set_level(TFT_RST, 0);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    gpio_set_level(TFT_RST, 1);
    vTaskDelay(150 / portTICK_PERIOD_MS);

    // Initialization commands for the display
    tft_send_command(0x01); // Software reset
    vTaskDelay(150 / portTICK_PERIOD_MS);
    tft_send_command(0x11); // Sleep out
    vTaskDelay(500 / portTICK_PERIOD_MS);
    tft_send_command(0x3A); // Set color mode
    tft_send_data(0x05);    // 16-bit color
    tft_send_command(0x29); // Display on
}

void tft_send_command(uint8_t cmd) {
    gpio_set_level(TFT_DC, 0); // Command mode
    spi_transaction_t trans = {
        .length = 8,
        .tx_buffer = &cmd,
    };
    spi_device_transmit(tft_spi, &trans);
}

void tft_send_data(uint8_t data) {
    gpio_set_level(TFT_DC, 1); // Data mode
    spi_transaction_t trans = {
        .length = 8,
        .tx_buffer = &data,
    };
    spi_device_transmit(tft_spi, &trans);
}

void tft_fill_screen(uint16_t color) {
    tft_send_command(0x2A); // Column address set
    tft_send_data(0x00);
    tft_send_data(0x00); // Start column
    tft_send_data(0x00);
    tft_send_data(0x7F); // End column

    tft_send_command(0x2B); // Row address set
    tft_send_data(0x00);
    tft_send_data(0x00); // Start row
    tft_send_data(0x00);
    tft_send_data(0x7F); // End row

    tft_send_command(0x2C); // Write to RAM
    for (int i = 0; i < TFT_WIDTH * TFT_HEIGHT; i++) {
        tft_send_data(color >> 8);   // Send high byte
        tft_send_data(color & 0xFF); // Send low byte
    }
}

void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
    tft_send_command(0x2A); // Column address set
    tft_send_data(x >> 8);  // Start column high byte
    tft_send_data(x & 0xFF); // Start column low byte
    tft_send_data((x + width - 1) >> 8); // End column high byte
    tft_send_data((x + width - 1) & 0xFF); // End column low byte

    tft_send_command(0x2B); // Row address set
    tft_send_data(y >> 8);  // Start row high byte
    tft_send_data(y & 0xFF); // Start row low byte
    tft_send_data((y + height - 1) >> 8); // End row high byte
    tft_send_data((y + height - 1) & 0xFF); // End row low byte

    tft_send_command(0x2C); // Write to RAM
    for (int i = 0; i < width * height; i++) {
        tft_send_data(color >> 8);   // Send high byte
        tft_send_data(color & 0xFF); // Send low byte
    }
}