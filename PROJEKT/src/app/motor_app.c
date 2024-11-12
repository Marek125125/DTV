#include <stdint.h>

#include "modulhardwarecode.h"

#include "motor_app.h"

#include "user_api_io.h"
#include "user_api_pwm.h"
#include "user_api_timer.h"
#include "user_api_ai.h"

#include "graph_code.h"

#include "sfl_timer.h"






static struct_motor_handle_t*  motor_handles_ptr = NULL;



static void motor_state_machine(struct_motor_handle_t* handle, void* pins);

static void motor_control_inhibit( struct_motor_handle_t* motor_ctrl_handle, uint8_t inhibit_flag );
static void motor_stop( struct_motor_handle_t* motor_ctrl_handle );
static void motor_run( struct_motor_handle_t* motor_ctrl_handle );
static void motor_check_overcurrent( struct_motor_handle_t *motor_ctrl_handle, uint16_t limit, uint32_t* timer, uint32_t time );
static void motor_filter_current( uint8_t ch, uint32_t* ptr_saved, uint32_t* ptr_filtered, uint32_t* ptr_ofst, uint16_t get_curr_gain );
static uint8_t motor_ramp_up_dwn( uint16_t target_duty, uint16_t max_duty, uint16_t up_speed, uint16_t down_speed, uint16_t* counter, uint16_t* current_duty );
static uint32_t get_curr( uint8_t channel, uint32_t offset, uint16_t gain );


void motor_set_handle(struct_motor_handle_t* ext_motor_handles_ptr)
{
    motor_handles_ptr = ext_motor_handles_ptr;
}

enum_motor_err motor_set_hb_type(enum_motor_channel motor, enum_motor_bridge_mode hb_mode)
{
    enum_motor_err err = MOTOR_ERR_GENERAL;

    if (motor_handles_ptr == NULL) 
    {
        err = MOTOR_ERR_NULL_POINTER;
    }
    else if (motor >= MOTOR_MAX) 
    {
        err = MOTOR_ERR_INVALID_MOTOR;
    }

    else
    {
        struct_motor_handle_t* motor_handle = &motor_handles_ptr[motor];

        if (motor_handle->config->ch_config.channel.bridge_mode != MOTOR_HALF_BRIDGE_HS || motor_handle->config->ch_config.channel.bridge_mode != MOTOR_HALF_BRIDGE_LS)
        {
            err = MOTOR_ERR_INVALID_MOTOR;
        }
        else if (hb_mode != MOTOR_HALF_BRIDGE_HS && hb_mode != MOTOR_HALF_BRIDGE_LS)
        {
            err = MOTOR_ERR_INVALID_BRIDGE_MODE;
        }
        else 
        {
            motor_handle->config->ch_config.channel.bridge_mode = hb_mode;
            err = MOTOR_ERR_OK;
        }
    }

    return err;
}

enum_motor_err motor_set_direction(enum_motor_channel motor, enum_motor_dir dir)
{
    enum_motor_err err = MOTOR_ERR_GENERAL;

    if (motor_handles_ptr == NULL) 
    {
        err = MOTOR_ERR_NULL_POINTER;
    }
    else if (motor >= MOTOR_MAX) 
    {
        err = MOTOR_ERR_INVALID_MOTOR;
    }

    else
    {
        struct_motor_handle_t* motor_handle = &motor_handles_ptr[motor];

        if (dir != MOTOR_DIR_FORWARD && dir != MOTOR_DIR_BACKWARD)
        {
            err = MOTOR_ERR_INVALID_DIRECTION;
        }

        else if (motor_handle->config->direction == dir)
        {
            err = MOTOR_ERR_OK;
        }
        else 
        {
            motor_handle->config->direction = dir;
            err = MOTOR_ERR_OK;
        }
    }

    return err;
}

