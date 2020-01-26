/*
 * leds.h
 *
 *  Created on: Dec 10, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 2: Night Rider Lights
 *
 *      Declares functions for interfacing with LED peripheral for Devboard
 */
#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>
#include "registers.h"

/*
 * Initializes LEDs on devboard in RCC and MODER
 * No Params
 * No Returns
 */
void LED_init(void);
/*
 * Turns on specified bit mask on LEDs
 * Params num, mask used to modify bits
 * No returns
 */
void light_LED(uint32_t num);

#endif
