/*
 * leds.c
 *
 *  Created on: Dec 10, 2019
*      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 2: Night Rider Lights
 *
 *      Implements functions for interfacing with LED peripheral for Devboard
 */

#include "leds.h"
#include "registers.h"

static volatile uint32_t * const gpiob = (uint32_t *) GPIOB_BASE;

/*
 * Initializes LEDs on devboard in RCC and MODER
 * No Params
 * No Returns
 */
void LED_init(void) {
	//Turn on in RCC
	volatile uint32_t * const rcc = (uint32_t *) RCC_BASE;
	rcc[AHB1ENR] |= (1<<1);

	//Make pins outputs
	gpiob[MODER] |= (0xAA2AA800 >> 1);
}

/*
 * Turns on specified bit mask on LEDs
 * Params num, mask used to modify bits
 * No returns
 */
void light_LED(uint32_t num) {
	//init pointer of GPIOB_ODR
	//uint32_t *gpioBOdr = GPIOB_ODR;

	//clear bits
	gpiob[ODR] &= ~(0x0000F7E0);

	//modify bits
	gpiob[ODR] |= num;

}
