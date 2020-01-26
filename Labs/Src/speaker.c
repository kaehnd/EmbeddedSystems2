/*
 * speaker.c
 *
 *  Created on: Dec 23, 2019
 *      Author: Daniel Kaehn
 *      Class: CE 2812-021
 *      Assignment: Lab 3: LCD and Keypad API
 *
 *      Implements API for speaker peripheral
 */


#include "registers.h"
#include "speaker.h"
#include "keypad.h"
#include "delays.h"

#include <stdint.h>

static int keyCheckDelay(int millis);

#define HALF_TICKS_S 8000000 //half of 16000000, the number of ticks of the clock per second

//register pointers
static volatile uint32_t * const gpiob = (uint32_t *) GPIOB_BASE;
static volatile uint32_t * const timer3 = (uint32_t *) TIM3_BASE;

/*
 * Initialize Timer3 and GPIOB for use with speaker peripheral
 * MUST BE CALLED BEFORE ANY OTHER FUNCTION IN THIS FILE
 * Arguments: none
 * Returns: none
 */
void speaker_init(void)
{
	volatile uint32_t * const rcc = (uint32_t *) RCC_BASE;

	//enable RCC for GPIOB
	rcc[AHB1ENR] |= 1<<GPIOBEN;

	//enable RCC for Timer 3
	rcc[APB1ENR] |= 1<<TIM3EN;

	//Enable alternate function 2 for GPIOB pin 4 in AFRL
	gpiob[AFLR] |= AF2 << AFLR4;

	//Enable alternate function in MODER for pin 4
	gpiob[MODER] |= ALTERNATE<<MODER4;

	//Set timer output mode to toggle
	//todo make better defines
	timer3[CCMR1] = 0b011<<4;

	//compare output enable
	timer3[CCER] = 1;

}

/*
 * Plays a continuous square wave tone at the specified frequency
 * Arguments: int frequency, frequency in Hz of tone to be played
 * Returns: none
 */
void speaker_play_tone(int frequency)
{
	//convert frequency to ticks, half total ticks low, half total ticks high
	int ticks = HALF_TICKS_S / frequency;

	//set ARR and CCR1 to frequency ticks
	timer3[ARR] = ticks;
	timer3[CCR1] = ticks;

	//enable and start buzz
	timer3[CR1] = 1;
}

/*
 * Stops any playing tone
 * Arguments: none
 * Returns: none
 */
void speaker_stop(void)
{
	timer3[CR1] = 0;
}

/*
 * Plays tones at specified frequency and blocks until input is recieved from the keypad
 * Arguments: int frequency, frequency in Hz of tone to be played
 * Returns: none
 */
void speaker_alarm(int frequency)
{
	while (1)
	{
		speaker_play_tone(frequency);

		//would be better implemented with interrupts
		if (keyCheckDelay(150))
		{
			return;
		}
		speaker_stop();
		if (keyCheckDelay(150))
		{
			return;
		}
		speaker_play_tone(frequency);
		if (keyCheckDelay(150))
		{
			return;
		}
		speaker_stop();
		if (keyCheckDelay(500))
		{
			return;
		}
	}
}

/*
 * Delays for the specified number of milliseconds but checks
 * keypad every millisecond to stop speaker and delay if input is received
 * Arguments: int millis, milliseconds to delay
 * Returns: int, 1 if input was received from keypad, 0 if delay was uninterrupted
 */
static int keyCheckDelay(int millis)
{
	while (millis--)
	{
		delay_ms(1);
		if (keypad_getkey_noblock() != -1)
		{
			speaker_stop();
			return 1;
		}
	}
	return 0;
}
