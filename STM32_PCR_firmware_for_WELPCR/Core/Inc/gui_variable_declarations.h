/*
 * gui_variable_declarations.h
 *
 *  Created on: Dec 25, 2021
 *      Author: vivek
 */

#ifndef INC_GUI_VARIABLE_DECLARATIONS_H_
#define INC_GUI_VARIABLE_DECLARATIONS_H_

parameter block_temp_OLED, total_cycles_OLED, final_extension_time,denaturation_temp,extension_temp,extension_time;
parameter denaturation_time,annealing_time,initial_denaturation_time;
joystick_pin_map joystick_pins;

//pcr_menu_t current_menu = PCR_GRAPH_menu;


menu_t PCR_graph_menu;

menu_t  parameter_display_menu =
{
	.menu_name = DISPLAY_PAR_menu,
	.next_menu = &PCR_graph_menu
};


menu_t PCR_graph_menu =
{
	.menu_name = PCR_GRAPH_menu,
	.next_menu = &parameter_display_menu
};


menu_t* current_menu = &PCR_graph_menu;


parameter block_temp_OLED =
{
	.ID = 0,
	.value = 0,
	.x_cord = 55,
	.ycord = 0,
	.background= BLACK   ,
	.foreground= LIGHTGREY,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = PCR_GRAPH_menu | DISPLAY_PAR_menu,
	.left = &total_cycles_OLED,
	.right= &total_cycles_OLED,
	.up =   &denaturation_time,
	.down=  &denaturation_temp,
};

parameter total_cycles_OLED =
{
		.ID = 1,
		.value = 3,
		.x_cord = 255,
		.ycord = 0,
		.background= BLACK  ,
		.foreground= WHITE ,
		.state = NO_HOVER,
		.type  = INT,
	    .menu_name = PCR_GRAPH_menu | DISPLAY_PAR_menu,
		.left = &denaturation_temp,
		.right= &denaturation_temp,
		.up = &extension_time,
	    .down= &extension_temp,
};

parameter current_cycle_OLED =
{
	.ID = 1,
	.value = 0,
	.x_cord = 225,
	.ycord = 0,
	.background= BLACK  ,
	.foreground= LIGHTGREY ,
	.state = NO_HOVER,
	.type  = INT,
    .menu_name = PCR_GRAPH_menu | DISPLAY_PAR_menu,
	.left = NULL,
	.right= NULL,
	.up = NULL,
    .down= NULL,
};


parameter denaturation_temp =
{
	.ID = 3,
	.value = 94.5,
	.x_cord = 13,
	.ycord = 53,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = PCR_GRAPH_menu,
	.left = &extension_temp,
	.right= &extension_temp,
	.up = &block_temp_OLED,
    .down= &initial_denaturation_time,
};

parameter annealing_temp =
{
	.ID = 4,
	.value = 52.1,
	.x_cord = 115,
	.ycord = 146,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = PCR_GRAPH_menu,
	.left = &initial_denaturation_time,
	.right= &final_extension_time,
	.up = &total_cycles_OLED,
    .down= &annealing_time,
};

parameter extension_temp =
{
	.ID = 5,
	.value = 71.75,
	.x_cord = 220,
	.ycord = 75,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = PCR_GRAPH_menu,
	.left = &denaturation_temp,
	.right= &denaturation_temp,
	.up = &total_cycles_OLED,
    .down= &final_extension_time,
};


parameter initial_denaturation_time =
{
	.ID = 6,
	.value = 30,//183,//120
	.x_cord = 13,
	.ycord = 150,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = INT,
    .menu_name = PCR_GRAPH_menu,
	.left = &final_extension_time,
	.right= &annealing_temp,
	.up = &denaturation_temp,
    .down= &denaturation_time,
};

parameter denaturation_time =
{
	.ID = 7,
	.value = 60,
	.x_cord = 13,
	.ycord = 199,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = INT,
    .menu_name = PCR_GRAPH_menu,
	.left = &extension_time,
	.right= &annealing_time,
	.up = &initial_denaturation_time,
    .down= &denaturation_temp,
};

parameter annealing_time =
{
	.ID = 7,
	.value = 61,
	.x_cord = 115,
	.ycord = 199,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = INT,
    .menu_name = PCR_GRAPH_menu,
	.left = &denaturation_time,
	.right= &extension_time,
	.up = &annealing_temp,
    .down= &total_cycles_OLED,
};

parameter extension_time =
{
	.ID = 8,
	.value = 50,
	.x_cord = 220,
	.ycord = 199,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = INT,
    .menu_name = PCR_GRAPH_menu,
	.left = &annealing_time,
	.right= &denaturation_time,
	.up = &final_extension_time,
    .down= &total_cycles_OLED,
};

