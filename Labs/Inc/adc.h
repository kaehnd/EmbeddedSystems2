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

#define ADC1EN 0
#define ADCEXT_TIM5 0b1010
#define ADCEXT1 24
#define ADCTGMTCH 0b11
#define ADC1_TGMODE 28
#define TIM_TGMTCH 0b11
#define TIM_OCM1 4

/*
 * Initializes ADC using TIM5 to trigger with the specified frequency
 * Parameters: int frequency, frequency for sample rate
 * Returns: None
 */
void adc_init(int frequency);

/*
 * Gets 12-bit data from the ADC 
 * **BLOCKS*
 * Parameters: None
 * Returns: 12-bit Voltage ratio 0-4095 corresponding to 0-3.3V 
 */
uint16_t adc_getData(void);

#endif