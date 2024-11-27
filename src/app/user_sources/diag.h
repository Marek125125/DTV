#ifndef _DIAG_H_
#define _DIAG_H_


#include "user_code.h"
#include "input_proc.h"

#define FILLER_BYTE 0x55
#define RECIEVED_ID 0x7E0
#define RESPONSE_ID 0x7E8

#define RESPONSE_ADDER 0x40
#define RESPONSE_DENIED 0x7F
#define GENERAL_REJECT 0x10

#define SID_DIAG_START 0x10
#define SID_DIAG_START_RESPONSE 0x50
#define SID_READ_DATA 0x22
#define SID_READ_DATA_RESPONSE 0x62

#define DID_ODX_FILE_BYTE_1 0xF1
#define DID_ODX_FILE_BYTE_2 0x9E
#define ODX_FILE_SIZE 0x1D

#define DID_ASAM_ODX_FILE_BYTE_1 0xF1
#define DID_ASAM_ODX_FILE_BYTE_2 0xA2
#define ASAM_ODX_SIZE 0x09

#define DID_MAN_SPARE_BYTE_1 0xF1
#define DID_MAN_SPARE_BYTE_2 0x87
#define MAN_SPARE_SIZE 0x0E

#define DID_APP_SW_BYTE_1 0xF1
#define DID_APP_SW_BYTE_2 0x89

#define DID_ECU_HW_BYTE_1 0xF1
#define DID_ECU_HW_BYTE_2 0x91
#define ECU_HW_SIZE 0x0E

#define DID_ENG_TYPE_BYTE_1 0xF1
#define DID_ENG_TYPE_BYTE_2 0x97
#define ENG_TYPE_SIZE 0x10

#define UNKNOWN_DID_1_BYTE_1 0xF1
#define UNKNOWN_DID_1_BYTE_2 0xA3

#define UNKNOWN_DID_2_BYTE_1 0xF1
#define UNKNOWN_DID_2_BYTE_2 0xA5
#define UNKNOWN_DID_2_SIZE 0x09

#define UNKNOWN_DID_3_BYTE_1 0x06
#define UNKNOWN_DID_3_BYTE_2 0x00
#define UNKNOWN_DID_3_SIZE 0x0D

#define DID_PEDAL_POS_BYTE_1 0xF4
#define DID_PEDAL_POS_BYTE_2 0x49
#define DID_PEDAL_POS_SIZE 0x04

#define DID_CHARGE_PRS_BYTE_1 0x11
#define DID_CHARGE_PRS_BYTE_2 0xF0
#define DID_CHARGE_PRS_SIZE 0x05


#define FIRST_FRAME 0x10
#define FLOW_CONTROL 0x30


//BYTE[0] - size
//BYTE[1] - MODE
//BYTE[2] - PID
//BYTE[3] - DATA
//BYTE[4] - DATA
//BYTE[5] - DATA
//BYTE[6] - DATA
//BYTE[7] - DATA

void DecToHexStr(int dec, char *str);
void diag_start_request(bios_can_msg_typ* msg);
void read_data_request(bios_can_msg_typ* msg,data_struct *data);
void consecutive_frame_sending(bios_can_msg_typ* msg);

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif
/*! @}*/

#endif
