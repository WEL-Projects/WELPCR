/*
 * thermister_10k.h
 *
 *  Created on: 27-Dec-2021
 *      Author: vivek
 */


#ifndef INC_THERMISTER_10K_H_
#define INC_THERMISTER_10K_H_

#define R_SERIES_BLOCK_THERMISTER (float)975.0
#define ADC_HIGHEST               (float)4096
#define R_SERIES_BLOCK_Beta       (float)3993.61
#define BLOCK_THERMISTOR_R0       (float)10000
#define T0                        (float)298.15        // 25 degree to kelvin
#define ADC_OFFSET                (float)57.1

#include "stm32f1xx_hal.h"

float get_thermister_temp(ADC_HandleTypeDef* hadc1, uint32_t channel);




#endif /* INC_THERMISTER_10K_H_ */
