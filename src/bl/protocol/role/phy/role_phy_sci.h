
#ifndef SRC_ROLE_PHY_SCI_H_
#define SRC_ROLE_PHY_SCI_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#if defined(SPECIFICS_SCI_PROTOCOL) || defined(SPECIFICS_MTS_PROTOCOL)
#if !defined(ROLE_MINIMAL)

#define SCI_TX_MB_NOT_INIT ((uint8_t)(-1))

#define ROLE_DO_PARAM_LIST_SCI \
        , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_SCI_SETTINGS*, ROLE_SCI_PARAM_SETTINGS) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(struct_ROLE_SCI_RXD*, ROLE_SCI_PARAM_RX_MSG) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(const struct_ROLE_SCI_TXD*, ROLE_SCI_PARAM_TX_MSG) \
        , ROLE_MAP_REAL_PARAM_TO_NUM(const struct_ROLE_SCI_TXD_WITH_TIMEOUT*, ROLE_SCI_PARAM_TXD_WITH_TIMEOUT)



typedef enum
{
    ROLE_SCI_PARAM_SETTINGS = ROLE_COMPOSE_DO_PARAM_BASE(ROLE_TYPE_SPI),
    ROLE_SCI_PARAM_RX_MSG,
    ROLE_SCI_PARAM_TX_MSG,
    ROLE_SCI_PARAM_TXD_WITH_TIMEOUT
} enum_ROLE_SCI_DO_PARAM;

typedef enum
{

    ROLE_SCI_CONFIGURE          = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     0u, ROLE_SCI_PARAM_SETTINGS),

    ROLE_SCI_SET_BAUDRATE       = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     1u, ROLE_ANY_PARAM_UINT32),

    ROLE_SCI_WRITE_MSG          = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     2u, ROLE_SCI_PARAM_TX_MSG),
    ROLE_SCI_WRITE_WITH_TIMEOUT = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     3u, ROLE_SCI_PARAM_TXD_WITH_TIMEOUT),
    ROLE_SCI_WRITE_AND_WAIT_TILL_SENT_WITH_TIMEOUT
                                = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     4u, ROLE_SCI_PARAM_TXD_WITH_TIMEOUT),
    ROLE_SCI_READ_MSG           = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     5u, ROLE_SCI_PARAM_RX_MSG),

    ROLE_SCI_RX_PURGE           = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     6u, ROLE_ANY_PARAM_UINT32),
    ROLE_SCI_TX_PURGE           = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     7u, ROLE_ANY_PARAM_UINT32),

    ROLE_SCI_DEACTIVATE         = ROLE_COMPOSE_DO_ID(ROLE_TYPE_SCI,     8u, ROLE_ANY_PARAM_UINT32)

} enum_ROLE_SCI_DO_ID;


#endif 
#endif 

typedef struct
{
    uint8_t bus;
    uint32_t baudrate;
    uint32_t reserved;
} __attribute__((packed)) struct_ROLE_SCI_SETTINGS;

#if defined(SPECIFICS_SCI_PROTOCOL) || defined(SPECIFICS_MTS_PROTOCOL)
#if !defined(ROLE_MINIMAL)

typedef struct
{
    uint8_t    *data;
    uint16_t    len;
} struct_ROLE_SCI_MSG;

typedef struct_ROLE_SCI_MSG struct_ROLE_SCI_RXD;
typedef struct_ROLE_SCI_MSG struct_ROLE_SCI_TXD;

typedef struct
{
    struct_ROLE_SCI_MSG     msg;
    uint16_t                timeout;
} struct_ROLE_SCI_TXD_WITH_TIMEOUT;


void role_phy_sci_constructor(role_id_t id, pf_role_constructor parent);

#endif 
#endif 

#ifdef __cplusplus
}
#endif 

#endif 