enum_motor_err motor_ramp_up(enum_motor_channel motor, uint16_t target_duty, uint16_t up_speed_ms)
{
    enum_motor_err err = MOTOR_ERR_GENERAL;

    if (motor_handles_ptr == NULL) 
    {
        err = MOTOR_ERR_NULL_POINTER;
    }
    else if (motor >= MOTOR_MAX) 
    {
        err = MOTOR_ERR_INVALID_MOTOR;
    }

    else
    {
        target_duty = (target_duty > PWM_DUTY_MAX_VAL) ? PWM_DUTY_MAX_VAL : target_duty;

        struct_motor_handle_t* motor_handle = &motor_handles_ptr[motor];

        motor_handle->config->pwm_duty = target_duty;
        motor_handle->config->ramp_max = PWM_DUTY_MAX_VAL;
        motor_handle->config->ramp_up = up_speed_ms;

        err = MOTOR_ERR_OK;
    }

    return err;
}

enum_motor_err motor_ramp_down(enum_motor_channel motor, uint16_t target_duty, uint16_t down_speed_ms)
{
    enum_motor_err err = MOTOR_ERR_GENERAL;

    if (motor_handles_ptr == NULL) 
    {
        err = MOTOR_ERR_NULL_POINTER;
    }
    else if (motor >= MOTOR_MAX) 
    {
        err = MOTOR_ERR_INVALID_MOTOR;
    }

    else
    {
        target_duty = (target_duty > PWM_DUTY_MAX_VAL) ? PWM_DUTY_MAX_VAL : target_duty;

        struct_motor_handle_t* motor_handle = &motor_handles_ptr[motor];

        motor_handle->config->pwm_duty = target_duty;
        motor_handle->config->ramp_max = PWM_DUTY_MAX_VAL;
        motor_handle->config->ramp_dwn = down_speed_ms;

        err = MOTOR_ERR_OK;
    }

    return err;
}

enum_motor_err motor_set_frequency(enum_motor_channel motor, uint16_t frequency)
{
    enum_motor_err err = MOTOR_ERR_GENERAL;

    if (motor_handles_ptr == NULL) 
    {
        err = MOTOR_ERR_NULL_POINTER;
    }
    else if (motor >= MOTOR_MAX) 
    {
        err = MOTOR_ERR_INVALID_MOTOR;
    }

    else
    {
        struct_motor_handle_t* motor_handle = &motor_handles_ptr[motor];

        if (frequency > motor_handle->config->max_pwm_freq)
        {
            err = MOTOR_ERR_INVALID_PWM;
        }
        else 
        {
            motor_handle->config->pwm_freq = frequency;

            err = MOTOR_ERR_OK;
        }
    }

    return err;
}

enum_motor_err motor_set_duty_cycle(enum_motor_channel motor, uint16_t duty_cycle)
{
    enum_motor_err err = MOTOR_ERR_GENERAL;

    if (motor_handles_ptr == NULL) 
    {
        err = MOTOR_ERR_NULL_POINTER;
    }
    else if (motor >= MOTOR_MAX) 
    {
        err = MOTOR_ERR_INVALID_MOTOR;
    }

    else
    {
        struct_motor_handle_t* motor_handle = &motor_handles_ptr[motor];

        if (duty_cycle > PWM_DUTY_MAX_VAL)
        {
            err = MOTOR_ERR_INVALID_PWM;
        }
        else 
        {
            motor_handle->config->pwm_duty = duty_cycle;

            err = MOTOR_ERR_OK;
        }
    }

    return err;
}

enum_motor_err motor_update(enum_motor_channel motor, uint32_t *curr)
{

    enum_motor_err err = MOTOR_ERR_GENERAL;

    if (motor_handles_ptr == NULL) 
    {
        err = MOTOR_ERR_NULL_POINTER;
    }
    else if (motor >= MOTOR_MAX) 
    {
        err = MOTOR_ERR_INVALID_MOTOR;
    }

    else
    {
        struct_motor_handle_t* motor_handle = &motor_handles_ptr[motor];

        uint16_t pwm_freq = motor_handle->config->pwm_freq;
        uint16_t pwm_duty = motor_handle->config->pwm_duty; 
        uint16_t ramp_max = motor_handle->config->ramp_max;
        uint16_t ramp_up =  motor_handle->config->ramp_up;
        uint16_t ramp_dwn = motor_handle->config->ramp_dwn;
        uint8_t dir = motor_handle->config->direction;
        uint8_t motor_check = motor_handle->config->motor_check;

        err = motor_cyclic(motor, pwm_freq, pwm_duty, ramp_max, ramp_up, ramp_dwn, dir, motor_check, curr);

    }

    return err;
}

