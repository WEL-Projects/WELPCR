/*
 * thermocycler.h
 *
 *  Created on: Dec 27, 2021
 *      Author: vivek
 */

#ifndef INC_THERMOCYCLER_H_
#define INC_THERMOCYCLER_H_



#define SEC_TO_MILLISEC(sec)	(sec * 1000)

#include "stm32f1xx_hal.h"



typedef enum {
		IDEL_state                 = (uint8_t) 0x00,
		Initial_denaturation_state = (uint8_t) 0x01,
		cycling_state              = (uint8_t) 0x02,
		final_extension_state      = (uint8_t) 0x03,
		cycles_completed           = (uint8_t) 0x04
	} thermocycler_state_t;

typedef enum {
		denaturation_state         = (uint8_t) 0x00,
		annealing_state            = (uint8_t) 0x01,
		extension_state            = (uint8_t) 0x02
	} cycle_state_t;


uint16_t Total_time_require;
uint32_t Test_start_time;


void set_themocycler_state(thermocycler_state_t state, cycle_state_t cycle);
float get_block_set_point(void);
void start_thermocycler(void);
void stop_thermocycler(void);
thermocycler_state_t get_current_thermocycler_state(void);

#endif /* INC_THERMOCYCLER_H_ */
