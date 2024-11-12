

#include "Cpu.h"
#include "dmaControllerCom1.h"
#include "flexTimerCom1.h"
#include "core_cm4.h"

#include "hal_lin.h"
#include "hal_sci.h"
#include "hal_can.h"
#include "hal_io.h"
#include "hal_pwm.h"
#include "hal_nvm.h"
#include "hal_tick.h"
#include "hal_watchdog.h"

#include "user_code.h"
#include "graph_code.h"
#include "adc_app.h"
#include "lin_app.h"
#include "sci_app.h"
#include "can_app.h"

#include "sfl_can_db_tables_data.h"
#include "sfl_can_db.h"
#include "sfl_bl_protocol.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "watchdog1.h"
#include "modulhardwarecode.h"
#include "irq_priority.h"


void supporting_driver_init(void);

int main(void)
{
	uint8_t multiplex_group = 0u;
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

	hal_io_init(NUM_OF_CONFIGURED_PINS);

	(void)sfl_bl_protocol_s32k_init();

	supporting_driver_init();

	hal_tick_init();
	uint32_t current_sysTick_1ms = 0; 
	uint32_t cycle_timestamp = 0;       

	hal_nvm_init();

	ADC_init();

	hal_pwm_init(PWM_MAX);

	hal_freq_init(FREQ_MAX);

	irq_priority_set_all_predefined();

	lin_init_all();

	modulhardwarecode_init();

#ifdef SET_CALLBACK_1MS_TIMER
	set_callback_timer_1ms(user_int_timer_1ms);
#endif

#ifdef SET_CALLBACK_CAN_MESSAGE_RECEIVE
	set_callback_can_msg_receive(user_can_receive_msg_callback);
#endif

	adc_wait_first_measurements(&multiplex_group, HW_CALIBRATION_SUPPORT);

	usercode_init();

	graphcode_init();

	while(1)
	{
		hal_watchdog_signal(WDT_SIGNAL_MAIN_LOOP_BIT | WATCHDOG_KICK_REQUEST);

		hal_get_timestamp(&current_sysTick_1ms, HAL_PRECISION_1MS);

		(void)sfl_bl_protocol_s32k_cyclic();

		adc_processing(&multiplex_group, HW_CALIBRATION_SUPPORT);


		sfl_can_queue_in_process();

		lin_cyclic();

		modulhardwarecode_cyclic();

		if ( (current_sysTick_1ms - cycle_timestamp) >= ext_graph_cycle_time)
		{
			cycle_timestamp = current_sysTick_1ms;

			usercode();

			graphcode();
		}


		sfl_can_db_output_to_bus();
	}
}

void supporting_driver_init(void)
{
	EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);
}