enum_motor_err motor_cyclic(enum_motor_channel motor, uint16_t pwm_freq, uint16_t pwm_duty, uint16_t ramp_max, uint16_t ramp_up, uint16_t ramp_dwn, uint8_t dir, uint8_t motor_check, uint32_t* curr)
{
    enum_motor_err err = MOTOR_ERR_GENERAL;

    if (motor_handles_ptr == NULL) 
    {
        err = MOTOR_ERR_NULL_POINTER;
    }

    else if (motor >= MOTOR_MAX) 
    {
        err = MOTOR_ERR_INVALID_MOTOR;
    }

    else
    {
        struct_motor_handle_t* motor_handle = &motor_handles_ptr[motor];

        if( pwm_freq > motor_handle->config->max_pwm_freq )
        {
            err = MOTOR_ERR_INVALID_PWM;
        }

        else if (dir != MOTOR_DIR_FORWARD && dir != MOTOR_DIR_BACKWARD)
        {
            err = MOTOR_ERR_INVALID_DIRECTION;
        }

        else
        {
            pwm_duty = (pwm_duty > PWM_DUTY_MAX_VAL) ? PWM_DUTY_MAX_VAL : pwm_duty;

            void* motor_pins = NULL;

            motor_handle->config->pwm_freq = pwm_freq;
            motor_handle->config->pwm_duty = pwm_duty;
            motor_handle->config->ramp_max = ramp_max;
            motor_handle->config->ramp_up = ramp_up;
            motor_handle->config->ramp_dwn = ramp_dwn;
            motor_handle->config->direction = dir;
            motor_handle->config->motor_check = motor_check;

            switch (motor_handle->config->ch_config.channel.bridge_mode)
            {
                case MOTOR_FULL_BRIDGE:
                    motor_pins = motor_handle->config->ch_config.motor_pins.fb_motor_pins;
                    break;
                case MOTOR_FULL_BRIDGE_DISCRETE:
                	motor_pins = motor_handle->config->ch_config.motor_pins.fb_discrete_motor_pins;
                	break;
                case MOTOR_HALF_BRIDGE_HS:
                    motor_pins = motor_handle->config->ch_config.motor_pins.hb_motor_pins;
                    break;
                case MOTOR_HALF_BRIDGE_LS:
                    motor_pins = motor_handle->config->ch_config.motor_pins.hb_motor_pins;
                    break;
                case MOTOR_HSD:
                    motor_pins = motor_handle->config->ch_config.motor_pins.hsd_motor_pins;
                    break;

                default:
                    return MOTOR_ERR_INVALID_BRIDGE_MODE;
            }

            if (!motor_pins)
            {
                err = MOTOR_ERR_INVALID_MOTOR;
            }

            else
            {
                motor_state_machine(motor_handle, motor_pins);

                if (curr) 
                {
                    *curr = motor_handle->curr;
                }

                err = motor_handle->err;
            }
        }
    }

    return err;
}


