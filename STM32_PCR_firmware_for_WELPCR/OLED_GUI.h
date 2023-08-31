/*
 * OLED_GUI_declearations.h
 *
 *  Created on: Dec 20, 2021
 *      Author: vivek
 */

#ifndef INC_OLED_GUI_H_
#define INC_OLED_GUI_H_

#include "ssd1306.h"
#include "fonts.h"

typedef enum {
	SELECTED = 0x00, /*!< selected */
	HOVER    = 0x01,  /*!< hover */
	NO_HOVER = 0x02
} para_state_t;


typedef enum {
	FLOAT = 0x00, /*!< selected */
	INT = 0x01,  /*!< hover */
} para_type_t;

typedef struct
{
	uint8_t ID;
	float value;
	uint8_t x_cord;
	uint8_t ycord;
	SSD1306_COLOR_t background;
	SSD1306_COLOR_t foreground;
	para_state_t state;
	para_type_t type;
	struct  parameter* left;
	struct  parameter* right;
	struct  parameter* up;
	struct  parameter* down;

}parameter;

typedef struct
{
	struct  parameter* current_selection;
	para_state_t state;


}joystick_pointer;




//void OLED_GUI_init(void);

#endif /* INC_OLED_GUI_H_ */
