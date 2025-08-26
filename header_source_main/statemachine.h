/*
 * statemachine.h
 *
 *  Created on: Aug 26, 2025
 *      Author: Enes
 */

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

struct state_machine;
typedef struct state_machine* state_machine_handle;

typedef enum{
	state_1,
	state_2,
	state_3,
	state_4,
	state_5
}state_type;

typedef struct{
	TIM_HandleTypeDef *tim_handler;
	uint8_t user_step_size;
}state_machine_configs;

state_machine_handle state_machine_init(state_machine_configs* config);

void StateMachine_Run(state_machine_handle dev);

#endif /* INC_STATEMACHINE_H_ */
