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

//1D Table pro teplotu
Table1d *temp_table;

void Fill_lookup_tables(void)
{
    //lookup table struktura pro temp
    *temp_table->ncols = temp_lookup_count;
    *temp_table->columns = &temp_arr_x_1d[];
    *temp_table->table = &temp_arr_y_1d[];

    //lookup pro pressure...
    //lookup pro pedal..
    return 0;
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

/**
 * 1-D table lookup.
 *
 * This function performs a 1-D table lookup with interpolation.  The output
 * value is clamped to either of the table end values when the input value is
 * out of bounds.
 *
 * @param[in]   t      table data structure
 * @param[in]   ix     input (X-axis) value
 * @param[out]  o      output data
 *
 * @retval      true   if the lookup result is suspect due to clipping
 * @retval      false  on successful lookup
 */


bool lookup1d (Table1d *t, int16_t ix, int16_t *o)
{
  uint16_t i;
  
  /* ------------------------------------------------------------------------ */
  /* Off the end of the table */
  if (ix > t->columns[t->ncols - 1])
  {
    *o = t->table[t->ncols - 1];
    return true;
  }
  
  /* Off beginning of the table */
  else if (ix < t->columns[0])
  {
    *o = t->table[0];
    return true;
  }

  /* Within the bounds of the table */
  for (i = 0; i < t->ncols - 1; ++i)
  {
    if (   ix >= t->columns[i]
        && ix <= t->columns[i + 1])
    {
      /* Output (table) low value */
      int o_low   = t->table[i];
      /* Input (X-axis) low value */
      int i_low   = t->columns[i];
      /* Spead between the two adjacent input values */
      int i_delta = t->columns[i + 1] - t->columns[i];
      /* Spread between the two adjacent table output values */
      int o_delta = t->table[i + 1]   - t->table[i];
      
      /* Prevent division by zero.  We could get here if two consecutive
         input values in the table are the same. */
      if (o_delta == 0)
      {
        *o = o_low;
        return true;
      }
      
      *o = o_low + ((ix - i_low) * (long)o_delta) / i_delta;
      return false;
    }
  }

  /* Didn't find it (we shouldn't ever get here). */
  return true;
}


void Get_input_data(data_struct *var)
{
    int16_t  buffer = 0;
    buffer = user_ai_get_mv(AI_A_IN0);

    //SDK lookup table :
    //var->temp = user_util_get_lookup_value_1D_16(temp_arr_x_1d, temp_arr_y_1d, temp_lookup_count, (int16_t)temp, LUT_MODE_EXTRAPOLATION); 

    //new 1D lookup funkce:
    lookup1d(temp_table, buffer, &var->temp);
}
