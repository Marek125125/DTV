#include "input_proc.h"
#include "io_tables.h"

#include "user_api_util.h"
#include "user_api_ai.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// TEMPERATURE sensor lookup table values 
static  int16_t temp_arr_x_1d[16] = {455, 652, 833, 1000, 1154, 1296, 1429, 1552, 1667, 1774, 1875, 1970, 2059, 2143, 2222, 2500}; // mV
static  int16_t temp_arr_y_1d[16] = {432, 270, 243, 171, 144, 108, 72, 36, 18, 0, -27, -36, -54, -72, -81, -126}; // Celsius * 10
static  uint16_t temp_lookup_count = 16;

// pressure sensor lookup table values 
static  int16_t pressure_arr_x_1d[9] = {800, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500}; // mV
static  int16_t pressure_arr_y_1d[9] = {438, 571, 907, 1234, 1570, 1897, 2233, 2560, 2601}; // mbar
static  uint16_t pressure_lookup_count = 9;

// throttle pedal position lookup table values 
static  int16_t throttle_arr_x_1d[8] = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000}; // mV
static  int16_t throttle_arr_y_1d[8] = {1000, 936, 769, 590, 430, 250, 92, 0}; // position in % *10
static  uint16_t throttle_lookup_count = 8;


/*void Fill_lookup_tables(void)
{
    //lookup table struktura pro temp
    
//temp_table.ncols = temp_lookup_count;
//temp_table.columns = temp_arr_x_1d;
//temp_table.table = temp_arr_y_1d;

// preassure_table.ncols = pressure_lookup_count;
// preassure_table.columns = pressure_arr_x_1d;
// preassure_table.table = pressure_arr_y_1d;

throttle_table.ncols = throttle_lookup_count;
throttle_table.columns = throttle_arr_x_1d;
throttle_table.table = throttle_arr_y_1d;
    //lookup pro pressure...
    //lookup pro pedal..
}
//stare volani interpolacni funkce z SDK -- nefungovalo
/* void Get_input_data(data_struct *var)
{
    uint16_t temp =0;
    temp= user_ai_get_mv(AI_A_IN0);
    var->temp = user_util_get_lookup_value_1D_16(temp_arr_x_1d, temp_arr_y_1d, temp_lookup_count, (int16_t)temp, LUT_MODE_EXTRAPOLATION); 
    temp= user_ai_get_mv(AI_A_IN1);
    var->pressure = user_util_get_lookup_value_1D_16(pressure_arr_x_1d, pressure_arr_y_1d, pressure_lookup_count,  (int16_t)temp, LUT_MODE_EXTRAPOLATION);
    temp= user_ai_get_mv(AI_A_IN2);
    var->throttle = user_util_get_lookup_value_1D_16(throttle_arr_x_1d, throttle_arr_y_1d, throttle_lookup_count,  (int16_t)temp, LUT_MODE_EXTRAPOLATION);
} */
*/


int16_t lookup1D(int16_t input, LookupEntry *table, int tableSize) {
    if (tableSize <= 0) {
        fprintf(stderr, "Error: Table size must be greater than zero.\n");
        return 0;
    }

    // Hledání nejbližší hodnoty
    int16_t closestValue = table[0].value; // Výchozí hodnota
    int16_t closestDistance = abs(input - table[0].key); // Výchozí vzdálenost

    for (int i = 1; i < tableSize; i++) {
        int16_t distance = abs(input - table[i].key);
        if (distance < closestDistance) {
            closestDistance = distance;
            closestValue = table[i].value;
        }
    }

    return closestValue;
}


/*void Get_input_data(data_struct *var)
{

    // buffer = user_ai_get_mv(AI_A_IN0);

    // //SDK lookup table :
    // //var->temp = user_util_get_lookup_value_1D_16(temp_arr_x_1d, temp_arr_y_1d, temp_lookup_count, (int16_t)temp, LUT_MODE_EXTRAPOLATION); 

    // //new 1D lookup funkce:
    // lookup1d(&temp_table, buffer, &var->temp);

    // buffer = user_ai_get_mv(AI_A_IN1);
    // lookup1d(&preassure_table, buffer, &var->pressure);

    //var->= user_ai_get_mv(AI_A_IN0);
    
    
    //lookup1d(&throttle_table, buffer, &var->throttle);
}
*/

int16_t Get_pressure()
{
  return (user_ai_get_mv(AI_A_IN0) / 2);
}

int16_t Get_throttle()
{
  return (user_ai_get_mv(AI_A_IN1) / 100);
}

 LookupEntry table[] = {
        {455, 432},
        {652, 270},
        {833, 243},
        {1000, 171}
        {1154, 144}
        {1296, 108}
        {1429, 72}
        {1552, 36}
        {1667, 18}
        {1774, 0}
        {1875, -27}
        {1970, -36}
        {2059, -54}
        {2143, -72}
        {2222, -81}
        {2500, -126}
    };

int16_t Get_temp()
{
  int16_t input = user_ai_get_mv(AI_A_IN2);
    return lookup1D(input, table, tableSize);
}