static void motor_state_machine(struct_motor_handle_t* handle, void* pins_generic)
{
    uint8_t curr_p_pin = ADC_MAX;
    uint8_t curr_n_pin = ADC_MAX;

    uint16_t ai_motor_vm_pin = ADC_MAX;

    struct_motor_curr *motor_curr_p;
    struct_motor_curr *motor_curr_n;



    switch (handle->config->ch_config.channel.bridge_mode)
    {
        case MOTOR_FULL_BRIDGE:
        {
        	struct_fb_motor_pins_t* pins = (struct_fb_motor_pins_t*)pins_generic;
            motor_curr_p = &handle->curr_m_p;
            motor_curr_n = &handle->curr_m_n;
            curr_p_pin = pins->ai_motor_positive;
            curr_n_pin = pins->ai_motor_negative;
            ai_motor_vm_pin = pins->ai_motor_vm;
            break;
        }
        case MOTOR_FULL_BRIDGE_DISCRETE:
        {
        	struct_fb_discrete_motor_pins_t* pins = (struct_fb_discrete_motor_pins_t*)pins_generic;
        	motor_curr_p = &handle->curr_m_p;
            motor_curr_n = &handle->curr_m_n;
            curr_p_pin = pins->ai_m_curr;
            curr_n_pin = pins->ai_m_curr;
        	ai_motor_vm_pin = pins->ai_motor_vm;
            break;
        }
        case MOTOR_HALF_BRIDGE_HS:
        {
        	struct_hb_motor_pins_t* pins = (struct_hb_motor_pins_t*)pins_generic;
            motor_curr_p = &handle->curr_m_p;
            motor_curr_n = NULL;
            curr_p_pin = pins->ai_motor;
            break;
        }
        case MOTOR_HALF_BRIDGE_LS:
        {
        	struct_hb_motor_pins_t* pins = (struct_hb_motor_pins_t*)pins_generic;
            motor_curr_p = NULL;
            motor_curr_n = &handle->curr_m_n;
            curr_n_pin = pins->ai_motor;
            break;
        }
        case MOTOR_HSD:
        {
        	struct_hsd_motor_pins_t* pins = (struct_hsd_motor_pins_t*)pins_generic;

            motor_curr_p = &handle->curr_m_p;
            motor_curr_n = NULL;
            curr_p_pin = pins->ai_motor_curr_ina;
            if( curr_p_pin == ADC_MAX )
            {
            	curr_p_pin = pins->ai_motor_curr_hsd;
            }
            break;
        }
        default:
            handle->err = MOTOR_ERR_INVALID_BRIDGE_MODE;
        return;
    }

    switch( handle->state )
    {
        case STATE_INIT:
        {
            motor_control_inhibit( handle, TRUE );
            motor_stop( handle );
            handle->err = MOTOR_ERR_OK;
            handle->curr = 0;
            handle->duty_out = 0;

            if( user_time_past( handle->timer, STARTUP_TIMER, PRECISION_1MS ) )
            {
                if (motor_curr_p) 
                {
                    motor_curr_p->offset = user_ai_get( curr_p_pin );
                }

                if (motor_curr_n) 
                {
                   motor_curr_n->offset = user_ai_get( curr_n_pin );
                }

                motor_control_inhibit( handle, FALSE );

                user_set_timestamp( &handle->timer, PRECISION_1MS );

                handle->state = STATE_CHECK;
            }
            else
            {
                handle->state = STATE_INIT;
            }
        }
        break;

        case STATE_CHECK:
        {
            if (handle->config->ch_config.channel.bridge_mode != MOTOR_FULL_BRIDGE)
            {
                handle->state = STATE_RUN;
            }
            else
            {
                motor_stop( handle );
                handle->err = MOTOR_ERR_OK;
                handle->curr = 0;

                if( user_time_past( handle->timer, MOTOR_CHECK_TIMER, PRECISION_1MS ) == TRUE )
                {
                    uint16_t ai_motor_vm = user_ai_get_mv( ai_motor_vm_pin );
                    if( ai_motor_vm >= MOTOR_CHECK_OK )
                    {
                        handle->state = STATE_RUN;
                    }
                    else
                    {
                        handle->err = MOTOR_ERR_CHECK_FAILED;
                        handle->state = STATE_ERR;
                    }
                }
                else
                {
                    handle->state = STATE_CHECK;
                }

                if( handle->config->direction != handle->dir_saved )
                {
                    handle->dir_saved = handle->config->direction;
                }
            }
        }
        break;

        case STATE_RUN:
        {
            if (motor_curr_p) 
            {
                motor_filter_current( curr_p_pin, &motor_curr_p->saved, &motor_curr_p->filtered, &motor_curr_p->offset, handle->config->get_curr_gain );
            }

            if (motor_curr_n) 
            {
                motor_filter_current( curr_n_pin, &motor_curr_n->saved, &motor_curr_n->filtered, &motor_curr_n->offset, handle->config->get_curr_gain );
            }

            motor_check_overcurrent( handle, handle->config->overcurrent_config.overcurrent_limit_2s, &handle->overcurrenttime2s, TWO_SECONDS_MS );
            motor_check_overcurrent( handle, handle->config->overcurrent_config.overcurrent_limit_5s, &handle->overcurrenttime5s, FIVE_SECONDS_MS );
            motor_check_overcurrent( handle, handle->config->overcurrent_config.overcurrent_limit_5m, &handle->overcurrenttime5m, FIVE_MINS_MS );

            if( ( user_timer_time_elapsed( handle->overcurrenttimereset, handle->config->overcurrent_config.overcurrent_limit_5m, PRECISION_1MS ) ) && !handle->overcurrent_flag )
            {
                user_set_timestamp( &handle->overcurrenttime5m, PRECISION_1MS );
                user_set_timestamp( &handle->overcurrenttimereset, PRECISION_1MS );
            }

            if( handle->config->pwm_duty == 0 )
            {
                if( handle->duty_out == 0 )
                {
                    user_set_timestamp( &handle->overcurrenttime2s, PRECISION_1MS );
                    user_set_timestamp( &handle->overcurrenttime5s, PRECISION_1MS );
                    user_set_timestamp( &handle->overcurrenttime5m, PRECISION_1MS );
                    user_set_timestamp( &handle->overcurrenttimereset, PRECISION_1MS );

                    motor_control_inhibit( handle, FALSE );

                    if( handle->config->motor_check == TRUE )
                    {
                        user_set_timestamp( &handle->timer, PRECISION_1MS );
                        handle->state = STATE_CHECK;
                        break;
                    }
                }
            }
            else
            {
                motor_control_inhibit( handle, TRUE );
            }

            if( handle->config->ramp_max == 0 )
            {
            	handle->duty_out = handle->config->pwm_duty;
            }
            else
            {
            	if(handle->config->direction != handle->dir_saved)
            	{
					(void) motor_ramp_up_dwn( 0, handle->config->ramp_max, handle->config->ramp_up, handle->config->ramp_dwn, &handle->cnt, &handle->duty_out );
						if( handle->duty_out == 0)
						{
						handle->dir_saved = handle->config->direction;
						}
            	}
				else
				{
				(void) motor_ramp_up_dwn( handle->config->pwm_duty, handle->config->ramp_max, handle->config->ramp_up, handle->config->ramp_dwn, &handle->cnt, &handle->duty_out );
				}
            }

            motor_run( handle );

            if( handle->dir_saved == MOTOR_DIR_FORWARD )
            {
                handle->curr = handle->curr_m_p.filtered;
            }
            else if( handle->dir_saved == MOTOR_DIR_BACKWARD )
            {
                handle->curr = handle->curr_m_n.filtered;
            }
            else if( handle->config->direction != handle->dir_saved )
            {
                handle->dir_saved = handle->config->direction;
            }
            else
            {
                handle->state = STATE_ERR;
                handle->err = MOTOR_ERR_GENERAL;
                motor_stop( handle );
                handle->curr = 0;
            }
        }
        break;

        case STATE_ERR:
        {
            user_set_timestamp( &handle->overcurrenttime2s, PRECISION_1MS );
            user_set_timestamp( &handle->overcurrenttime5s, PRECISION_1MS );
            user_set_timestamp( &handle->overcurrenttime5m, PRECISION_1MS );
            user_set_timestamp( &handle->overcurrenttimereset, PRECISION_1MS );

            motor_stop( handle );
            motor_control_inhibit( handle, FALSE );

            handle->curr = 0;
            handle->duty_out = 0;
            handle->curr_m_p.filtered = 0;
            handle->curr_m_n.filtered = 0;

            if( handle->config->direction != handle->dir_saved )
            {
                handle->dir_saved = handle->config->direction;
            }

            if( ( handle->err_ack_flg == FALSE ) && ( handle->config->pwm_duty == 0 ) )
            {
                handle->err_ack_flg = TRUE;
            }

            if( ( handle->err_ack_flg == TRUE ) && ( handle->config->pwm_duty > 0 ) )
            {
                handle->err_ack_flg = FALSE;

                if( handle->config->motor_check == TRUE )
                {
                    user_set_timestamp( &handle->timer, PRECISION_1MS );

                    handle->state = STATE_CHECK;
                }

                else
                {
                    handle->state = STATE_RUN;
                }

                handle->err = MOTOR_ERR_OK;
            }
        }
        break;

        default:
        {
            handle->state = STATE_ERR;
            handle->err = MOTOR_ERR_GENERAL;
            motor_stop( handle );
            handle->duty_out = 0;
        }
        break;
    }
}

