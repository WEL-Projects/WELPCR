/*
 * 5D_joystick.c
 *
 *  Created on: 22-Dec-2021
 *      Author: vivek
 */

#include "5D_joystick.h"

void joystick_init(joystick_pin_map* joystick_pins)
{


	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*Configure GPIO pins : J_UP_Pin J_DOWN_Pin J_LEFT_Pin J_RIGHT_Pin
							 J_MID_Pin J_SET_Pin J_RST_Pin */
	GPIO_InitStruct.Pin = joystick_pins->UP_key|joystick_pins->DOWN_key|joystick_pins->LEFT_key|joystick_pins->RIGHT_key
							|joystick_pins->MID_key|joystick_pins->SET_key|joystick_pins->RST_key;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(joystick_pins->GPIOx, &GPIO_InitStruct);
}

joystick_out_t read_joystick(joystick_pin_map* joystick_pins)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint16_t all_switches;
	uint8_t return_value=0;
	GPIO_InitStruct.Pin = joystick_pins->UP_key|joystick_pins->DOWN_key|joystick_pins->LEFT_key|joystick_pins->RIGHT_key
								|joystick_pins->MID_key|joystick_pins->SET_key|joystick_pins->RST_key;





	all_switches = joystick_pins->GPIOx->IDR;//HAL_GPIO_ReadPin(joystick_pins->GPIOx,GPIO_InitStruct.Pin);

	if     (all_switches & joystick_pins->UP_key)    return _UP;
	else if(all_switches & joystick_pins->DOWN_key)  return _DOWN;
	else if(all_switches & joystick_pins->LEFT_key)  return _LEFT;
	else if(all_switches & joystick_pins->RIGHT_key) return _RIGHT;
	else if(all_switches & joystick_pins->MID_key)   return _MID;
	else if(all_switches & joystick_pins->SET_key)   return _SET;
	else if(all_switches & joystick_pins->RST_key)   return _RST;
	else                                             return NONE;

	//return all_switches;

}
