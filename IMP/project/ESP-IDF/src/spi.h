#ifndef _SPI_H_
#define _SPI_H_

#include "driver/spi_master.h"

void initSPI(spi_device_handle_t *handle);
void sendDataSPI(spi_device_handle_t handle, const uint8_t *data, size_t length);

#endif //_SPI_H_
