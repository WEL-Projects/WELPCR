/*
 * OLED_GUI_declearations.h
 *
 *  Created on: Dec 20, 2021
 *      Author: vivek
 */

#ifndef INC_OLED_GUI_H_
#define INC_OLED_GUI_H_

//#include "ssd1306.h"
//#include "fonts.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"


#define JOYSTICK_DELAY 			100
#define LONG_PRESS_TIME         500//2000



typedef enum {
	PCR_GRAPH_menu = 0x01,   //use only one bit 0000 0001
	DISPLAY_PAR_menu = 0x02, //use only one bit 0000 0010
} pcr_menu_t;

typedef enum {
	SELECTED = 0x00, /*!< selected */
	HOVER    = 0x01,  /*!< hover */
	NO_HOVER = 0x02
} para_state_t;


typedef enum {
	FLOAT = 0x00, /*!< selected */
	INT = 0x01,  /*!< hover */
} para_type_t;

 typedef struct parameter
{
	uint8_t ID;
	volatile float value;
	uint16_t x_cord;
	uint16_t ycord;
	uint16_t background;
	uint16_t foreground;
	para_state_t state;
	para_type_t type;
	pcr_menu_t menu_name;
	struct  parameter* left;
	struct  parameter* right;
	struct  parameter* up;
	struct  parameter* down;

}parameter;

typedef struct PCR_settings_t
{
	struct  parameter* denaturation_temp;
	struct  parameter* annealing_temp;
	struct  parameter* extension_temp;
	struct  parameter* initial_denaturation_time;
	struct  parameter* denaturation_time;
	struct  parameter* annealing_time;
	struct  parameter* extension_time;
	struct  parameter* final_extension_time;
	struct  parameter* total_cycles_OLED;

}PCR_settings_t;





typedef enum {
	BLOCK_TEAMPERATURE = 0x00,
	RAMP_RATE          = 0x01,
	CURRENT_CYCLE      = 0X02,
	SET_POINT          = 0X03,
	LID_TEMP           = 0x04,
	TEMP_1             = 0x05,
	TEMP_2             = 0x06,
	TIME_REMAINING     = 0x07
} parameter_name_t;



typedef struct joystick_pointer
{
	parameter* selection;
	para_state_t state;


}joystick_pointer;





typedef struct menu_t
{
	pcr_menu_t menu_name;
	struct menu_t*   next_menu
}menu_t;



typedef struct gui_label_t
{
	char value[20];
	uint16_t x_cord;
	uint16_t ycord;
	uint8_t* font;
	uint16_t background;
	uint16_t foreground;
}gui_label_t;



void display_parameter(parameter* par);
void update_GUI(void);
void OLED_GUI_init(void);
void update_parameter_value(parameter_name_t parameter_name, float value);
void get_PCR_settings(void* PCR_settings);
void display_big_message(char* message1,char* message2,uint8_t* Font, uint8_t time_s);

#endif /* INC_OLED_GUI_H_ */
