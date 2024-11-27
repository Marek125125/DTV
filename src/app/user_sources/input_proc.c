#include "input_proc.h"
#include "io_tables.h"

#include "user_api_util.h"
#include "user_api_ai.h"

// TEMPERATURE sensor lookup table values 
int16_t temp_arr_x_1d[16] = {455, 652, 833, 1000, 1154, 1296, 1429, 1552, 1667, 1774, 1875, 1970, 2059, 2143, 2222, 2500}; // mV
int16_t temp_arr_y_1d[16] = {432, 270, 243, 171, 144, 108, 72, 36, 18, 0, -27, -36, -54, -72, -81, -126}; // Celsius * 10
uint16_t temp_lookup_count = 16;

// pressure sensor lookup table values 
int16_t pressure_arr_x_1d[9] = {800, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500}; // mV
int16_t pressure_arr_y_1d[9] = {438, 571, 907, 1234, 1570, 1897, 2233, 2560, 2601}; // mbar
uint16_t pressure_lookup_count = 9;

// throttle pedal position lookup table values 
int16_t throttle_arr_x_1d[8] = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000}; // mV
int16_t throttle_arr_y_1d[8] = {1000, 936, 769, 590, 430, 250, 92, 0}; // position in % *10
uint16_t throttle_lookup_count = 8;



void Get_input_data(data_struct *var)
{
    uint16_t temp =0;
    temp= user_ai_get_mv(AI_A_IN0);
    var->temp = user_util_get_lookup_value_1D_16(temp_arr_x_1d, temp_arr_y_1d, temp_lookup_count, (int16_t)temp, LUT_MODE_EXTRAPOLATION); 
    temp= user_ai_get_mv(AI_A_IN1);
    var->pressure = user_util_get_lookup_value_1D_16(pressure_arr_x_1d, pressure_arr_y_1d, pressure_lookup_count,  (int16_t)temp, LUT_MODE_EXTRAPOLATION);
    temp= user_ai_get_mv(AI_A_IN2);
    var->throttle = user_util_get_lookup_value_1D_16(throttle_arr_x_1d, throttle_arr_y_1d, throttle_lookup_count,  (int16_t)temp, LUT_MODE_EXTRAPOLATION);
}


