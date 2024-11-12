#include <string.h>
#include "device_registers.h"
#include "hal_spi.h"
#include "spi_app.h"
#include "modulhardwarecode.h"



static struct_hal_spi_master_config spi_master_config[NUMBER_OF_SPI_INSTANCES_MAX] =
{
	{ .spi_baudrate = 50000U, .spi_pcs_polarity = LPSPI_ACTIVE_HIGH, .spi_pcs_contiuous_mode = false, .spi_clock_phase = LPSPI_CLOCK_PHASE_1ST_EDGE, .spi_clock_polarity = LPSPI_SCK_ACTIVE_HIGH, .spi_lsb_first = false, },
	{ .spi_baudrate = 50000U, .spi_pcs_polarity = LPSPI_ACTIVE_HIGH, .spi_pcs_contiuous_mode = false, .spi_clock_phase = LPSPI_CLOCK_PHASE_1ST_EDGE, .spi_clock_polarity = LPSPI_SCK_ACTIVE_HIGH, .spi_lsb_first = false, },
	{ .spi_baudrate = 50000U, .spi_pcs_polarity = LPSPI_ACTIVE_HIGH, .spi_pcs_contiuous_mode = false, .spi_clock_phase = LPSPI_CLOCK_PHASE_1ST_EDGE, .spi_clock_polarity = LPSPI_SCK_ACTIVE_HIGH, .spi_lsb_first = false  }
};

static const struct_spi_hardware_config * spi_hardware_config = NULL; 
static struct_hal_spi_handle spi_handle[NUMBER_OF_SPI_INSTANCES];     


static enum_HAL_SPI_RETURN_VALUE spi_set_pcs_master( uint8_t spi_channel, uint8_t pcs );


void spi_set_configuration( const struct_spi_hardware_config * spi_hw_cfg )
{
    if ( spi_hw_cfg != NULL )
    {
    	spi_hardware_config = spi_hw_cfg;
    }
}

enum_HAL_SPI_RETURN_VALUE spi_master_init( uint8_t spi_channel )
{
    enum_HAL_SPI_RETURN_VALUE ret_val = HAL_SPI_ERROR_INIT_FAILED;

    if ( ( spi_hardware_config != NULL ) && ( spi_channel < NUMBER_OF_SPI_INSTANCES) )
    {
    	ret_val = hal_spi_master_init( &spi_handle[spi_channel], spi_channel, &spi_master_config[spi_channel] );
    }

    return ret_val;
}

enum_HAL_SPI_RETURN_VALUE spi_master_deinit( uint8_t spi_channel )
{
    enum_HAL_SPI_RETURN_VALUE ret_val = HAL_SPI_ERROR_INIT_FAILED;

    if ( ( spi_hardware_config != NULL ) && ( spi_channel < NUMBER_OF_SPI_INSTANCES) )
    {
    	ret_val = hal_spi_master_deinit( &spi_handle[spi_channel] );
    	if (HAL_SPI_OK == ret_val )
    	{
    		spi_handle[spi_channel].ptr_handle = NULL;
    	}
    }

    return ret_val;
}

enum_HAL_SPI_RETURN_VALUE spi_master_transfer( uint8_t spi_channel, uint8_t pcs, struct_hal_spi_frame *frm )
{
    enum_HAL_SPI_RETURN_VALUE ret_val = HAL_SPI_ERROR_GENERAL;

    if ( ( spi_hardware_config != NULL ) && ( spi_channel < NUMBER_OF_SPI_INSTANCES) )
    {
        if ( spi_handle[spi_channel].ptr_handle )
        {
            if ( spi_hardware_config[spi_channel].pcs_pin_configured[pcs] )
            {
                if ( HAL_SPI_OK == spi_set_pcs_master( spi_channel, pcs ) )
                {
                    ret_val = hal_spi_master_transfer( &spi_handle[spi_channel], frm );
                }
            }
        }
    }

    return ret_val;
}

enum_HAL_SPI_RETURN_VALUE spi_master_transfer_blocking( uint8_t spi_channel, uint8_t pcs, struct_hal_spi_frame *frm, uint32_t timeout )
{
    enum_HAL_SPI_RETURN_VALUE ret_val = HAL_SPI_ERROR_GENERAL;

    if ( ( spi_hardware_config != NULL ) && ( spi_channel < NUMBER_OF_SPI_INSTANCES) )
    {
        if ( spi_handle[spi_channel].ptr_handle )
        {
            if ( spi_hardware_config[spi_channel].pcs_pin_configured[pcs] )
            {
                if ( HAL_SPI_OK == spi_set_pcs_master( spi_channel, pcs ) )
                {
                    ret_val = hal_spi_master_transfer_blocking( &spi_handle[spi_channel], frm, timeout );
                }
            }
        }
    }

    return ret_val;
}

enum_HAL_SPI_RETURN_VALUE spi_master_send( uint8_t spi_channel, uint8_t pcs, uint8_t *ptr_tx_data, uint8_t size )
{
    enum_HAL_SPI_RETURN_VALUE ret_val = HAL_SPI_ERROR_GENERAL;

    if ( ( spi_hardware_config != NULL ) && ( spi_channel < NUMBER_OF_SPI_INSTANCES) )
    {
        if ( spi_handle[spi_channel].ptr_handle )
        {
            if ( spi_hardware_config[spi_channel].pcs_pin_configured[pcs] )
            {
                if ( HAL_SPI_OK == spi_set_pcs_master( spi_channel, pcs ) )
                {
                    ret_val = hal_spi_master_send( &spi_handle[spi_channel], ptr_tx_data, size );
                }
            }
        }
    }

    return ret_val;
}

enum_HAL_SPI_RETURN_VALUE spi_master_send_blocking( uint8_t spi_channel, uint8_t pcs, uint8_t *ptr_tx_data, uint8_t size, uint32_t timeout )
{
    enum_HAL_SPI_RETURN_VALUE ret_val = HAL_SPI_ERROR_GENERAL;

    if ( ( spi_hardware_config != NULL ) && ( spi_channel < NUMBER_OF_SPI_INSTANCES) )
    {
        if ( spi_handle[spi_channel].ptr_handle )
        {
            if ( spi_hardware_config[spi_channel].pcs_pin_configured[pcs] )
            {
                if ( HAL_SPI_OK == spi_set_pcs_master( spi_channel, pcs ) )
                {
                    ret_val = hal_spi_master_send_blocking( &spi_handle[spi_channel], ptr_tx_data, size, timeout );
                }
            }
        }
    }

    return ret_val;
}

static enum_HAL_SPI_RETURN_VALUE spi_set_pcs_master( uint8_t spi_channel, uint8_t pcs)
{
    enum_HAL_SPI_RETURN_VALUE ret_val = HAL_SPI_OK;

    uint8_t configured_pcs = spi_master_config[spi_channel].spi_which_pcs;
    if ( configured_pcs != pcs )
    {
    	spi_master_config[spi_channel].spi_which_pcs = pcs;

    	uint8_t pcs_polarity = spi_master_config[spi_channel].spi_pcs_polarity;
        ret_val = hal_spi_set_pcs(&spi_handle[spi_channel], pcs, pcs_polarity);
    }

    return ret_val;
}


