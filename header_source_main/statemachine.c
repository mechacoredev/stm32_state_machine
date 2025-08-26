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
	GPIO_TypeDef* leds_port;
	uint16_t led1_pin;
	uint16_t led2_pin;
	uint16_t led3_pin;
	uint16_t led4_pin;
	state_type current_state;
	uint8_t clockcount;
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

	dev->current_state=state_1;
	dev->clockcount=0;
	dev->leds_port=config->leds_port;
	dev->led1_pin=config->led1_pin;
	dev->led2_pin=config->led2_pin;
	dev->led3_pin=config->led3_pin;
	dev->led4_pin=config->led4_pin;
	HAL_GPIO_WritePin(dev->leds_port, dev->led1_pin | dev->led2_pin | dev->led3_pin | dev->led4_pin, GPIO_PIN_RESET);

	return dev;
}

void StateMachine_Run(state_machine_handle dev)
{
    if (dev == NULL) return;

    dev->clockcount++;
    state_table[dev->current_state](dev);
}

static void state_1_function(state_machine_handle dev){
	if(dev->clockcount==1){
		dev->current_state=state_2;
		HAL_GPIO_WritePin(dev->leds_port, dev->led1_pin, GPIO_PIN_SET);
	}
}

static void state_2_function(state_machine_handle dev){
	if(dev->clockcount==2){
		dev->current_state=state_3;
		HAL_GPIO_WritePin(dev->leds_port, dev->led2_pin, GPIO_PIN_SET);
	}
}

static void state_3_function(state_machine_handle dev){
	if(dev->clockcount==3){
		dev->current_state=state_4;
		HAL_GPIO_WritePin(dev->leds_port, dev->led3_pin, GPIO_PIN_SET);
	}
}

static void state_4_function(state_machine_handle dev){
	if(dev->clockcount==4){
		dev->current_state=state_5;
		HAL_GPIO_WritePin(dev->leds_port, dev->led4_pin, GPIO_PIN_SET);
	}
}

static void state_5_function(state_machine_handle dev){
	if(dev->clockcount==5){
		dev->current_state=state_1;
		HAL_GPIO_WritePin(dev->leds_port, dev->led1_pin | dev->led2_pin | dev->led3_pin | dev->led4_pin, GPIO_PIN_RESET);
		dev->clockcount=0;
	}
}
