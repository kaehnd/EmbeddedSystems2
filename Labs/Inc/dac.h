/*
 * dac.h
 *
 *  Created on: Feb 10, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Final Project, do something cool
 *
 *      Guitar Signal Processor
 */
#ifndef DAC_H
#define DAC_H

#include <stdint.h>

#define DACEN1 0

/*
 * Initializes DAC1 for output
 * Parameters: None
 * Returns: None
 */
void dac_init(void);

/*
 * Puts sample to DAC 0-4095 corresponding to 0-3.3V
 * Parameters: uint16_t 12-bit value 0<value<4095
 * Returns: None
 */
void dac_put(uint16_t value);

#endif