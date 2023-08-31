/*
 * thermister_10k.c
 *
 *  Created on: 27-Dec-2021
 *      Author: vivek
 */


#include "thermister_10k.h"



float get_thermister_temp(ADC_HandleTypeDef* hadc1, uint32_t channel)
{
	uint32_t adc_val;
	float thermistor_resistor;
	float temperature;
	//float voltage;

	ADC_ChannelConfTypeDef sConfig = {0};


	  /** Configure Regular Channel
	  */
	  sConfig.Channel = channel;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  if (HAL_ADC_ConfigChannel(hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }


	HAL_ADC_Start(hadc1);  // start the adc
	HAL_ADC_PollForConversion(hadc1, 100);  // poll for conversion
	adc_val = HAL_ADC_GetValue(hadc1)+ADC_OFFSET ;
	HAL_ADC_Stop(hadc1);  // stop adc
	thermistor_resistor = R_SERIES_BLOCK_THERMISTER * ((ADC_HIGHEST/ adc_val) -1);
	//voltage = ((adc_val/ADC_HIGHEST)*3.3);
	temperature =(-273.15)+ 1/((1/T0) + (1/R_SERIES_BLOCK_Beta) * log(thermistor_resistor/BLOCK_THERMISTOR_R0));
	//temperature =(-267.15)+ 1/((1/T0) + (1/R_SERIES_BLOCK_Beta) * log(thermistor_resistor/BLOCK_THERMISTOR_R0));
	//temperature = -28.32* log(thermistor_resistor) + 287.87;
	//return temperature;
	return temperature;

}
