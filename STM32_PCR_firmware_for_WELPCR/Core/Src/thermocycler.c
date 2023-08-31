/*
 * thermocycler.c
 *
 *  Created on: Dec 27, 2021
 *      Author: vivek
 */

#include "thermocycler.h"
#include "OLED_GUI.h"
#define ABS(x)                    ((x) > 0? (x) :-(x) )


              cycle_state_t cycle_state = denaturation_state;

uint32_t target_count;
PCR_settings_t* PCR_settings;
uint8_t cycle_count = 0;
uint8_t Total_cycles;
thermocycler_state_t thermocycler_state = IDEL_state;

void set_themocycler_state(thermocycler_state_t state, cycle_state_t cycle)
{
	thermocycler_state = state;
		   cycle_state = cycle;
}

void start_thermocycler(void)
{
	get_PCR_settings(&PCR_settings);
	cycle_count = 0;
	Total_cycles = (uint8_t)(*((*PCR_settings).total_cycles_OLED)).value ;
	update_parameter_value(CURRENT_CYCLE,  cycle_count);

	 set_themocycler_state(Initial_denaturation_state, denaturation_state);
	 test_start_buzzer();

	display_big_message("Test", "Started",FONT4,2);
	update_play_symbol(Initial_denaturation_state);


	int cycles = (*((*PCR_settings).total_cycles_OLED)).value ;

	Total_time_require = (*((*PCR_settings).initial_denaturation_time)).value ;
	Total_time_require +=  cycles * ((*((*PCR_settings).denaturation_time)).value) ;
	Total_time_require +=  cycles * ((*((*PCR_settings).annealing_time)).value) ;
	Total_time_require +=  cycles * ((*((*PCR_settings).extension_time)).value) ;
	Total_time_require +=  ((*((*PCR_settings).final_extension_time)).value) ;
	Total_time_require = Total_time_require;
	Test_start_time    =  HAL_GetTick();

}


void stop_thermocycler(void)
{
	cycle_state       = denaturation_state;
   thermocycler_state = IDEL_state;
   display_big_message("Test", "Stopped",FONT4,2);

   update_play_symbol(IDEL_state);

}

thermocycler_state_t get_current_thermocycler_state(void)
{
	return thermocycler_state;
}




float get_block_set_point(void)
{
	static float set_point = 35;
	static float exess_set;
	static float exess_time;
	static float exess_slope;
	float modified_set_point = 35;
	uint16_t temp;
	if(target_count< HAL_GetTick())
	{
		switch(thermocycler_state)
		{
			case IDEL_state:{

				set_point          = 35;
				thermocycler_state = IDEL_state;

				break;
			}

			case Initial_denaturation_state:{

				set_point          = (*((*PCR_settings).denaturation_temp)).value ;
				cycle_count        = 0;
				temp               =  (*((*PCR_settings).initial_denaturation_time)).value ;
				target_count       =  HAL_GetTick()+ SEC_TO_MILLISEC(temp);

				exess_set = 0;
				exess_time = 45;//70
				exess_slope = exess_set/(exess_time/0.1);

				thermocycler_state = cycling_state;
				cycle_state        = denaturation_state;

				break;
			}
			case cycling_state:{

				switch(cycle_state)
						{
							case denaturation_state:{

								set_point          = (*((*PCR_settings).denaturation_temp)).value ;
								temp               =  (*((*PCR_settings).denaturation_time)).value ;
								target_count      =  HAL_GetTick()+ SEC_TO_MILLISEC(temp);

								if(cycle_count==0)
								{
									exess_set = 0;
								}
								else
								{
									exess_set =0; //10;//20
								}


								exess_time = 22;//18;//22;//27 ,42
								exess_slope = exess_set/(exess_time/0.1);

								cycle_state       = annealing_state;
								thermocycler_state = cycling_state;
								break;
							}

							case annealing_state:{
								set_point          =  (*((*PCR_settings).annealing_temp)).value ;
								temp               =  (*((*PCR_settings).annealing_time)).value ;
								target_count      =  HAL_GetTick()+ SEC_TO_MILLISEC(temp);

								exess_set = 0;//-10;//-40;
								exess_time = 36;//25;//36;
								exess_slope = exess_set/(exess_time/0.1);

								cycle_state       = extension_state;
								thermocycler_state = cycling_state;
								break;
							}

							case extension_state:{
								set_point          = (*((*PCR_settings).extension_temp)).value ;
								temp               =  (*((*PCR_settings).extension_time)).value ;
								target_count      =  HAL_GetTick()+ SEC_TO_MILLISEC(temp);

								exess_set = 0;//20;//10;
								exess_time = 14;//10;//18
								exess_slope = exess_set/(exess_time/0.1);

								cycle_state       =  denaturation_state;
								thermocycler_state = cycling_state;

								cycle_count++;
								update_parameter_value(CURRENT_CYCLE,  cycle_count);

								if(cycle_count == Total_cycles)
									{
										thermocycler_state = final_extension_state;
									}
								else
									{
										thermocycler_state = cycling_state;

									}

								break;
							}



						}


					break;
				}

			case final_extension_state:{

				set_point          = (*((*PCR_settings).extension_temp)).value ;
				temp               =  (*((*PCR_settings).final_extension_time)).value ;
				target_count      =  HAL_GetTick()+ SEC_TO_MILLISEC(temp);

				exess_set = 0;
				exess_time = 18;
				exess_slope = exess_set/(exess_time/0.1);

				cycle_state       = denaturation_state;
				thermocycler_state = cycles_completed;
				break;
			}

			case cycles_completed : {
				set_point         = -2;
				target_count      =  HAL_GetTick()+ SEC_TO_MILLISEC(5);

				display_big_message("Test", "Completed",FONT4,2);
				update_play_symbol(IDEL_state);

				cycle_state       = denaturation_state;
				thermocycler_state = IDEL_state;


				break;
			}
		}
	}



if (ABS(exess_set) >0.5)
{
	exess_set =(exess_set-exess_slope);
	modified_set_point = set_point + exess_set;
}
else
{
	modified_set_point = set_point;
}

return modified_set_point;
}



