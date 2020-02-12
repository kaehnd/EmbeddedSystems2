/*
 * adc.h
 *
 *  Created on: Feb 10, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Final Project, do something cool
 *
 *      Guitar Signal Processor
 */
#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>


void adc_init(int frequency);
uint16_t adc_getData();

#endif