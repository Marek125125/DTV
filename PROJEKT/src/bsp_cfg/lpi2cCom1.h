
#ifndef BSP_CFG_LPI2CCOM1_H_
#define BSP_CFG_LPI2CCOM1_H_


#include "lpi2c_driver.h"
#include "clockMan1.h"
#include "Cpu.h"
#include "dmaControllerCom1.h"
#include "osif1.h"


#define INST_LPI2C1 0
#if defined(CPU_S32K148)
#define INST_LPI2C2 1
#endif

#if defined(CPU_S32K148)
#define NUMBER_OF_I2C_INSTANCES     (2U)
#else 
#define NUMBER_OF_I2C_INSTANCES     (1U)
#endif



typedef struct
{
    uint8_t i2c_instance_number;                            
    lpi2c_master_state_t * i2c_state_struct;                     
    const lpi2c_master_user_config_t * i2c_init_config_struct;   
}struct_i2c_config_t;


extern const lpi2c_master_user_config_t lpi2cCom1_MasterConfig0;
#if defined(CPU_S32K148)
extern const lpi2c_master_user_config_t lpi2cCom2_MasterConfig0;
#endif

extern lpi2c_master_state_t lpi2cCom1MasterState;
#if defined(CPU_S32K148)
extern lpi2c_master_state_t lpi2cCom2MasterState;
#endif
extern struct_i2c_config_t struct_i2c_config_tbl[NUMBER_OF_I2C_INSTANCES];


#endif 

