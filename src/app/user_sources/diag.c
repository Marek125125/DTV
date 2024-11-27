/*==================================================================================================
*                                      FILE DESCRIPTION
==================================================================================================*/
/**
 * \file        diag.c
 * \brief       Implementation of can_handler
 *
 * \version     0.0.1
 *
 */

/*==================================================================================================
*                                          INCLUDES
==================================================================================================*/
// include standard libs

// include platform libs

// include specific header files for this SW component
#include "diag.h"
#include "stdio.h"
#include "input_proc.h"

void DecToHexStr(int dec, char *str)
  { sprintf(str, "%X", dec); }

uint16_t first_frame_sent = 0;
void diag_start_request(
    bios_can_msg_typ *msg) { //                                                       size,respone_SID ,DID,       ,RAND
  user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x06, SID_DIAG_START_RESPONSE, msg->data[2], 0x00, 0x1E,
                    0x01, 0xE0, FILLER_BYTE);
}

void read_data_request(bios_can_msg_typ *msg,data_struct *data) {
  if (msg->data[2] == DID_CHARGE_PRS_BYTE_1 && msg->data[3] == DID_CHARGE_PRS_BYTE_2) {
    char val[4];
    DecToHexStr(data->pressure,val);
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, DID_CHARGE_PRS_SIZE, SID_READ_DATA_RESPONSE, DID_CHARGE_PRS_BYTE_1,
                      DID_CHARGE_PRS_BYTE_2, val[0], val[1], FILLER_BYTE, FILLER_BYTE);
    

  }
  else if (msg->data[2] == DID_PEDAL_POS_BYTE_1 && msg->data[3] == DID_PEDAL_POS_BYTE_2) {
    char val[4];
    DecToHexStr(data->throttle,val);
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, DID_PEDAL_POS_SIZE, SID_READ_DATA_RESPONSE, DID_PEDAL_POS_BYTE_1,
                      DID_PEDAL_POS_BYTE_2, val[0], FILLER_BYTE, FILLER_BYTE, FILLER_BYTE);
    

  }
  else if (msg->data[2] == UNKNOWN_DID_3_BYTE_1 && msg->data[3] == UNKNOWN_DID_3_BYTE_2) {
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, FIRST_FRAME, UNKNOWN_DID_3_SIZE, SID_READ_DATA_RESPONSE,
                      msg->data[2], msg->data[3], 0x01, 0x1D, 0x40);
    first_frame_sent = UNKNOWN_DID_3_BYTE_2;

  } else if (msg->data[2] == DID_ODX_FILE_BYTE_1 || msg->data[2] == DID_ASAM_ODX_FILE_BYTE_1 ||
             msg->data[2] == DID_MAN_SPARE_BYTE_1 || msg->data[2] == DID_APP_SW_BYTE_1 ||
             msg->data[2] == DID_ECU_HW_BYTE_1 || msg->data[2] == DID_ENG_TYPE_BYTE_1) {
    switch (msg->data[3]) {
    case DID_ODX_FILE_BYTE_2:
      user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, FIRST_FRAME, ODX_FILE_SIZE, SID_READ_DATA_RESPONSE,
                        msg->data[2], msg->data[3], 0x45, 0x56, 0x5F);
      first_frame_sent = DID_ODX_FILE_BYTE_2;
      break;
    case DID_ASAM_ODX_FILE_BYTE_2:
      user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, FIRST_FRAME, ASAM_ODX_SIZE, SID_READ_DATA_RESPONSE,
                        msg->data[2], msg->data[3], 0x30, 0x30, 0x32);
      first_frame_sent = DID_ASAM_ODX_FILE_BYTE_2;
      break;
    case DID_MAN_SPARE_BYTE_2:
      user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, FIRST_FRAME, MAN_SPARE_SIZE, SID_READ_DATA_RESPONSE,
                        msg->data[2], msg->data[3], 0x30, 0x34, 0x45);
      first_frame_sent = DID_MAN_SPARE_BYTE_2;
      break;
    case DID_APP_SW_BYTE_2:
      user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x03, SID_READ_DATA_RESPONSE, msg->data[2],
                        msg->data[3], 0x39, 0x38, 0x32, 0x30);
      break;
    case DID_ECU_HW_BYTE_2:
      user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, FIRST_FRAME, ECU_HW_SIZE, SID_READ_DATA_RESPONSE,
                        msg->data[2], msg->data[3], 0x30, 0x34, 0x45);
      first_frame_sent = DID_ECU_HW_BYTE_2;
      break;
    case DID_ENG_TYPE_BYTE_2:
      user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, FIRST_FRAME, ENG_TYPE_SIZE, SID_READ_DATA_RESPONSE,
                        msg->data[2], msg->data[3], 0x31, 0x2E, 0x34);
      first_frame_sent = DID_ENG_TYPE_BYTE_2;
      break;
    case UNKNOWN_DID_1_BYTE_2:
      user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x06, SID_READ_DATA_RESPONSE, msg->data[2],
                        msg->data[3], 0x48, 0x32, 0x30, FILLER_BYTE);
      break;
    case UNKNOWN_DID_2_BYTE_2:
      user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, FIRST_FRAME, UNKNOWN_DID_2_SIZE, SID_READ_DATA_RESPONSE,
                        msg->data[2], msg->data[3], 0x80, 0x00, 0x00);
      first_frame_sent = UNKNOWN_DID_2_BYTE_2;
      break;
      break;
    }
  } else {
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x03, RESPONSE_DENIED, msg->data[1], FILLER_BYTE,
                      FILLER_BYTE, FILLER_BYTE, FILLER_BYTE, FILLER_BYTE);
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x03, RESPONSE_DENIED, msg->data[1], FILLER_BYTE,
                      FILLER_BYTE, FILLER_BYTE, FILLER_BYTE, FILLER_BYTE);
  }
}

