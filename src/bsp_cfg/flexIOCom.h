#ifndef FLEXIOCOM_H
#define FLEXIOCOM_H


#include "flexio.h"
#include "flexio_spi_driver.h"

#include "clockMan.h"
#include "Cpu.h"
#include "dmaController.h"
#include "osif1.h"


#define FLEXIO0  (0U)

#define EMETER_FLEXIO (FLEXIO0) 


extern const flexio_spi_master_user_config_t  flexIO0_SpiMasterUserConfig0;

#endif 


