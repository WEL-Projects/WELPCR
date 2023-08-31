/*
 * thermistor.c
 *
 *  Created on: 19-Apr-2022
 *      Author: vivek
 */

#include "thermistor.h"

void  get_thermistor_temp_(thermistor_t *themistor)
{

	uint32_t adc_val;
	float thermistor_resistor;
	float temperature;
	//float voltage;

	ADC_ChannelConfTypeDef sConfig = {0};


	  /** Configure Regular Channel
	  */
	  sConfig.Channel = themistor->channel;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  if (HAL_ADC_ConfigChannel(themistor->adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }


	HAL_ADC_Start(themistor->adc);  // start the adc
	HAL_ADC_PollForConversion(themistor->adc, 100);  // poll for conversion
	adc_val = HAL_ADC_GetValue(themistor->adc)+ ADC_OFFSET;
	HAL_ADC_Stop(themistor->adc);  // stop adc
	thermistor_resistor = themistor->Rs * ((ADC_HIGHEST/ adc_val) -1);
	//voltage = ((adc_val/ADC_HIGHEST)*3.3);
	themistor->temperature =(-273.15)+ 1/((1/T0) + (1/themistor->beta) * log(thermistor_resistor/themistor->R0));
	//temperature =(-267.15)+ 1/((1/T0) + (1/R_SERIES_BLOCK_Beta) * log(thermistor_resistor/BLOCK_THERMISTOR_R0));
	//temperature = -28.32* log(thermistor_resistor) + 287.87;
	//return themistor->temperature;
}