static void motor_control_inhibit( struct_motor_handle_t* handle, uint8_t inhibit_flag )
{
    if( handle->config->ch_config.channel.bridge_mode == MOTOR_FULL_BRIDGE && handle->config->ch_config.motor_pins.fb_motor_pins != NULL )
    {
        user_io_set_do( handle->config->ch_config.motor_pins.fb_motor_pins->do_motor_inh, inhibit_flag );
        user_io_set_do( handle->config->ch_config.motor_pins.fb_motor_pins->do_motor_inh_2, inhibit_flag );
    }
    else if( handle->config->ch_config.channel.bridge_mode == MOTOR_FULL_BRIDGE_DISCRETE && handle->config->ch_config.motor_pins.fb_discrete_motor_pins != NULL)
    {
    	uint8_t di_oc_stat = user_di_get(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->di_oc_stat);
    	if( di_oc_stat == TRUE && inhibit_flag == TRUE )
    		user_io_set_do( handle->config->ch_config.motor_pins.fb_discrete_motor_pins->do_oc_reset, inhibit_flag );
    	else if ( di_oc_stat == FALSE && inhibit_flag == FALSE)
    		user_io_set_do( handle->config->ch_config.motor_pins.fb_discrete_motor_pins->do_oc_reset, inhibit_flag );
    }
    else if( handle->config->ch_config.channel.bridge_mode != MOTOR_FULL_BRIDGE && handle->config->ch_config.motor_pins.hb_motor_pins != NULL )
    {
        user_io_set_do( handle->config->ch_config.motor_pins.hb_motor_pins->do_motor_inh, inhibit_flag );
    }
}

