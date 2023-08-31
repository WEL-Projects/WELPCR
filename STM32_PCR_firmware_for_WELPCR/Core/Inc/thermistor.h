/*
 * thermistor.h
 *
 *  Created on: 19-Apr-2022
 *      Author: vivek
 */

#ifndef INC_THERMISTOR_H_
#define INC_THERMISTOR_H_

#include "stm32f1xx_hal.h"

#define T0                        (float)298.15        // 25 degree to kelvin
#define ADC_OFFSET                (uint32_t)53
#define ADC_HIGHEST               (float)4096


typedef struct {
	float R0;  //resistance of thermistor at 25 degree
	float Rs;  //resistance of resistor connected in series, connected to ground
	float beta;
	ADC_HandleTypeDef* adc;
	uint32_t channel;
	float temperature;

} thermistor_t;


void get_thermistor_temp(thermistor_t *themistor);

#endif /* INC_THERMISTOR_H_ */
