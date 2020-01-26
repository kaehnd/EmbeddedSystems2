/*
 * delays.h
 *
 *  Created on: Dec 10, 2019
*      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 2: Night Rider Lights
 *
 *       Declares delays for general use with the Systick timer
 */

#ifndef DELAYS_H_
#define DELAYS_H_

#include <stdint.h>


//Tick counts for time units
#define TICKS_US_SHIFTS 4
#define TICKS_MS 16000
#define TICKS_S 16000000

//bits for STK timer
#define STK_CNTFLAG 1<<16
#define STK_ENABLE 1 <<0
#define STK_CLKSRC 1<<2



void delay_s(uint32_t s);
/*
 * ***BLOCKS***
 * Delays ms amount of milliseconds using Systick timer
 * Params: int amount of milliseconds to delay
 * No Returns
 */
void delay_ms(uint32_t ms);

/*
 * ***BLOCKS***
 * Delays ms amount of milliseconds using Systick timer
 * Params: uint32_t amount of milliseconds to delay
 * Returns: none
 */
void delay_us(uint32_t us);

#endif