static void motor_stop( struct_motor_handle_t* handle )
{
    switch( handle->config->ch_config.channel.bridge_mode )
    {
        case MOTOR_FULL_BRIDGE:
            if( handle->config->ch_config.motor_pins.fb_motor_pins != NULL )
            {
                user_pwm_set_freq( handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_negative, handle->config->pwm_freq );
                user_pwm_set_duty( handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_positive, PWM_DUTY_OFF );
                user_pwm_set_duty( handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_negative, PWM_DUTY_OFF );
            }
        break;
        case MOTOR_FULL_BRIDGE_DISCRETE:
        	if(handle->config->ch_config.motor_pins.fb_discrete_motor_pins != NULL)
        	{
        		user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_positive_H1, handle->config->pwm_freq, PWM_DUTY_OFF);
        		user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_positive_H2, handle->config->pwm_freq, PWM_DUTY_OFF);
        		user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_negative_L1, handle->config->pwm_freq, PWM_DUTY_OFF);
        		user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_negative_L2, handle->config->pwm_freq, PWM_DUTY_OFF);
        	}
        	break;
        case MOTOR_HALF_BRIDGE_HS:
            if( handle->config->ch_config.motor_pins.hb_motor_pins != NULL )
            {
                user_pwm_set_freq( handle->config->ch_config.motor_pins.hb_motor_pins->pwm_motor, handle->config->pwm_freq );
                user_pwm_set_duty( handle->config->ch_config.motor_pins.hb_motor_pins->pwm_motor, PWM_DUTY_OFF );
            }
        break;
        case MOTOR_HALF_BRIDGE_LS:
            if( handle->config->ch_config.motor_pins.hb_motor_pins != NULL )
            {
                user_pwm_set_freq( handle->config->ch_config.motor_pins.hb_motor_pins->pwm_motor, handle->config->pwm_freq );
                user_pwm_set_duty( handle->config->ch_config.motor_pins.hb_motor_pins->pwm_motor, PWM_DUTY_MAX_VAL );
            }
        break;
        case MOTOR_HSD:
            if( handle->config->ch_config.motor_pins.hsd_motor_pins != NULL )
            {
                user_pwm_set_freq( handle->config->ch_config.motor_pins.hsd_motor_pins->pwm_motor, handle->config->pwm_freq );
                user_pwm_set_duty( handle->config->ch_config.motor_pins.hsd_motor_pins->pwm_motor, PWM_DUTY_OFF );
            }
        break;
        default:
        break;
    }
}

