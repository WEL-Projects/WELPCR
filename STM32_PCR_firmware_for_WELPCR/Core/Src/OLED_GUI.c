/*
 * OLED_GUI.c
 *
 *  Created on: Dec 20, 2021
 *      Author: vivek
 */
#include "OLED_GUI.h"
//#include "ssd1306.h"
#include "fonts.h"
#include "5D_joystick.h"
#include "gui_variable_declarations.h"
#include "thermocycler.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

void OLED_PCR_config_GUI_init(void)
{

	joystick_init_in_GUI();
	//SSD1306_Clear();
	ILI9341_FillScreen(BLACK);
	//SSD1306_GotoXY (0,0);
	//SSD1306_Puts ("Temp:", &Font_7x10, 1);
	//ILI9341_DrawRectangle(0, 0, 320, 39,GREEN );
	display_label(&Temp_L);
	display_label(&Cycle_L);


	//SSD1306_GotoXY (70,0);
	//SSD1306_Puts ("Cy:", &Font_7x10, 1);

/*
	SSD1306_DrawLine(0, 11, 127, 11, WHITE);//top horizontal line
	SSD1306_DrawLine(67, 0, 67, 11, WHITE);//top vertical separation line
	SSD1306_DrawLine(0, 25, 33, 25, WHITE);//denaturation line
	SSD1306_DrawLine(33, 25, 43, 50, WHITE);//ramp after denaturation
	SSD1306_DrawLine(43, 50,76,50, WHITE);//annealing line
	SSD1306_DrawLine(76,50,86,35, WHITE); // ramp after annealing
	SSD1306_DrawLine(86,35,128,35, WHITE); // extension line
	*/


	ILI9341_DrawHLine(0, 25, 320, YELLOW);  //top horizontal line
	ILI9341_DrawHLine(0, 29, 320, YELLOW);  //top horizontal line
	ILI9341_DrawVLine(158,0, 29, YELLOW);  //top seperation verticle line
	ILI9341_DrawVLine(162,0, 29, YELLOW);  //top seperation verticle line

	ILI9341_DrawHLine(0, 94, 83, PINK);     //denaturation line
	ILI9341_DrawLine(83, 94, 108, 188,GREEN);  //ramp after denaturation
	ILI9341_DrawLine(190,188,215,131,GREEN);     // ramp after annealing
	ILI9341_DrawHLine(108, 188, 83, GREENYELLOW);    //annealing line
	ILI9341_DrawHLine(215,131, 83, RED);     // extension line
	//ILI9341_DrawLine(270, 0, 265, 41, WHITE);//slash between cycles
	//ILI9341_DrawLine(0, 0, 320, 240, WHITE);
	//ILI9341_DrawVLine(250,0, 100, WHITE);



	//SSD1306_UpdateScreen(); //display

	display_parameter(&block_temp_OLED);
	display_parameter(&current_cycle_OLED);

	//SSD1306_DrawLine(108, 0, 106, 11, WHITE);//slash between cycles

	display_parameter(&total_cycles_OLED);
	display_parameter(&final_extension_time);
	display_parameter(&annealing_temp);
	display_parameter(&denaturation_temp);
	display_parameter(&extension_temp);
	display_parameter(&initial_denaturation_time);
	display_parameter(&denaturation_time);
	display_parameter(&annealing_time);
	display_parameter(&extension_time);
	update_play_symbol(get_current_thermocycler_state());

	ILI9341_DrawText("C", FONT4,95, 0,WHITE , BLACK); //block temp
	ILI9341_DrawText("C", FONT4,53, 53,WHITE , BLACK); //den temp
	ILI9341_DrawText("C", FONT4,155, 146,WHITE , BLACK);//annealing temp
	ILI9341_DrawText("C", FONT4,260, 75,WHITE , BLACK);//ex temp
	ILI9341_DrawText("sec", FONT4,50, 150,WHITE , BLACK); //initial den time
	ILI9341_DrawText("sec", FONT4,40, 199,WHITE , BLACK);// den time
	ILI9341_DrawText("sec", FONT4,142, 199,WHITE , BLACK);// an time
	ILI9341_DrawText("sec", FONT4,247, 199,WHITE , BLACK);// ex time
	ILI9341_DrawText("sec", FONT4,257, 150,WHITE , BLACK);// end ex time

	/*
	block_temp_OLED.state =NO_HOVER;
	end_cycle_time.state = HOVER;

	HAL_Delay(1000);
	display_parameter(&block_temp_OLED);
    display_parameter(&end_cycle_time);

    block_temp_OLED.state =NO_HOVER;
    end_cycle_time.state = SELECTED;
    HAL_Delay(1000);
    display_parameter(&end_cycle_time);

    block_temp_OLED.state =NO_HOVER;
    end_cycle_time.state = HOVER;

    HAL_Delay(1000);
    display_parameter(&end_cycle_time);

    total_cycles_OLED.state = HOVER ;
    end_cycle_time.state = NO_HOVER;

    HAL_Delay(1000);
    display_parameter(&total_cycles_OLED);
    display_parameter(&end_cycle_time);
	*/

}



