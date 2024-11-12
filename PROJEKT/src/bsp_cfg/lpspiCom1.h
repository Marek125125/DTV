#ifndef lpspiCom1_H
#define lpspiCom1_H


#include "clockMan1.h"
#include "Cpu.h"
#include "osif1.h"
#include "device_registers.h"

typedef enum
{
	SPI_0 = 0u,
	SPI_1,
	SPI_2,
} lpspi_instance;

#define NUMBER_OF_SPI_INSTANCES_MAX     (3u)
#define NUMBER_OF_SPI_INSTANCES         (LPSPI_INSTANCE_COUNT)

#define NUMBER_OF_SPI_PCS_MAX           (4u)

extern const lpspi_master_config_t lpspiCom1_MasterConfig_Initial;
extern const lpspi_slave_config_t  lpspiCom1_SlaveConfig_Initial;
extern lpspi_state_t spi_state_struct[NUMBER_OF_SPI_INSTANCES_MAX];

#endif


