#ifndef _SPI_APP_H_
#define _SPI_APP_H_

#include "hal_spi.h"
#include "lpspiCom1.h"
#include <stdbool.h>

typedef struct {
    uint8_t  instance_number; 
    bool     pcs_pin_configured[NUMBER_OF_SPI_PCS_MAX]; 

} struct_spi_hardware_config;

void spi_set_configuration( const struct_spi_hardware_config * spi_hw_cfg );

enum_HAL_SPI_RETURN_VALUE spi_master_init( uint8_t spi_channel );

enum_HAL_SPI_RETURN_VALUE spi_master_deinit( uint8_t spi_channel );

enum_HAL_SPI_RETURN_VALUE spi_master_transfer( uint8_t spi_channel, uint8_t pcs, struct_hal_spi_frame *frm );

enum_HAL_SPI_RETURN_VALUE spi_master_transfer_blocking( uint8_t spi_channel, uint8_t pcs, struct_hal_spi_frame *frm, uint32_t timeout );

enum_HAL_SPI_RETURN_VALUE spi_master_send( uint8_t spi_channel, uint8_t pcs,  uint8_t *ptr_tx_data, uint8_t size );

enum_HAL_SPI_RETURN_VALUE spi_master_send_blocking( uint8_t spi_channel, uint8_t pcs, uint8_t *ptr_tx_data, uint8_t size, uint32_t timeout );

#endif