parameter final_extension_time =
{
	.ID = 2,
	.value = 600,
	.x_cord = 220,
	.ycord = 150,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = INT,
    .menu_name = PCR_GRAPH_menu,
	.left = &annealing_temp,
	.right= &block_temp_OLED,
	.up = &extension_temp,
    .down= &extension_time,
};

parameter ramp_rate_P =
{
	.ID = 2,
	.value = 0,
	.x_cord = 100,
	.ycord = 45,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = DISPLAY_PAR_menu,
	.left = &annealing_temp,
	.right= &block_temp_OLED,
	.up = &extension_temp,
    .down= &extension_time,
};

parameter lid_temp_P =
{
	.ID = 2,
	.value = 0,
	.x_cord = 83,
	.ycord = 90,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = DISPLAY_PAR_menu,
	.left = &annealing_temp,
	.right= &block_temp_OLED,
	.up = &extension_temp,
    .down= &extension_time,
};

parameter temp_1_P =
{
	.ID = 2,
	.value = 0,
	.x_cord = 83,
	.ycord = 135,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = DISPLAY_PAR_menu,
	.left = &annealing_temp,
	.right= &block_temp_OLED,
	.up = &extension_temp,
    .down= &extension_time,
};

parameter temp_2_P =
{
	.ID = 2,
	.value = 0,
	.x_cord = 83,
	.ycord = 180,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = DISPLAY_PAR_menu,
	.left = &annealing_temp,
	.right= &block_temp_OLED,
	.up = &extension_temp,
    .down= &extension_time,
};

parameter progress_P =
{
	.ID = 2,
	.value = 0,
	.x_cord = 270,
	.ycord = 135,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = DISPLAY_PAR_menu,
	.left = &annealing_temp,
	.right= &block_temp_OLED,
	.up = &extension_temp,
    .down= &extension_time,
};

parameter time_remaining_P =
{
	.ID = 2,
	.value = 0,
	.x_cord = 190,
	.ycord = 200,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = DISPLAY_PAR_menu,
	.left = &annealing_temp,
	.right= &block_temp_OLED,
	.up = &extension_temp,
    .down= &extension_time,
};





parameter set_point_P =
{
	.ID = 2,
	.value = 25,
	.x_cord = 225,
	.ycord = 45,
	.background=  BLACK,
	.foreground= WHITE,
	.state = NO_HOVER,
	.type  = FLOAT,
    .menu_name = DISPLAY_PAR_menu,
	.left = &annealing_temp,
	.right= &block_temp_OLED,
	.up = &extension_temp,
    .down= &extension_time,
};

PCR_settings_t PCR_settings_in_GUI =
	{
		.denaturation_temp 		=	&denaturation_temp,
		.annealing_temp    		=	&annealing_temp,
		.extension_temp    		=	&extension_temp,
		.initial_denaturation_time	=	&initial_denaturation_time,
		.denaturation_time			=	&denaturation_time,
		.annealing_time			=	&annealing_time,
		.extension_time			=	&extension_time,
		.final_extension_time	=	&final_extension_time,
		.total_cycles_OLED		=   &total_cycles_OLED
	};

joystick_pointer joystick_current_position =
{
		.selection= &total_cycles_OLED,
		.state = HOVER,
};



gui_label_t   Cycle_L =
{
		.value = "Cy:",
		.x_cord = 175,
		.ycord = 0,
		.font = &Arial_Narrow15x19,
		.background = BLACK ,
		.foreground =WHITE
};

gui_label_t Temp_L =
{
		.value = "Temp:",
		.x_cord = 0,
		.ycord = 0,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};

gui_label_t Temp_1_L =
{
		.value = "Temp:",
		.x_cord = 0,
		.ycord = 0,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};
gui_label_t Ramp_rate_L =
{
		.value = "Ramp rate:",
		.x_cord = 0,
		.ycord = 45,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};


gui_label_t Set_point_L =
{
		.value = "SP:",
		.x_cord = 175,
		.ycord = 45,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};


gui_label_t Lid_temp_L =
{
		.value = "Lid:",
		.x_cord = 0,
		.ycord = 90,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};

gui_label_t extra_1_temp_L =
{
		.value = "Temp 1:",
		.x_cord = 0,
		.ycord = 135,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};


gui_label_t progress_L =
{
		.value = "%Progress:",
		.x_cord = 175,
		.ycord = 135,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};

gui_label_t extra_2_temp_L =
{
		.value = "Temp 2:",
		.x_cord = 0,
		.ycord = 180,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};

gui_label_t Time_remaining_L =
{
		.value = "Time remaining:",
		.x_cord = 175,
		.ycord = 180,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};
gui_label_t minutes_L =
{
		.value = "minutes",
		.x_cord = 240,
		.ycord = 200,
		.font = &Arial_Narrow15x19,
		.background = BLACK,
		.foreground = WHITE
};


#endif /* INC_GUI_VARIABLE_DECLARATIONS_H_ */
