
#ifndef SRC_ROLE_MTS_PROTOCOL_H_
#define SRC_ROLE_MTS_PROTOCOL_H_

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(SPECIFICS_MTS_PROTOCOL)



#define HAL_ERR_MCU_OFFSET                  20u
#define HAL_ERR_CAN_OFFSET                  30u
#define HAL_ERR_SPI_OFFSET                  40u
#define HAL_ERR_SCI_OFFSET                  50u
#define HAL_ERR_FLASH_OFFSET                60u

#define MASK_DEVICE_TYPE_CAN_ID (uint32_t)0x380   
#define SHIFT_DEVICE_TYPE_CAN_ID             7    
#define MASK_DEVICE_TYPE_CAN_DATA (uint8_t)0x70   
#define SHIFT_DEVICE_TYPE_CAN_DATA           4    

#define MASK_MODULE_ID_CAN_ID   (uint32_t)0x78    
#define SHIFT_MODULE_ID_CAN_ID               3    
#define MASK_MODULE_ID_CAN_DATA  (uint8_t)0x0F    
#define SHIFT_MODULE_ID_CAN_DATA             0    

#define MASK_TRANSFER_TYPE (uint32_t)0x407 

#define MASK_BURST_BIT  0x80  
#define SHIFT_BURST_BIT    7  
#define MASK_SLOT_ID    0x7F  
#define SHIFT_SLOT_ID      0  

#define MASK_DATA_TYPE  0xF0  
#define SHIFT_DATA_TYPE    4  
#define MASK_CHANNEL    0x0F  
#define SHIFT_CHANNEL      0  

typedef enum
{
    HAL_ERR_OK = 0                              ,       
    HAL_ERR_GENERAL                             ,       
    HAL_ERR_NOT_IMPLEMENTED                     ,       
    HAL_ERR_INIT                                ,       
    HAL_ERR_PARAM                               ,       
    HAL_ERR_BUSY                                ,       
    HAL_ERR_NOT_AVAILABLE                       ,       
    HAL_ERR_TIMEOUT                             ,       
    HAL_ERR_TX                                  ,       
    HAL_ERR_RX                                  ,       

    HAL_ERR_MCU_FAULT = HAL_ERR_MCU_OFFSET      ,
    HAL_ERR_MCU_RAM                             ,
    HAL_ERR_MCU_HARDFAULT                       ,
    HAL_ERR_CAN_FAULT = HAL_ERR_CAN_OFFSET      ,

    HAL_ERR_SPI_FAULT = HAL_ERR_SPI_OFFSET      ,
    HAL_ERR_SCI_FAULT = HAL_ERR_SCI_OFFSET      ,
    HAL_ERR_FLASH_FAULT = HAL_ERR_FLASH_OFFSET  ,

    HAL_ERR_MAX                                         
} enum_hal_errorcodes_t;

#define ROLE_DO_PARAM_LIST_MTS_PROTOCOL

typedef enum
{
    MTS_ERR_OK = 0                  ,       
    MTS_ERR_OK_NOT_SEND_ANSWER      ,       
    MTS_ERR_GENERAL                 ,       
    MTS_ERR_INIT                    ,       
    MTS_ERR_PARAM                   ,       
    MTS_ERR_WRONG_TYPE              ,       
    MTS_ERR_RX_TRANSFER_TYPE        ,       
    MTS_ERR_RX_DEVICE_TYPE          ,       
    MTS_ERR_TX_DEVICE_TYPE          ,       
    MTS_ERR_RX_DATA_TYPE            ,       
    MTS_ERR_TRANSFER_TYPE           ,       
    MTS_ERR_DATA_TYPE               ,       
    MTS_ERR_RX_LENGTH               ,       
    MTS_ERR_RX_BURST_LENGTH         ,       
    MTS_ERR_TX_LENGTH               ,       
    MTS_ERR_CNF_TYPE                ,       
    MTS_ERR_MODULE_ID               ,       
    MTS_ERR_ACTION_NOT_CONFIG       ,       
    MTS_ERR_CONFIGURATION           ,       
    MTS_ERR_CHANNEL_NUMBER_NOT_FOUND,       
    MTS_ERR_SLOT_NUMBER_NOT_FOUND   ,       
    MTS_ERR_HARDWARE                ,       
    MTS_ERR_CONFIG_DEBUG_MESSAGE    ,       
    MTS_ERR_MAX                             
}enum_mts_protocol_errorcodes_t;