static void motor_run( struct_motor_handle_t* handle )
{
    switch( handle->config->ch_config.channel.bridge_mode )
    {
        case MOTOR_FULL_BRIDGE:
            if(handle->config->ch_config.motor_pins.fb_motor_pins != NULL )
            {
                if( handle->dir_saved == MOTOR_DIR_FORWARD )
                {
                    user_pwm_set_freq(handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_positive, handle->config->pwm_freq );
                    user_pwm_set_duty(handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_positive, handle->duty_out );
                    user_pwm_set_duty(handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_negative, PWM_DUTY_OFF );
                }
                else
                {
                    user_pwm_set_freq(handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_negative, handle->config->pwm_freq );
                    user_pwm_set_duty(handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_negative, handle->duty_out );
                    user_pwm_set_duty(handle->config->ch_config.motor_pins.fb_motor_pins->pwm_motor_positive, PWM_DUTY_OFF );
                }
            }
        break;
        case MOTOR_FULL_BRIDGE_DISCRETE:
        	if( handle->config->ch_config.motor_pins.fb_discrete_motor_pins != NULL )
        	{
        		if( handle->dir_saved == MOTOR_DIR_FORWARD )
        		{
                    user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_positive_H1, handle->config->pwm_freq, PWM_DUTY_OFF);
                    user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_positive_H2, handle->config->pwm_freq, PWM_DUTY_MAX_VAL);
                    user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_negative_L1, handle->config->pwm_freq, handle->duty_out);
                    user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_negative_L2, handle->config->pwm_freq, PWM_DUTY_OFF);
        		}
        		else
        		{
                    user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_positive_H1, handle->config->pwm_freq, PWM_DUTY_MAX_VAL);
                    user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_positive_H2, handle->config->pwm_freq, PWM_DUTY_OFF);
                    user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_negative_L1, handle->config->pwm_freq, PWM_DUTY_OFF);
                    user_pwm_set_pwm(handle->config->ch_config.motor_pins.fb_discrete_motor_pins->pwm_motor_negative_L2, handle->config->pwm_freq, handle->duty_out);

        		}
        	}
        break;
        case MOTOR_HALF_BRIDGE_HS:
            if( handle->config->ch_config.motor_pins.hb_motor_pins != NULL )
            {
                uint16_t hs_pwm_duty = ( handle->dir_saved == MOTOR_DIR_FORWARD ) ? handle->duty_out : PWM_DUTY_OFF;

                user_io_set_do( handle->config->ch_config.motor_pins.hb_motor_pins->do_motor_inh, TRUE );
                user_pwm_set_freq( handle->config->ch_config.motor_pins.hb_motor_pins->pwm_motor, handle->config->pwm_freq );
                user_pwm_set_duty( handle->config->ch_config.motor_pins.hb_motor_pins->pwm_motor, hs_pwm_duty );
            }
        break;
        case MOTOR_HALF_BRIDGE_LS:
            if( handle->config->ch_config.motor_pins.hb_motor_pins != NULL )
            {
                uint16_t ls_pwm_duty = ( handle->dir_saved == MOTOR_DIR_FORWARD ? ( PWM_DUTY_MAX_VAL - handle->duty_out ) : PWM_DUTY_OFF );

                user_io_set_do( handle->config->ch_config.motor_pins.hb_motor_pins->do_motor_inh, TRUE );
                user_pwm_set_freq( handle->config->ch_config.motor_pins.hb_motor_pins->pwm_motor, handle->config->pwm_freq );
                user_pwm_set_duty( handle->config->ch_config.motor_pins.hb_motor_pins->pwm_motor, ls_pwm_duty );
            }
        break;
        case MOTOR_HSD:
            if( handle->config->ch_config.motor_pins.hsd_motor_pins != NULL )
            {
                uint16_t hsd_pwm_duty = ( handle->dir_saved == MOTOR_DIR_FORWARD ) ? handle->duty_out : PWM_DUTY_OFF;

                user_pwm_set_freq( handle->config->ch_config.motor_pins.hsd_motor_pins->pwm_motor, handle->config->pwm_freq );
                user_pwm_set_duty( handle->config->ch_config.motor_pins.hsd_motor_pins->pwm_motor, hsd_pwm_duty );
            }
        break;
        default:
        break;
    }
}

