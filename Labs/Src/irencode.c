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
#define FREQUENCY 38000

#include "registers.h"
#include "speaker.h"
#include "keypad.h"
#include "delays.h"

#include <stdint.h>

static int keyCheckDelay(int millis);

#define HALF_TICKS_S 8000000 //half of 16000000, the number of ticks of the clock per second

#define HDR_LOW_TIME 8222
#define HDR_HIGH_TIME 4086
#define SND_LOW_TIME 527
#define SND_HIGH_TIME_1 1531
#define SND_HIGH_TIME_0 514



//register pointers
static volatile uint32_t * const gpioc = (uint32_t *) GPIOC_BASE;
static volatile uint32_t * const timer2 = (uint32_t *) TIM2_BASE;
static volatile uint32_t * const timer3 = (uint32_t *) TIM3_BASE;
static volatile uint32_t * nvic = (uint32_t *) NVIC_BASE;


static int sending = 0;
static uint16_t dataToSend = 0;
static int bitsSent = 0;

static void startDelay(int us);
static void pwm_start();
static void pwm_stop(void);

static enum {HDR_LOW, HDR_HIGH, SND_LOW, SND_HIGH, PAUSE_LOW, PAUSE_HIGH, DONE} state;



/*
 * Initialize Timer3 and GPIOB for use with speaker peripheral
 * MUST BE CALLED BEFORE ANY OTHER FUNCTION IN THIS FILE
 * Arguments: none
 * Returns: none
 */
void irEncodeInit(void)
{
	volatile uint32_t * const rcc = (uint32_t *) RCC_BASE;

	//enable RCC for GPIOB
	rcc[AHB1ENR] |= 1<<GPIOCEN;

	//enable RCC for Timer 3
	rcc[APB1ENR] |= 1<<TIM3EN;

	//Enable alternate function 2 for GPIOB pin 4 in AFRL
	gpioc[AFRL] |= AF2 << AFLR6;

	//PC6

	//Enable alternate function in MODER for pin 4
	gpioc[MODER] |= ALTERNATE<<MODER6;

	//Set timer output mode to toggle
	//todo make better defines
	timer3[CCMR1] = 0b011<<4;

	//compare output enable
	timer3[CCER] = 1;


    rcc[APB1ENR] |= 1<<TIM2EN;
    timer2[PSC] = 0x0F;
	timer2[EGR] = 1;
	timer2[ARR] = 0xFFFFFFFF;
	timer2[SR] = 0;

	timer2[DIER] = 1;
	nvic[ISER0] |= 1<<28;

	startDelay(500);
}

void sendCode(uint16_t code) {
	sending = 1;
	dataToSend = code;

	pwm_start();
	startDelay(HDR_LOW_TIME);
	state = HDR_LOW;
}

void TIM2_IRQHandler() {
	timer2[SR] = 0;

	switch (state) {

		case HDR_LOW:
			pwm_stop();
			startDelay(HDR_HIGH_TIME);
			state = HDR_HIGH;	
			break;

		case HDR_HIGH:
			pwm_start();
			startDelay(SND_LOW_TIME);
			state = SND_LOW;
			
			break;

		case SND_LOW:
			pwm_stop();

			if (dataToSend & 0x01) {
				startDelay(SND_HIGH_TIME_1);
			} else {
				startDelay(SND_HIGH_TIME_0);
			}
			dataToSend >>= 1;
			bitsSent++;

			state = SND_HIGH;
			
			break;

		case SND_HIGH:
			pwm_start();
			startDelay(SND_LOW_TIME);

			if (bitsSent == 8) {
				state = HDR_LOW;
			} else if (bitsSent >= 16) {
				state = DONE;
			} else {
				state = SND_LOW;
			}
			
			break;
		
		case DONE:

			pwm_stop();
			sending = 0;
			break;

		default: 
			
			break;
	}
}


static void pwm_start()
{
	//convert frequency to ticks, half total ticks low, half total ticks high
	int ticks = HALF_TICKS_S / FREQUENCY;

	//set ARR and CCR1 to frequency ticks
	timer3[ARR] = ticks;
	timer3[CCR1] = ticks;

	//enable 
	timer3[CR1] = 1;
}

static void pwm_stop(void)
{
	timer3[CR1] = 0;
}
static void startDelay(int us) {
	timer2[ARR] = us;

	timer2[CNT] = 0;
	timer2[DIER] = 1;
	timer2[CR1] = 1<<3 | 1;

}