typedef enum
{
    MTS_DEVICE_TYPE_KSR        = 0x0,   
    MTS_DEVICE_TYPE_MTS_IO     = 0x1,  
    MTS_DEVICE_TYPE_SILO_CAN_1 = 0x2,   
    MTS_DEVICE_TYPE_SILO_CAN_2 = 0x3,   
    MTS_DEVICE_TYPE_DUMMY      = 0x4,   
    MTS_DEVICE_TYPE_ILM        = 0x5,   
    MTS_DEVICE_TYPE_SPS        = 0x6,   
    MTS_DEVICE_TYPE_HTRB_NEST  = 0x7,   
    MTS_DEVICE_TYPE_MAX                
}enum_mts_device_type_t;

typedef enum
{
    MTS_SHORT_WRITE         = 0x0,      
    MTS_SHORT_READ_REQUEST  = 0x1,      
    MTS_SHORT_DATA          = 0x2,      
    MTS_SHORT_REQUEST       = 0x3,      
    MTS_LONG_ANSWER         = 0x4,      
    MTS_LONG_START          = 0x5,      
    MTS_LONG_DATA           = 0x6,      
    MTS_LONG_COUNTER        = 0x7,      
    MTS_LONG_ANSWER_OLD     = 0x400,    
    MTS_LONG_START_OLD      = 0x401,    
    MTS_LONG_DATA_OLD       = 0x402,    
    MTS_LONG_COUNTER_OLD    = 0x403     
}enum_mts_transfer_type_t;

#define MTS_HTRB_INTR_NEST_WRITE   MTS_LONG_COUNTER  
#define MTS_HTRB_INTR_NEST_REQUEST MTS_LONG_ANSWER   

typedef enum
{
    MTS_BIT             = 0x00, 
    MTS_BYTE            = 0x01, 
    MTS_UINT            = 0x02, 
    MTS_SINT            = 0x03, 
    MTS_ULONG           = 0x04, 
    MTS_SLONG           = 0x05, 
    MTS_FLOAT           = 0x06, 
    MTS_5_BYTE_ARRAY    = 0x07  
}enum_mts_msg_data_type_t;

typedef enum
{
    MTS_SLOT_CNF_NUL            = 0x00  , 
    MTS_SLOT_CNF_DI8            = 0x80  , 
    MTS_SLOT_CNF_DO8            = 0x40  , 
    MTS_SLOT_CNF_ADC10V         = 0xA0  , 
    MTS_SLOT_CNF_DA2            = 0x60  , 
    MTS_SLOT_CNF_PWM8S1         = 0x61  , 
    MRST_6_K_TEMP               = 0x1F  , 
    MTS_SLOT_CNF_DYN            = 0x05  , 
    MTS_SLOT_CNF_HEINZ          = 0x06  , 
    MTS_SLOT_CNF_FUG            = 0x07  , 
    MTS_SLOT_CNF_CAN_SYNC       = 0x08  , 
    MTS_SLOT_CNF_NOPHUT_TO_MRS  = 0x09    
} enum_mts_set_slot_cnf_t; 

typedef enum
{
    MTS_CNF_MODULE      = 0x00  , 
    MTS_CNF_SEND        = 0x01  , 
    MTS_CNF_CALIBRATE   = 0x02  , 
    MTS_CNF_ERROR       = 0xFF  , 
} enum_mts_cnf_type_t;

typedef struct
{
    uint8_t send_note_ID:4;                     
    enum_mts_device_type_t send_device_type;    
    uint8_t module_id:4;                        
    enum_mts_device_type_t device_type;         
    uint8_t length;                             
    enum_mts_transfer_type_t transfer_type;     
    uint8_t slot_id;                            
    uint8_t burst_mode:1;                       
    uint8_t channel_id:4;                       
    enum_mts_msg_data_type_t data_type;         
    uint8_t burst_data_0;                       
    uint8_t data[8];                            
}struct_mts_msg_t; 

