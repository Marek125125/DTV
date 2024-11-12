
#ifndef SRC_ROLE_SCI_PROTOCOL_H_
#define SRC_ROLE_SCI_PROTOCOL_H_

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(SPECIFICS_SCI_PROTOCOL)

#include <assert.h>


#define ROLE_DO_PARAM_LIST_SCI_PROTOCOL

#define SCI_RESET_STOP_DELAY_MS 10000  


#define s19_rec_type_pos 0
#define s19_size_pos 2
#define s19_addr_pos 4
#define s19_payload_pos 12

#define s19_rec_type_length (s19_size_pos - s19_rec_type_pos) 
#define s19_size_length (s19_addr_pos - s19_size_pos)
#define s19_addr_length (s19_payload_pos - s19_addr_pos)
#define s19_checksum_length 2

#define s19_all_headers_length (s19_rec_type_length + s19_size_length + s19_addr_length + s19_checksum_length)

typedef enum
{
    SREC_UNKNOWN = 0,
    SREC_PAYLOAD,
    SREC_START,
    SREC_END,
    SREC_METADATA,

    SREC_S0 = SREC_START,
    SREC_S3 = SREC_PAYLOAD,
    SREC_S5 = SREC_METADATA,
    SREC_S6 = SREC_METADATA,
    SREC_S7 = SREC_END

} enum_S19_RECORD_TYPE;

typedef struct __attribute__((__packed__))
{
    enum_S19_RECORD_TYPE record_type;
    uint8_t checksum;
    uint8_t size;
    uint32_t address;
    uint8_t payload[100];
    uint8_t payload_length;
} struct_S19_DECODED_STRING;

typedef enum {XMODEM_RECEIVE_INITIAL,
              XMODEM_RECEIVE_SEND_C,
              XMODEM_RECEIVE_WAIT_FOR_BYTE,
              XMODEM_RECEIVE_READ_BLOCK,
              XMODEM_RECEIVE_BLOCK_INVALID,
              XMODEM_RECEIVE_BLOCK_VALID,
              XMODEM_RECEIVE_BLOCK_ACK,
              XMODEM_RECEIVE_BLOCK_NACK,
              XMODEM_RECEIVE_READ_BLOCK_COMPLETED,
              XMODEM_RECEIVE_CAN_RECEIVED,
              XMODEM_RECEIVE_TIMEOUT,
              XMODEM_RECEIVE_ABORT_TRANSFER,
              XMODEM_RECEIVE_SEND_FINAL_ACK,
              XMODEM_RECEIVE_TRANSFER_COMPLETE,
              XMODEM_RECEIVE_TRANSFER_ERROR,
              XMODEM_RECEIVE_UNKNOWN } xmodem_receive_state_t;

typedef bool (*xmodem_callback_set_buff)(const uint32_t buffer_size, uint8_t *buffer);

enum XMODEM_CONTROL_CHARACTERS {SOH = 0x01, EOT = 0x04, ACK = 0x06, NACK = 0x15, ETB = 0x17, CAN = 0x18, C = 0x43};

#define XMODEM_PAYLOAD_SIZE 128u
#define XMODEM_FULL_PACKET_SIZE (XMODEM_PAYLOAD_SIZE + 5)

#ifdef __GNUC__
typedef struct __attribute__((__packed__))
{
  uint8_t  preamble;
  uint8_t  id;
  uint8_t  id_complement;
  uint8_t  data[XMODEM_PAYLOAD_SIZE];
  uint16_t crc;
} xmodem_packet_t;
#else
#pragma pack(push,1)
typedef struct
{
  uint8_t  preamble;
  uint8_t  id;
  uint8_t  id_complement;
  uint8_t  data[XMODEM_PAYLOAD_SIZE];
  uint16_t crc;
} xmodem_packet_t;
#pragma pack(pop)
#endif
static_assert ((sizeof(xmodem_packet_t) == XMODEM_FULL_PACKET_SIZE), "struct xmodem_packet_t should be packed. Add packing for your compiler above");

typedef bool (*xmodem_callback_read_data)(const uint32_t requested_size, uint8_t *buffer, uint32_t *returned_size);

typedef bool (*xmodem_callback_write_data)(const uint32_t requested_size, uint8_t *buffer, bool *write_success);


void role_protocol_sci_constructor(role_id_t id, pf_role_constructor parent);

#endif 

#ifdef __cplusplus
}
#endif

#endif 

