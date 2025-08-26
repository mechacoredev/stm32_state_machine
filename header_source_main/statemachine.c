/*
 * statemachine.c
 *
 *  Created on: Aug 26, 2025
 *      Author: Enes
 */

#include "statemachine.h"
#include "stdlib.h"

static void state_1_function(state_machine_handle dev);
static void state_2_function(state_machine_handle dev);
static void state_3_function(state_machine_handle dev);
static void state_4_function(state_machine_handle dev);
static void state_5_function(state_machine_handle dev);


struct state_machine{
	TIM_HandleTypeDef *tim_handler;
	uint32_t Channel1;
	uint32_t Channel2;
	uint32_t Channel3;
	uint32_t Channel4;
	state_type current_state;
	uint16_t step_size_compare;
	uint16_t max_compare_value;
	uint16_t current_pwm_compare;
};

static void (*state_table[])(state_machine_handle dev) = {
    state_1_function,
    state_2_function,
    state_3_function,
    state_4_function,
    state_5_function
};

state_machine_handle state_machine_init(state_machine_configs* config){
	state_machine_handle dev = (state_machine_handle)malloc(sizeof(struct state_machine));
	if(dev==NULL) return NULL;

	dev->tim_handler=config->tim_handler;
	dev->Channel1=TIM_CHANNEL_1;
	dev->Channel2=TIM_CHANNEL_2;
	dev->Channel3=TIM_CHANNEL_3;
	dev->Channel4=TIM_CHANNEL_4;
	dev->max_compare_value=__HAL_TIM_GET_AUTORELOAD(dev->tim_handler);
	dev->step_size_compare=(config->user_step_size*(dev->max_compare_value+1))/100;
	dev->current_state=state_1;
	dev->current_pwm_compare=0;

    HAL_TIM_PWM_Start(dev->tim_handler, dev->Channel1);
    HAL_TIM_PWM_Start(dev->tim_handler, dev->Channel2);
    HAL_TIM_PWM_Start(dev->tim_handler, dev->Channel3);
    HAL_TIM_PWM_Start(dev->tim_handler, dev->Channel4);

    __HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel1, 0);
    __HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel2, 0);
    __HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel3, 0);
    __HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel4, 0);

	return dev;
}

void StateMachine_Run(state_machine_handle dev)
{
    if (dev == NULL) return;

    state_table[dev->current_state](dev);
}

static void state_1_function(state_machine_handle dev){
	dev->current_pwm_compare+=dev->step_size_compare;
	__HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel1, dev->current_pwm_compare);
	if(dev->current_pwm_compare+dev->step_size_compare>dev->max_compare_value){
		dev->current_pwm_compare=0;
		dev->current_state=state_2;
	}
}

static void state_2_function(state_machine_handle dev){
	dev->current_pwm_compare+=dev->step_size_compare;
	__HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel2, dev->current_pwm_compare);
	if(dev->current_pwm_compare+dev->step_size_compare>dev->max_compare_value){
		dev->current_pwm_compare=0;
		dev->current_state=state_3;
	}
}

static void state_3_function(state_machine_handle dev){
	dev->current_pwm_compare+=dev->step_size_compare;
	__HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel3, dev->current_pwm_compare);
	if(dev->current_pwm_compare+dev->step_size_compare>dev->max_compare_value){
		dev->current_pwm_compare=0;
		dev->current_state=state_4;
	}
}

static void state_4_function(state_machine_handle dev){
	dev->current_pwm_compare+=dev->step_size_compare;
	__HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel4, dev->current_pwm_compare);
	if(dev->current_pwm_compare+dev->step_size_compare>dev->max_compare_value){
		dev->current_state=state_5;
	}
}

static void state_5_function(state_machine_handle dev){
	if((int32_t)dev->current_pwm_compare - (int32_t)dev->step_size_compare < 0)
	{
		dev->current_pwm_compare=0;
	}
	else{
		dev->current_pwm_compare-=dev->step_size_compare;
	}
	__HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel1, dev->current_pwm_compare);
	__HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel2, dev->current_pwm_compare);
	__HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel3, dev->current_pwm_compare);
	__HAL_TIM_SET_COMPARE(dev->tim_handler, dev->Channel4, dev->current_pwm_compare);
	if(dev->current_pwm_compare==0)
	{
		dev->current_state=state_1;
	}
}