void OLED_display_parameters_GUI_init(void)
{


	//SSD1306_Clear();
	ILI9341_FillScreen(BLACK);
	display_label(&Temp_1_L);
	display_label(&Cycle_L);
	display_label(&Ramp_rate_L);
	display_label(&Set_point_L);
	display_label(&Set_point_L);
	display_label(&Lid_temp_L);
	display_label(&extra_1_temp_L);
	display_label(&extra_2_temp_L);
	display_label(&Time_remaining_L);
	display_label(&minutes_L);
	display_label(&progress_L);

	display_parameter(&block_temp_OLED);
	display_parameter(&current_cycle_OLED);
	display_parameter(&total_cycles_OLED);
	display_parameter(&ramp_rate_P);
	display_parameter(&set_point_P);
	display_parameter(&temp_1_P);
	display_parameter(&temp_2_P);
	display_parameter(&time_remaining_P);
	display_parameter(&progress_P);



	SSD1306_DrawLine(108, 0, 106, 11, WHITE);//slash between cycles
	SSD1306_DrawLine(67, 0, 67, 63, WHITE);//top vertical separation line
	SSD1306_DrawLine(0, 11, 127, 11, WHITE);// horizontal line1
	SSD1306_DrawLine(0, 23, 127, 23, WHITE);// horizontal line2
	SSD1306_DrawLine(0, 35, 127, 35, WHITE);// horizontal line3
	SSD1306_DrawLine(0, 47, 127, 47, WHITE);// horizontal line4

	ILI9341_DrawVLine(167, 0, 240, WHITE);
	ILI9341_DrawHLine(0, 41, 320, WHITE);
	ILI9341_DrawHLine(0, 86, 320, WHITE);
	ILI9341_DrawHLine(0, 131, 320, WHITE);
	ILI9341_DrawHLine(0, 176, 320, WHITE);


	SSD1306_UpdateScreen(); //display*/





}


void joystick_init_in_GUI(void)
{
	  joystick_pins.GPIOx    = GPIOB;
	  joystick_pins.UP_key   =  J_UP_Pin;
	  joystick_pins.DOWN_key =  J_DOWN_Pin;
	  joystick_pins.LEFT_key =  J_LEFT_Pin;
	  joystick_pins.RIGHT_key=  J_RIGHT_Pin;
	  joystick_pins.MID_key  =  J_MID_Pin;
	  joystick_pins.SET_key  =  J_SET_Pin;
	  joystick_pins.RST_key  =  J_RST_Pin;
	  joystick_init(&joystick_pins);


}


void display_label(gui_label_t* label)
{

	/*SSD1306_GotoXY (label->x_cord,label->ycord);
	SSD1306_Puts (label->value, label->font, label->foreground);
	SSD1306_UpdateScreen(); //display*/

	ILI9341_DrawText(label->value, label->font,label->x_cord,label->ycord, label->foreground, label->background);
}



