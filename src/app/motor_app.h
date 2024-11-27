#ifndef MOTOR_APP_H
#define MOTOR_APP_H

#include <stdint.h>
#include <stdbool.h>

#define PWM_DUTY_MAX_VAL                1000
#define PWM_DUTY_ON                     PWM_DUTY_MAX_VAL
#define PWM_DUTY_OFF                    0

#define TWO_SECONDS_MS                  2000
#define FIVE_SECONDS_MS                 5000
#define TEN_SECONDS_MS                  10000
#define FIVE_MINS_MS                    30000

#define STARTUP_TIMER                   500
#define MOTOR_CHECK_TIMER               100


#define LP_FILTER_MAX                   20

#define MOTOR_CHECK_OK                  10

#define PWM_FREQ_MAX_HZ_VNQ5050AK       1000
#define PWM_FREQ_MAX_HZ_BTS6143         500
#define PWM_FREQ_MAX_HZ_BTS50055        500
#define PWM_FREQ_MAX_HZ_BTN8962         20000
#define PWM_FREQ_MAX_HZ_BTN9970LV       20000
#define PWM_FREQ_MAX_DISCRETE			20000
#include "sci_app.h"


#define PWM_FREQ_BTN9970                2000
#define PWM_FREQ_BTN8962                2000
#define PWM_FREQ_BTS6143                200
#define PWM_FREQ_BTS50055               200
#define PWM_FREQ_DISCRETE				2000
#define PWM_DUTY                        500

#define RAMP_MAX                        0
#define RAMP_UP                         0
#define RAMP_DOWN                       0

typedef enum 
{
    STATE_INIT,
    STATE_IDLE,
    STATE_START,
    STATE_RUN,
    STATE_CHECK,
    STATE_ERR,
    STATE_STOP
} enum_motor_ctrl_state;

typedef enum
{
    MOTOR_HSD,
    MOTOR_HALF_BRIDGE_HS,
    MOTOR_HALF_BRIDGE_LS,
    MOTOR_FULL_BRIDGE,
	MOTOR_FULL_BRIDGE_DISCRETE
} enum_motor_bridge_mode;

typedef enum
{
    MOTOR_DIR_FORWARD   =   0,
    MOTOR_DIR_BACKWARD
} enum_motor_dir;

typedef enum
{
    MOTOR_ERR_OK,                   

    MOTOR_ERR_NOT_SUPPORTED,        
    MOTOR_ERR_NULL_POINTER,         

    MOTOR_ERR_INVALID_MOTOR,        
    MOTOR_ERR_INVALID_BRIDGE_MODE,  
    MOTOR_ERR_INVALID_DIRECTION,    
    MOTOR_ERR_INVALID_PWM,          
    MOTOR_ERR_INVALID_RAMP,         

    MOTOR_ERR_CHECK_FAILED,         
    MOTOR_ERR_OVERCURRENT,          

    MOTOR_ERR_GENERAL
} enum_motor_err;

typedef enum 
{

    FB_MOTOR_1                 ,
    FB_MOTOR_2                 ,
    FB_MOTOR_3                 ,

    HB_MOTOR_1                 ,
    HB_MOTOR_2                 ,
    HB_MOTOR_3                 ,
    HB_MOTOR_4                 ,
    HB_MOTOR_5                 ,
    HB_MOTOR_6                 ,

    HSD_MOTOR_1                ,
    HSD_MOTOR_2                ,
    HSD_MOTOR_3                ,
    HSD_MOTOR_4                ,
    HSD_MOTOR_5                ,
    HSD_MOTOR_6                ,
    HSD_MOTOR_7                ,
    HSD_MOTOR_8                ,
    HSD_MOTOR_9                ,
    HSD_MOTOR_10               ,
    HSD_MOTOR_11               ,
    HSD_MOTOR_12               ,
    HSD_MOTOR_13               ,
    HSD_MOTOR_14               ,
    HSD_MOTOR_15               ,
    HSD_MOTOR_16               ,

    MOTOR_MAX
} enum_motor_channel;


typedef struct
{
    uint8_t idx;
    uint32_t saved;
    uint32_t filtered;
    uint32_t offset;
} struct_motor_curr;