typedef enum
{
   SLOT_CONFIG_NO,      
   SLOT_CONFIG_STATIC,  
   SLOT_CONFIG_DO,      
   SLOT_CONFIG_DI,      
   SLOT_CONFIG_AI,      
   SLOT_CONFIG_AO,      
   SLOT_CONFIG_SPECIAL, 
   SLOT_CONFIG_FUG,     
   SLOT_CONFIG_HEINZ,   
   SLOT_CONFIG_MAX      
}enum_mts_slot_configuration_t;

typedef enum_hal_errorcodes_t (*mts_can_tx_function_type_t)(uint32_t id, uint8_t dlc, uint8_t * data);

typedef enum_mts_protocol_errorcodes_t (*mts_channel_write_function_type_t)( enum_mts_msg_data_type_t data_type, uint8_t* ptr_value );

typedef enum_mts_protocol_errorcodes_t (*mts_channel_read_function_type_t)( enum_mts_msg_data_type_t data_type, uint8_t* ptr_value );

typedef enum_mts_protocol_errorcodes_t (*mts_slot_config_request_function_type_t)( enum_mts_cnf_type_t cnf_type, uint8_t slot_number, uint8_t channel, uint8_t* ptr_value, enum_mts_device_type_t send_device_type, uint8_t send_device );

typedef enum_mts_protocol_errorcodes_t (*mts_slot_write_function_type_t)( uint8_t channel, enum_mts_msg_data_type_t data_type, uint8_t* ptr_value );

typedef enum_mts_protocol_errorcodes_t (*mts_slot_read_function_type_t)( uint8_t channel, enum_mts_msg_data_type_t data_type, uint8_t* ptr_value );

typedef enum_mts_protocol_errorcodes_t (*mts_slot_burst_write_function_type_t)( uint8_t* ptr_value );

typedef enum_mts_protocol_errorcodes_t (*mts_slot_burst_read_function_type_t)( uint8_t* ptr_value );

typedef enum_mts_protocol_errorcodes_t (*mts_bl_reset_function_type_t)( uint8_t request_device, uint8_t board_number );

typedef enum
{
    MTS_CHANNEL_0,  
    MTS_CHANNEL_1,  
    MTS_CHANNEL_2,  
    MTS_CHANNEL_3,  
    MTS_CHANNEL_4,  
    MTS_CHANNEL_5,  
    MTS_CHANNEL_6,  
    MTS_CHANNEL_7,  
    MTS_CHANNEL_8,  
    MTS_CHANNEL_9,  
    MTS_CHANNEL_10, 
    MTS_CHANNEL_11, 
    MTS_CHANNEL_12, 
    MTS_CHANNEL_13, 
    MTS_CHANNEL_14, 
    MTS_CHANNEL_15, 
    MTS_CHANNEL_MAX, 
}enum_mts_channel_t;

typedef struct
{
    enum_mts_channel_t                name;         
    uint8_t                           number;       
    mts_channel_write_function_type_t write_action; 
    mts_channel_read_function_type_t  read_action;  
    const char*                       description;  
}struct_mts_channel;

typedef struct
{
    uint8_t                                 name;                       
    uint8_t                                 number;                     
    enum_mts_slot_configuration_t           config;                     
    mts_slot_config_request_function_type_t config_action;              
    struct_mts_channel*                     channel;                    
    mts_slot_write_function_type_t          write_action;               
    mts_slot_read_function_type_t           read_action;                
    const char*                             description;                
}struct_mts_slot_t;


typedef struct
{
    uint8_t                                 slot_nb_start;              
    uint8_t                                 slot_nb_end;                
    mts_slot_burst_write_function_type_t    write_action;               
    mts_slot_burst_read_function_type_t     read_action;                
    const char*                             description;                
}struct_mts_burst_slot_t;

typedef struct
{
    void*   ptr_mts_handle;                              
    enum_mts_device_type_t device_type;                  
    uint8_t module_id;                                   
    mts_can_tx_function_type_t can_tx;                   
    struct_mts_slot_t* ptr_slot_cfg;                     
    struct_mts_burst_slot_t* ptr_slot_burst_cfg;         
    uint8_t slot_count;                                  
    struct_mts_msg_t rx_message;                         
    struct_mts_msg_t tx_message;                         
    uint8_t enable_debug_message;                        
    mts_bl_reset_function_type_t bl_reset;               
} struct_mts_protocol_handle_t;


void role_protocol_mts_constructor(role_id_t id, pf_role_constructor parent);

#endif 

#ifdef __cplusplus
}
#endif

#endif 