void display_parameter(parameter* par)
{
	char num_string[10];


	if (!((*current_menu).menu_name & (*par).menu_name))
	{
		return; //if parameter doesnot belong to current menu
	}


	if (par->type == FLOAT)
		sprintf(num_string,"%0.1f",par->value);
	else
		sprintf(num_string,"%d",(int)par->value);


	if(par->state == SELECTED)
	{
		par->foreground = RED;
		par->background = GREEN;
		//SSD1306_DrawFilledRectangle(par->x_cord, par->ycord,30,10,WHITE);
		//ILI9341_DrawRectangle(par->x_cord, par->ycord, 40, 20,GREEN );

	}
	else if (par->state == HOVER)
	{
		par->foreground = BLUE;
		par->background = WHITE;

		//ILI9341_DrawRectangle(par->x_cord, par->ycord, 40, 20,YELLOW );
	}
	else if (par->state == NO_HOVER)
	{
		par->foreground = WHITE;
		par->background = BLACK;
		//ILI9341_DrawRectangle(par->x_cord, par->ycord, 40, 20,par->background );
	}

	//SSD1306_GotoXY (par->x_cord, par->ycord);
	//SSD1306_Puts (num_string, &Font_7x10, par->foreground);
	//SSD1306_UpdateScreen(); //display*/

	ILI9341_DrawText(num_string, FONT4,par->x_cord, par->ycord,par->foreground , par->background);


}


void scan_joystick_in_PCR_graph_menu()
{
	joystick_out_t  joystick_out;
	joystick_pointer joystick_next_position = {0};
	para_state_t current_para_state;

	joystick_out = read_joystick(&joystick_pins);


	current_para_state = (*(joystick_current_position.selection)).state;



	 if(joystick_out == _MID)
		{
			if(current_para_state == SELECTED)
			{
				(*(joystick_current_position.selection)).state = HOVER;
			}
			else
			{
				(*(joystick_current_position.selection)).state = SELECTED;
			}

			display_parameter(joystick_current_position.selection);

		}
	 else if(current_para_state == SELECTED)
	{

		 para_type_t para_type = (*(joystick_current_position.selection)).type;


		if(joystick_out == _UP )
		{
			if(para_type == FLOAT)
				(*(joystick_current_position.selection)).value += 0.1;
			else
				(*(joystick_current_position.selection)).value++;
		}
		else if(joystick_out == _DOWN)
		{
			if(para_type == FLOAT)
							(*(joystick_current_position.selection)).value -= 0.1;
						else
							(*(joystick_current_position.selection)).value--;

		}
		else if(joystick_out == _LEFT)
				{
					(*(joystick_current_position.selection)).value -= 10;

				}
		else if(joystick_out == _RIGHT)
				{
					(*(joystick_current_position.selection)).value += 10;

				}

		display_parameter(joystick_current_position.selection);
	}

	else if(joystick_out == _LEFT ||joystick_out == _RIGHT ||joystick_out == _UP ||joystick_out == _DOWN )
	{
		if(joystick_out == _LEFT)
			{
					joystick_next_position.selection = (*(joystick_current_position.selection)).left;

			}
		else if(joystick_out == _RIGHT)
			{
					joystick_next_position.selection = (*(joystick_current_position.selection)).right;
			}
		else if(joystick_out == _UP)
				{
						joystick_next_position.selection = (*(joystick_current_position.selection)).up;

				}
		else if(joystick_out == _DOWN)
				{
					joystick_next_position.selection = (*(joystick_current_position.selection)).down;

				}


		joystick_next_position.state = HOVER;
		(*(joystick_next_position.selection)).state = HOVER;
		(*(joystick_current_position.selection)).state = NO_HOVER;
		display_parameter(joystick_current_position.selection);


		joystick_current_position = joystick_next_position;
		display_parameter(joystick_current_position.selection);

	}
	else if(joystick_out == _RST)
	{

		change_menu(current_menu->next_menu);
	}

	else if(joystick_out == _SET)
		{
			uint32_t init_count = HAL_GetTick();

			while(read_joystick(&joystick_pins) == _SET)
			{

			}

			if(HAL_GetTick() - init_count > LONG_PRESS_TIME)
			{
				thermocycler_state_t state;
				state =  get_current_thermocycler_state();
				if(state == IDEL_state)
				{
					start_thermocycler();
				}
				else
				{
					stop_thermocycler();
				}
			}
		}

	 if(joystick_out != NONE)
	 {

		 HAL_Delay(JOYSTICK_DELAY);
	 }

}