typedef struct
{
    uint16_t overcurrent_limit_2s;
    uint16_t overcurrent_limit_5s;
    uint16_t overcurrent_limit_5m;
} struct_overcurrent_config_t;

typedef struct
{
    uint8_t do_motor_inh;
    uint8_t do_motor_inh_2;
    uint8_t pwm_motor_positive;
    uint8_t pwm_motor_negative;
    uint8_t ai_motor_vm;
    uint8_t ai_motor_positive;
    uint8_t ai_motor_negative;
} struct_fb_motor_pins_t;

typedef struct
{
    uint8_t pwm_motor_positive_H1;
    uint8_t pwm_motor_negative_L1;
    uint8_t pwm_motor_positive_H2;
    uint8_t pwm_motor_negative_L2;
    uint8_t ai_motor_vm;
    uint8_t ai_m_curr;
    uint8_t di_oc_stat;
    uint8_t do_oc_reset;

} struct_fb_discrete_motor_pins_t;

typedef struct
{
    uint8_t do_motor_inh;
    uint8_t pwm_motor;
    uint8_t ai_motor;
} struct_hb_motor_pins_t;

typedef struct
{
    uint8_t pwm_motor;
    uint8_t ai_motor_curr_hsd;
    uint8_t ai_motor_curr_ina;
    uint8_t ai_motor_vm;
} struct_hsd_motor_pins_t;

typedef union {
    struct_fb_motor_pins_t* fb_motor_pins;
    struct_hb_motor_pins_t* hb_motor_pins;
    struct_hsd_motor_pins_t* hsd_motor_pins;
    struct_fb_discrete_motor_pins_t* fb_discrete_motor_pins;
} union_motor_ctrl_pins_t;

typedef struct 
{
    uint8_t index;
    enum_motor_bridge_mode bridge_mode;
} struct_motor_channel_t;

typedef struct 
{
    uint8_t total_channels;
    struct_motor_channel_t channel;
    union_motor_ctrl_pins_t motor_pins;
} struct_motor_channel_config_t;

typedef struct
{
    uint16_t max_pwm_freq;
    uint16_t pwm_freq;
    uint16_t pwm_duty;
    uint16_t ramp_max;
    uint16_t ramp_up;
    uint16_t ramp_dwn;
    uint8_t motor_check;
    enum_motor_dir direction;
    struct_motor_channel_config_t ch_config;
    struct_overcurrent_config_t overcurrent_config;
    uint16_t get_curr_gain;
} struct_motor_config_t;

typedef struct
{
    uint16_t duty_out;
    enum_motor_dir dir_saved;
    uint16_t cnt;
    uint32_t timer;
    uint8_t err_ack_flg;
    uint32_t curr;
    enum_motor_err err;
    struct_motor_curr curr_m_p;
    struct_motor_curr curr_m_n;
    enum_motor_ctrl_state state;
    struct_motor_config_t* config;
    bool overcurrent_flag;
    uint32_t overcurrenttime2s;
    uint32_t overcurrenttime5s;
    uint32_t overcurrenttime5m;
    uint32_t overcurrenttimereset;
} struct_motor_handle_t;


void motor_set_handle(struct_motor_handle_t* ext_motor_handles_ptr);

enum_motor_err motor_set_hb_type(enum_motor_channel motor, enum_motor_bridge_mode hb_mode);

enum_motor_err motor_set_direction(enum_motor_channel motor, enum_motor_dir dir);

enum_motor_err motor_ramp_up(enum_motor_channel motor, uint16_t target_duty, uint16_t up_speed_ms);

enum_motor_err motor_ramp_down(enum_motor_channel motor, uint16_t target_duty, uint16_t down_speed_ms);

enum_motor_err motor_set_frequency(enum_motor_channel motor, uint16_t frequency);

enum_motor_err motor_set_duty_cycle(enum_motor_channel motor, uint16_t duty_cycle);

enum_motor_err motor_update(enum_motor_channel motor, uint32_t *curr);


enum_motor_err motor_cyclic(enum_motor_channel motor, uint16_t pwm_freq, uint16_t pwm_duty, uint16_t ramp_max, uint16_t ramp_up, uint16_t ramp_dwn, uint8_t dir, uint8_t motor_check, uint32_t* curr);


#endif 