static void motor_check_overcurrent( struct_motor_handle_t *handle, uint16_t limit, uint32_t* timer, uint32_t time )
{
    uint8_t overcurrent = 0;

    overcurrent = ( ( handle->curr_m_p.filtered > limit ) || ( handle->curr_m_n.filtered > limit ) );

    handle->overcurrent_flag = overcurrent;

    if(overcurrent)
    {
    	if( user_time_past( *timer, time, PRECISION_1MS ) )
    	{
			handle->overcurrent_flag = FALSE;   
			handle->state = STATE_ERR;
			handle->err = MOTOR_ERR_OVERCURRENT;
    	}
    }
    else
    {
    	user_set_timestamp( timer, PRECISION_1MS );
    }
}

static void motor_filter_current( uint8_t ch, uint32_t* ptr_saved, uint32_t* ptr_filtered, uint32_t* ptr_ofst, uint16_t get_curr_gain )
{
        *ptr_filtered = get_curr(ch, *ptr_ofst, get_curr_gain );
}

static uint8_t motor_ramp_up_dwn( uint16_t target_duty, uint16_t max_duty, uint16_t up_speed, uint16_t down_speed, uint16_t* counter, uint16_t* current_duty )
{
    uint16_t ms_per_100;    
    uint16_t max_ramp;      

    if( target_duty > max_duty )
    {
        target_duty = max_duty;
    }
    else if( target_duty < 0 )
    {
        target_duty = 0;
    }

    ms_per_100 = target_duty > *current_duty ? up_speed : down_speed;

    if( target_duty == *current_duty )
    {
        return 0;
    }

    else if( ms_per_100 < 1 || ms_per_100 <= ext_graph_cycle_time )
    {
        *current_duty = target_duty;
    }

    else if( (uint32_t) ms_per_100 <= (uint32_t) ext_graph_cycle_time * (uint32_t) max_duty )
    {
        max_ramp = (uint16_t) ( ( (uint32_t) ext_graph_cycle_time * (uint32_t) max_duty ) / (uint32_t) ms_per_100 );

        if( target_duty >= ( *current_duty + max_ramp ) )
        {
            *current_duty += max_ramp; 
        }
        else if( ( target_duty <= ( *current_duty - max_ramp ) ) && ( *current_duty >= max_ramp ) )
        {
            *current_duty -= max_ramp; 
        }
        else
        {
            *current_duty = target_duty; 
        }
    }

    else
    {
        uint16_t cycles_per_duty_unit = ms_per_100 / max_duty;

        uint16_t total_cycles_required = cycles_per_duty_unit / ext_graph_cycle_time;

        ( *counter )++;

        if( *counter >= total_cycles_required )
        {
            if( target_duty > *current_duty )
            {
                ( *current_duty )++;
            }
            else
            {
                ( *current_duty )--;
            }

            *counter = 0; 
        }
    }

    return 1;
}

static uint32_t get_curr( uint8_t channel, uint32_t offset, uint16_t gain )
{
    volatile uint32_t val = 0;


    val = user_ai_get_filtered(channel);

	if( adc_config_tbl[channel].cal_type != CALIB_NONE )
	{
		return val;
	}
	else
	{
		if (val <= offset)
		{
			return 0;
		}
    	else
    	{
    		return ((gain * (val - offset))/100);
    	}

    }
}



