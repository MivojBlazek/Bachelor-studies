#include "spi.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

void initSPI(spi_device_handle_t *handle)
{
    spi_bus_config_t busConfig = {
        .miso_io_num = -1,
        .mosi_io_num = GPIO_NUM_23,
        .sclk_io_num = GPIO_NUM_18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    spi_bus_initialize(HSPI_HOST, &busConfig, SPI_DMA_CH_AUTO);

    spi_device_interface_config_t devConfig = {
        .clock_speed_hz = SPI_MASTER_FREQ_8M,
        .mode = 0,
        .spics_io_num = GPIO_NUM_5,
        .queue_size = 1
    };
    spi_bus_add_device(HSPI_HOST, &devConfig, handle);
}

void sendDataSPI(spi_device_handle_t handle, const uint8_t *data, size_t length)
{
    spi_transaction_t trans = {
        .length = length * 8,
        .tx_buffer = data
    };
    spi_device_transmit(handle, &trans);
}
