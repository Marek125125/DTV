#ifndef _INPUT_PROC_H_
#define _INPUT_PROC_H_

#include <stdint.h>
#include "sfl_math.h"
#include <stdbool.h>

typedef struct {
    int16_t key;
    int16_t value;
} LookupEntry;


//void Get_input_data(data_struct *var);
//void Fill_lookup_tables(void);
int16_t lookup1D(int16_t input, LookupEntry *table, int tableSize);



#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif
/*! @}*/

#endif
