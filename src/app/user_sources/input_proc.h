#ifndef _INPUT_PROC_H_
#define _INPUT_PROC_H_

#include <stdint.h>
#include "sfl_math.h"
#include <stdbool.h>

typedef struct 
{
	int16_t temp;
	int16_t pressure;
	int16_t throttle;
}data_struct;

/** One dimensional lookup table. */
typedef struct
{
  /** Number of elements in the table.  This must be at least 2. */
  uint16_t ncols;
  /** List of input values. */
  int16_t *columns;
  /** Table data (output values).  The output values list must have the same
      length as the input list. */
  int16_t *table;
} Table1d;

void Get_input_data(data_struct *var);
void Fill_lookup_tables(void);
bool lookup1d (Table1d *t, int16_t ix, int16_t *o);





#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif
/*! @}*/

#endif
