/*
 * 5D_joystick.h
 *
 *  Created on: 22-Dec-2021
 *      Author: vivek
 */

#ifndef INC_5D_JOYSTICK_H_
#define INC_5D_JOYSTICK_H_

#include "main.h"

typedef struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t     UP_key;
	uint16_t     DOWN_key;
	uint16_t     RIGHT_key;
	uint16_t     LEFT_key;
	uint16_t     MID_key;
	uint16_t     SET_key;
	uint16_t     RST_key;
}joystick_pin_map;


typedef enum {
	NONE  =  0x00,
	_LEFT  =  0x01,
	_RIGHT =  0x02,
	_UP    =  0x03,
	_DOWN  =  0x04,
	_MID   =  0x05,
	_SET   = 0x06,
	_RST   =  0x07


} joystick_out_t;


#endif /* INC_5D_JOYSTICK_H_ */