void scan_joystick_in_display_par_menu()
{

	joystick_out_t  joystick_out;
	joystick_out = read_joystick(&joystick_pins);

	if(joystick_out == _RST)
		{

			change_menu(current_menu->next_menu);
		}

	if(joystick_out != NONE)
		 {
			 HAL_Delay(JOYSTICK_DELAY);
		 }
}




void update_GUI(void)
{
	if(current_menu->menu_name == PCR_GRAPH_menu)
		{
			scan_joystick_in_PCR_graph_menu();
		}
	else if(current_menu->menu_name == DISPLAY_PAR_menu)
		{
			scan_joystick_in_display_par_menu();
		}
}

void change_menu(menu_t* new_menu)
{
	/*
	if((*new_menu).menu_name == (*current_menu).menu_name)
	{
		return;
	}
*/
	current_menu = new_menu;
	if((*new_menu).menu_name == PCR_GRAPH_menu)
	{
		OLED_PCR_config_GUI_init();
	}
	else if((*new_menu).menu_name == DISPLAY_PAR_menu)
	{
		OLED_display_parameters_GUI_init();
	}

}


void update_parameter_value(parameter_name_t parameter_name, float value)
{
	if(parameter_name == BLOCK_TEAMPERATURE)
	{
		block_temp_OLED.value = value;
		display_parameter(&block_temp_OLED);
	}
	else if(parameter_name == RAMP_RATE)
	{
		ramp_rate_P.value = value;
		display_parameter(&ramp_rate_P);
	}
	else if(parameter_name == LID_TEMP)
		{
			lid_temp_P.value = value;
			display_parameter(&lid_temp_P);
		}
	else if(parameter_name == SET_POINT)
		{
			set_point_P.value = value;
			display_parameter(&set_point_P);
		}
	else if(parameter_name == CURRENT_CYCLE)
		{
			current_cycle_OLED.value = value;
			display_parameter(&current_cycle_OLED);
			display_parameter(&total_cycles_OLED);


		}
	else if(parameter_name == TEMP_1)
			{
				temp_1_P.value = value;
				display_parameter(&temp_1_P);


			}
	else if(parameter_name == TEMP_2)
			{
				temp_2_P.value = value;
				display_parameter(&temp_2_P);


			}
	else if(parameter_name == TIME_REMAINING)
				{
					if (get_current_thermocycler_state() == IDEL_state)
						{
							time_remaining_P.value = Total_time_require ;
							progress_P.value =  0;
						}
					else
						{
							time_remaining_P.value =Total_time_require -  (HAL_GetTick() - Test_start_time)/1000;
							progress_P.value      =  (float)(100*(Total_time_require - time_remaining_P.value )/Total_time_require);

							time_remaining_P.value = time_remaining_P.value/60;
						}

					display_parameter(&time_remaining_P);
					display_parameter(&progress_P);

				}



}

void get_PCR_settings(void* PCR_settings)
{
	*(PCR_settings_t **)PCR_settings = &PCR_settings_in_GUI;
}

void display_big_message(char* message1,char* message2,uint8_t* Font, uint8_t time_s)
{
	//SSD1306_Clear();
	//SSD1306_GotoXY (0,0);
	//SSD1306_Puts (message1, Font, WHITE);
	//SSD1306_GotoXY (0,30);
	//SSD1306_Puts (message2, Font, WHITE);
	//SSD1306_UpdateScreen(); //display*/
	ILI9341_FillScreen(BLACK);
	ILI9341_DrawText(message1, FONT4,130, 90, WHITE, RED);
	ILI9341_DrawText(message2, FONT4,130, 140, WHITE, RED);

	HAL_Delay(time_s*1000);
	change_menu(current_menu);


}



void update_play_symbol(thermocycler_state_t thermocycler_state)
{
	if (thermocycler_state == IDEL_state)
	{
		//SSD1306_DrawFilledRectangle(59, 20,10,10,WHITE);
		//ILI9341_DrawRectangle(120, 75, 50, 38,RED );
		ILI9341_DrawText("STOPPED", FONT4,120, 40, WHITE, RED);

	}
	else
	{
		//ILI9341_DrawRectangle(120, 75, 50, 38,RED );
		ILI9341_DrawText("STARTED", FONT4,120, 40, WHITE, RED);
	}
}