void consecutive_frame_sending(bios_can_msg_typ *msg) {
  switch (first_frame_sent) {
  case DID_ODX_FILE_BYTE_2:
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x21, 0x45, 0x43, 0x4D, 0x31, 0x34, 0x54, 0x46);
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x22, 0x53, 0x30, 0x31, 0x31, 0x30, 0x34,
                      0x45); 
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x23, 0x39, 0x30, 0x36, 0x30, 0x32, 0x34, 0x41);
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x24, 0x45, 0x00, FILLER_BYTE, FILLER_BYTE, FILLER_BYTE,
                      FILLER_BYTE, FILLER_BYTE);
    first_frame_sent = 0x00;
    break;
  case DID_ASAM_ODX_FILE_BYTE_2:
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x21, 0x30, 0x30, 0x34, FILLER_BYTE, FILLER_BYTE,
                      FILLER_BYTE, FILLER_BYTE);
    first_frame_sent = 0x00;
    break;
  case DID_MAN_SPARE_BYTE_2:
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x21, 0x39, 0x30, 0x36, 0x30, 0x32, 0x34, 0x41);
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x22, 0x45, FILLER_BYTE, FILLER_BYTE, FILLER_BYTE,
                      FILLER_BYTE, FILLER_BYTE, FILLER_BYTE);
    first_frame_sent = 0x00;
    break;
  case DID_ECU_HW_BYTE_2:
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x21, 0x39, 0x30, 0x37, 0x33, 0x30, 0x39, 0x43);
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x22, 0x47, FILLER_BYTE, FILLER_BYTE, FILLER_BYTE,
                      FILLER_BYTE, FILLER_BYTE, FILLER_BYTE);
    first_frame_sent = 0x00;
    break;
  case UNKNOWN_DID_2_BYTE_2:
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x21, 0x06, 0xEF, 0x62, FILLER_BYTE, FILLER_BYTE,
                      FILLER_BYTE, FILLER_BYTE);
    first_frame_sent = 0x00;
    break;
  case DID_ENG_TYPE_BYTE_2:
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x21, 0x20, 0x54, 0x46, 0x53, 0x49, 0x20, 0x68);
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x22, 0x79, 0x20, 0x20, FILLER_BYTE, FILLER_BYTE,
                      FILLER_BYTE, FILLER_BYTE);
    first_frame_sent = 0x00;
    break;
  case UNKNOWN_DID_3_BYTE_2:
    user_can_msg_send(CAN_BUS_0, RESPONSE_ID, STANDARD_ID, 8, 0x21, 0x12, 0x63, 0x64, 0x05, 0x08, 0x30, 0x08);
    first_frame_sent = 0x00;
    break;
  }
}