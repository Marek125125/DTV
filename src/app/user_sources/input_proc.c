#include "input_proc.h"
#include "io_tables.h"

#include "user_api_util.h"
#include "user_api_ai.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// TEMPERATURE sensor lookup table values 
//static  int16_t temp_arr_x_1d[20] = {150,200,250,300,455, 652, 833, 1000, 1154, 1296, 1429, 1552, 1667, 1774, 1875, 1970, 2059, 2143, 2222, 2500}; // mV
//static  int16_t temp_arr_y_1d[20] = {1167,1029,856,608,432, 270, 243, 171, 144, 108, 72, 36, 18, 0, -27, -36, -54, -72, -81, -126}; // Celsius * 10
static  uint16_t temp_lookup_count = 23;

/* // pressure sensor lookup table values 
static  int16_t pressure_arr_x_1d[9] = {800, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500}; // mV
static  int16_t pressure_arr_y_1d[9] = {438, 571, 907, 1234, 1570, 1897, 2233, 2560, 2601}; // mbar
static  uint16_t pressure_lookup_count = 9;

// throttle pedal position lookup table values 
static  int16_t throttle_arr_x_1d[8] = {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000}; // mV
static  int16_t throttle_arr_y_1d[8] = {1000, 936, 769, 590, 430, 250, 92, 0}; // position in % *10
static  uint16_t throttle_lookup_count = 8;
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

uint8_t err_ctr_preasure;
uint8_t last_val_preasure;
uint16_t Get_pressure()
{
    uint16_t tmp;
    uint16_t res;
    res=0;
    tmp = user_ai_get_mv(AI_A_IN0);
    if(tmp<500)
    {
        res=400;
        last_val_preasure=res;
        if(err_ctr_preasure<200)
        {
          err_ctr_preasure++;  
        }
      
    }
    else if (tmp>4500)
    {
        res=2600;
        last_val_preasure=res;
        if(err_ctr_preasure<200)
        {
          err_ctr_preasure++;  
        }
    }
    else if(err_ctr_preasure<100){
        res=tmp/1.65;
        if(err_ctr_preasure>0)
        {
           err_ctr_preasure--; 
        }
        
    }
    if(err_ctr_preasure>100){
        res=last_val_preasure;
        //call var to save error
    }
  return (uint16_t)(res);
}
uint8_t err_ctr_throttle;
uint8_t last_val_pedal;
uint8_t Get_throttle()
{
    uint16_t tmp;
    uint16_t res;
    res=0;
    tmp = user_ai_get_mv(AI_A_IN1);
    if(tmp<500)
    {
        res=100;
        last_val_pedal=res;
        if(err_ctr_throttle<200)
        {
          err_ctr_throttle++;  
        }
    }
    else if (tmp>4000)
    {
        res=0;
        last_val_pedal=res;
        if(err_ctr_throttle<200)
        {
          err_ctr_throttle++;  
        }
    }
    else if(err_ctr_throttle<100){
        res=100-((tmp*25)/1000);
        if(err_ctr_throttle>0)
        {err_ctr_throttle--;}
    }
    if(err_ctr_throttle>100){
        res=last_val_pedal;
        //call var to save error
    }
    
  return (uint8_t)res;
} 

LookupEntry table[] = {
        {150, 1167},
        {200, 1029},
        {250, 856},
        {300, 608},
        {455, 432},
        {652, 270},
        {833, 243},
        {1000, 171},
        {1154, 144},
        {1296, 108},
        {1429, 72},
        {1552, 36},
        {1667, 18},
        {1774, 0},
        {1875, -27},
        {1970, -36},
        {2059, -54},
        {2143, -72},
        {2222, -81},
        {2500, -126},
        {2700, -166},
        {3000, -218},
        {3200, -326},
    };
int8_t err_ctr_temp;
int8_t last_val_temp;
static int8_t last_val_temp_check;

int8_t Get_temp()
{
  uint16_t tmp = user_ai_get_mv(AI_A_IN2);
  int8_t res;
  if (tmp>3500)
    {
        res=-40;
        last_val_temp=res;
        
        if(err_ctr_temp<200)
        {
          err_ctr_temp++;  
        }
    }
    else if(err_ctr_temp<100){
        res=(int8_t)lookup1D(tmp, table, temp_lookup_count)/10;
        last_val_temp=res;
        
        if(err_ctr_temp>0)
        {err_ctr_temp--;}
    }
    if(err_ctr_temp>100){
        res=last_val_temp;
        //call var to save error
    }
    last_val_temp_check=res;
    int8_t diff;
    diff=last_val_temp_check-res;
    if(diff<2||diff>-2)
    {
       res=last_val_temp; 
       //call var to save error
    }
  return res;
    //int16_t temp;
    //temp=user_util_get_lookup_value_1D_16(temp_arr_x_1d, temp_arr_y_1d, temp_lookup_count, input, LUT_MODE_EXTRAPOLATION);
    //return (int8_t)temp;
}