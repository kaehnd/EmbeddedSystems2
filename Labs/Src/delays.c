/*
 * delays.c
 *
 *  Created on: Dec 10, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 2: Night Rider Lights
 *
 *      Implements delays for general use with the Systick timer
 */

#include "delays.h"
#include "registers.h"

//Systick base pointer
static volatile uint32_t * const stk = (uint32_t *) STK_BASE;

/*
 * **BLOCKS**
 * Delays ticks amount of ticks on the Systick timer without x8 pre-scalar
 * Params: uint32_t ticks, amount of ticks to delay
 */
static void delayTicks(uint32_t ticks);

/*
 * ***BLOCKS***
 * Delays s amount of seconds using Systick timer
 * Params: int amount of seconds to delay
 * Returns: none
 */
void delay_s(uint32_t s)
{
	delayTicks(s * TICKS_S);
}

void startDelay_s(uint32_t s)
{
		//Clear vals
	stk[STK_CTRL] = 0;
	stk[STK_LOAD] = 0;
	stk[STK_VAL] = 0;

	stk[STK_LOAD] = s * TICKS_S;
	stk[STK_CTRL] |= (STK_ENABLE|STK_CLKSRC);
}

int pollStk()
{
	return (stk[STK_CTRL] & STK_CNTFLAG);
}

/*
 * ***BLOCKS***
 * Delays ms amount of milliseconds using Systick timer
 * Params: int amount of milliseconds to delay
 * Returns: none
 */
void delay_ms(uint32_t ms)
{
	delayTicks(ms * TICKS_MS);
}

/*
 * ***BLOCKS***
 * Delays ms amount of milliseconds using Systick timer
 * Params: uint32_t amount of milliseconds to delay
 * Returns: none
 */
void delay_us(uint32_t us)
{
	delayTicks(us << TICKS_US_SHIFTS); //rapid multiplication by 16 to convert to num ticks
}

/*
 * **BLOCKS**
 * Delays ticks amount of ticks on the Systick timer without x8 pre-scalar
 * Params: uint32_t ticks, amount of ticks to delay
 */
static void delayTicks(uint32_t ticks)
{
	//Clear vals
	stk[STK_CTRL] = 0;
	stk[STK_LOAD] = 0;
	stk[STK_VAL] = 0;

	stk[STK_LOAD] = ticks;
	stk[STK_CTRL] |= (STK_ENABLE|STK_CLKSRC);

	//Loop until count flag goes high
	while (!(stk[STK_CTRL] & STK_CNTFLAG)) {}
}


