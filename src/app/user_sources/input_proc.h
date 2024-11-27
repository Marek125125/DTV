#ifndef _INPUT_PROC_H_
#define _INPUT_PROC_H_

#include <stdint.h>
#include "sfl_math.h"

typedef struct 
{
	int temp;
	int pressure;
	int throttle;
}data_struct;

void Get_input_data(data_struct *var);

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif
/*! @}*/

#endif
