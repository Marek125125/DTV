
#ifndef APP_USER_API_USER_API_MOTOR_H_
#define APP_USER_API_USER_API_MOTOR_H_

#include "motor_app.h"

uint32_t user_motor_control(enum_motor_channel motor, uint16_t pwm_freq, uint16_t duty, uint16_t ramp_max, uint16_t ramp_up, uint16_t ramp_dwn, enum_motor_dir dir, uint8_t motor_check, uint32_t* curr);

#endif 

